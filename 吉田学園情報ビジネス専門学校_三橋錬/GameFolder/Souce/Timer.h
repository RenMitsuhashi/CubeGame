#ifndef _TIMER_H_
#define _TIMER_H_

#include "main.h"

//�}�N����`
#define MAX_TIME (999)			//���Ԃ̍ő吔
#define MAX_TIMEPORYGON (3)		//���Ԃ̌���

#define TIMER_X (30)
#define TIMER_Y (70)


//�v���g�^�C�v�錾
void InitTimer(void);
void UninitTimer(void);
void UpdateTimer(void);
void DrawTimer(void);
void SetTimer(D3DXVECTOR3 pos);
int GetTimer(void);

#endif //_TIMER_H_