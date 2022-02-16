#include "Player.h"
#include "input.h"
#include "block.h"
#include "fade.h"
#include "databox.h"
#include "sound.h"

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;
LPDIRECT3DTEXTURE9 g_pTexturePlayer[MAX_PLAYER] = {};
int g_nCounterAnim = 0;
int g_nPatternAnim = 0;
float fTexX = 0.0f;						//�e�N�X�`�����WX
float fTexY = 0.0f;						//�e�N�X�`�����WY
float fMoveD = 0.0f;					//�ړ��ʂ̍ō�
float fMoveA = 0.0f;					//�ړ��ʂ̍ō�
int nPlayerType = TYPE_RED;				//����L�����N�^�[����
bool bLock;
PLAYER g_Player[MAX_PLAYER];
DATA *pData;

//������
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X���擾

	g_nCounterAnim = 0;					//�J�E���^�[������
	g_nPatternAnim = 0;					//�p�^�[��No������

	pData = Getdata();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\picture\\black.png",
		&g_pTexturePlayer[0]);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PLAYER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	for(int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		g_Player[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[nCnt].lastpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[nCnt].nJump = JUMP_MAX;
		g_Player[nCnt].bUse = false;
		fTexX = 0.0f;
		fTexY = 0.0f;
		fMoveD = 0.0f;
		fMoveA = 0.0f;
		bLock = false;
		nPlayerType = TYPE_RED;

		VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

							//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y, 0.0f);

		//rhw�̐ݒ�(1.0f�Œ�)
		pVtx[nCnt * 4 + 0].rhw = 1.0f;
		pVtx[nCnt * 4 + 1].rhw = 1.0f;
		pVtx[nCnt * 4 + 2].rhw = 1.0f;
		pVtx[nCnt * 4 + 3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[nCnt * 4 + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[nCnt * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[nCnt * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[nCnt * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffPlayer->Unlock();
	}
}

//�I��
void UninitPlayer(void)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		//�e�N�X�`���j��
		if (g_pTexturePlayer[nCnt] != NULL)
		{
			g_pTexturePlayer[nCnt]->Release();
			g_pTexturePlayer[nCnt] = NULL;
		}

		//���_�o�b�t�@�j��
		if (g_pVtxBuffPlayer != NULL)
		{
			g_pVtxBuffPlayer->Release();
			g_pVtxBuffPlayer = NULL;
		}
	}
}

//�X�V
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	BLOCK *pBlock;
	pBlock = GetBlock();

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);



	//--------------------------------------------------------------
	//lastpos�X�V

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		g_Player[nCnt].lastpos = g_Player[nCnt].pos;
	}
	//--------------------------------------------------------------



	//--------------------------------------------------------------
	//����L�����ύX

	if (GetKeyboardTrigger(DIK_A) == true)		//A
	{
		nPlayerType = TYPE_RED;
	}

	if (GetKeyboardTrigger(DIK_S) == true)		//S
	{
		nPlayerType = TYPE_BLUE;
	}

	if (GetKeyboardTrigger(DIK_D) == true)		//D
	{
		nPlayerType = TYPE_YELLOW;
	}
	//--------------------------------------------------------------



	//--------------------------------------------------------------
	//�d�͊֌W

	//�W�����v
	if (GetKeyboardTrigger(DIK_SPACE) == true && g_Player[nPlayerType].nJump < JUMP_MAX)
	{
		g_Player[nPlayerType].move.y = -12.5f;
		g_Player[nPlayerType].nJump++;
		PlaySound(SOUND_LABEL_SEJUMP);
	}

	//--------------------------------------------------------------



	//--------------------------------------------------------------
	//�L�[���͈ړ�

	if (GetKeyboardPress(DIK_RIGHT) == true)	//�E
	{
		g_Player[nPlayerType].pos.x += g_Player[nPlayerType].move.x;
	}

	if (GetKeyboardPress(DIK_LEFT) == true)		//��
	{
		g_Player[nPlayerType].pos.x -= g_Player[nPlayerType].move.x;
	}

	//--------------------------------------------------------------



	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		//--------------------------------------------------------------
		//�d�͊֌W

		//�d�͂����߂�
		g_Player[nCnt].move.y += 0.8f;

		//�d�͂̌��E
		if (g_Player[nCnt].move.y >= MAX_GRAVITY)
		{
			g_Player[nCnt].move.y = MAX_GRAVITY;
		}

		//��ɍ~��������
		g_Player[nCnt].pos.y += g_Player[nCnt].move.y;

		if (g_Player[nCnt].nJump == 0)
		{
			g_Player[nCnt].nJump = 1;
		}

		//--------------------------------------------------------------



		//--------------------------------------------------------------
		//��ʒ[�̓����蔻��

		if (g_Player[nCnt].pos.x + PLAYER_X >= SCREEN_WIDTH)		//��ʉE
		{
			g_Player[nCnt].pos.x = SCREEN_WIDTH - PLAYER_X;
		}

		if (g_Player[nCnt].pos.x - PLAYER_X <= 0)					//��ʍ�
		{
			g_Player[nCnt].pos.x = 0 + PLAYER_X;
		}

		if (g_Player[nCnt].pos.y - PLAYER_Y >= SCREEN_HEIGHT && bLock == false)		//��ʉ�
		{
			bLock = true;
			pData->nMiss++;

			if (pData->nMiss >= 99)
			{
				pData->nMiss = 99;
			}

			SetFade(MODE_GAME);
		}

		//--------------------------------------------------------------
	}



	//--------------------------------------------------------------
	//�v���C���[���m�̓����蔻��

	for (int nCnt2 = 0; nCnt2 < MAX_PLAYER; nCnt2++)
	{
		if (nPlayerType != nCnt2)
		{
			ColisionPlayer(nPlayerType, nCnt2);
		}
	}

	//--------------------------------------------------------------



	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		//--------------------------------------------------------------
		//�u���b�N�Ƃ̓����蔻��

		ColisionBlock(&g_Player[nCnt].pos, &g_Player[nCnt].lastpos, &g_Player[nCnt].move, &g_Player[nCnt].nJump, nCnt);

		//--------------------------------------------------------------



		//--------------------------------------------------------------
		//�v���C���[���m�̓����蔻��

		for (int nCnt2 = 0; nCnt2 < MAX_PLAYER; nCnt2++)
		{
			if (nCnt != nCnt2)
			{
				ColisionPlayer(nCnt, nCnt2);
			}
		}

		//--------------------------------------------------------------
	}



	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		//--------------------------------------------------------------
		//���_���W�̐ݒ�
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y, 0.0f);

		//rhw�̐ݒ�(1.0f�Œ�)
		pVtx[nCnt * 4 + 0].rhw = 1.0f;
		pVtx[nCnt * 4 + 1].rhw = 1.0f;
		pVtx[nCnt * 4 + 2].rhw = 1.0f;
		pVtx[nCnt * 4 + 3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[nCnt * 4 + 0].col = D3DCOLOR_RGBA(255, 255, 255, 64);
		pVtx[nCnt * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, 64);
		pVtx[nCnt * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, 64);
		pVtx[nCnt * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, 64);

		//�e�N�X�`�����W�̐ݒ�
		pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//--------------------------------------------------------------
	}

	//���_�J���[�̐ݒ�
	pVtx[nPlayerType * 4 + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[nPlayerType * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[nPlayerType * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[nPlayerType * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffPlayer->Unlock();
}

//�`��
void DrawPlayer(void)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (g_Player[nCnt].bUse == true)
		{
			LPDIRECT3DDEVICE9 pDevice;						//�f�o�C�X�̃|�C���^
			pDevice = GetDevice();							//�f�o�C�X���擾

															//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffPlayer,
				0,
				sizeof(VERTEX_2D));

			pDevice->SetFVF(FVF_VERTEX_2D);					//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetTexture(0, g_pTexturePlayer[nCnt]);	//�e�N�X�`���̐ݒ�

															//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,						//�v���~�e�B�u�̎��
				nCnt * 4,									//�J�n���钸�_�̃C���f�b�N�X
				2);											//�v���~�e�B�u�̐�
		}
	}
}

