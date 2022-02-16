#include "Player.h"
#include "input.h"
#include "block.h"
#include "fade.h"
#include "databox.h"
#include "sound.h"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffPlayer = NULL;
LPDIRECT3DTEXTURE9 g_pTexturePlayer[MAX_PLAYER] = {};
int g_nCounterAnim = 0;
int g_nPatternAnim = 0;
float fTexX = 0.0f;						//テクスチャ座標X
float fTexY = 0.0f;						//テクスチャ座標Y
float fMoveD = 0.0f;					//移動量の最高
float fMoveA = 0.0f;					//移動量の最高
int nPlayerType = TYPE_RED;				//操作キャラクター判別
bool bLock;
PLAYER g_Player[MAX_PLAYER];
DATA *pData;

//初期化
void InitPlayer(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスのポインタ
	pDevice = GetDevice();				//デバイスを取得

	g_nCounterAnim = 0;					//カウンター初期化
	g_nPatternAnim = 0;					//パターンNo初期化

	pData = Getdata();

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\picture\\black.png",
		&g_pTexturePlayer[0]);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PLAYER,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffPlayer,
		NULL);

	for(int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		g_Player[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[nCnt].lastpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Player[nCnt].nJump = JUMP_MAX;
		g_Player[nCnt].bUse = false;
		fTexX = 0.0f;
		fTexY = 0.0f;
		fMoveD = 0.0f;
		fMoveA = 0.0f;
		bLock = false;
		nPlayerType = TYPE_RED;

		VERTEX_2D *pVtx;	//頂点情報へのポインタ

							//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y, 0.0f);

		//rhwの設定(1.0f固定)
		pVtx[nCnt * 4 + 0].rhw = 1.0f;
		pVtx[nCnt * 4 + 1].rhw = 1.0f;
		pVtx[nCnt * 4 + 2].rhw = 1.0f;
		pVtx[nCnt * 4 + 3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[nCnt * 4 + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[nCnt * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[nCnt * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
		pVtx[nCnt * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//テクスチャ座標の設定
		pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//頂点バッファをアンロック
		g_pVtxBuffPlayer->Unlock();
	}
}

//終了
void UninitPlayer(void)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		//テクスチャ破棄
		if (g_pTexturePlayer[nCnt] != NULL)
		{
			g_pTexturePlayer[nCnt]->Release();
			g_pTexturePlayer[nCnt] = NULL;
		}

		//頂点バッファ破棄
		if (g_pVtxBuffPlayer != NULL)
		{
			g_pVtxBuffPlayer->Release();
			g_pVtxBuffPlayer = NULL;
		}
	}
}

//更新
void UpdatePlayer(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	BLOCK *pBlock;
	pBlock = GetBlock();

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);



	//--------------------------------------------------------------
	//lastpos更新

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		g_Player[nCnt].lastpos = g_Player[nCnt].pos;
	}
	//--------------------------------------------------------------



	//--------------------------------------------------------------
	//操作キャラ変更

	if (GetKeyboardTrigger(DIK_A) == true)		//A
	{
		nPlayerType = TYPE_RED;
	}

	if (GetKeyboardTrigger(DIK_S) == true)		//S
	{
		nPlayerType = TYPE_BLUE;
	}

	if (GetKeyboardTrigger(DIK_D) == true)		//D
	{
		nPlayerType = TYPE_YELLOW;
	}
	//--------------------------------------------------------------



	//--------------------------------------------------------------
	//重力関係

	//ジャンプ
	if (GetKeyboardTrigger(DIK_SPACE) == true && g_Player[nPlayerType].nJump < JUMP_MAX)
	{
		g_Player[nPlayerType].move.y = -12.5f;
		g_Player[nPlayerType].nJump++;
		PlaySound(SOUND_LABEL_SEJUMP);
	}

	//--------------------------------------------------------------



	//--------------------------------------------------------------
	//キー入力移動

	if (GetKeyboardPress(DIK_RIGHT) == true)	//右
	{
		g_Player[nPlayerType].pos.x += g_Player[nPlayerType].move.x;
	}

	if (GetKeyboardPress(DIK_LEFT) == true)		//左
	{
		g_Player[nPlayerType].pos.x -= g_Player[nPlayerType].move.x;
	}

	//--------------------------------------------------------------



	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		//--------------------------------------------------------------
		//重力関係

		//重力を強める
		g_Player[nCnt].move.y += 0.8f;

		//重力の限界
		if (g_Player[nCnt].move.y >= MAX_GRAVITY)
		{
			g_Player[nCnt].move.y = MAX_GRAVITY;
		}

		//常に降下させる
		g_Player[nCnt].pos.y += g_Player[nCnt].move.y;

		if (g_Player[nCnt].nJump == 0)
		{
			g_Player[nCnt].nJump = 1;
		}

		//--------------------------------------------------------------



		//--------------------------------------------------------------
		//画面端の当たり判定

		if (g_Player[nCnt].pos.x + PLAYER_X >= SCREEN_WIDTH)		//画面右
		{
			g_Player[nCnt].pos.x = SCREEN_WIDTH - PLAYER_X;
		}

		if (g_Player[nCnt].pos.x - PLAYER_X <= 0)					//画面左
		{
			g_Player[nCnt].pos.x = 0 + PLAYER_X;
		}

		if (g_Player[nCnt].pos.y - PLAYER_Y >= SCREEN_HEIGHT && bLock == false)		//画面下
		{
			bLock = true;
			pData->nMiss++;

			if (pData->nMiss >= 99)
			{
				pData->nMiss = 99;
			}

			SetFade(MODE_GAME);
		}

		//--------------------------------------------------------------
	}



	//--------------------------------------------------------------
	//プレイヤー同士の当たり判定

	for (int nCnt2 = 0; nCnt2 < MAX_PLAYER; nCnt2++)
	{
		if (nPlayerType != nCnt2)
		{
			ColisionPlayer(nPlayerType, nCnt2);
		}
	}

	//--------------------------------------------------------------



	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		//--------------------------------------------------------------
		//ブロックとの当たり判定

		ColisionBlock(&g_Player[nCnt].pos, &g_Player[nCnt].lastpos, &g_Player[nCnt].move, &g_Player[nCnt].nJump, nCnt);

		//--------------------------------------------------------------



		//--------------------------------------------------------------
		//プレイヤー同士の当たり判定

		for (int nCnt2 = 0; nCnt2 < MAX_PLAYER; nCnt2++)
		{
			if (nCnt != nCnt2)
			{
				ColisionPlayer(nCnt, nCnt2);
			}
		}

		//--------------------------------------------------------------
	}



	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		//--------------------------------------------------------------
		//頂点座標の設定
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y, 0.0f);

		//rhwの設定(1.0f固定)
		pVtx[nCnt * 4 + 0].rhw = 1.0f;
		pVtx[nCnt * 4 + 1].rhw = 1.0f;
		pVtx[nCnt * 4 + 2].rhw = 1.0f;
		pVtx[nCnt * 4 + 3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[nCnt * 4 + 0].col = D3DCOLOR_RGBA(255, 255, 255, 64);
		pVtx[nCnt * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, 64);
		pVtx[nCnt * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, 64);
		pVtx[nCnt * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, 64);

		//テクスチャ座標の設定
		pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

		//--------------------------------------------------------------
	}

	//頂点カラーの設定
	pVtx[nPlayerType * 4 + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[nPlayerType * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[nPlayerType * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[nPlayerType * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//頂点バッファをアンロック
	g_pVtxBuffPlayer->Unlock();
}

//描画
void DrawPlayer(void)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (g_Player[nCnt].bUse == true)
		{
			LPDIRECT3DDEVICE9 pDevice;						//デバイスのポインタ
			pDevice = GetDevice();							//デバイスを取得

															//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffPlayer,
				0,
				sizeof(VERTEX_2D));

			pDevice->SetFVF(FVF_VERTEX_2D);					//頂点フォーマットの設定
			pDevice->SetTexture(0, g_pTexturePlayer[nCnt]);	//テクスチャの設定

															//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,						//プリミティブの種類
				nCnt * 4,									//開始する頂点のインデックス
				2);											//プリミティブの数
		}
	}
}

