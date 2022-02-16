#include "Player.h"
#include "Title_cube.h"
#include <stdlib.h>
#include <time.h>

#define COOLTIME (8)
#define CUBE_MOVE_X (10)

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCube = NULL;
LPDIRECT3DTEXTURE9 g_pTextureCube[MAX_PLAYER] = {};
CUBE g_Cube[MAX_PLAYER];

//������
void InitCube(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X���擾

	for(int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		//�e�N�X�`���ǂݍ���
		D3DXCreateTextureFromFile(
			pDevice,
			"data\\picture\\black.png",
			&g_pTextureCube[0]);

			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PLAYER,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pVtxBuffCube,
				NULL);

		g_Cube[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Cube[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Cube[nCnt].bHit = false;
		g_Cube[nCnt].bUse = false;
		g_Cube[nCnt].nCounterAnim = 0;
		g_Cube[nCnt].fTex = 0.0f;
		g_Cube[nCnt].nTimer = 0;

		VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

							//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
		g_pVtxBuffCube->Lock(0, 0, (void**)&pVtx, 0);

		//���_���W�̐ݒ�
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_Cube[nCnt].pos.x - PLAYER_X, g_Cube[nCnt].pos.y - PLAYER_Y, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_Cube[nCnt].pos.x + PLAYER_X, g_Cube[nCnt].pos.y - PLAYER_Y, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_Cube[nCnt].pos.x - PLAYER_X, g_Cube[nCnt].pos.y, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_Cube[nCnt].pos.x + PLAYER_X, g_Cube[nCnt].pos.y, 0.0f);

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
		g_pVtxBuffCube->Unlock();
	}

	g_Cube[TYPE_RED].nRBY = TYPE_RED;
	g_Cube[TYPE_BLUE].nRBY = TYPE_BLUE;
	g_Cube[TYPE_YELLOW].nRBY = TYPE_YELLOW;

	srand((unsigned int)time(NULL));
}

//�I��
void UninitCube(void)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		//�e�N�X�`���j��
		if (g_pTextureCube[nCnt] != NULL)
		{
			g_pTextureCube[nCnt]->Release();
			g_pTextureCube[nCnt] = NULL;
		}

		//���_�o�b�t�@�j��
		if (g_pVtxBuffCube != NULL)
		{
			g_pVtxBuffCube->Release();
			g_pVtxBuffCube = NULL;
		}
	}
}

//�X�V
void UpdateCube(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffCube->Lock(0, 0, (void**)&pVtx, 0);



	//--------------------------------------------------------------
	//�d�͊֌W

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (g_Cube[nCnt].bUse == true)
		{
			//--------------------------------------------------------------
			//�p�x

			float moveX = (float)(rand() % CUBE_MOVE_X) - (CUBE_MOVE_X / 2);
			g_Cube[nCnt].nTimer++;

			if (g_Cube[nCnt].nTimer >= 6)
			{
				g_Cube[nCnt].nTimer = 0;

				if (g_Cube[nCnt].fTex > 0)
				{
					g_Cube[nCnt].fTex += 0.25f;
				}

				else if (g_Cube[nCnt].fTex < 0)
				{
					g_Cube[nCnt].fTex -= 0.25f;
				}
			}

			//--------------------------------------------------------------



			//--------------------------------------------------------------
			//�d�͊֌W

			//�d�͂����߂�
			g_Cube[nCnt].move.y += 0.8f;

			//�d�͂̌��E
			if (g_Cube[nCnt].move.y >= MAX_GRAVITY)
			{
				g_Cube[nCnt].move.y = MAX_GRAVITY;
			}

			//��ɍ~��������
			g_Cube[nCnt].pos.y += g_Cube[nCnt].move.y;
			g_Cube[nCnt].pos.x += g_Cube[nCnt].move.x;

			//--------------------------------------------------------------



			//--------------------------------------------------------------
			//��ʒ[�̓����蔻��

			if (g_Cube[nCnt].pos.y - PLAYER_Y >= SCREEN_HEIGHT)					//��ʉ�
			{
				g_Cube[nCnt].bUse = false;
			}

			//--------------------------------------------------------------



			//--------------------------------------------------------------
			//�����蔻��
			if (g_Cube[nCnt].pos.y >= 600 && g_Cube[nCnt].bHit == false)
			{
				g_Cube[nCnt].move.x = moveX;
				g_Cube[nCnt].move.y = -14;
				g_Cube[nCnt].bHit = true;

				if (moveX > 0)
				{
					g_Cube[nCnt].fTex += 0.25f;
				}

				else if (moveX < 0)
				{
					g_Cube[nCnt].fTex -= 0.25f;
				}
			}

			//--------------------------------------------------------------



			//--------------------------------------------------------------
			//���_���W�̐ݒ�
			pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_Cube[nCnt].pos.x - PLAYER_X, g_Cube[nCnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_Cube[nCnt].pos.x + PLAYER_X, g_Cube[nCnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_Cube[nCnt].pos.x - PLAYER_X, g_Cube[nCnt].pos.y, 0.0f);
			pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_Cube[nCnt].pos.x + PLAYER_X, g_Cube[nCnt].pos.y, 0.0f);

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
			pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(g_Cube[nCnt].fTex, 0.0f);
			pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(g_Cube[nCnt].fTex + 0.25f, 0.0f);
			pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(g_Cube[nCnt].fTex, 1.0f);
			pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(g_Cube[nCnt].fTex + 0.25f, 1.0f);

			//--------------------------------------------------------------
		}

		else
		{
			float nXPos = (float)(rand() % SCREEN_WIDTH);
			float moveY = (float)(rand() % 11);

			g_Cube[nCnt].nCoolTime++;

			if (g_Cube[nCnt].nCoolTime >= COOLTIME)
			{
				g_Cube[nCnt].nCoolTime = 0;

				switch (g_Cube[nCnt].nRBY)
				{
				case TYPE_RED:
					SetCubeRed(D3DXVECTOR3(nXPos, 0.0f - PLAYER_Y, 0.0f), moveY, nCnt);
					break;

				case TYPE_BLUE:
					SetCubeBlue(D3DXVECTOR3(nXPos, 0.0f - PLAYER_Y, 0.0f), moveY, nCnt);
					break;

				case TYPE_YELLOW:
					SetCubeYellow(D3DXVECTOR3(nXPos, 0.0f - PLAYER_Y, 0.0f), moveY, nCnt);
					break;
				}
			}
		}
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffCube->Unlock();
}

