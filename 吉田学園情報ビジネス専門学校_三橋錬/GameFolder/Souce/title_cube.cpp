#include "Player.h"
#include "Title_cube.h"
#include <stdlib.h>
#include <time.h>

#define COOLTIME (8)
#define CUBE_MOVE_X (10)

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffCube = NULL;
LPDIRECT3DTEXTURE9 g_pTextureCube[MAX_PLAYER] = {};
CUBE g_Cube[MAX_PLAYER];

//初期化
void InitCube(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスのポインタ
	pDevice = GetDevice();				//デバイスを取得

	for(int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		//テクスチャ読み込み
		D3DXCreateTextureFromFile(
			pDevice,
			"data\\picture\\black.png",
			&g_pTextureCube[0]);

			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PLAYER,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pVtxBuffCube,
				NULL);

		g_Cube[nCnt].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Cube[nCnt].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Cube[nCnt].bHit = false;
		g_Cube[nCnt].bUse = false;
		g_Cube[nCnt].nCounterAnim = 0;
		g_Cube[nCnt].fTex = 0.0f;
		g_Cube[nCnt].nTimer = 0;

		VERTEX_2D *pVtx;	//頂点情報へのポインタ

							//頂点バッファをロックし、頂点データへのポインタを取得
		g_pVtxBuffCube->Lock(0, 0, (void**)&pVtx, 0);

		//頂点座標の設定
		pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_Cube[nCnt].pos.x - PLAYER_X, g_Cube[nCnt].pos.y - PLAYER_Y, 0.0f);
		pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_Cube[nCnt].pos.x + PLAYER_X, g_Cube[nCnt].pos.y - PLAYER_Y, 0.0f);
		pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_Cube[nCnt].pos.x - PLAYER_X, g_Cube[nCnt].pos.y, 0.0f);
		pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_Cube[nCnt].pos.x + PLAYER_X, g_Cube[nCnt].pos.y, 0.0f);

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
		g_pVtxBuffCube->Unlock();
	}

	g_Cube[TYPE_RED].nRBY = TYPE_RED;
	g_Cube[TYPE_BLUE].nRBY = TYPE_BLUE;
	g_Cube[TYPE_YELLOW].nRBY = TYPE_YELLOW;

	srand((unsigned int)time(NULL));
}

//終了
void UninitCube(void)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		//テクスチャ破棄
		if (g_pTextureCube[nCnt] != NULL)
		{
			g_pTextureCube[nCnt]->Release();
			g_pTextureCube[nCnt] = NULL;
		}

		//頂点バッファ破棄
		if (g_pVtxBuffCube != NULL)
		{
			g_pVtxBuffCube->Release();
			g_pVtxBuffCube = NULL;
		}
	}
}