//セット
void SetPlayerRed(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスのポインタ
	pDevice = GetDevice();				//デバイスを取得

	g_nCounterAnim = 0;					//カウンター初期化
	g_nPatternAnim = 0;					//パターンNo初期化

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (g_Player[nCnt].bUse == false)
		{
			//テクスチャ読み込み
			D3DXCreateTextureFromFile(
				pDevice,
				"data\\picture\\red.png",
				&g_pTexturePlayer[nCnt]);

			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PLAYER,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pVtxBuffPlayer,
				NULL);

			g_Player[nCnt].pos = pos;
			g_Player[nCnt].lastpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Player[nCnt].move = D3DXVECTOR3(5.0f, 0.0f, 0.0f);
			g_Player[nCnt].nJump = JUMP_MAX;
			g_Player[nCnt].bUse = true;
			fTexX = 0.0f;
			fTexY = 0.0f;
			fMoveD = 0.0f;
			fMoveA = 0.0f;
			nPlayerType = TYPE_RED;

			VERTEX_2D *pVtx;	//頂点情報へのポインタ

								//頂点バッファをロックし、頂点データへのポインタを取得
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//頂点座標の設定
			pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y, 0.0f);
			pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y, 0.0f);

			//rhwの設定(1.0f固定)
			pVtx[nCnt * 4 + 0].rhw = 1.0f;
			pVtx[nCnt * 4 + 1].rhw = 1.0f;
			pVtx[nCnt * 4 + 2].rhw = 1.0f;
			pVtx[nCnt * 4 + 3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[nCnt * 4 + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCnt * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCnt * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCnt * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//テクスチャ座標の設定
			pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//頂点バッファをアンロック
			g_pVtxBuffPlayer->Unlock();

			break;
		}
	}
}

