#ifndef _DATABOX_H_
#define _DATABOX_H_

#include "main.h"

typedef struct
{
	int nNowStage;
	int nScore;
	int nMiss;
	int nTime;
}DATA;

//プロトタイプ宣言
void Initdata(void);		//初期化
DATA *Getdata(void);

#endif //_BG_TUTORIAL_H_