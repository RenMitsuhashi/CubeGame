#include "main.h"
#include "Miss.h"
#include "databox.h"

//グローバル変数
int g_nMiss;							//時間
D3DXVECTOR3 nposMiss;					//時間の表示位置
int g_nDrawMiss[MAX_MISSPORYGON];		//時間の描画用

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureMiss = NULL;							//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffMiss = NULL;						//頂点バッファへのポインタ

//初期化処理
void InitMiss(void)
{
	DATA *pData;
	pData = Getdata();

	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	//初期化
	nposMiss = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nMiss = pData->nMiss;

	//テクスチャ読み込み
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

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffMiss->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_MISSPORYGON; nCntScore++, pVtx += 4)
	{
		g_nDrawMiss[nCntScore] = 0;

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
		pVtx[0].tex = D3DXVECTOR2(g_nDrawMiss[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_nDrawMiss[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_nDrawMiss[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_nDrawMiss[nCntScore] * 0.1f + 0.1f, 1.0f);
	}
	//頂点バッファをアンロック
	g_pVtxBuffMiss->Unlock();
}

//終了処理
void UninitMiss(void)
{
	//テクスチャ破棄
	if (g_pTextureMiss != NULL)
	{
		g_pTextureMiss->Release();
		g_pTextureMiss = NULL;
	}

	//頂点バッファ破棄
	if (g_pVtxBuffMiss != NULL)
	{
		g_pVtxBuffMiss->Release();
		g_pVtxBuffMiss = NULL;
	}
}

//更新処理
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

	//頂点バッファをロックし、頂点データへのポインタを取得
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

	//頂点バッファをアンロック
	g_pVtxBuffMiss->Unlock();
}

//描画処理
void DrawMiss(void)
{
	int nCntScore;
	LPDIRECT3DDEVICE9 pDevice;							//デバイスのポインタ
	pDevice = GetDevice();								//デバイスを取得

														//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffMiss,
		0,
		sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);						//頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureMiss);			//テクスチャの設定

	for (nCntScore = 0; nCntScore < MAX_MISSPORYGON; nCntScore++)
	{
			//描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,					//プリミティブの種類
				nCntScore * 4,							//開始する頂点のインデックス
				2);										//プリミティブの数
	}
}

void SetMiss(D3DXVECTOR3 pos)
{
	int nCntMiss;

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点データへのポインタを取得

	g_pVtxBuffMiss->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntMiss = 0; nCntMiss < MAX_MISSPORYGON; nCntMiss++, pVtx += 4)
	{
		g_nDrawMiss[nCntMiss] = 0;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pos.x + (nCntMiss * MISS_X), pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((pos.x + MISS_X) + (nCntMiss * MISS_X), pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x + (nCntMiss * MISS_X), pos.y + MISS_Y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((pos.x + MISS_X) + (nCntMiss * MISS_X), pos.y + MISS_Y, 0.0f);
	}

	//頂点バッファをアンロック
	g_pVtxBuffMiss->Unlock();
	nposMiss = pos;
}

int GetMiss(void)
{
	return g_nMiss;
}