//�Z�b�g
void SetPlayerRed(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X���擾

	g_nCounterAnim = 0;					//�J�E���^�[������
	g_nPatternAnim = 0;					//�p�^�[��No������

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (g_Player[nCnt].bUse == false)
		{
			//�e�N�X�`���ǂݍ���
			D3DXCreateTextureFromFile(
				pDevice,
				"data\\picture\\red.png",
				&g_pTexturePlayer[nCnt]);

			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PLAYER,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pVtxBuffPlayer,
				NULL);

			g_Player[nCnt].pos = pos;
			g_Player[nCnt].lastpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Player[nCnt].move = D3DXVECTOR3(5.0f, 0.0f, 0.0f);
			g_Player[nCnt].nJump = JUMP_MAX;
			g_Player[nCnt].bUse = true;
			fTexX = 0.0f;
			fTexY = 0.0f;
			fMoveD = 0.0f;
			fMoveA = 0.0f;
			nPlayerType = TYPE_RED;

			VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

								//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//���_���W�̐ݒ�
			pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y, 0.0f);
			pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y, 0.0f);

			//rhw�̐ݒ�(1.0f�Œ�)
			pVtx[nCnt * 4 + 0].rhw = 1.0f;
			pVtx[nCnt * 4 + 1].rhw = 1.0f;
			pVtx[nCnt * 4 + 2].rhw = 1.0f;
			pVtx[nCnt * 4 + 3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[nCnt * 4 + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCnt * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCnt * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCnt * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//���_�o�b�t�@���A�����b�N
			g_pVtxBuffPlayer->Unlock();

			break;
		}
	}
}

