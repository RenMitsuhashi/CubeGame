#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

#define MAX_LINE (37)
#define MAX_ROW (65)

typedef enum
{
	STAGE_TUTORIAL = 0,
	STAGE_1,
	STAGE_2,
	STAGE_MAX
} STAGETYPE;

typedef struct
{
	int nStageType;
	bool bSetScan;
	int nTime;
	int nMiss;
	int nScore;
	bool bPause;
	bool bClear;
	bool bLock;
} STAGE;

//プロトタイプ宣言
void InitGame(void);	//初期化
void UninitGame(void);	//終了
void UpdateGame(void);	//更新
void DrawGame(void);	//描画
void LoadStage(void);	//ステージデータのロード
STAGE *GetStage(void);

#endif //_GAME_H_