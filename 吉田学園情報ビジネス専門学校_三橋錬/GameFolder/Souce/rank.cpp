#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "rank.h"
#include "Score.h"
#include "databox.h"
#include "input.h"
#include "fade.h"

//�O���[�o���ϐ�
int g_nRank[MAX_RANK];								//�X�R�A
int g_nNowScore;									//�N���A���̃X�R�A
D3DXVECTOR3 nposrank;								//�X�R�A�̕\���ʒu
RANK g_nDrawRank[MAX_RANK][MAX_SCOREPORYGON];		//�X�R�A�̕`��p
bool bLokku;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureRank = NULL;							//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffRank = NULL;						//���_�o�b�t�@�ւ̃|�C���^

																	//����������
void InitRank(void)
{
	bLokku = false;

	DATA *pData;
	pData = Getdata();

	LPDIRECT3DDEVICE9 pDevice;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	for (int nCntrank = 0; nCntrank < MAX_RANK - 1; nCntrank++)
	{
		for (int nCntrank2 = 0; nCntrank2 < MAX_SCOREPORYGON; nCntrank2++)
		{
			g_nDrawRank[nCntrank][nCntrank2].pos = (D3DXVECTOR3(((float)(60 * nCntrank2) + 400.0f), (float)((100 * nCntrank) + 240.0f), 0.0f));
			g_nDrawRank[nCntrank][nCntrank2].nNum = 0;
			g_nDrawRank[nCntrank][nCntrank2].col = 255;
		}
		g_nRank[nCntrank] = 0;
	}

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\picture\\time.png",
		&g_pTextureRank);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCOREPORYGON * MAX_RANK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffRank,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntrank = 0; nCntrank < MAX_RANK - 1; nCntrank++)
	{
		for (int nCntrank2 = 0; nCntrank2 < MAX_SCOREPORYGON; nCntrank2++, pVtx += 4)
		{
			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_nDrawRank[nCntrank][nCntrank2].pos.x - RANK_X, g_nDrawRank[nCntrank][nCntrank2].pos.y - RANK_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_nDrawRank[nCntrank][nCntrank2].pos.x + RANK_X, g_nDrawRank[nCntrank][nCntrank2].pos.y - RANK_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_nDrawRank[nCntrank][nCntrank2].pos.x - RANK_X, g_nDrawRank[nCntrank][nCntrank2].pos.y + RANK_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_nDrawRank[nCntrank][nCntrank2].pos.x + RANK_X, g_nDrawRank[nCntrank][nCntrank2].pos.y + RANK_Y, 0.0f);

			//rhw�̐ݒ�(1.0f�Œ�)
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffRank->Unlock();

	FILE *pFile;
	pFile = fopen("data\\rank.txt", "r");

	if (pFile != NULL)
	{
		for (int nCntrank = 0; nCntrank < MAX_RANK - 1; nCntrank++)
		{
			fscanf(pFile, "%d", &g_nRank[nCntrank]);
		}
		fclose(pFile);
	}
	g_nRank[MAX_RANK - 1] = pData->nScore;
}

//�I������
void UninitRank(void)
{
	//�e�N�X�`���j��
	if (g_pTextureRank != NULL)
	{
		g_pTextureRank->Release();
		g_pTextureRank = NULL;
	}

	//���_�o�b�t�@�j��
	if (g_pVtxBuffRank != NULL)
	{
		g_pVtxBuffRank->Release();
		g_pVtxBuffRank = NULL;
	}
}