//�Z�b�g
void SetPlayerBlue(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X���擾

	g_nCounterAnim = 0;					//�J�E���^�[������
	g_nPatternAnim = 0;					//�p�^�[��No������

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (g_Player[nCnt].bUse == false)
		{
			//�e�N�X�`���ǂݍ���
			D3DXCreateTextureFromFile(
				pDevice,
				"data\\picture\\blue.png",
				&g_pTexturePlayer[nCnt]);

			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PLAYER,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pVtxBuffPlayer,
				NULL);

			g_Player[nCnt].pos = pos;
			g_Player[nCnt].lastpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Player[nCnt].move = D3DXVECTOR3(5.0f, 0.0f, 0.0f);
			g_Player[nCnt].nJump = JUMP_MAX;
			g_Player[nCnt].bUse = true;
			fTexX = 0.0f;
			fTexY = 0.0f;
			fMoveD = 0.0f;
			fMoveA = 0.0f;
			nPlayerType = TYPE_RED;

			VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

								//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//���_���W�̐ݒ�
			pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y, 0.0f);
			pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y, 0.0f);

			//rhw�̐ݒ�(1.0f�Œ�)
			pVtx[nCnt * 4 + 0].rhw = 1.0f;
			pVtx[nCnt * 4 + 1].rhw = 1.0f;
			pVtx[nCnt * 4 + 2].rhw = 1.0f;
			pVtx[nCnt * 4 + 3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[nCnt * 4 + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCnt * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCnt * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCnt * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//���_�o�b�t�@���A�����b�N
			g_pVtxBuffPlayer->Unlock();

			break;
		}
	}
}

