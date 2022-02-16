#include "PauseMenu.h"
#include "input.h"
#include "fade.h"
#include "bg_pause.h"
#include "game.h"
#include "databox.h"
#include "sound.h"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPauseMenu = NULL;		//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTexturePauseMenu[3] = {};			//テクスチャへのポインタ
bool bScanFadeMenu = false;
int nCntPause = 0;

//初期化処理
void InitPauseMenu(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスのポインタ
	pDevice = GetDevice();				//デバイスを取得

		//テクスチャ読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			"data\\picture\\pause00.png",
			&g_pTexturePauseMenu[0]);

		//テクスチャ読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			"data\\picture\\pause01.png",
			&g_pTexturePauseMenu[1]);

		//テクスチャ読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			"data\\picture\\pause02.png",
			&g_pTexturePauseMenu[2]);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPauseMenu,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPauseMenu->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffPauseMenu->Unlock();

	bScanFadeMenu = false;
	nCntPause = 0;
}

//終了処理
void UninitPauseMenu(void)
{
	for (int nCnt = 0; nCnt < 3; nCnt++)
	{
		//テクスチャ破棄
		if (g_pTexturePauseMenu[nCnt] != NULL)
		{
			g_pTexturePauseMenu[nCnt]->Release();
			g_pTexturePauseMenu[nCnt] = NULL;
		}
	}

	//頂点バッファ破棄
	if (g_pVtxBuffPauseMenu != NULL)
	{
		g_pVtxBuffPauseMenu->Release();
		g_pVtxBuffPauseMenu = NULL;
	}
}

//更新処理
void UpdatePauseMenu(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスのポインタ
	pDevice = GetDevice();				//デバイスを取得

	STAGE *pStage;
	pStage = GetStage();

	DATA *pData;
	pData = Getdata();

	if (bScanFadeMenu == false)
	{
		if (GetKeyboardTrigger(DIK_UP) == true)
		{
			nCntPause++;
			PlaySound(SOUND_LABEL_SECURSOR);

			if (nCntPause > 2)
			{
				nCntPause = 0;
			}
		}

		if (GetKeyboardTrigger(DIK_DOWN) == true)
		{
			nCntPause--;
			PlaySound(SOUND_LABEL_SECURSOR);

			if (nCntPause < 0)
			{
				nCntPause = 2;
			}
		}

		if (GetKeyboardTrigger(DIK_RETURN) == true)
		{
			switch (nCntPause)
			{
			case 0:
				PlaySound(SOUND_LABEL_SEENTER);
				pStage->bPause = false;
				break;

			case 1:
				StopSound();
				PlaySound(SOUND_LABEL_SEENTER);
				pData->nMiss++;
				SetFade(MODE_GAME);
				break;

			case 2:
				StopSound();
				PlaySound(SOUND_LABEL_SEENTER);
				SetFade(MODE_TITLE);
				break;
			}
		}
	}
}

//描画処理
void DrawPauseMenu(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスのポインタ
	pDevice = GetDevice();								//デバイスを取得

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffPauseMenu,
		0,
		sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);									//頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTexturePauseMenu[nCntPause]);			//テクスチャの設定

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,							//開始する頂点のインデックス
		2);							//プリミティブの数
}