#include "block.h"
#include "Player.h"
#include "game.h"
#include "databox.h"
#include "fade.h"
#include "sound.h"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBlock = NULL;		//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureBlock[MAX_BLOCK] = {};	//テクスチャへのポインタ
BLOCK g_Block[MAX_BLOCK];							//ブロックの構造体
bool bOnceFlag;
bool bOnceDoor;

//初期化処理
void InitBlock(void)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスのポインタ
	pDevice = GetDevice();				//デバイスを取得

										//テクスチャ読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\picture\\block.png",
		&g_pTextureBlock[0]);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BLOCK,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBlock,
		NULL);

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	bOnceFlag = false;
	bOnceDoor = false;

	for (int nCount = 0; nCount < MAX_BLOCK; nCount++)
	{
		g_Block[nCount].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Block[nCount].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_Block[nCount].fWidth = 0.0f;
		g_Block[nCount].fHeight = 0.0f;
		g_Block[nCount].bUse = false;
		g_Block[nCount].nType = BLOCK_NORMAL;
		g_Block[nCount].fMove = 0.0f;
		g_Block[nCount].fMoveMax = 0.0f;
		g_Block[nCount].UDMoveScan = 0;
		g_Block[nCount].LRMoveScan = 0;
		g_Block[nCount].bSwitch = false;
		g_Block[nCount].nHitP = HIT_NONE;
		g_Block[nCount].col = D3DCOLOR_RGBA(255, 255, 255, 255);

		//頂点座標の設定
		pVtx[nCount * 4 + 0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[nCount * 4 + 1].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[nCount * 4 + 2].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		pVtx[nCount * 4 + 3].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

		//rhwの設定(1.0f固定)
		pVtx[nCount * 4 + 0].rhw = 1.0f;
		pVtx[nCount * 4 + 1].rhw = 1.0f;
		pVtx[nCount * 4 + 2].rhw = 1.0f;
		pVtx[nCount * 4 + 3].rhw = 1.0f;

		//頂点カラーの設定
		pVtx[nCount * 4 + 0].col = g_Block[nCount].col;
		pVtx[nCount * 4 + 1].col = g_Block[nCount].col;
		pVtx[nCount * 4 + 2].col = g_Block[nCount].col;
		pVtx[nCount * 4 + 3].col = g_Block[nCount].col;

		//テクスチャ座標の設定
		pVtx[nCount * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[nCount * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[nCount * 4 + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[nCount * 4 + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	//頂点バッファをアンロック
	g_pVtxBuffBlock->Unlock();
}

//終了処理
void UninitBlock(void)
{
	for (int nCount = 0; nCount < MAX_BLOCK; nCount++)
	{
		//テクスチャ破棄
		if (g_pTextureBlock[nCount] != NULL)
		{
			g_pTextureBlock[nCount]->Release();
			g_pTextureBlock[nCount] = NULL;
		}

		//頂点バッファ破棄
		if (g_pVtxBuffBlock != NULL)
		{
			g_pVtxBuffBlock->Release();
			g_pVtxBuffBlock = NULL;
		}
	}
}

//更新処理
void UpdateBlock(void)
{
	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCount = 0; nCount < MAX_BLOCK; nCount++)
	{
		if (g_Block[nCount].bUse == true)
		{
			for (int nCnt = 0; nCnt < MAX_PLAYER; nCnt++)
			{
				//それぞれの処理
				switch (g_Block[nCount].nType)
				{
				case BLOCK_NORMAL:
					break;

				case BLOCK_DOOR:
					Door(nCount);
					break;
				}

				//プレイヤーの当たり判定
				ColisionBlock(&pPlayer->pos, &pPlayer->lastpos, &pPlayer->move, &pPlayer->nJump, nCnt);

				//頂点座標の設定
				pVtx[nCount * 4 + 0].pos = D3DXVECTOR3(g_Block[nCount].pos.x - g_Block[nCount].fWidth / 2.0f, g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f, 0.0f);
				pVtx[nCount * 4 + 1].pos = D3DXVECTOR3(g_Block[nCount].pos.x + g_Block[nCount].fWidth / 2.0f, g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f, 0.0f);
				pVtx[nCount * 4 + 2].pos = D3DXVECTOR3(g_Block[nCount].pos.x - g_Block[nCount].fWidth / 2.0f, g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f, 0.0f);
				pVtx[nCount * 4 + 3].pos = D3DXVECTOR3(g_Block[nCount].pos.x + g_Block[nCount].fWidth / 2.0f, g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f, 0.0f);

				//rhwの設定(1.0f固定)
				pVtx[nCount * 4 + 0].rhw = 1.0f;
				pVtx[nCount * 4 + 1].rhw = 1.0f;
				pVtx[nCount * 4 + 2].rhw = 1.0f;
				pVtx[nCount * 4 + 3].rhw = 1.0f;

				//頂点カラーの設定
				pVtx[nCount * 4 + 0].col = g_Block[nCount].col;
				pVtx[nCount * 4 + 1].col = g_Block[nCount].col;
				pVtx[nCount * 4 + 2].col = g_Block[nCount].col;
				pVtx[nCount * 4 + 3].col = g_Block[nCount].col;

				//テクスチャ座標の設定
				pVtx[nCount * 4 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
				pVtx[nCount * 4 + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
				pVtx[nCount * 4 + 2].tex = D3DXVECTOR2(0.0f, 0.99f);
				pVtx[nCount * 4 + 3].tex = D3DXVECTOR2(1.0f, 0.99f);
			}
		}
		//頂点バッファをアンロック
		g_pVtxBuffBlock->Unlock();
	}
}

//描画処理
void DrawBlock(void)
{

	LPDIRECT3DDEVICE9 pDevice;							//デバイスのポインタ
	pDevice = GetDevice();								//デバイスを取得

														//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(
		0,
		g_pVtxBuffBlock,
		0,
		sizeof(VERTEX_2D));

	pDevice->SetFVF(FVF_VERTEX_2D);						//頂点フォーマットの設定

	for (int nCount = 0; nCount < MAX_BLOCK; nCount++)
	{
		if (g_Block[nCount].bUse == true)
		{
			pDevice->SetTexture(0, g_pTextureBlock[nCount]);	//テクスチャの設定

			//ポリゴンの描画
			pDevice->DrawPrimitive(
				D3DPT_TRIANGLESTRIP,		//プリミティブの種類
				nCount * 4,					//開始する頂点のインデックス
				2);							//プリミティブの数
		}
	}
}

//セット
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, int nType, float movemax)
{
	LPDIRECT3DDEVICE9 pDevice;			//デバイスのポインタ
	pDevice = GetDevice();				//デバイスを取得

	VERTEX_2D *pVtx;					//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffBlock->Lock(0, 0, (void**)&pVtx, 0);

	bOnceFlag = false;

	for (int nCount = 0; nCount < MAX_BLOCK; nCount++)
	{
		if (g_Block[nCount].bUse == false)
		{
			g_Block[nCount].pos = pos;
			g_Block[nCount].move = move;
			g_Block[nCount].fWidth = fWidth;
			g_Block[nCount].fHeight = fHeight;
			g_Block[nCount].bUse = true;
			g_Block[nCount].nType = nType;
			g_Block[nCount].fMove = 0.0f;
			g_Block[nCount].fMoveMax = movemax;
			g_Block[nCount].UDMoveScan = 0;
			g_Block[nCount].LRMoveScan = 0;
			g_Block[nCount].bSwitch = false;
			g_Block[nCount].nHitP = HIT_NONE;


			switch (g_Block[nCount].nType)
			{
			default:
				g_Block[nCount].col = D3DCOLOR_RGBA(0, 0, 0, 255);
				break;

			case BLOCK_DOOR:
				g_Block[nCount].col = D3DCOLOR_RGBA(190, 255, 166, 255);
				break;

			case BLOCK_TOGE:
				D3DXCreateTextureFromFile(
					pDevice,
					"data\\picture\\toge.png",
					&g_pTextureBlock[nCount]);
				break;

			case BLOCK_BUTTON:
				D3DXCreateTextureFromFile(
					pDevice,
					"data\\picture\\button.png",
					&g_pTextureBlock[nCount]);
			}

			//頂点座標の設定
			pVtx[nCount * 4 + 0].pos = D3DXVECTOR3(g_Block[nCount].pos.x - g_Block[nCount].fWidth / 2.0f, g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f, 0.0f);
			pVtx[nCount * 4 + 1].pos = D3DXVECTOR3(g_Block[nCount].pos.x + g_Block[nCount].fWidth / 2.0f, g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f, 0.0f);
			pVtx[nCount * 4 + 2].pos = D3DXVECTOR3(g_Block[nCount].pos.x - g_Block[nCount].fWidth / 2.0f, g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f, 0.0f);
			pVtx[nCount * 4 + 3].pos = D3DXVECTOR3(g_Block[nCount].pos.x + g_Block[nCount].fWidth / 2.0f, g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f, 0.0f);

			//rhwの設定(1.0f固定)
			pVtx[nCount * 4 + 0].rhw = 1.0f;
			pVtx[nCount * 4 + 1].rhw = 1.0f;
			pVtx[nCount * 4 + 2].rhw = 1.0f;
			pVtx[nCount * 4 + 3].rhw = 1.0f;

			//頂点カラーの設定
			pVtx[nCount * 4 + 0].col = g_Block[nCount].col;
			pVtx[nCount * 4 + 1].col = g_Block[nCount].col;
			pVtx[nCount * 4 + 2].col = g_Block[nCount].col;
			pVtx[nCount * 4 + 3].col = g_Block[nCount].col;

			//テクスチャ座標の設定
			pVtx[nCount * 4 + 0].tex = D3DXVECTOR2(0.01f, 0.01f);
			pVtx[nCount * 4 + 1].tex = D3DXVECTOR2(0.99f, 0.01f);
			pVtx[nCount * 4 + 2].tex = D3DXVECTOR2(0.01f, 0.99f);
			pVtx[nCount * 4 + 3].tex = D3DXVECTOR2(0.99f, 0.99f);

			break;
		}
	}
	//頂点バッファをアンロック
	g_pVtxBuffBlock->Unlock();
}

BLOCK *GetBlock(void)
{
	return &g_Block[0];
}

void ColisionBlock(D3DXVECTOR3 *pos, D3DXVECTOR3 *lastpos, D3DXVECTOR3 *move, int *nJump, int nCnt)
{
	STAGE *pStage;
	pStage = GetStage();

	DATA *pData;
	pData = Getdata();

	for (int nCount = 0; nCount < MAX_BLOCK; nCount++)
	{
		if (g_Block[nCount].bUse == true)
		{
			switch (g_Block[nCount].nType)
			{
			case BLOCK_UD_MOVE:
				(*pos).y -= g_Block[nCount].move.y;
				break;
			}

			if (g_Block[nCount].pos.x - g_Block[nCount].fWidth / 2.0f <= (*pos).x + PLAYER_X &&								//ブロックの左端
				g_Block[nCount].pos.x + g_Block[nCount].fWidth / 2.0f >= (*pos).x - PLAYER_X &&								//ブロックの右端
				g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f <= (*pos).y &&										//ブロックに乗る
				g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f >= (*pos).y - PLAYER_Y)								//ブロックを下からたたく
			{
				if (g_Block[nCount].pos.x - (g_Block[nCount].fWidth / 2.0f) >= (*lastpos).x + PLAYER_X &&
					(g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f <= (*pos).y - 1.0f ||
						g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f >= (*pos).y - PLAYER_Y + 1.0f))		//ブロックの左端
				{
					if (g_Block[nCount].pos.x - (g_Block[nCount].fWidth / 2.0f) >= (*lastpos).x + PLAYER_X &&
						(g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f <= (*pos).y - 1.0f &&
							g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f >= (*pos).y - PLAYER_Y + 1.0f))		//ブロックの左端
					{
						(*pos).x = (g_Block[nCount].pos.x - g_Block[nCount].fWidth / 2.0f) - PLAYER_X - 1.0f;

						g_Block[nCount].nHitP = HIT_LEFT;
					}

					if (g_Block[nCount].nType == BLOCK_BUTTON && bOnceFlag == false)
					{
						for (int n = 0; n < MAX_BLOCK; n++)
						{
							bOnceFlag = true;
							g_Block[n].bSwitch = true;
							g_Block[nCount].col = D3DCOLOR_RGBA(255, 0, 0, 255);
						}
					}

					continue;
				}

				else if (g_Block[nCount].pos.x + (g_Block[nCount].fWidth / 2.0f) <= (*lastpos).x - PLAYER_X &&
						(g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f <= (*pos).y - 1.0f ||
						g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f >= (*pos).y - PLAYER_Y + 1.0f))	//ブロックの右端
				{
					if (g_Block[nCount].pos.x + (g_Block[nCount].fWidth / 2.0f) <= (*lastpos).x - PLAYER_X &&
						(g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f <= (*pos).y - 1.0f &&
							g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f >= (*pos).y - PLAYER_Y + 1.0f))	//ブロックの右端
					{
						(*pos).x = (g_Block[nCount].pos.x + g_Block[nCount].fWidth / 2.0f) + PLAYER_X + 1.0f;

						g_Block[nCount].nHitP = HIT_RIGHT;
					}

					if (g_Block[nCount].nType == BLOCK_BUTTON && bOnceFlag == false)
					{
						for (int n = 0; n < MAX_BLOCK; n++)
						{
							bOnceFlag = true;
							g_Block[n].bSwitch = true;
							g_Block[nCount].col = D3DCOLOR_RGBA(255, 0, 0, 255);
						}
					}

					continue;
				}

				if (g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f >= (*lastpos).y &&
						(g_Block[nCount].pos.x - g_Block[nCount].fWidth / 2.0f <= (*pos).x + PLAYER_X - 1.0f ||
						g_Block[nCount].pos.x + g_Block[nCount].fWidth / 2.0f >= (*pos).x - PLAYER_X + 1.0f))	//ブロックに乗る
				{
					(*pos).y = g_Block[nCount].pos.y - g_Block[nCount].fHeight / 2.0f;
					(*move).y = 0;

					g_Block[nCount].nHitP = HIT_DOWN;
					(*nJump) = 0;

					if (g_Block[nCount].nType == BLOCK_TOGE && pStage->bLock == false)
					{
						pStage->bLock = true;
						pData->nMiss++;
						SetFade(MODE_GAME);
					}

					if (g_Block[nCount].nType == BLOCK_BUTTON && bOnceFlag == false)
					{
						for (int n = 0; n < MAX_BLOCK; n++)
						{
							bOnceFlag = true;
							g_Block[n].bSwitch = true;
							g_Block[nCount].col = D3DCOLOR_RGBA(255, 0, 0, 255);
						}
					}

					continue;
				}

				if (g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f <= (*lastpos).y - PLAYER_Y &&
						(g_Block[nCount].pos.x - g_Block[nCount].fWidth / 2.0f <= (*pos).x + PLAYER_X + 1.0f ||
						g_Block[nCount].pos.x + g_Block[nCount].fWidth / 2.0f >= (*pos).x - PLAYER_X - 1.0f))	//ブロックを下からたたく
				{
					(*pos).y = (g_Block[nCount].pos.y + g_Block[nCount].fHeight / 2.0f) + PLAYER_Y;
					(*move).y = 0.0f;

					g_Block[nCount].nHitP = HIT_UP;
					continue;
				}

				if (g_Block[nCount].nType == BLOCK_BUTTON && bOnceFlag == false)
				{
					for (int n = 0; n < MAX_BLOCK; n++)
					{
						bOnceFlag = true;
						g_Block[n].bSwitch = true;
						g_Block[nCount].col = D3DCOLOR_RGBA(255, 0, 0, 255);
					}
				}

				(*lastpos) = (*pos);
			}
		}
	}
}

void UDMove(int nBlockCount, D3DXVECTOR3 *pos, D3DXVECTOR3 *lastpos, D3DXVECTOR3 *move, int *nJump, int nCnt)
{
	switch (g_Block[nBlockCount].UDMoveScan)
	{
	case 0:
		g_Block[nBlockCount].pos.y -= g_Block[nBlockCount].move.y;
		g_Block[nBlockCount].fMove += fabsf(g_Block[nBlockCount].move.y);

		if (g_Block[nBlockCount].nHitP == HIT_DOWN)
		{
			(*pos).y -= g_Block[nBlockCount].move.y;
			(*lastpos).y -= g_Block[nBlockCount].move.y;
		}

		if (g_Block[nBlockCount].fMove >= g_Block[nBlockCount].fMoveMax)
		{
			g_Block[nBlockCount].fMove = 0.0f;
			g_Block[nBlockCount].UDMoveScan++;
		}
		break;

	case 1:
		g_Block[nBlockCount].fMove++;

		if (g_Block[nBlockCount].fMove >= 32.0f)
		{
			g_Block[nBlockCount].move.y *= -1;
			g_Block[nBlockCount].fMove = 0.0f;
			g_Block[nBlockCount].UDMoveScan++;
		}
		break;

	case 2:
		g_Block[nBlockCount].pos.y -= g_Block[nBlockCount].move.y;
		g_Block[nBlockCount].fMove += fabsf(g_Block[nBlockCount].move.y);

		if (g_Block[nBlockCount].nHitP == HIT_DOWN)
		{
			(*pos).y -= g_Block[nBlockCount].move.y;
			(*lastpos).y -= g_Block[nBlockCount].move.y;
		}

		if (g_Block[nBlockCount].fMove >= g_Block[nBlockCount].fMoveMax)
		{
			g_Block[nBlockCount].fMove = 0.0f;
			g_Block[nBlockCount].UDMoveScan++;
		}
		break;

	case 3:
		g_Block[nBlockCount].fMove++;
		if (g_Block[nBlockCount].fMove >= 32.0f)
		{
			g_Block[nBlockCount].move.y *= -1;
			g_Block[nBlockCount].fMove = 0.0f;
			g_Block[nBlockCount].UDMoveScan = 0;
		}
		break;
	}
}

void LRMove(int nBlockCount, D3DXVECTOR3 *pos, D3DXVECTOR3 *lastpos, D3DXVECTOR3 *move, int *nJump, int nCnt)
{
	PLAYER *pPlayer;
	pPlayer = GetPlayer();

	switch (g_Block[nBlockCount].LRMoveScan)
	{
	case 0:
		g_Block[nBlockCount].pos.x -= g_Block[nBlockCount].move.x;
		g_Block[nBlockCount].fMove += fabsf(g_Block[nBlockCount].move.x);

		if (g_Block[nBlockCount].nHitP == HIT_DOWN)
		{
			pPlayer->pos.x -= g_Block[nBlockCount].move.x;
		}

		if (g_Block[nBlockCount].fMove >= g_Block[nBlockCount].fMoveMax)
		{
			g_Block[nBlockCount].fMove = 0.0f;
			g_Block[nBlockCount].LRMoveScan++;

		}
		break;

	case 1:
		g_Block[nBlockCount].fMove++;

		if (g_Block[nBlockCount].fMove >= 32.0f)
		{
			g_Block[nBlockCount].move.x *= -1;
			g_Block[nBlockCount].fMove = 0.0f;
			g_Block[nBlockCount].LRMoveScan++;
		}

		break;

	case 2:
		g_Block[nBlockCount].pos.x -= g_Block[nBlockCount].move.x;
		g_Block[nBlockCount].fMove += fabsf(g_Block[nBlockCount].move.x);

		if (g_Block[nBlockCount].nHitP == HIT_DOWN)
		{
			pPlayer->pos.x -= g_Block[nBlockCount].move.x;
		}

		if (g_Block[nBlockCount].fMove >= g_Block[nBlockCount].fMoveMax)
		{
			g_Block[nBlockCount].fMove = 0.0f;
			g_Block[nBlockCount].LRMoveScan++;
		}
		break;

	case 3:
		g_Block[nBlockCount].fMove++;

		if (g_Block[nBlockCount].fMove >= 32.0f)
		{
			g_Block[nBlockCount].move.x *= -1;
			g_Block[nBlockCount].fMove = 0.0f;
			g_Block[nBlockCount].LRMoveScan = 0;
		}
		break;
	}
}

void Door(int nBlockCount)
{
	if (g_Block[nBlockCount].bSwitch == true)
	{
		if (bOnceDoor == false)
		{
			PlaySound(SOUND_LABEL_SEDOOR);
			bOnceDoor = !bOnceDoor;
		}
		g_Block[nBlockCount].pos.x += g_Block[nBlockCount].move.x;
		abs(g_Block[nBlockCount].LRMoveScan++);

		if (g_Block[nBlockCount].LRMoveScan >= g_Block[nBlockCount].fMoveMax)
		{
			g_Block[nBlockCount].LRMoveScan = 0;
			g_Block[nBlockCount].move.x *= -1;
			g_Block[nBlockCount].bSwitch = false;
		}
	}
}