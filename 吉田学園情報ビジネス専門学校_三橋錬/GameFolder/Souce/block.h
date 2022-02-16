#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

#define MAX_BLOCK (1024)

//ブロックの構造体
typedef struct
{
	D3DXVECTOR3 pos;		//位置
	D3DXVECTOR3 move;		//移動量
	float fWidth;			//幅
	float fHeight;			//高さ
	bool bUse;				//使用判定
	int nType;				//ブロックの種類
	float fMove;			//計移動量
	float fMoveMax;			//移動量の限界
	int UDMoveScan;			//UDMoveでつかうよ
	int LRMoveScan; 		//LRMoveでつかうよ
	bool bSwitch;			//(Door)スイッチが押されたか
	int nHitP;				//どこに当たったか判定(プレイヤー)
	D3DCOLOR col;			//色
} BLOCK;

//ブロックの動き方
typedef enum
{
	BLOCK_NORMAL = 1,		//静止
	BLOCK_LR_MOVE,			//左右移動
	BLOCK_UD_MOVE,			//上下移動
	BLOCK_DOOR,				//スイッチに反応して開くやつ
	//BLOCK_GOAL,
	BLOCK_TOGE = 6,			//上踏んだらいたいやつ
	BLOCK_BUTTON,			//ドア用のボタン
	BLOCK_MAX
} BLOCKTYPE;

//どこに当たったか
typedef enum
{
	HIT_NONE = 0,
	HIT_LEFT,
	HIT_RIGHT,
	HIT_DOWN,
	HIT_UP,
	HIT_MAX
} HITBLOCK;

//プロトタイプ宣言
void InitBlock(void);		//初期化
void UninitBlock(void);		//終了
void UpdateBlock(void);		//更新
void DrawBlock(void);		//描画
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, int nType, float movemax);
BLOCK *GetBlock(void);
void ColisionBlock(D3DXVECTOR3 *pos, D3DXVECTOR3 *lastpos, D3DXVECTOR3 *move, int *nJump, int nCnt);

//ブロックの種類別の行動
void UDMove(int nBlockCount, D3DXVECTOR3 *pos, D3DXVECTOR3 *lastpos, D3DXVECTOR3 *move, int *nJump, int nCnt);
void LRMove(int nBlockCount, D3DXVECTOR3 *pos, D3DXVECTOR3 *lastpos, D3DXVECTOR3 *move, int *nJump, int nCnt);
void Door(int nBlockCount);

#endif //_BLOCK_H_