//セット
void SetPlayerBlue(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスのポインタ
	pDevice = GetDevice();				//デバイスを取得

	g_nCounterAnim = 0;					//カウンター初期化
	g_nPatternAnim = 0;					//パターンNo初期化

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (g_Player[nCnt].bUse == false)
		{
			//テクスチャ読み込み
			D3DXCreateTextureFromFile(
				pDevice,
				"data\\picture\\blue.png",
				&g_pTexturePlayer[nCnt]);

			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PLAYER,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pVtxBuffPlayer,
				NULL);

			g_Player[nCnt].pos = pos;
			g_Player[nCnt].lastpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Player[nCnt].move = D3DXVECTOR3(5.0f, 0.0f, 0.0f);
			g_Player[nCnt].nJump = JUMP_MAX;
			g_Player[nCnt].bUse = true;
			fTexX = 0.0f;
			fTexY = 0.0f;
			fMoveD = 0.0f;
			fMoveA = 0.0f;
			nPlayerType = TYPE_RED;

			VERTEX_2D *pVtx;	//頂点情報へのポインタ

								//頂点バッファをロックし、頂点データへのポインタを取得
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//頂点座標の設定
			pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y, 0.0f);
			pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y, 0.0f);

			//rhwの設定(1.0f固定)
			pVtx[nCnt * 4 + 0].rhw = 1.0f;
			pVtx[nCnt * 4 + 1].rhw = 1.0f;
			pVtx[nCnt * 4 + 2].rhw = 1.0f;
			pVtx[nCnt * 4 + 3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[nCnt * 4 + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCnt * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCnt * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCnt * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//テクスチャ座標の設定
			pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//頂点バッファをアンロック
			g_pVtxBuffPlayer->Unlock();

			break;
		}
	}
}

