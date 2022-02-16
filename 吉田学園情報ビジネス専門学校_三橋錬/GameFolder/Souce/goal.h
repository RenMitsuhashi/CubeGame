#ifndef _GOAL_H_
#define _GOAL_H_

#include "main.h"

#define BLOCK_GOAL (5)

typedef struct
{
	D3DXVECTOR3 pos;
	bool bHit;
	bool bUse;
}GOAL;

//プロトタイプ宣言
void Initgoal(void);	//初期化
void Uninitgoal(void);	//終了
void Updategoal(void);	//更新
void Drawgoal(void);	//描画
void Setgoal(D3DXVECTOR3 pos);

#endif //_GOAL_H_