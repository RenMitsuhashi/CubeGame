#ifndef _FADE_H_
#define _FADE_H_

#include "main.h"

typedef enum
{
	FADE_NONE = 0,
	FADE_IN,
	FADE_OUT,
	FADE_MAX
} FADE;

//プロトタイプ宣言
void InitFade(MODE modeNext);	//初期化
void UninitFade(void);			//終了
void UpdateFade(void);			//更新
void DrawFade(void);			//描画
void SetFade(MODE modeNext);
FADE GetFade(void);

#endif //_FADE_H_