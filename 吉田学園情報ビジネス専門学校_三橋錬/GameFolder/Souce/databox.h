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

//�v���g�^�C�v�錾
void Initdata(void);		//������
DATA *Getdata(void);

#endif //_BG_TUTORIAL_H_