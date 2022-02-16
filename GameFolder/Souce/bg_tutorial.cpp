#include "bg_tutorial.h"
#include "input.h"
#include "game.h"
#include "block.h"
#include "fade.h"
#include "sound.h"
#include "databox.h"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffbgtuto = NULL;		//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTexturebgtuto = NULL;				//テクスチャへのポインタ
float fTexXtuto;
float fTexYtuto;
int nPage;												//頁数
bool bStop;												//エンター止めとく
int nCool;												//エンターのクールタイム
bool bStop8Page;										//8頁に使うよ！！！！！

DATA *pDataa;
BLOCK *g_pBlock;										//ブロックの構造体

//初期化処理
void Initbgtuto(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスのポインタ
	pDevice = GetDevice();				//デバイスを取得

	pDataa = Getdata();
	g_pBlock = GetBlock();

	//テクスチャ読み込み
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

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	fTexXtuto = 0.0f;
	fTexYtuto = 0.0f;
	nPage = 0;
	bStop = true;
	nCool = 0;
	bStop8Page = false;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffbgtuto->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	g_pVtxBuffbgtuto->Unlock();
}

//終了処理
void Uninitbgtuto(void)
{
	//テクスチャ破棄
	if (g_pTexturebgtuto != NULL)
	{
		g_pTexturebgtuto->Release();
		g_pTexturebgtuto = NULL;
	}

	//頂点バッファ破棄
	if (g_pVtxBuffbgtuto != NULL)
	{
		g_pVtxBuffbgtuto->Release();
		g_pVtxBuffbgtuto= NULL;
	}
}

//更新処理
void Updatebgtuto(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	STAGE *pStage;
	pStage = GetStage();

	//--------------------------------------------------------------
	//最初にスライドしてくる
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
	//8頁目になったらDoorのスイッチをtrueにする
	if (nPage == 8 && bStop8Page == false)
	{
		for (int nCount = 0; nCount < MAX_BLOCK; nCount++, g_pBlock++)	g_pBlock->bSwitch = true;
		bStop8Page = true;
	}

	//--------------------------------------------------------------



	//--------------------------------------------------------------
	//Enterを押すと次へ進む
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
	//9頁目にEnterを押すと次のステージへ
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
	//Enterのクールタイム
	if (bStop == true)
	{
		nCool++;
		
		if (nCool >= 32)	bStop = false, nCool = 0;
	}

	//--------------------------------------------------------------



	//--------------------------------------------------------------
	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffbgtuto->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].tex = D3DXVECTOR2(fTexXtuto + 0.0f, fTexYtuto + 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTexXtuto + 0.2f, fTexYtuto + 0.0f);
	pVtx[2].tex = D3DXVECTOR2(fTexXtuto + 0.0f, fTexYtuto + 0.5f);
	pVtx[3].tex = D3DXVECTOR2(fTexXtuto + 0.2f, fTexYtuto + 0.5f);

	//頂点バッファをアンロック
	g_pVtxBuffbgtuto->Unlock();

	//--------------------------------------------------------------
}

//描画処理
void Drawbgtuto(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスのポインタ
	pDevice = GetDevice();								//デバイスを取得

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffbgtuto,
		0,
		sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);						//頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTexturebgtuto);			//テクスチャの設定

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,							//開始する頂点のインデックス
		2);							//プリミティブの数
}

void Setbgtuto(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	fTexXtuto = 0.0f;
	fTexYtuto = 0.0f;
	nPage = 0;
	bStop = true;
	nCool = 0;
	bStop8Page = false;

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffbgtuto->Lock(0, 0, (void**)&pVtx, 0);

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
	pVtx[0].tex = D3DXVECTOR2(fTexXtuto + 0.0f, fTexYtuto + 0.0f);
	pVtx[1].tex = D3DXVECTOR2(fTexXtuto + 0.2f, fTexYtuto + 0.0f);
	pVtx[2].tex = D3DXVECTOR2(fTexXtuto + 0.0f, fTexYtuto + 0.5f);
	pVtx[3].tex = D3DXVECTOR2(fTexXtuto + 0.2f, fTexYtuto + 0.5f);

	//頂点バッファをアンロック
	g_pVtxBuffbgtuto->Unlock();
}