#include "result.h"
#include "fade.h"
#include "Timer.h"
#include "Score.h"
#include "Miss.h"
#include "input.h"
#include "databox.h"
#include "game.h"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffResult = NULL;	//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureResult = NULL;			//テクスチャへのポインタ
bool bSwitch = false;
bool bOneFlame;

//初期化処理
void InitResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスのポインタ
	pDevice = GetDevice();				//デバイスを取得

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\picture\\result.png",
		&g_pTextureResult);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffResult,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffResult->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffResult->Unlock();

	bSwitch = false;
	bOneFlame = false;

	InitTimer();
	InitScore();
	InitMiss();

	SetTimer(D3DXVECTOR3(SCREEN_WIDTH / 2, 260.0f, 0.0f));
	SetMiss (D3DXVECTOR3(SCREEN_WIDTH / 2, 390.0f, 0.0f));
	SetScore(D3DXVECTOR3(SCREEN_WIDTH / 2, 520.0f, 0.0f));
}

//終了処理
void UninitResult(void)
{
	//テクスチャ破棄
	if (g_pTextureResult != NULL)
	{
		g_pTextureResult->Release();
		g_pTextureResult = NULL;
	}

	//頂点バッファ破棄
	if (g_pVtxBuffResult != NULL)
	{
		g_pVtxBuffResult->Release();
		g_pVtxBuffResult = NULL;
	}
	UninitTimer();
	UninitScore();
	UninitMiss();
}

//更新処理
void UpdateResult(void)
{
	DATA *pData;
	pData = Getdata();

	if (bOneFlame == false)
	{
		UpdateTimer();
		UpdateScore();
		UpdateMiss();

		bOneFlame = true;
	}

	if (GetKeyboardTrigger(DIK_RETURN) == true && bSwitch == false)
	{
		if (pData->nNowStage < STAGE_MAX)
		{
			pData->nTime = 0;
			pData->nMiss = 0;
			bSwitch = true;
			SetFade(MODE_GAME);
		}

		else
		{
			SetFade(MODE_RANK);
		}
	}
}

//描画処理
void DrawResult(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスのポインタ
	pDevice = GetDevice();								//デバイスを取得

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffResult,
		0,
		sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);						//頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureResult);			//テクスチャの設定

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,							//開始する頂点のインデックス
		2);							//プリミティブの数

	DrawTimer();
	DrawScore();
	DrawMiss();
}