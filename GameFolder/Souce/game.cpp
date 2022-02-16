#include "game.h"
#include "bg.h"
#include "input.h"
#include "fade.h"
#include "bg_tutorial.h"
#include "bg_pause.h"
#include "PauseMenu.h"
#include "bg.h"
#include "Player.h"
#include "block.h"
#include "Timer.h"
#include "goal.h"
#include "databox.h"
#include <stdio.h>
#include "sound.h"

//グローバル変数
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffGame = NULL;		//頂点バッファへのポインタ
LPDIRECT3DTEXTURE9 g_pTextureGame = NULL;			//テクスチャへのポインタ
int IsSwitchButton = false;
STAGE g_Stage;
int g_aStageData[MAX_LINE][MAX_ROW];//ステージの地形データ(チュートリアル)

//初期化処理
void InitGame(void)
{
	DATA *pData;
	pData = Getdata();

	LPDIRECT3DDEVICE9 pDevice;			//デバイスのポインタ
	pDevice = GetDevice();				//デバイスを取得

	//テクスチャ読み込み
	D3DXCreateTextureFromFile(
		pDevice,
		"data\\picture\\black.png",
		&g_pTextureGame);

	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffGame,
		NULL);

	g_Stage.nStageType = pData->nNowStage;
	g_Stage.nScore = pData->nScore;
	g_Stage.bSetScan = false;
	g_Stage.bClear = false;
	g_Stage.bPause = false;
	g_Stage.nMiss = pData->nMiss;
	g_Stage.nTime = 0;
	g_Stage.bLock = false;
	

	VERTEX_2D *pVtx;	//頂点情報へのポインタ

	//頂点バッファをロックし、頂点データへのポインタを取得
	g_pVtxBuffGame->Lock(0, 0, (void**)&pVtx, 0);

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
	g_pVtxBuffGame->Unlock();

	InitPlayer();
	Initgoal();
	Initbg();
	InitBlock();
	InitPause();
	InitPauseMenu();
	InitTimer();
	SetTimer(D3DXVECTOR3(180.0f, 33.0f, 0.0f));

	if (g_Stage.nStageType == STAGE_TUTORIAL)	Initbgtuto();

	IsSwitchButton = false;
}

//終了処理
void UninitGame(void)
{
	//テクスチャ破棄
	if (g_pTextureGame != NULL)
	{
		g_pTextureGame->Release();
		g_pTextureGame = NULL;
	}

	//頂点バッファ破棄
	if (g_pVtxBuffGame != NULL)
	{
		g_pVtxBuffGame->Release();
		g_pVtxBuffGame = NULL;
	}
	UninitPlayer();
	Uninitgoal();
	Uninitbg();
	UninitBlock();
	UninitPause();
	UninitPauseMenu();
	UninitTimer();

	if (g_Stage.nStageType == STAGE_TUTORIAL)	Uninitbgtuto();
}

