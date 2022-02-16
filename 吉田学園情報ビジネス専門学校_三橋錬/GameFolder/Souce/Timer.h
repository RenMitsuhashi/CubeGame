#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"

//マクロ定義
#define MAX_TIME (999)			//時間の最大数
#define MAX_TIMEPORYGON (3)		//時間の桁数

#define TIMER_X (30)
#define TIMER_Y (70)


//プロトタイプ宣言
void InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);
void SetTimer(D3DXVECTOR3 pos);
int GetTimer(void);

#endif //_TIMER_H_