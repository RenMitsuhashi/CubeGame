#include "Score.h"
#include "databox.h"

//グローバル変数
int g_nScore;							//スコア
D3DXVECTOR3 npos;						//スコアの表示位置
int g_nDrawScore[MAX_SCOREPORYGON];		//スコアの描画用
int nScore1 = 1000000;
int nScore2 = 100000;

//グローバル変数
LPDIRECT3DTEXTURE9 g_pTextureScore = NULL;							//テクスチャへのポインタ
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffScore = NULL;						//頂点バッファへのポインタ

//初期化処理
void InitScore(void)
{
	DATA *pData;
	pData = Getdata();

	LPDIRECT3DDEVICE9 pDevice;
	int nCntScore;

	//デバイスの取得
	pDevice = GetDevice();

	//初期化
	npos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_nScore = pData->nScore;

	//テクスチャ読み込み
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

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_SCOREPORYGON; nCntScore++, pVtx += 4)
	{
		g_nDrawScore[nCntScore] = 0;

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
		pVtx[0].tex = D3DXVECTOR2(g_nDrawScore[nCntScore] * 0.1f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(g_nDrawScore[nCntScore] * 0.1f + 0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(g_nDrawScore[nCntScore] * 0.1f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(g_nDrawScore[nCntScore] * 0.1f + 0.1f, 1.0f);
	}
	//頂点バッファをアンロック
	g_pVtxBuffScore->Unlock();

	nScore1 = 1000000;
	nScore2 = 100000;
}

//終了処理
void UninitScore(void)
{
	//テクスチャ破棄
	if (g_pTextureScore != NULL)
	{
		g_pTextureScore->Release();
		g_pTextureScore = NULL;
	}

	//頂点バッファ破棄
	if (g_pVtxBuffScore != NULL)
	{
		g_pVtxBuffScore->Release();
		g_pVtxBuffScore = NULL;
	}
}

//更新処理
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

	//頂点バッファをロックし、頂点データへのポインタを取得
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

	//頂点バッファをアンロック
	g_pVtxBuffScore->Unlock();
}

//描画処理
void DrawScore(void)
{
	int nCntScore;
	LPDIRECT3DDEVICE9 pDevice;							//デバイスのポインタ
	pDevice = GetDevice();								//デバイスを取得

														//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffScore,
		0,
		sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);						//頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTextureScore);			//テクスチャの設定

	for (nCntScore = 0; nCntScore < MAX_SCOREPORYGON; nCntScore++)
	{
			//描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,					//プリミティブの種類
				nCntScore * 4,							//開始する頂点のインデックス
				2);										//プリミティブの数
	}
}

//スコア追加
void AddScore(int nValue)
{
	g_nScore += nValue;
}

void SetScore(D3DXVECTOR3 pos)
{
	DATA *pData;
	pData = Getdata();

	int nCntScore;

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点データへのポインタを取得

	g_pVtxBuffScore->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntScore = 0; nCntScore < MAX_SCOREPORYGON; nCntScore++, pVtx += 4)
	{
		g_nDrawScore[nCntScore] = 0;

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(pos.x + (nCntScore * 80), pos.y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3((pos.x + 60) +(nCntScore * 80), pos.y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(pos.x + (nCntScore * 80), pos.y + 100, 0.0f);
		pVtx[3].pos = D3DXVECTOR3((pos.x + 60) + (nCntScore * 80), pos.y + 100, 0.0f);
	}

	//頂点バッファをアンロック
	g_pVtxBuffScore->Unlock();

	g_nScore = pData->nScore;
	npos = pos;
}