//�`��
void DrawCube(void)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (g_Cube[nCnt].bUse == true)
		{
			LPDIRECT3DDEVICE9 pDevice;						//�f�o�C�X�̃|�C���^
			pDevice = GetDevice();							//�f�o�C�X���擾

															//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffCube,
				0,
				sizeof(VERTEX_2D));

			pDevice->SetFVF(FVF_VERTEX_2D);					//���_�t�H�[�}�b�g�̐ݒ�
			pDevice->SetTexture(0, g_pTextureCube[nCnt]);	//�e�N�X�`���̐ݒ�

															//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,						//�v���~�e�B�u�̎��
				nCnt * 4,									//�J�n���钸�_�̃C���f�b�N�X
				2);											//�v���~�e�B�u�̐�
		}
	}
}

//�Z�b�g
void SetCubeRed(D3DXVECTOR3 pos, float move, int cnt)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X���擾

		if (g_Cube[cnt].bUse == false)
		{
			//�e�N�X�`���ǂݍ���
			D3DXCreateTextureFromFile(
				pDevice,
				"data\\picture\\title_red.png",
				&g_pTextureCube[cnt]);

			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PLAYER,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pVtxBuffCube,
				NULL);

			g_Cube[cnt].pos = pos;
			g_Cube[cnt].move = D3DXVECTOR3(0.0f, move, 0.0f);
			g_Cube[cnt].bHit = false;
			g_Cube[cnt].nCoolTime = 0;
			g_Cube[cnt].bUse = true;
			g_Cube[cnt].nCounterAnim = 0;
			g_Cube[cnt].fTex = 0;
			g_Cube[cnt].nTimer = 0;

			VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

								//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
			g_pVtxBuffCube->Lock(0, 0, (void**)&pVtx, 0);

			//���_���W�̐ݒ�
			pVtx[cnt * 4 + 0].pos = D3DXVECTOR3(g_Cube[cnt].pos.x - PLAYER_X, g_Cube[cnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[cnt * 4 + 1].pos = D3DXVECTOR3(g_Cube[cnt].pos.x + PLAYER_X, g_Cube[cnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[cnt * 4 + 2].pos = D3DXVECTOR3(g_Cube[cnt].pos.x - PLAYER_X, g_Cube[cnt].pos.y, 0.0f);
			pVtx[cnt * 4 + 3].pos = D3DXVECTOR3(g_Cube[cnt].pos.x + PLAYER_X, g_Cube[cnt].pos.y, 0.0f);

			//rhw�̐ݒ�(1.0f�Œ�)
			pVtx[cnt * 4 + 0].rhw = 1.0f;
			pVtx[cnt * 4 + 1].rhw = 1.0f;
			pVtx[cnt * 4 + 2].rhw = 1.0f;
			pVtx[cnt * 4 + 3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[cnt * 4 + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[cnt * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[cnt * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[cnt * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[cnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[cnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[cnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[cnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//���_�o�b�t�@���A�����b�N
			g_pVtxBuffCube->Unlock();
		}
}

//�Z�b�g
void SetCubeBlue(D3DXVECTOR3 pos, float move, int cnt)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X���擾

		if (g_Cube[cnt].bUse == false)
		{
			//�e�N�X�`���ǂݍ���
			D3DXCreateTextureFromFile(
				pDevice,
				"data\\picture\\title_blue.png",
				&g_pTextureCube[cnt]);

			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PLAYER,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pVtxBuffCube,
				NULL);

			g_Cube[cnt].pos = pos;
			g_Cube[cnt].move = D3DXVECTOR3(0.0f, move, 0.0f);
			g_Cube[cnt].bHit = false;
			g_Cube[cnt].nCoolTime = 0;
			g_Cube[cnt].bUse = true;
			g_Cube[cnt].nCounterAnim = 0;
			g_Cube[cnt].fTex = 0;
			g_Cube[cnt].nTimer = 0;

			VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

								//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
			g_pVtxBuffCube->Lock(0, 0, (void**)&pVtx, 0);

			//���_���W�̐ݒ�
			pVtx[cnt * 4 + 0].pos = D3DXVECTOR3(g_Cube[cnt].pos.x - PLAYER_X, g_Cube[cnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[cnt * 4 + 1].pos = D3DXVECTOR3(g_Cube[cnt].pos.x + PLAYER_X, g_Cube[cnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[cnt * 4 + 2].pos = D3DXVECTOR3(g_Cube[cnt].pos.x - PLAYER_X, g_Cube[cnt].pos.y, 0.0f);
			pVtx[cnt * 4 + 3].pos = D3DXVECTOR3(g_Cube[cnt].pos.x + PLAYER_X, g_Cube[cnt].pos.y, 0.0f);

			//rhw�̐ݒ�(1.0f�Œ�)
			pVtx[cnt * 4 + 0].rhw = 1.0f;
			pVtx[cnt * 4 + 1].rhw = 1.0f;
			pVtx[cnt * 4 + 2].rhw = 1.0f;
			pVtx[cnt * 4 + 3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[cnt * 4 + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[cnt * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[cnt * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[cnt * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[cnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[cnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[cnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[cnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//���_�o�b�t�@���A�����b�N
			g_pVtxBuffCube->Unlock();
		}
}

//�Z�b�g
void SetCubeYellow(D3DXVECTOR3 pos, float move, int cnt)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X���擾

		if (g_Cube[cnt].bUse == false)
		{
			//�e�N�X�`���ǂݍ���
			D3DXCreateTextureFromFile(
				pDevice,
				"data\\picture\\title_yellow.png",
				&g_pTextureCube[cnt]);

			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PLAYER,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pVtxBuffCube,
				NULL);

			g_Cube[cnt].pos = pos;
			g_Cube[cnt].move = D3DXVECTOR3(0.0f, move, 0.0f);
			g_Cube[cnt].bHit = false;
			g_Cube[cnt].nCoolTime = 0;
			g_Cube[cnt].bUse = true;
			g_Cube[cnt].nCounterAnim = 0;
			g_Cube[cnt].fTex = 0;
			g_Cube[cnt].nTimer = 0;

			VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

								//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
			g_pVtxBuffCube->Lock(0, 0, (void**)&pVtx, 0);

			//���_���W�̐ݒ�
			pVtx[cnt * 4 + 0].pos = D3DXVECTOR3(g_Cube[cnt].pos.x - PLAYER_X, g_Cube[cnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[cnt * 4 + 1].pos = D3DXVECTOR3(g_Cube[cnt].pos.x + PLAYER_X, g_Cube[cnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[cnt * 4 + 2].pos = D3DXVECTOR3(g_Cube[cnt].pos.x - PLAYER_X, g_Cube[cnt].pos.y, 0.0f);
			pVtx[cnt * 4 + 3].pos = D3DXVECTOR3(g_Cube[cnt].pos.x + PLAYER_X, g_Cube[cnt].pos.y, 0.0f);

			//rhw�̐ݒ�(1.0f�Œ�)
			pVtx[cnt * 4 + 0].rhw = 1.0f;
			pVtx[cnt * 4 + 1].rhw = 1.0f;
			pVtx[cnt * 4 + 2].rhw = 1.0f;
			pVtx[cnt * 4 + 3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[cnt * 4 + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[cnt * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[cnt * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[cnt * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[cnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[cnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[cnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[cnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//���_�o�b�t�@���A�����b�N
			g_pVtxBuffCube->Unlock();
		}
}