#ifndef _MISS_H_
#define _MISS_H_

#include "main.h"

//マクロ定義
#define MAX_MISS (99)			//時間の最大数
#define MAX_MISSPORYGON (2)		//時間の桁数

#define MISS_X (30)
#define MISS_Y (70)


//プロトタイプ宣言
void InitMiss(void);
void UninitMiss(void);
void UpdateMiss(void);
void DrawMiss(void);
void SetMiss(D3DXVECTOR3 pos);

#endif //_MISS_H_