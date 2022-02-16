#include "button.h"
#include "input.h"
#include "sound.h"
#include "fade.h"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffButton = NULL;		//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureButton = NULL;			//テクスチャへのポインタ
bool IsSwitchButton = false;
bool bBtnAlphaScan = false;
int nBtnAlpha = 255;

//初期化処理
void InitButton(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスのポインタ
	pDevice = GetDevice();				//デバイスを取得

	//テクスチャ読み込み
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

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffButton->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0, 0.0, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	//rhwの設定(1.0f固定)
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャ座標の設定
	pVtx[0].tex = D3DXVECTOR2(0.0, 0.0);
	pVtx[1].tex = D3DXVECTOR2(1.0, 0.0);
	pVtx[2].tex = D3DXVECTOR2(0.0, 1.0);
	pVtx[3].tex = D3DXVECTOR2(1.0, 1.0);

	//頂点バッファをアンロック
	g_pVtxBuffButton->Unlock();
	IsSwitchButton = false;
	bBtnAlphaScan = false;
	nBtnAlpha = 255;
}

//終了処理
void UninitButton(void)
{
	//テクスチャ破棄
	if (g_pTextureButton != NULL)
	{
		g_pTextureButton->Release();
		g_pTextureButton = NULL;
	}

	//頂点バッファ破棄
	if (g_pVtxBuffButton != NULL)
	{
		g_pVtxBuffButton->Release();
		g_pVtxBuffButton = NULL;
	}
}

//更新処理
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

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffButton->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, nBtnAlpha);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, nBtnAlpha);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, nBtnAlpha);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, nBtnAlpha);

	//頂点バッファをアンロック
	g_pVtxBuffButton->Unlock();
}

//描画処理
void DrawButton(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスのポインタ
	pDevice = GetDevice();								//デバイスを取得

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffButton,
		0,
		sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);						//頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureButton);			//テクスチャの設定

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,							//開始する頂点のインデックス
		2);							//プリミティブの数
}