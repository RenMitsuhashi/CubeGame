#include "Score.h"
#include "databox.h"

//�O���[�o���ϐ�
int g_nScore;							//�X�R�A
D3DXVECTOR3 npos;						//�X�R�A�̕\���ʒu
int g_nDrawScore[MAX_SCOREPORYGON];		//�X�R�A�̕`��p
int nScore1 = 1000000;
int nScore2 = 100000;

//�O���[�o���ϐ�
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;							//�e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;						//���_�o�b�t�@�ւ̃|�C���^

//����������
void InitScore(void)
{
	DATA *pData;
	pData = Getdata();

	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	//�f�o�C�X�̎擾
	pDevice = GetDevice();

	//������
	npos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nScore = pData->nScore;

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\picture\\time.png",
		&g_pTextureScore);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_SCOREPORYGON,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffScore,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_SCOREPORYGON; nCntScore++, pVtx += 4)
	{
		g_nDrawScore[nCntScore] = 0;

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
		pVtx[0].tex = D3DXVECTOR2(g_nDrawScore[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_nDrawScore[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_nDrawScore[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_nDrawScore[nCntScore] * 0.1f + 0.1f, 1.0f);
	}
	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffScore->Unlock();

	nScore1 = 1000000;
	nScore2 = 100000;
}

//�I������
void UninitScore(void)
{
	//�e�N�X�`���j��
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//���_�o�b�t�@�j��
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//�X�V����
void UpdateScore(void)
{
	int nCount;
	VERTEX_2D *pVtx;

	nScore1 = 1000000;
	nScore2 = 100000;

	for (nCount = 0; nCount < MAX_SCOREPORYGON; nCount++)
	{
		g_nDrawScore[nCount] = g_nScore % nScore1 / nScore2;

		nScore1 *= 0.1f;
		nScore2 *= 0.1f;
	}

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCount = 0; nCount < MAX_SCOREPORYGON; nCount++, pVtx += 4)
	{
		pVtx[0].tex = D3DXVECTOR2(g_nDrawScore[nCount] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_nDrawScore[nCount] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_nDrawScore[nCount] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_nDrawScore[nCount] * 0.1f + 0.1f, 1.0f);

		pVtx[0].pos = D3DXVECTOR3(npos.x + (nCount * 60), npos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((npos.x + 60) + (nCount * 60), npos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(npos.x + (nCount * 60), npos.y + 100, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((npos.x + 60) + (nCount * 60), npos.y + 100, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffScore->Unlock();
}

//�`�揈��
void DrawScore(void)
{
	int nCntScore;
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();								//�f�o�C�X���擾

														//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffScore,
		0,
		sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);						//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, g_pTextureScore);			//�e�N�X�`���̐ݒ�

	for (nCntScore = 0; nCntScore < MAX_SCOREPORYGON; nCntScore++)
	{
			//�`��
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,					//�v���~�e�B�u�̎��
				nCntScore * 4,							//�J�n���钸�_�̃C���f�b�N�X
				2);										//�v���~�e�B�u�̐�
	}
}

//�X�R�A�ǉ�
void AddScore(int nValue)
{
	g_nScore += nValue;
}

void SetScore(D3DXVECTOR3 pos)
{
	DATA *pData;
	pData = Getdata();

	int nCntScore;

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

						//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾

	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_SCOREPORYGON; nCntScore++, pVtx += 4)
	{
		g_nDrawScore[nCntScore] = 0;

		//���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(pos.x + (nCntScore * 80), pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((pos.x + 60) +(nCntScore * 80), pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x + (nCntScore * 80), pos.y + 100, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((pos.x + 60) + (nCntScore * 80), pos.y + 100, 0.0f);
	}

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffScore->Unlock();

	g_nScore = pData->nScore;
	npos = pos;
}