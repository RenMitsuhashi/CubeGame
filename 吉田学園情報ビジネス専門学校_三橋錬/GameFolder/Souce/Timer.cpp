#include "main.h"
#include "Timer.h"
#include "databox.h"

//�O���[�o���ϐ�
int g_nTime;							//����
int nFlame;								//����(1�t���[������)
D3DXVECTOR3 npostime;					//���Ԃ̕\���ʒu
int g_nDrawTimer[MAX_TIMEPORYGON];		//���Ԃ̕`��p
int nTime1 = 1000;
int nTime2 = 100;
DATA *pDatat;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureTimer = NULL;							//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;						//���_�o�b�t�@�ւ̃|�C���^

//����������
void InitTimer(void)
{
	pDatat = Getdata();

	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//������
	npostime = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nTime = pDatat->nTime;

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\picture\\time.png",
		&g_pTextureTimer);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_TIMEPORYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTimer,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_TIMEPORYGON; nCntScore++, pVtx += 4)
	{
		g_nDrawTimer[nCntScore] = 0;

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
		pVtx[0].tex = D3DXVECTOR2(g_nDrawTimer[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_nDrawTimer[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_nDrawTimer[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_nDrawTimer[nCntScore] * 0.1f + 0.1f, 1.0f);
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTimer->Unlock();

	nTime1 = 1000;
	nTime2 = 100;
}

//�I������
void UninitTimer(void)
{
	//�e�N�X�`���j��
	if (g_pTextureTimer != NULL)
	{
		g_pTextureTimer->Release();
		g_pTextureTimer = NULL;
	}

	//���_�o�b�t�@�j��
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}
}

//�X�V����
void UpdateTimer(void)
{
	int nCount;
	VERTEX_2D *pVtx;

	nTime1 = 1000;
	nTime2 = 100;
	nFlame++;

	if (nFlame >= 60)
	{
		nFlame = 0;
		g_nTime++;
		pDatat->nTime++;

		if (g_nTime >= 999)
		{
			g_nTime = 999;
		}
	}

	for (nCount = 0; nCount < MAX_TIMEPORYGON; nCount++)
	{
		g_nDrawTimer[nCount] = g_nTime % nTime1 / nTime2;

		nTime1 *= 0.1f;
		nTime2 *= 0.1f;
	}

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCount = 0; nCount < MAX_TIMEPORYGON; nCount++, pVtx += 4)
	{
		pVtx[0].tex = D3DXVECTOR2(g_nDrawTimer[nCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_nDrawTimer[nCount] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_nDrawTimer[nCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_nDrawTimer[nCount] * 0.1f + 0.1f, 1.0f);

		pVtx[0].pos = D3DXVECTOR3(npostime.x + (nCount * TIMER_X), npostime.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((npostime.x + TIMER_X) + (nCount * TIMER_X), npostime.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(npostime.x + (nCount * TIMER_X), npostime.y + TIMER_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((npostime.x + TIMER_X) + (nCount * TIMER_X), npostime.y + TIMER_Y, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTimer->Unlock();
}

//�`�揈��
void DrawTimer(void)
{
	int nCntScore;
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();								//�f�o�C�X���擾

														//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffTimer,
		0,
		sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);						//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, g_pTextureTimer);			//�e�N�X�`���̐ݒ�

	for (nCntScore = 0; nCntScore < MAX_TIMEPORYGON; nCntScore++)
	{
			//�`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,					//�v���~�e�B�u�̎��
				nCntScore * 4,							//�J�n���钸�_�̃C���f�b�N�X
				2);										//�v���~�e�B�u�̐�
	}
}

void SetTimer(D3DXVECTOR3 pos)
{
	int nCntTimer;

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾

	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTimer = 0; nCntTimer < MAX_TIMEPORYGON; nCntTimer++, pVtx += 4)
	{
		g_nDrawTimer[nCntTimer] = 0;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pos.x + (nCntTimer * TIMER_X), pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((pos.x + TIMER_X) + (nCntTimer * TIMER_X), pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x + (nCntTimer * TIMER_X), pos.y + TIMER_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((pos.x + TIMER_X) + (nCntTimer * TIMER_X), pos.y + TIMER_Y, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffTimer->Unlock();
	npostime = pos;
}

int GetTimer(void)
{
	return g_nTime;
}