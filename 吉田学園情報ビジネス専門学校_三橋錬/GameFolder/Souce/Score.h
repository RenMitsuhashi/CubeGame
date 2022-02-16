#ifndef _SCORE_H_
#define _SCORE_H_

#include <math.h>
#include "main.h"

//マクロ定義
#define MAX_SCORE (999999)			//スコアの最大数
#define MAX_SCOREPORYGON (6)		//スコアの桁数

//プロトタイプ宣言
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void AddScore(int nValue);
void SetScore(D3DXVECTOR3 pos);

#endif //_SCORE_H_