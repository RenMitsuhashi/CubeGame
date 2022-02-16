#include "main.h"
#include "Miss.h"
#include "databox.h"

//�O���[�o���ϐ�
int g_nMiss;							//����
D3DXVECTOR3 nposMiss;					//���Ԃ̕\���ʒu
int g_nDrawMiss[MAX_MISSPORYGON];		//���Ԃ̕`��p

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureMiss = NULL;							//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMiss = NULL;						//���_�o�b�t�@�ւ̃|�C���^

//����������
void InitMiss(void)
{
	DATA *pData;
	pData = Getdata();

	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//������
	nposMiss = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nMiss = pData->nMiss;

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\picture\\time.png",
		&g_pTextureMiss);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_MISSPORYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffMiss,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffMiss->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_MISSPORYGON; nCntScore++, pVtx += 4)
	{
		g_nDrawMiss[nCntScore] = 0;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(600.0f + (nCntScore * 80), 10, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(680.0f + (nCntScore * 80), 10, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(600.0f + (nCntScore * 80), 110, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(680.0f + (nCntScore * 80), 110, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(g_nDrawMiss[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_nDrawMiss[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_nDrawMiss[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_nDrawMiss[nCntScore] * 0.1f + 0.1f, 1.0f);
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffMiss->Unlock();
}

//�I������
void UninitMiss(void)
{
	//�e�N�X�`���j��
	if (g_pTextureMiss != NULL)
	{
		g_pTextureMiss->Release();
		g_pTextureMiss = NULL;
	}

	//���_�o�b�t�@�j��
	if (g_pVtxBuffMiss != NULL)
	{
		g_pVtxBuffMiss->Release();
		g_pVtxBuffMiss = NULL;
	}
}

//�X�V����
void UpdateMiss(void)
{
	int nCount;
	VERTEX_2D *pVtx;

	int nMiss1 = 100;
	int nMiss2 = 10;

	for (nCount = 0; nCount < MAX_MISSPORYGON; nCount++)
	{
		g_nDrawMiss[nCount] = g_nMiss % nMiss1 / nMiss2;

		nMiss1 *= 0.1f;
		nMiss2 *= 0.1f;
	}

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffMiss->Lock(0, 0, (void**)&pVtx, 0);

	for (nCount = 0; nCount < MAX_MISSPORYGON; nCount++, pVtx += 4)
	{
		pVtx[0].tex = D3DXVECTOR2(g_nDrawMiss[nCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_nDrawMiss[nCount] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_nDrawMiss[nCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_nDrawMiss[nCount] * 0.1f + 0.1f, 1.0f);

		pVtx[0].pos = D3DXVECTOR3(nposMiss.x + (nCount * MISS_X), nposMiss.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((nposMiss.x + MISS_X) + (nCount * MISS_X), nposMiss.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(nposMiss.x + (nCount * MISS_X), nposMiss.y + MISS_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((nposMiss.x + MISS_X) + (nCount * MISS_X), nposMiss.y + MISS_Y, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffMiss->Unlock();
}

//�`�揈��
void DrawMiss(void)
{
	int nCntScore;
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();								//�f�o�C�X���擾

														//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffMiss,
		0,
		sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);						//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, g_pTextureMiss);			//�e�N�X�`���̐ݒ�

	for (nCntScore = 0; nCntScore < MAX_MISSPORYGON; nCntScore++)
	{
			//�`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,					//�v���~�e�B�u�̎��
				nCntScore * 4,							//�J�n���钸�_�̃C���f�b�N�X
				2);										//�v���~�e�B�u�̐�
	}
}

void SetMiss(D3DXVECTOR3 pos)
{
	int nCntMiss;

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾

	g_pVtxBuffMiss->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMiss = 0; nCntMiss < MAX_MISSPORYGON; nCntMiss++, pVtx += 4)
	{
		g_nDrawMiss[nCntMiss] = 0;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pos.x + (nCntMiss * MISS_X), pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((pos.x + MISS_X) + (nCntMiss * MISS_X), pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x + (nCntMiss * MISS_X), pos.y + MISS_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((pos.x + MISS_X) + (nCntMiss * MISS_X), pos.y + MISS_Y, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffMiss->Unlock();
	nposMiss = pos;
}

int GetMiss(void)
{
	return g_nMiss;
}