//�Z�b�g
void SetPlayerYellow(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X���擾

	g_nCounterAnim = 0;					//�J�E���^�[������
	g_nPatternAnim = 0;					//�p�^�[��No������

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (g_Player[nCnt].bUse == false)
		{
			//�e�N�X�`���ǂݍ���
			D3DXCreateTextureFromFile(
				pDevice,
				"data\\picture\\yellow.png",
				&g_pTexturePlayer[nCnt]);

			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PLAYER,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pVtxBuffPlayer,
				NULL);

			g_Player[nCnt].pos = pos;
			g_Player[nCnt].lastpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Player[nCnt].move = D3DXVECTOR3(5.0f, 0.0f, 0.0f);
			g_Player[nCnt].nJump = JUMP_MAX;
			g_Player[nCnt].bUse = true;
			fTexX = 0.0f;
			fTexY = 0.0f;
			fMoveD = 0.0f;
			fMoveA = 0.0f;
			nPlayerType = TYPE_RED;

			VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

								//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//���_���W�̐ݒ�
			pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y, 0.0f);
			pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y, 0.0f);

			//rhw�̐ݒ�(1.0f�Œ�)
			pVtx[nCnt * 4 + 0].rhw = 1.0f;
			pVtx[nCnt * 4 + 1].rhw = 1.0f;
			pVtx[nCnt * 4 + 2].rhw = 1.0f;
			pVtx[nCnt * 4 + 3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[nCnt * 4 + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCnt * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCnt * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCnt * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//���_�o�b�t�@���A�����b�N
			g_pVtxBuffPlayer->Unlock();

			break;
		}
	}
}

PLAYER *GetPlayer(void)
{
	return &g_Player[0];
}

void ColisionPlayer(int nCnt, int nCnt2)
{
	if (g_Player[nCnt].bUse == true)
	{
		if (g_Player[nCnt2].pos.x - PLAYER_X < g_Player[nCnt].pos.x + PLAYER_X &&			//���[
			g_Player[nCnt2].pos.x + PLAYER_X > g_Player[nCnt].pos.x - PLAYER_X &&			//�E�[
			g_Player[nCnt2].pos.y - PLAYER_Y < g_Player[nCnt].pos.y &&						//���
			g_Player[nCnt2].pos.y > g_Player[nCnt].pos.y - PLAYER_Y)						//�����炽����
		{
			if (g_Player[nCnt2].pos.x - PLAYER_X >= g_Player[nCnt].lastpos.x + PLAYER_X &&
				(g_Player[nCnt2].pos.y - PLAYER_Y < g_Player[nCnt].pos.y + 1.0f ||
				g_Player[nCnt2].pos.y > g_Player[nCnt].pos.y + PLAYER_Y - 1.0f))						//���[
			{
				g_Player[nCnt].pos.x = (g_Player[nCnt2].pos.x - PLAYER_X) - PLAYER_X;
			}

			else if (g_Player[nCnt2].pos.x + PLAYER_X <= g_Player[nCnt].lastpos.x - PLAYER_X &&
				(g_Player[nCnt2].pos.y - PLAYER_Y < g_Player[nCnt].pos.y - 1.0f ||
				g_Player[nCnt2].pos.y > g_Player[nCnt].pos.y + PLAYER_Y + 1.0f))						//�E�[
			{
				g_Player[nCnt].pos.x = (g_Player[nCnt2].pos.x + PLAYER_X) + PLAYER_X;
			}

			else if (g_Player[nCnt2].pos.y - PLAYER_Y >= (g_Player[nCnt].lastpos.y - g_Player[nCnt].move.y))	//���
			{
				g_Player[nCnt].pos.y = g_Player[nCnt2].pos.y - PLAYER_Y;
				g_Player[nCnt].move.y = 0;
				g_Player[nCnt].nJump = 0;
			}

			else if (g_Player[nCnt2].pos.y <= (g_Player[nCnt].lastpos.y - g_Player[nCnt].move.y) - PLAYER_Y)	//�����炽����
			{
				g_Player[nCnt].pos.y = g_Player[nCnt2].pos.y + PLAYER_Y;
				g_Player[nCnt].move.y = 0.0f;
			}
		}
	}
}