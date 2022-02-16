#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

#define COUNTER_ANIM (8)
#define MAX_PLAYER (3)
#define JUMP_MAX (1)					//ジャンプの最大数
#define MOVE_MAX (5.0f)					//最大移動量
#define MAX_GRAVITY (18.0f)				//落下の最高速度
#define PLAYER_X (40.0f)
#define PLAYER_Y (80.0f)

typedef struct
{
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 lastpos;				//前回位置
	D3DXVECTOR3 move;					//移動量
	bool bUse;							//使用判定
	int nJump = JUMP_MAX;				//ジャンプできる最大数
} PLAYER;

typedef enum
{
	TYPE_RED = 0,
	TYPE_BLUE,
	TYPE_YELLOW,
	TYPE_MAX
} TYPE;

typedef enum
{
	BLOCK_RED = 10,
	BLOCK_BLUE,
	BLOCK_YELLOW,
	SET_MAX
} SETPLAYER;

//プロトタイプ宣言
void InitPlayer(void);							//初期化
void UninitPlayer(void);						//終了
void UpdatePlayer(void);						//更新
void DrawPlayer(void);							//描画
void SetPlayerRed(D3DXVECTOR3 pos);				//セット
void SetPlayerBlue(D3DXVECTOR3 pos);			//セット
void SetPlayerYellow(D3DXVECTOR3 pos);			//セット
PLAYER *GetPlayer(void);						//情報取得
void ColisionPlayer(int nCnt, int nCnt2);		//プレイヤー同士の当たり判定

#endif //_PLAYER_H_