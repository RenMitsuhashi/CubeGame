#include "button.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//�O���[�o���ϐ�
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffButton = NULL;		//���_�o�b�t�@�ւ̃|�C���^
LPDIRECT3DTEXTURE9 g_pTextureButton = NULL;			//�e�N�X�`���ւ̃|�C���^
bool IsSwitchButton = false;
bool bBtnAlphaScan = false;
int nBtnAlpha = 255;

//����������
void InitButton(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();				//�f�o�C�X���擾

	//�e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\picture\\press_enter.png",
		&g_pTextureButton);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffButton,
		NULL);

	VERTEX_2D *pVtx;	//���_���ւ̃|�C���^

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffButton->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[1].tex = D3DXVECTOR2(1.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 1.0);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffButton->Unlock();
	IsSwitchButton = false;
	bBtnAlphaScan = false;
	nBtnAlpha = 255;
}

//�I������
void UninitButton(void)
{
	//�e�N�X�`���j��
	if (g_pTextureButton != NULL)
	{
		g_pTextureButton->Release();
		g_pTextureButton = NULL;
	}

	//���_�o�b�t�@�j��
	if (g_pVtxBuffButton != NULL)
	{
		g_pVtxBuffButton->Release();
		g_pVtxBuffButton = NULL;
	}
}

//�X�V����
void UpdateButton(void)
{
	VERTEX_2D *pVtx;

	if (IsSwitchButton == false)
	{
		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			SetFade(MODE_GAME);
			IsSwitchButton = true;
			bBtnAlphaScan = true;

			StopSound();
			PlaySound(SOUND_LABEL_SETITLE);
		}
	}

	if (IsSwitchButton == true)
	{
		switch (bBtnAlphaScan)
		{
		case true:
			bBtnAlphaScan = false;
			nBtnAlpha = 0;
			break;

		case false:
			bBtnAlphaScan = true;
			nBtnAlpha = 255;
			break;
		}
	}

	//���_�o�b�t�@�����b�N���A���_�f�[�^�ւ̃|�C���^���擾
	g_pVtxBuffButton->Lock(0, 0, (void**)&pVtx, 0);

	//���_�J���[�̐ݒ�
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, nBtnAlpha);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, nBtnAlpha);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, nBtnAlpha);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, nBtnAlpha);

	//���_�o�b�t�@���A�����b�N
	g_pVtxBuffButton->Unlock();
}

//�`�揈��
void DrawButton(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//�f�o�C�X�̃|�C���^
	pDevice = GetDevice();								//�f�o�C�X���擾

	//���_�o�b�t�@���f�o�C�X�̃f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffButton,
		0,
		sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);						//���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetTexture(0, g_pTextureButton);			//�e�N�X�`���̐ݒ�

	//�|���S���̕`��
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//�v���~�e�B�u�̎��
		0,							//�J�n���钸�_�̃C���f�b�N�X
		2);							//�v���~�e�B�u�̐�
}