//�X�V����
void UpdateRank(void)
{
	if (GetKeyboardTrigger(DIK_RETURN) == true && bLokku == false)
	{
		bLokku = true;
		SetFade(MODE_TITLE);
	}

	DATA *pData;
	pData = Getdata();

	VERTEX_2D *pVtx;

	int nSubData = 0;

	for (int nRank1 = 0; nRank1 < MAX_RANK - 2; nRank1++)
	{
		if (g_nRank[nRank1] == g_nRank[MAX_RANK - 1])
		{
			g_nRank[MAX_RANK - 1] = 0;
			break;
		}
	}

	for (int nRank1 = MAX_RANK - 1; nRank1 >= 0; nRank1--)
	{
		for (int nRank2 = nRank1 - 1; nRank2 >= 0; nRank2--)
		{
			if (g_nRank[nRank1] > g_nRank[nRank2])
			{
				nSubData = g_nRank[nRank1];
				g_nRank[nRank1] = g_nRank[nRank2];
				g_nRank[nRank2] = nSubData;
			}
		}
	}

	FILE *pFile;
	pFile = fopen("data\\rank.txt", "w");
	
	if (pFile != NULL)
	{
		for (int nCnt = 0; nCnt < MAX_RANK - 1; nCnt++)
		{
			fprintf(pFile, "%d\n", g_nRank[nCnt]);
		}
		fclose(pFile);
	}

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffRank->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntrank = 0; nCntrank < MAX_RANK - 1; nCntrank++)
	{
		int nRank1 = 1000000;
		int nRank2 = 100000;

		for (int nCntrank2 = 0; nCntrank2 < MAX_SCOREPORYGON; nCntrank2++)
		{
			g_nDrawRank[nCntrank][nCntrank2].nNum = g_nRank[nCntrank] % nRank1 / nRank2;

			nRank1 *= 0.1f;
			nRank2 *= 0.1f;
		}
	}

	for (int nCntrank = 0; nCntrank < MAX_RANK - 1; nCntrank++)
	{
		for (int nCntrank2 = 0; nCntrank2 < MAX_SCOREPORYGON; nCntrank2++, pVtx += 4)
		{
			if (g_nRank[nCntrank] == pData->nScore)
			{
				g_nDrawRank[nCntrank][nCntrank2].col = g_nDrawRank[nCntrank][nCntrank2].col ? 0 : 255;
			}

			else g_nDrawRank[nCntrank][nCntrank2].col = 255;

			//���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_nDrawRank[nCntrank][nCntrank2].pos.x - RANK_X, g_nDrawRank[nCntrank][nCntrank2].pos.y - RANK_Y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_nDrawRank[nCntrank][nCntrank2].pos.x + RANK_X, g_nDrawRank[nCntrank][nCntrank2].pos.y - RANK_Y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_nDrawRank[nCntrank][nCntrank2].pos.x - RANK_X, g_nDrawRank[nCntrank][nCntrank2].pos.y + RANK_Y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_nDrawRank[nCntrank][nCntrank2].pos.x + RANK_X, g_nDrawRank[nCntrank][nCntrank2].pos.y + RANK_Y, 0.0f);

			//rhw�̐ݒ�(1.0f�Œ�)
			pVtx[0].rhw = 1.0f;
			pVtx[1].rhw = 1.0f;
			pVtx[2].rhw = 1.0f;
			pVtx[3].rhw = 1.0f;

			//���_�J���[�̐ݒ�
			pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, g_nDrawRank[nCntrank][nCntrank2].col);
			pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, g_nDrawRank[nCntrank][nCntrank2].col);
			pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, g_nDrawRank[nCntrank][nCntrank2].col);
			pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, g_nDrawRank[nCntrank][nCntrank2].col);

			//�e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(g_nDrawRank[nCntrank][nCntrank2].nNum * 0.1f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(g_nDrawRank[nCntrank][nCntrank2].nNum * 0.1f + 0.1f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(g_nDrawRank[nCntrank][nCntrank2].nNum * 0.1f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(g_nDrawRank[nCntrank][nCntrank2].nNum * 0.1f + 0.1f, 1.0f);
		}
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffRank->Unlock();
}

//�`�揈��
void DrawRank(void)
{
	int n = 0;
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();								//�f�o�C�X���擾

														//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffRank,
		0,
		sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);						//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, g_pTextureRank);			//�e�N�X�`���̐ݒ�

	for (int nCnt = 0; nCnt < MAX_RANK - 1; nCnt++)
	{
		for (int nCntRank = 0; nCntRank < MAX_SCOREPORYGON; nCntRank++)
		{
			//�`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,					//�v���~�e�B�u�̎��
				n * 4,							//�J�n���钸�_�̃C���f�b�N�X
				2);										//�v���~�e�B�u�̐�

			n++;
		}
	}
}