//更新処理
void UpdateGame(void)
{
	DATA *pData;
	pData = Getdata();

	if (g_Stage.bSetScan == false)
	{
		//--------------------------------------------------------------
		//ステージデータのリセット
		for (int nCntY = 0; nCntY < MAX_LINE; nCntY++)
		{
			for (int nCntX = 0; nCntX < MAX_ROW; nCntX++)
			{
				g_aStageData[nCntY][nCntX] = 0;
			}
		}
		StopSound();

		//--------------------------------------------------------------



		//--------------------------------------------------------------
		//ステージデータ読み込み、ステージ生成
		LoadStage();

		for (int nCntY = 0; nCntY < MAX_LINE; nCntY++)
		{
			for (int nCntX = 0; nCntX < MAX_ROW; nCntX++)
			{
				D3DXVECTOR3 pos = D3DXVECTOR3(0.0f + 20.0f * nCntX, 0.0f + 20.0f * nCntY, 0.0f);
				int nNum = g_aStageData[nCntY][nCntX];

				switch (nNum)
				{
				case BLOCK_NORMAL:
					SetBlock(pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), 20.0f, 20.0f, BLOCK_NORMAL, 0.0f);
					break;

				case BLOCK_LR_MOVE:
					SetBlock(pos, D3DXVECTOR3(0.2f, 0.0f, 0.0f), 20.0f, 20.0f, BLOCK_LR_MOVE, 100.0f);
					break;

				case BLOCK_DOOR:
					SetBlock(pos, D3DXVECTOR3(0.7f, 0.0f, 0.0f), 20.0f, 20.0f, BLOCK_DOOR, 220.0f);
					break;

				case BLOCK_GOAL:
					Setgoal(pos);
					break;

				case BLOCK_TOGE:
					SetBlock(pos, D3DXVECTOR3(0.7f, 0.0f, 0.0f), 20.0f, 20.0f, BLOCK_TOGE, 120.0f);
					break;

				case BLOCK_BUTTON:
					SetBlock(pos, D3DXVECTOR3(0.7f, 0.0f, 0.0f), PLAYER_X / 2, PLAYER_Y / 2, BLOCK_BUTTON, 120.0f);
					break;

				case BLOCK_RED:
					SetPlayerRed(pos);
					break;

				case BLOCK_BLUE:
					SetPlayerBlue(pos);
					break;

				case BLOCK_YELLOW:
					SetPlayerYellow(pos);
					break;

				default:
					break;
				}	
			}
		}

		//--------------------------------------------------------------



		switch (g_Stage.nStageType)
		{
		case STAGE_TUTORIAL:
			Setbgtuto();
			PlaySound(SOUND_LABEL_BGMTUTORIAL);
			break;

		default:
			PlaySound(SOUND_LABEL_BGMGAME);
			break;
		}
		g_Stage.bSetScan = true;
	}



	//--------------------------------------------------------------
	//それぞれ更新

	if (g_Stage.bPause == false)
	{
		if (g_Stage.bLock == false)
		{
			UpdatePlayer();
			Updatebg();
			UpdateBlock();

			if (g_Stage.nStageType != STAGE_TUTORIAL)	UpdateTimer();		//タイマー(チュートリアルでは使わないヨ)

		}

		Updategoal();

		if (g_Stage.nStageType == STAGE_TUTORIAL)	Updatebgtuto();		//チュートリアル専用のやつ
	}

	else
	{
		UpdatePause();
		UpdatePauseMenu();
	}

	//--------------------------------------------------------------



	//--------------------------------------------------------------
	//ゴール
	if (g_Stage.bClear == true &&
		g_Stage.nStageType != STAGE_TUTORIAL &&
		g_Stage.bLock == false)
	{
		//--------------------------------------------------------------
		//スコア加算
		pData->nScore += ((1000 - pData->nTime) * 8);
		pData->nScore -= pData->nMiss * 128;

		//--------------------------------------------------------------



		pData->nNowStage++;
		SetFade(MODE_RESULT);
		g_Stage.bLock = true;
	}

	//--------------------------------------------------------------



	//--------------------------------------------------------------
	//ポーズ
	if (GetKeyboardTrigger(DIK_ESCAPE) == true)
	{
		g_Stage.bPause = !g_Stage.bPause;
	}

	//--------------------------------------------------------------
}

//描画処理
void DrawGame(void)
{
	if (g_Stage.bPause == false)
	{
		Drawbg();

		if (g_Stage.nStageType == STAGE_TUTORIAL)	Drawbgtuto();

		DrawPlayer();
		DrawBlock();
		Drawgoal();
		DrawTimer();
	}

	else
	{
		DrawPause();
		DrawPauseMenu();
	}
}

//ステージのロード
void LoadStage(void)
{
	DATA *pData;
	pData = Getdata();

	FILE *pFile;
	int nCntX = 0, nCntY = 0;

	g_Stage.nStageType = pData->nNowStage;

	switch (g_Stage.nStageType)
	{
	case STAGE_TUTORIAL:
		pFile = fopen("data\\stage\\Tutorial.csv", "r");
		break;

	case STAGE_1:
		pFile = fopen("data\\stage\\Stage1.csv", "r");
		break;

	case STAGE_2:
		pFile = fopen("data\\stage\\Stage2.csv", "r");
		break;

	default:
		pFile = fopen("data\\stage\\Tutorial.csv", "r");
		break;
	}

	char aLine[256];
	while (fgets(aLine, 256, pFile) != NULL)
	{
		nCntX = 0;
		char *pToken = strtok(aLine, ",");
		while (pToken != NULL)
		{
			int nNum = atoi(pToken);
			g_aStageData[nCntY][nCntX] = nNum;
			pToken = strtok(NULL, ",");
			nCntX++;
		}
		nCntY++;
	}
	fclose(pFile);
}

STAGE *GetStage(void)
{
	return &g_Stage;
}