#include "block.h"
#include "Player.h"
#include "game.h"
#include "databox.h"
#include "fade.h"
#include "sound.h"

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureBlock[MAX_BLOCK] = {};	//�e�N�X�`���ւ̃|�C���^
BLOCK g_Block[MAX_BLOCK];							//�u���b�N�̍\����
bool bOnceFlag;
bool bOnceDoor;

//����������
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X���擾

										//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\picture\\block.png",
		&g_pTextureBlock[0]);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	bOnceFlag = false;
	bOnceDoor = false;

	for (int nCount = 0; nCount < MAX_BLOCK; nCount++)
	{
		g_Block[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Block[nCount].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Block[nCount].fWidth = 0.0f;
		g_Block[nCount].fHeight = 0.0f;
		g_Block[nCount].bUse = false;
		g_Block[nCount].nType = BLOCK_NORMAL;
		g_Block[nCount].fMove = 0.0f;
		g_Block[nCount].fMoveMax = 0.0f;
		g_Block[nCount].UDMoveScan = 0;
		g_Block[nCount].LRMoveScan = 0;
		g_Block[nCount].bSwitch = false;
		g_Block[nCount].nHitP = HIT_NONE;
		g_Block[nCount].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//���_���W�̐ݒ�
		pVtx[nCount * 4 + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[nCount * 4 + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[nCount * 4 + 2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[nCount * 4 + 3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhw�̐ݒ�(1.0f�Œ�)
		pVtx[nCount * 4 + 0].rhw = 1.0f;
		pVtx[nCount * 4 + 1].rhw = 1.0f;
		pVtx[nCount * 4 + 2].rhw = 1.0f;
		pVtx[nCount * 4 + 3].rhw = 1.0f;

		//���_�J���[�̐ݒ�
		pVtx[nCount * 4 + 0].col = g_Block[nCount].col;
		pVtx[nCount * 4 + 1].col = g_Block[nCount].col;
		pVtx[nCount * 4 + 2].col = g_Block[nCount].col;
		pVtx[nCount * 4 + 3].col = g_Block[nCount].col;

		//�e�N�X�`�����W�̐ݒ�
		pVtx[nCount * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCount * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCount * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCount * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBlock->Unlock();
}

//�I������
void UninitBlock(void)
{
	for (int nCount = 0; nCount < MAX_BLOCK; nCount++)
	{
		//�e�N�X�`���j��
		if (g_pTextureBlock[nCount] != NULL)
		{
			g_pTextureBlock[nCount]->Release();
			g_pTextureBlock[nCount] = NULL;
		}

		//���_�o�b�t�@�j��
		if (g_pVtxBuffBlock != NULL)
		{
			g_pVtxBuffBlock->Release();
			g_pVtxBuffBlock = NULL;
		}
	}
}

//�X�V����
void UpdateBlock(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_BLOCK; nCount++)
	{
		if (g_Block[nCount].bUse == true)
		{
			for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
			{
				//���ꂼ��̏���
				switch (g_Block[nCount].nType)
				{
				case BLOCK_NORMAL:
					break;

				case BLOCK_DOOR:
					Door(nCount);
					break;
				}

				//�v���C���[�̓����蔻��
				ColisionBlock(&pPlayer->pos, &pPlayer->lastpos, &pPlayer->move, &pPlayer->nJump, nCnt);

				//���_���W�̐ݒ�
				pVtx[nCount * 4 + 0].pos = D3DXVECTOR3(g_Block[nCount].pos.x - g_Block[nCount].fWidth / 2.0f, g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f, 0.0f);
				pVtx[nCount * 4 + 1].pos = D3DXVECTOR3(g_Block[nCount].pos.x + g_Block[nCount].fWidth / 2.0f, g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f, 0.0f);
				pVtx[nCount * 4 + 2].pos = D3DXVECTOR3(g_Block[nCount].pos.x - g_Block[nCount].fWidth / 2.0f, g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f, 0.0f);
				pVtx[nCount * 4 + 3].pos = D3DXVECTOR3(g_Block[nCount].pos.x + g_Block[nCount].fWidth / 2.0f, g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f, 0.0f);

				//rhw�̐ݒ�(1.0f�Œ�)
				pVtx[nCount * 4 + 0].rhw = 1.0f;
				pVtx[nCount * 4 + 1].rhw = 1.0f;
				pVtx[nCount * 4 + 2].rhw = 1.0f;
				pVtx[nCount * 4 + 3].rhw = 1.0f;

				//���_�J���[�̐ݒ�
				pVtx[nCount * 4 + 0].col = g_Block[nCount].col;
				pVtx[nCount * 4 + 1].col = g_Block[nCount].col;
				pVtx[nCount * 4 + 2].col = g_Block[nCount].col;
				pVtx[nCount * 4 + 3].col = g_Block[nCount].col;

				//�e�N�X�`�����W�̐ݒ�
				pVtx[nCount * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCount * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[nCount * 4 + 2].tex = D3DXVECTOR2(0.0f, 0.99f);
				pVtx[nCount * 4 + 3].tex = D3DXVECTOR2(1.0f, 0.99f);
			}
		}
		//���_�o�b�t�@���A�����b�N
		g_pVtxBuffBlock->Unlock();
	}
}

//�`�揈��
void DrawBlock(void)
{

	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();								//�f�o�C�X���擾

														//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffBlock,
		0,
		sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);						//���_�t�H�[�}�b�g�̐ݒ�

	for (int nCount = 0; nCount < MAX_BLOCK; nCount++)
	{
		if (g_Block[nCount].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureBlock[nCount]);	//�e�N�X�`���̐ݒ�

			//�|���S���̕`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
				nCount * 4,					//�J�n���钸�_�̃C���f�b�N�X
				2);							//�v���~�e�B�u�̐�
		}
	}
}

//�Z�b�g
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, int nType, float movemax)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X���擾

	VERTEX_2D *pVtx;					//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	bOnceFlag = false;

	for (int nCount = 0; nCount < MAX_BLOCK; nCount++)
	{
		if (g_Block[nCount].bUse == false)
		{
			g_Block[nCount].pos = pos;
			g_Block[nCount].move = move;
			g_Block[nCount].fWidth = fWidth;
			g_Block[nCount].fHeight = fHeight;
			g_Block[nCount].bUse = true;
			g_Block[nCount].nType = nType;
			g_Block[nCount].fMove = 0.0f;
			g_Block[nCount].fMoveMax = movemax;
			g_Block[nCount].UDMoveScan = 0;
			g_Block[nCount].LRMoveScan = 0;
			g_Block[nCount].bSwitch = false;
			g_Block[nCount].nHitP = HIT_NONE;


			switch (g_Block[nCount].nType)
			{
			default:
				g_Block[nCount].col = D3DCOLOR_RGBA(0, 0, 0, 255);
				break;

			case BLOCK_DOOR:
				g_Block[nCount].col = D3DCOLOR_RGBA(190, 255, 166, 255);
				break;

			case BLOCK_TOGE:
				D3DXCreateTextureFromFile(
					pDevice,
					"data\\picture\\toge.png",
					&g_pTextureBlock[nCount]);
				break;

			case BLOCK_BUTTON:
				D3DXCreateTextureFromFile(
					pDevice,
					"data\\picture\\button.png",
					&g_pTextureBlock[nCount]);
			}

			//���_���W�̐ݒ�
			pVtx[nCount * 4 + 0].pos = D3DXVECTOR3(g_Block[nCount].pos.x - g_Block[nCount].fWidth / 2.0f, g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f, 0.0f);
			pVtx[nCount * 4 + 1].pos = D3DXVECTOR3(g_Block[nCount].pos.x + g_Block[nCount].fWidth / 2.0f, g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f, 0.0f);
			pVtx[nCount * 4 + 2].pos = D3DXVECTOR3(g_Block[nCount].pos.x - g_Block[nCount].fWidth / 2.0f, g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f, 0.0f);
			pVtx[nCount * 4 + 3].pos = D3DXVECTOR3(g_Block[nCount].pos.x + g_Block[nCount].fWidth / 2.0f, g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f, 0.0f);

			//rhw�̐ݒ�(1.0f�Œ�)
			pVtx[nCount * 4 + 0].rhw = 1.0f;
			pVtx[nCount * 4 + 1].rhw = 1.0f;
			pVtx[nCount * 4 + 2].rhw = 1.0f;
			pVtx[nCount * 4 + 3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[nCount * 4 + 0].col = g_Block[nCount].col;
			pVtx[nCount * 4 + 1].col = g_Block[nCount].col;
			pVtx[nCount * 4 + 2].col = g_Block[nCount].col;
			pVtx[nCount * 4 + 3].col = g_Block[nCount].col;

			//�e�N�X�`�����W�̐ݒ�
			pVtx[nCount * 4 + 0].tex = D3DXVECTOR2(0.01f, 0.01f);
			pVtx[nCount * 4 + 1].tex = D3DXVECTOR2(0.99f, 0.01f);
			pVtx[nCount * 4 + 2].tex = D3DXVECTOR2(0.01f, 0.99f);
			pVtx[nCount * 4 + 3].tex = D3DXVECTOR2(0.99f, 0.99f);

			break;
		}
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffBlock->Unlock();
}

BLOCK *GetBlock(void)
{
	return &g_Block[0];
}

void ColisionBlock(D3DXVECTOR3 *pos, D3DXVECTOR3 *lastpos, D3DXVECTOR3 *move, int *nJump, int nCnt)
{
	STAGE *pStage;
	pStage = GetStage();

	DATA *pData;
	pData = Getdata();

	for (int nCount = 0; nCount < MAX_BLOCK; nCount++)
	{
		if (g_Block[nCount].bUse == true)
		{
			switch (g_Block[nCount].nType)
			{
			case BLOCK_UD_MOVE:
				(*pos).y -= g_Block[nCount].move.y;
				break;
			}

			if (g_Block[nCount].pos.x - g_Block[nCount].fWidth / 2.0f <= (*pos).x + PLAYER_X &&								//�u���b�N�̍��[
				g_Block[nCount].pos.x + g_Block[nCount].fWidth / 2.0f >= (*pos).x - PLAYER_X &&								//�u���b�N�̉E�[
				g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f <= (*pos).y &&										//�u���b�N�ɏ��
				g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f >= (*pos).y - PLAYER_Y)								//�u���b�N�������炽����
			{
				if (g_Block[nCount].pos.x - (g_Block[nCount].fWidth / 2.0f) >= (*lastpos).x + PLAYER_X &&
					(g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f <= (*pos).y - 1.0f ||
						g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f >= (*pos).y - PLAYER_Y + 1.0f))		//�u���b�N�̍��[
				{
					if (g_Block[nCount].pos.x - (g_Block[nCount].fWidth / 2.0f) >= (*lastpos).x + PLAYER_X &&
						(g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f <= (*pos).y - 1.0f &&
							g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f >= (*pos).y - PLAYER_Y + 1.0f))		//�u���b�N�̍��[
					{
						(*pos).x = (g_Block[nCount].pos.x - g_Block[nCount].fWidth / 2.0f) - PLAYER_X - 1.0f;

						g_Block[nCount].nHitP = HIT_LEFT;
					}

					if (g_Block[nCount].nType == BLOCK_BUTTON && bOnceFlag == false)
					{
						for (int n = 0; n < MAX_BLOCK; n++)
						{
							bOnceFlag = true;
							g_Block[n].bSwitch = true;
							g_Block[nCount].col = D3DCOLOR_RGBA(255, 0, 0, 255);
						}
					}

					continue;
				}

				else if (g_Block[nCount].pos.x + (g_Block[nCount].fWidth / 2.0f) <= (*lastpos).x - PLAYER_X &&
						(g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f <= (*pos).y - 1.0f ||
						g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f >= (*pos).y - PLAYER_Y + 1.0f))	//�u���b�N�̉E�[
				{
					if (g_Block[nCount].pos.x + (g_Block[nCount].fWidth / 2.0f) <= (*lastpos).x - PLAYER_X &&
						(g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f <= (*pos).y - 1.0f &&
							g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f >= (*pos).y - PLAYER_Y + 1.0f))	//�u���b�N�̉E�[
					{
						(*pos).x = (g_Block[nCount].pos.x + g_Block[nCount].fWidth / 2.0f) + PLAYER_X + 1.0f;

						g_Block[nCount].nHitP = HIT_RIGHT;
					}

					if (g_Block[nCount].nType == BLOCK_BUTTON && bOnceFlag == false)
					{
						for (int n = 0; n < MAX_BLOCK; n++)
						{
							bOnceFlag = true;
							g_Block[n].bSwitch = true;
							g_Block[nCount].col = D3DCOLOR_RGBA(255, 0, 0, 255);
						}
					}

					continue;
				}

				if (g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f >= (*lastpos).y &&
						(g_Block[nCount].pos.x - g_Block[nCount].fWidth / 2.0f <= (*pos).x + PLAYER_X - 1.0f ||
						g_Block[nCount].pos.x + g_Block[nCount].fWidth / 2.0f >= (*pos).x - PLAYER_X + 1.0f))	//�u���b�N�ɏ��
				{
					(*pos).y = g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f;
					(*move).y = 0;

					g_Block[nCount].nHitP = HIT_DOWN;
					(*nJump) = 0;

					if (g_Block[nCount].nType == BLOCK_TOGE && pStage->bLock == false)
					{
						pStage->bLock = true;
						pData->nMiss++;
						SetFade(MODE_GAME);
					}

					if (g_Block[nCount].nType == BLOCK_BUTTON && bOnceFlag == false)
					{
						for (int n = 0; n < MAX_BLOCK; n++)
						{
							bOnceFlag = true;
							g_Block[n].bSwitch = true;
							g_Block[nCount].col = D3DCOLOR_RGBA(255, 0, 0, 255);
						}
					}

					continue;
				}

				if (g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f <= (*lastpos).y - PLAYER_Y &&
						(g_Block[nCount].pos.x - g_Block[nCount].fWidth / 2.0f <= (*pos).x + PLAYER_X + 1.0f ||
						g_Block[nCount].pos.x + g_Block[nCount].fWidth / 2.0f >= (*pos).x - PLAYER_X - 1.0f))	//�u���b�N�������炽����
				{
					(*pos).y = (g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f) + PLAYER_Y;
					(*move).y = 0.0f;

					g_Block[nCount].nHitP = HIT_UP;
					continue;
				}

				if (g_Block[nCount].nType == BLOCK_BUTTON && bOnceFlag == false)
				{
					for (int n = 0; n < MAX_BLOCK; n++)
					{
						bOnceFlag = true;
						g_Block[n].bSwitch = true;
						g_Block[nCount].col = D3DCOLOR_RGBA(255, 0, 0, 255);
					}
				}

				(*lastpos) = (*pos);
			}
		}
	}
}

void UDMove(int nBlockCount, D3DXVECTOR3 *pos, D3DXVECTOR3 *lastpos, D3DXVECTOR3 *move, int *nJump, int nCnt)
{
	switch (g_Block[nBlockCount].UDMoveScan)
	{
	case 0:
		g_Block[nBlockCount].pos.y -= g_Block[nBlockCount].move.y;
		g_Block[nBlockCount].fMove += fabsf(g_Block[nBlockCount].move.y);

		if (g_Block[nBlockCount].nHitP == HIT_DOWN)
		{
			(*pos).y -= g_Block[nBlockCount].move.y;
			(*lastpos).y -= g_Block[nBlockCount].move.y;
		}

		if (g_Block[nBlockCount].fMove >= g_Block[nBlockCount].fMoveMax)
		{
			g_Block[nBlockCount].fMove = 0.0f;
			g_Block[nBlockCount].UDMoveScan++;
		}
		break;

	case 1:
		g_Block[nBlockCount].fMove++;

		if (g_Block[nBlockCount].fMove >= 32.0f)
		{
			g_Block[nBlockCount].move.y *= -1;
			g_Block[nBlockCount].fMove = 0.0f;
			g_Block[nBlockCount].UDMoveScan++;
		}
		break;

	case 2:
		g_Block[nBlockCount].pos.y -= g_Block[nBlockCount].move.y;
		g_Block[nBlockCount].fMove += fabsf(g_Block[nBlockCount].move.y);

		if (g_Block[nBlockCount].nHitP == HIT_DOWN)
		{
			(*pos).y -= g_Block[nBlockCount].move.y;
			(*lastpos).y -= g_Block[nBlockCount].move.y;
		}

		if (g_Block[nBlockCount].fMove >= g_Block[nBlockCount].fMoveMax)
		{
			g_Block[nBlockCount].fMove = 0.0f;
			g_Block[nBlockCount].UDMoveScan++;
		}
		break;

	case 3:
		g_Block[nBlockCount].fMove++;
		if (g_Block[nBlockCount].fMove >= 32.0f)
		{
			g_Block[nBlockCount].move.y *= -1;
			g_Block[nBlockCount].fMove = 0.0f;
			g_Block[nBlockCount].UDMoveScan = 0;
		}
		break;
	}
}

void LRMove(int nBlockCount, D3DXVECTOR3 *pos, D3DXVECTOR3 *lastpos, D3DXVECTOR3 *move, int *nJump, int nCnt)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	switch (g_Block[nBlockCount].LRMoveScan)
	{
	case 0:
		g_Block[nBlockCount].pos.x -= g_Block[nBlockCount].move.x;
		g_Block[nBlockCount].fMove += fabsf(g_Block[nBlockCount].move.x);

		if (g_Block[nBlockCount].nHitP == HIT_DOWN)
		{
			pPlayer->pos.x -= g_Block[nBlockCount].move.x;
		}

		if (g_Block[nBlockCount].fMove >= g_Block[nBlockCount].fMoveMax)
		{
			g_Block[nBlockCount].fMove = 0.0f;
			g_Block[nBlockCount].LRMoveScan++;

		}
		break;

	case 1:
		g_Block[nBlockCount].fMove++;

		if (g_Block[nBlockCount].fMove >= 32.0f)
		{
			g_Block[nBlockCount].move.x *= -1;
			g_Block[nBlockCount].fMove = 0.0f;
			g_Block[nBlockCount].LRMoveScan++;
		}

		break;

	case 2:
		g_Block[nBlockCount].pos.x -= g_Block[nBlockCount].move.x;
		g_Block[nBlockCount].fMove += fabsf(g_Block[nBlockCount].move.x);

		if (g_Block[nBlockCount].nHitP == HIT_DOWN)
		{
			pPlayer->pos.x -= g_Block[nBlockCount].move.x;
		}

		if (g_Block[nBlockCount].fMove >= g_Block[nBlockCount].fMoveMax)
		{
			g_Block[nBlockCount].fMove = 0.0f;
			g_Block[nBlockCount].LRMoveScan++;
		}
		break;

	case 3:
		g_Block[nBlockCount].fMove++;

		if (g_Block[nBlockCount].fMove >= 32.0f)
		{
			g_Block[nBlockCount].move.x *= -1;
			g_Block[nBlockCount].fMove = 0.0f;
			g_Block[nBlockCount].LRMoveScan = 0;
		}
		break;
	}
}

void Door(int nBlockCount)
{
	if (g_Block[nBlockCount].bSwitch == true)
	{
		if (bOnceDoor == false)
		{
			PlaySound(SOUND_LABEL_SEDOOR);
			bOnceDoor = !bOnceDoor;
		}
		g_Block[nBlockCount].pos.x += g_Block[nBlockCount].move.x;
		abs(g_Block[nBlockCount].LRMoveScan++);

		if (g_Block[nBlockCount].LRMoveScan >= g_Block[nBlockCount].fMoveMax)
		{
			g_Block[nBlockCount].LRMoveScan = 0;
			g_Block[nBlockCount].move.x *= -1;
			g_Block[nBlockCount].bSwitch = false;
		}
	}
}