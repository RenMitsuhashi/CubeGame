#ifndef _RANK_H_
#define _RANK_H_

#include "main.h"

#define MAX_RANK (6)
#define RANK_X (30.0f)
#define RANK_Y (60.0f)

typedef struct
{
	D3DXVECTOR3 pos;
	int nNum;
	int col;
} RANK;

//プロトタイプ宣言
void InitRank(void);
void UninitRank(void);
void UpdateRank(void);
void DrawRank(void);

#endif //_RANK_H_