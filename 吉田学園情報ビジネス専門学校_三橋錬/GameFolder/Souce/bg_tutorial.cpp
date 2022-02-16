#include "bg_tutorial.h"
#include "input.h"
#include "game.h"
#include "block.h"
#include "fade.h"
#include "sound.h"
#include "databox.h"

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffbgtuto = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTexturebgtuto = NULL;				//�e�N�X�`���ւ̃|�C���^
float fTexXtuto;
float fTexYtuto;
int nPage;												//�Ő�
bool bStop;												//�G���^�[�~�߂Ƃ�
int nCool;												//�G���^�[�̃N�[���^�C��
bool bStop8Page;										//8�łɎg����I�I�I�I�I

DATA *pDataa;
BLOCK *g_pBlock;										//�u���b�N�̍\����

//����������
void Initbgtuto(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X���擾

	pDataa = Getdata();
	g_pBlock = GetBlock();

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\picture\\bgtuto.png",
		&g_pTexturebgtuto);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffbgtuto,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	fTexXtuto = 0.0f;
	fTexYtuto = 0.0f;
	nPage = 0;
	bStop = true;
	nCool = 0;
	bStop8Page = false;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffbgtuto->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0, 0.0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffbgtuto->Unlock();
}

//�I������
void Uninitbgtuto(void)
{
	//�e�N�X�`���j��
	if (g_pTexturebgtuto != NULL)
	{
		g_pTexturebgtuto->Release();
		g_pTexturebgtuto = NULL;
	}

	//���_�o�b�t�@�j��
	if (g_pVtxBuffbgtuto != NULL)
	{
		g_pVtxBuffbgtuto->Release();
		g_pVtxBuffbgtuto= NULL;
	}
}

//�X�V����
void Updatebgtuto(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	STAGE *pStage;
	pStage = GetStage();

	//--------------------------------------------------------------
	//�ŏ��ɃX���C�h���Ă���
	if (nPage == 0)
	{
		fTexXtuto += 0.002f;
		
		if (fTexXtuto >= 0.2f)
		{
			fTexXtuto = 0.2f;
			nPage++;
		}
	}

	//--------------------------------------------------------------



	//--------------------------------------------------------------
	//8�ŖڂɂȂ�����Door�̃X�C�b�`��true�ɂ���
	if (nPage == 8 && bStop8Page == false)
	{
		for (int nCount = 0; nCount < MAX_BLOCK; nCount++, g_pBlock++)	g_pBlock->bSwitch = true;
		bStop8Page = true;
	}

	//--------------------------------------------------------------



	//--------------------------------------------------------------
	//Enter�������Ǝ��֐i��
	if ((GetKeyboardTrigger(DIK_RETURN) == true &&
		bStop == false &&
		nPage != 0 &&
		nPage != 8 &&
		nPage != 9) ||
		(nPage == 8 && pStage->bClear == true))
	{
		fTexXtuto += 0.2f;
		nPage++;

		if (nPage == 5)	fTexYtuto += 0.5f;

		PlaySound(SOUND_LABEL_SEENTER);

		bStop = true;
	}

	//--------------------------------------------------------------



	//--------------------------------------------------------------
	//9�Ŗڂ�Enter�������Ǝ��̃X�e�[�W��
	if (GetKeyboardTrigger(DIK_RETURN) == true && 
		nPage == 9 &&
		bStop8Page == true)
	{
		SetFade(MODE_GAME);
		pDataa->nNowStage++;
		bStop8Page = false;
	}

	//--------------------------------------------------------------



	//--------------------------------------------------------------
	//Enter�̃N�[���^�C��
	if (bStop == true)
	{
		nCool++;
		
		if (nCool >= 32)	bStop = false, nCool = 0;
	}

	//--------------------------------------------------------------



	//--------------------------------------------------------------
	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffbgtuto->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0, 0.0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
	pVtx[0].tex = D3DXVECTOR2(fTexXtuto + 0.0f, fTexYtuto + 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTexXtuto + 0.2f, fTexYtuto + 0.0f);
	pVtx[2].tex = D3DXVECTOR2(fTexXtuto + 0.0f, fTexYtuto + 0.5f);
	pVtx[3].tex = D3DXVECTOR2(fTexXtuto + 0.2f, fTexYtuto + 0.5f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffbgtuto->Unlock();

	//--------------------------------------------------------------
}

//�`�揈��
void Drawbgtuto(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();								//�f�o�C�X���擾

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffbgtuto,
		0,
		sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);						//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, g_pTexturebgtuto);			//�e�N�X�`���̐ݒ�

	//�|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,							//�J�n���钸�_�̃C���f�b�N�X
		2);							//�v���~�e�B�u�̐�
}

void Setbgtuto(void)
{
	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	fTexXtuto = 0.0f;
	fTexYtuto = 0.0f;
	nPage = 0;
	bStop = true;
	nCool = 0;
	bStop8Page = false;

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffbgtuto->Lock(0, 0, (void**)&pVtx, 0);

	//���_���W�̐ݒ�
	pVtx[0].pos = D3DXVECTOR3(0.0, 0.0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

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
	pVtx[0].tex = D3DXVECTOR2(fTexXtuto + 0.0f, fTexYtuto + 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTexXtuto + 0.2f, fTexYtuto + 0.0f);
	pVtx[2].tex = D3DXVECTOR2(fTexXtuto + 0.0f, fTexYtuto + 0.5f);
	pVtx[3].tex = D3DXVECTOR2(fTexXtuto + 0.2f, fTexYtuto + 0.5f);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffbgtuto->Unlock();
}