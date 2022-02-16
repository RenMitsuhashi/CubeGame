#include "main.h"
#include "Timer.h"
#include "databox.h"

//グローバル変数
int g_nTime;							//時間
int nFlame;								//時間(1フレームごと)
D3DXVECTOR3 npostime;					//時間の表示位置
int g_nDrawTimer[MAX_TIMEPORYGON];		//時間の描画用
int nTime1 = 1000;
int nTime2 = 100;
DATA *pDatat;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureTimer = NULL;							//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTimer = NULL;						//頂点バッファへのポインタ

//初期化処理
void InitTimer(void)
{
	pDatat = Getdata();

	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	//初期化
	npostime = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nTime = pDatat->nTime;

	//テクスチャ読み込み
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

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_TIMEPORYGON; nCntScore++, pVtx += 4)
	{
		g_nDrawTimer[nCntScore] = 0;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(600.0f + (nCntScore * 80), 10, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(680.0f + (nCntScore * 80), 10, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(600.0f + (nCntScore * 80), 110, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(680.0f + (nCntScore * 80), 110, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(g_nDrawTimer[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_nDrawTimer[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_nDrawTimer[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_nDrawTimer[nCntScore] * 0.1f + 0.1f, 1.0f);
	}
	//頂点バッファをアンロック
	g_pVtxBuffTimer->Unlock();

	nTime1 = 1000;
	nTime2 = 100;
}

//終了処理
void UninitTimer(void)
{
	//テクスチャ破棄
	if (g_pTextureTimer != NULL)
	{
		g_pTextureTimer->Release();
		g_pTextureTimer = NULL;
	}

	//頂点バッファ破棄
	if (g_pVtxBuffTimer != NULL)
	{
		g_pVtxBuffTimer->Release();
		g_pVtxBuffTimer = NULL;
	}
}

//更新処理
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

	//頂点バッファをロックし、頂点データへのポインタを取得
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

	//頂点バッファをアンロック
	g_pVtxBuffTimer->Unlock();
}

//描画処理
void DrawTimer(void)
{
	int nCntScore;
	LPDIRECT3DDEVICE9 pDevice;							//デバイスのポインタ
	pDevice = GetDevice();								//デバイスを取得

														//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffTimer,
		0,
		sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);						//頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureTimer);			//テクスチャの設定

	for (nCntScore = 0; nCntScore < MAX_TIMEPORYGON; nCntScore++)
	{
			//描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,					//プリミティブの種類
				nCntScore * 4,							//開始する頂点のインデックス
				2);										//プリミティブの数
	}
}

void SetTimer(D3DXVECTOR3 pos)
{
	int nCntTimer;

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点データへのポインタを取得

	g_pVtxBuffTimer->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntTimer = 0; nCntTimer < MAX_TIMEPORYGON; nCntTimer++, pVtx += 4)
	{
		g_nDrawTimer[nCntTimer] = 0;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pos.x + (nCntTimer * TIMER_X), pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((pos.x + TIMER_X) + (nCntTimer * TIMER_X), pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x + (nCntTimer * TIMER_X), pos.y + TIMER_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((pos.x + TIMER_X) + (nCntTimer * TIMER_X), pos.y + TIMER_Y, 0.0f);
	}

	//頂点バッファをアンロック
	g_pVtxBuffTimer->Unlock();
	npostime = pos;
}

int GetTimer(void)
{
	return g_nTime;
}