//セット
void SetPlayerYellow(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスのポインタ
	pDevice = GetDevice();				//デバイスを取得

	g_nCounterAnim = 0;					//カウンター初期化
	g_nPatternAnim = 0;					//パターンNo初期化

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (g_Player[nCnt].bUse == false)
		{
			//テクスチャ読み込み
			D3DXCreateTextureFromFile(
				pDevice,
				"data\\picture\\yellow.png",
				&g_pTexturePlayer[nCnt]);

			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PLAYER,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pVtxBuffPlayer,
				NULL);

			g_Player[nCnt].pos = pos;
			g_Player[nCnt].lastpos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			g_Player[nCnt].move = D3DXVECTOR3(5.0f, 0.0f, 0.0f);
			g_Player[nCnt].nJump = JUMP_MAX;
			g_Player[nCnt].bUse = true;
			fTexX = 0.0f;
			fTexY = 0.0f;
			fMoveD = 0.0f;
			fMoveA = 0.0f;
			nPlayerType = TYPE_RED;

			VERTEX_2D *pVtx;	//頂点情報へのポインタ

								//頂点バッファをロックし、頂点データへのポインタを取得
			g_pVtxBuffPlayer->Lock(0, 0, (void**)&pVtx, 0);

			//頂点座標の設定
			pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_Player[nCnt].pos.x - PLAYER_X, g_Player[nCnt].pos.y, 0.0f);
			pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_Player[nCnt].pos.x + PLAYER_X, g_Player[nCnt].pos.y, 0.0f);

			//rhwの設定(1.0f固定)
			pVtx[nCnt * 4 + 0].rhw = 1.0f;
			pVtx[nCnt * 4 + 1].rhw = 1.0f;
			pVtx[nCnt * 4 + 2].rhw = 1.0f;
			pVtx[nCnt * 4 + 3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[nCnt * 4 + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCnt * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCnt * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[nCnt * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//テクスチャ座標の設定
			pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//頂点バッファをアンロック
			g_pVtxBuffPlayer->Unlock();

			break;
		}
	}
}

PLAYER *GetPlayer(void)
{
	return &g_Player[0];
}

void ColisionPlayer(int nCnt, int nCnt2)
{
	if (g_Player[nCnt].bUse == true)
	{
		if (g_Player[nCnt2].pos.x - PLAYER_X < g_Player[nCnt].pos.x + PLAYER_X &&			//左端
			g_Player[nCnt2].pos.x + PLAYER_X > g_Player[nCnt].pos.x - PLAYER_X &&			//右端
			g_Player[nCnt2].pos.y - PLAYER_Y < g_Player[nCnt].pos.y &&						//乗る
			g_Player[nCnt2].pos.y > g_Player[nCnt].pos.y - PLAYER_Y)						//下からたたく
		{
			if (g_Player[nCnt2].pos.x - PLAYER_X >= g_Player[nCnt].lastpos.x + PLAYER_X &&
				(g_Player[nCnt2].pos.y - PLAYER_Y < g_Player[nCnt].pos.y + 1.0f ||
				g_Player[nCnt2].pos.y > g_Player[nCnt].pos.y + PLAYER_Y - 1.0f))						//左端
			{
				g_Player[nCnt].pos.x = (g_Player[nCnt2].pos.x - PLAYER_X) - PLAYER_X;
			}

			else if (g_Player[nCnt2].pos.x + PLAYER_X <= g_Player[nCnt].lastpos.x - PLAYER_X &&
				(g_Player[nCnt2].pos.y - PLAYER_Y < g_Player[nCnt].pos.y - 1.0f ||
				g_Player[nCnt2].pos.y > g_Player[nCnt].pos.y + PLAYER_Y + 1.0f))						//右端
			{
				g_Player[nCnt].pos.x = (g_Player[nCnt2].pos.x + PLAYER_X) + PLAYER_X;
			}

			else if (g_Player[nCnt2].pos.y - PLAYER_Y >= (g_Player[nCnt].lastpos.y - g_Player[nCnt].move.y))	//乗る
			{
				g_Player[nCnt].pos.y = g_Player[nCnt2].pos.y - PLAYER_Y;
				g_Player[nCnt].move.y = 0;
				g_Player[nCnt].nJump = 0;
			}

			else if (g_Player[nCnt2].pos.y <= (g_Player[nCnt].lastpos.y - g_Player[nCnt].move.y) - PLAYER_Y)	//下からたたく
			{
				g_Player[nCnt].pos.y = g_Player[nCnt2].pos.y + PLAYER_Y;
				g_Player[nCnt].move.y = 0.0f;
			}
		}
	}
}