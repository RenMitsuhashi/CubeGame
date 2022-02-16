#include "goal.h"
#include "game.h"
#include "Player.h"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffgoal = NULL;		//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTexturegoal = NULL;				//テクスチャへのポインタ
GOAL g_Goal;

//初期化処理
void Initgoal(void)
{
	g_Goal.pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	g_Goal.bHit = false;
	g_Goal.bUse = false;

	LPDIRECT3DDEVICE9 pDevice;			//デバイスのポインタ
	pDevice = GetDevice();				//デバイスを取得

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\picture\\goal.png",
		&g_pTexturegoal);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffgoal,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffgoal->Lock(0, 0, (void**)&pVtx, 0);

	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

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
	g_pVtxBuffgoal->Unlock();
}

//終了処理
void Uninitgoal(void)
{
	//テクスチャ破棄
	if (g_pTexturegoal != NULL)
	{
		g_pTexturegoal->Release();
		g_pTexturegoal = NULL;
	}

	//頂点バッファ破棄
	if (g_pVtxBuffgoal != NULL)
	{
		g_pVtxBuffgoal->Release();
		g_pVtxBuffgoal = NULL;
	}
}

//更新処理
void Updategoal(void)
{
	STAGE *pStage;
	pStage = GetStage();

	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	int col;

	if (pStage->bClear == false)	col = 0;
	else col = 255;

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++, pPlayer++)
	{
		if (g_Goal.pos.x - PLAYER_X <= pPlayer->pos.x + PLAYER_X &&								//ブロックの左端
			g_Goal.pos.x + PLAYER_X >= pPlayer->pos.x - PLAYER_X &&								//ブロックの右端
			g_Goal.pos.y - PLAYER_Y <= pPlayer->pos.y &&										//ブロックに乗る
			g_Goal.pos.y >= pPlayer->pos.y - PLAYER_Y)											//ブロックを下からたたく
		{
			if (g_Goal.pos.x - PLAYER_X >= pPlayer->lastpos.x + PLAYER_X)						//左端
			{
				pPlayer->pos.x = (g_Goal.pos.x - PLAYER_X) - PLAYER_X;
			}

			else if (g_Goal.pos.x + PLAYER_X <= pPlayer->lastpos.x - PLAYER_X)					//右端
			{
				pPlayer->pos.x = (g_Goal.pos.x + PLAYER_X) + PLAYER_X;
			}

			else if (g_Goal.pos.y - PLAYER_Y >= pPlayer->lastpos.y)								//乗る
			{
				pPlayer->pos.y = g_Goal.pos.y - PLAYER_Y;
				pPlayer->move.y = 0;
				pPlayer->nJump = 0;
			}

			else if (g_Goal.pos.y <= pPlayer->lastpos.y - PLAYER_Y)								//下からたたく
			{
				pPlayer->pos.y = g_Goal.pos.y + PLAYER_Y;
				pPlayer->move.y = 0.0f;
			}

			if (pStage->bClear == false)
			{
				pStage->bClear = true;
				break;
			}
		}
	}
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

						//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffgoal->Lock(0, 0, (void**)&pVtx, 0);

	//頂点カラーの設定
	pVtx[0].col = D3DCOLOR_RGBA(255, 255 - col, 255 - col, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255 - col, 255 - col, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255 - col, 255 - col, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255 - col, 255 - col, 255);

	//頂点バッファをアンロック
	g_pVtxBuffgoal->Unlock();
}

//描画処理
void Drawgoal(void)
{
	LPDIRECT3DDEVICE9 pDevice;							//デバイスのポインタ
	pDevice = GetDevice();								//デバイスを取得

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffgoal,
		0,
		sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);						//頂点フォーマットの設定
	pDevice->SetTexture(0, g_pTexturegoal);				//テクスチャの設定

	//ポリゴンの描画
	pDevice->DrawPrimitive(
		D3DPT_TRIANGLESTRIP,		//プリミティブの種類
		0,							//開始する頂点のインデックス
		2);							//プリミティブの数
}

void Setgoal(D3DXVECTOR3 pos)
{
	if (g_Goal.bUse == false)
	{
		g_Goal.pos = pos;
		g_Goal.pos.y += 10.0f;

		g_Goal.bHit = false;
		g_Goal.bUse = true;

		VERTEX_2D *pVtx;	//頂点情報へのポインタ

							//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffgoal->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[0].pos = D3DXVECTOR3(g_Goal.pos.x - PLAYER_X, g_Goal.pos.y - PLAYER_Y, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_Goal.pos.x + PLAYER_X, g_Goal.pos.y - PLAYER_Y, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_Goal.pos.x - PLAYER_X, g_Goal.pos.y, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_Goal.pos.x + PLAYER_X, g_Goal.pos.y, 0.0f);

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
		pVtx[0].tex = D3DXVECTOR2(0.01f, 0.01f);
		pVtx[1].tex = D3DXVECTOR2(0.99f, 0.01f);
		pVtx[2].tex = D3DXVECTOR2(0.01f, 0.99f);
		pVtx[3].tex = D3DXVECTOR2(0.99f, 0.99f);

		//頂点バッファをアンロック
		g_pVtxBuffgoal->Unlock();
	}
}