//更新
void UpdateCube(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffCube->Lock(0, 0, (void**)&pVtx, 0);



	//--------------------------------------------------------------
	//重力関係

	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (g_Cube[nCnt].bUse == true)
		{
			//--------------------------------------------------------------
			//角度

			float moveX = (float)(rand() % CUBE_MOVE_X) - (CUBE_MOVE_X / 2);
			g_Cube[nCnt].nTimer++;

			if (g_Cube[nCnt].nTimer >= 6)
			{
				g_Cube[nCnt].nTimer = 0;

				if (g_Cube[nCnt].fTex > 0)
				{
					g_Cube[nCnt].fTex += 0.25f;
				}

				else if (g_Cube[nCnt].fTex < 0)
				{
					g_Cube[nCnt].fTex -= 0.25f;
				}
			}

			//--------------------------------------------------------------



			//--------------------------------------------------------------
			//重力関係

			//重力を強める
			g_Cube[nCnt].move.y += 0.8f;

			//重力の限界
			if (g_Cube[nCnt].move.y >= MAX_GRAVITY)
			{
				g_Cube[nCnt].move.y = MAX_GRAVITY;
			}

			//常に降下させる
			g_Cube[nCnt].pos.y += g_Cube[nCnt].move.y;
			g_Cube[nCnt].pos.x += g_Cube[nCnt].move.x;

			//--------------------------------------------------------------



			//--------------------------------------------------------------
			//画面端の当たり判定

			if (g_Cube[nCnt].pos.y - PLAYER_Y >= SCREEN_HEIGHT)					//画面下
			{
				g_Cube[nCnt].bUse = false;
			}

			//--------------------------------------------------------------



			//--------------------------------------------------------------
			//当たり判定
			if (g_Cube[nCnt].pos.y >= 600 && g_Cube[nCnt].bHit == false)
			{
				g_Cube[nCnt].move.x = moveX;
				g_Cube[nCnt].move.y = -14;
				g_Cube[nCnt].bHit = true;

				if (moveX > 0)
				{
					g_Cube[nCnt].fTex += 0.25f;
				}

				else if (moveX < 0)
				{
					g_Cube[nCnt].fTex -= 0.25f;
				}
			}

			//--------------------------------------------------------------



			//--------------------------------------------------------------
			//頂点座標の設定
			pVtx[nCnt * 4 + 0].pos = D3DXVECTOR3(g_Cube[nCnt].pos.x - PLAYER_X, g_Cube[nCnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[nCnt * 4 + 1].pos = D3DXVECTOR3(g_Cube[nCnt].pos.x + PLAYER_X, g_Cube[nCnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[nCnt * 4 + 2].pos = D3DXVECTOR3(g_Cube[nCnt].pos.x - PLAYER_X, g_Cube[nCnt].pos.y, 0.0f);
			pVtx[nCnt * 4 + 3].pos = D3DXVECTOR3(g_Cube[nCnt].pos.x + PLAYER_X, g_Cube[nCnt].pos.y, 0.0f);

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
			pVtx[nCnt * 4 + 0].tex = D3DXVECTOR2(g_Cube[nCnt].fTex, 0.0f);
			pVtx[nCnt * 4 + 1].tex = D3DXVECTOR2(g_Cube[nCnt].fTex + 0.25f, 0.0f);
			pVtx[nCnt * 4 + 2].tex = D3DXVECTOR2(g_Cube[nCnt].fTex, 1.0f);
			pVtx[nCnt * 4 + 3].tex = D3DXVECTOR2(g_Cube[nCnt].fTex + 0.25f, 1.0f);

			//--------------------------------------------------------------
		}

		else
		{
			float nXPos = (float)(rand() % SCREEN_WIDTH);
			float moveY = (float)(rand() % 11);

			g_Cube[nCnt].nCoolTime++;

			if (g_Cube[nCnt].nCoolTime >= COOLTIME)
			{
				g_Cube[nCnt].nCoolTime = 0;

				switch (g_Cube[nCnt].nRBY)
				{
				case TYPE_RED:
					SetCubeRed(D3DXVECTOR3(nXPos, 0.0f - PLAYER_Y, 0.0f), moveY, nCnt);
					break;

				case TYPE_BLUE:
					SetCubeBlue(D3DXVECTOR3(nXPos, 0.0f - PLAYER_Y, 0.0f), moveY, nCnt);
					break;

				case TYPE_YELLOW:
					SetCubeYellow(D3DXVECTOR3(nXPos, 0.0f - PLAYER_Y, 0.0f), moveY, nCnt);
					break;
				}
			}
		}
	}

	//頂点バッファをアンロック
	g_pVtxBuffCube->Unlock();
}

//描画
void DrawCube(void)
{
	for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
	{
		if (g_Cube[nCnt].bUse == true)
		{
			LPDIRECT3DDEVICE9 pDevice;						//デバイスのポインタ
			pDevice = GetDevice();							//デバイスを取得

															//頂点バッファをデバイスのデータストリームに設定
			pDevice->SetStreamSource(
				0,
				g_pVtxBuffCube,
				0,
				sizeof(VERTEX_2D));

			pDevice->SetFVF(FVF_VERTEX_2D);					//頂点フォーマットの設定
			pDevice->SetTexture(0, g_pTextureCube[nCnt]);	//テクスチャの設定

															//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,						//プリミティブの種類
				nCnt * 4,									//開始する頂点のインデックス
				2);											//プリミティブの数
		}
	}
}

//セット
void SetCubeRed(D3DXVECTOR3 pos, float move, int cnt)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスのポインタ
	pDevice = GetDevice();				//デバイスを取得

		if (g_Cube[cnt].bUse == false)
		{
			//テクスチャ読み込み
			D3DXCreateTextureFromFile(
				pDevice,
				"data\\picture\\title_red.png",
				&g_pTextureCube[cnt]);

			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PLAYER,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pVtxBuffCube,
				NULL);

			g_Cube[cnt].pos = pos;
			g_Cube[cnt].move = D3DXVECTOR3(0.0f, move, 0.0f);
			g_Cube[cnt].bHit = false;
			g_Cube[cnt].nCoolTime = 0;
			g_Cube[cnt].bUse = true;
			g_Cube[cnt].nCounterAnim = 0;
			g_Cube[cnt].fTex = 0;
			g_Cube[cnt].nTimer = 0;

			VERTEX_2D *pVtx;	//頂点情報へのポインタ

								//頂点バッファをロックし、頂点データへのポインタを取得
			g_pVtxBuffCube->Lock(0, 0, (void**)&pVtx, 0);

			//頂点座標の設定
			pVtx[cnt * 4 + 0].pos = D3DXVECTOR3(g_Cube[cnt].pos.x - PLAYER_X, g_Cube[cnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[cnt * 4 + 1].pos = D3DXVECTOR3(g_Cube[cnt].pos.x + PLAYER_X, g_Cube[cnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[cnt * 4 + 2].pos = D3DXVECTOR3(g_Cube[cnt].pos.x - PLAYER_X, g_Cube[cnt].pos.y, 0.0f);
			pVtx[cnt * 4 + 3].pos = D3DXVECTOR3(g_Cube[cnt].pos.x + PLAYER_X, g_Cube[cnt].pos.y, 0.0f);

			//rhwの設定(1.0f固定)
			pVtx[cnt * 4 + 0].rhw = 1.0f;
			pVtx[cnt * 4 + 1].rhw = 1.0f;
			pVtx[cnt * 4 + 2].rhw = 1.0f;
			pVtx[cnt * 4 + 3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[cnt * 4 + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[cnt * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[cnt * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[cnt * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//テクスチャ座標の設定
			pVtx[cnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[cnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[cnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[cnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//頂点バッファをアンロック
			g_pVtxBuffCube->Unlock();
		}
}

//セット
void SetCubeBlue(D3DXVECTOR3 pos, float move, int cnt)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスのポインタ
	pDevice = GetDevice();				//デバイスを取得

		if (g_Cube[cnt].bUse == false)
		{
			//テクスチャ読み込み
			D3DXCreateTextureFromFile(
				pDevice,
				"data\\picture\\title_blue.png",
				&g_pTextureCube[cnt]);

			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PLAYER,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pVtxBuffCube,
				NULL);

			g_Cube[cnt].pos = pos;
			g_Cube[cnt].move = D3DXVECTOR3(0.0f, move, 0.0f);
			g_Cube[cnt].bHit = false;
			g_Cube[cnt].nCoolTime = 0;
			g_Cube[cnt].bUse = true;
			g_Cube[cnt].nCounterAnim = 0;
			g_Cube[cnt].fTex = 0;
			g_Cube[cnt].nTimer = 0;

			VERTEX_2D *pVtx;	//頂点情報へのポインタ

								//頂点バッファをロックし、頂点データへのポインタを取得
			g_pVtxBuffCube->Lock(0, 0, (void**)&pVtx, 0);

			//頂点座標の設定
			pVtx[cnt * 4 + 0].pos = D3DXVECTOR3(g_Cube[cnt].pos.x - PLAYER_X, g_Cube[cnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[cnt * 4 + 1].pos = D3DXVECTOR3(g_Cube[cnt].pos.x + PLAYER_X, g_Cube[cnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[cnt * 4 + 2].pos = D3DXVECTOR3(g_Cube[cnt].pos.x - PLAYER_X, g_Cube[cnt].pos.y, 0.0f);
			pVtx[cnt * 4 + 3].pos = D3DXVECTOR3(g_Cube[cnt].pos.x + PLAYER_X, g_Cube[cnt].pos.y, 0.0f);

			//rhwの設定(1.0f固定)
			pVtx[cnt * 4 + 0].rhw = 1.0f;
			pVtx[cnt * 4 + 1].rhw = 1.0f;
			pVtx[cnt * 4 + 2].rhw = 1.0f;
			pVtx[cnt * 4 + 3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[cnt * 4 + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[cnt * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[cnt * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[cnt * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//テクスチャ座標の設定
			pVtx[cnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[cnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[cnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[cnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//頂点バッファをアンロック
			g_pVtxBuffCube->Unlock();
		}
}

//セット
void SetCubeYellow(D3DXVECTOR3 pos, float move, int cnt)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスのポインタ
	pDevice = GetDevice();				//デバイスを取得

		if (g_Cube[cnt].bUse == false)
		{
			//テクスチャ読み込み
			D3DXCreateTextureFromFile(
				pDevice,
				"data\\picture\\title_yellow.png",
				&g_pTextureCube[cnt]);

			pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_PLAYER,
				D3DUSAGE_WRITEONLY,
				FVF_VERTEX_2D,
				D3DPOOL_MANAGED,
				&g_pVtxBuffCube,
				NULL);

			g_Cube[cnt].pos = pos;
			g_Cube[cnt].move = D3DXVECTOR3(0.0f, move, 0.0f);
			g_Cube[cnt].bHit = false;
			g_Cube[cnt].nCoolTime = 0;
			g_Cube[cnt].bUse = true;
			g_Cube[cnt].nCounterAnim = 0;
			g_Cube[cnt].fTex = 0;
			g_Cube[cnt].nTimer = 0;

			VERTEX_2D *pVtx;	//頂点情報へのポインタ

								//頂点バッファをロックし、頂点データへのポインタを取得
			g_pVtxBuffCube->Lock(0, 0, (void**)&pVtx, 0);

			//頂点座標の設定
			pVtx[cnt * 4 + 0].pos = D3DXVECTOR3(g_Cube[cnt].pos.x - PLAYER_X, g_Cube[cnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[cnt * 4 + 1].pos = D3DXVECTOR3(g_Cube[cnt].pos.x + PLAYER_X, g_Cube[cnt].pos.y - PLAYER_Y, 0.0f);
			pVtx[cnt * 4 + 2].pos = D3DXVECTOR3(g_Cube[cnt].pos.x - PLAYER_X, g_Cube[cnt].pos.y, 0.0f);
			pVtx[cnt * 4 + 3].pos = D3DXVECTOR3(g_Cube[cnt].pos.x + PLAYER_X, g_Cube[cnt].pos.y, 0.0f);

			//rhwの設定(1.0f固定)
			pVtx[cnt * 4 + 0].rhw = 1.0f;
			pVtx[cnt * 4 + 1].rhw = 1.0f;
			pVtx[cnt * 4 + 2].rhw = 1.0f;
			pVtx[cnt * 4 + 3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[cnt * 4 + 0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[cnt * 4 + 1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[cnt * 4 + 2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
			pVtx[cnt * 4 + 3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

			//テクスチャ座標の設定
			pVtx[cnt * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[cnt * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[cnt * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[cnt * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);

			//頂点バッファをアンロック
			g_pVtxBuffCube->Unlock();
		}
}