#ifndef _PAUSEMENU_H_
#define _PAUSEMENU_H_

#include "main.h"

//プロトタイプ宣言
void InitPauseMenu(void);		//初期化
void UninitPauseMenu(void);		//終了
void UpdatePauseMenu(void);		//更新
void DrawPauseMenu(void);		//描画
bool GetPauseMenu(void);

#endif //_PAUSEMENU_H_