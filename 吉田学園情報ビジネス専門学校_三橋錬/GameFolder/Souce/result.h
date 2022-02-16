#ifndef _RESULT_H_
#define _RESULT_H_

#include "main.h"

//マクロ定義
#define MAX_MISS (99)			//ミスの最大数
#define MAX_MISSPORYGON (2)		//ミスの桁数

//プロトタイプ宣言
void InitResult(void);		//初期化
void UninitResult(void);	//終了
void UpdateResult(void);	//更新
void DrawResult(void);		//描画

#endif //_RESULT_H_