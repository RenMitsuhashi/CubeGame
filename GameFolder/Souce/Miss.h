#ifndef _MISS_H_
#define _MISS_H_

#include "main.h"

//�}�N����`
#define MAX_MISS (99)			//���Ԃ̍ő吔
#define MAX_MISSPORYGON (2)		//���Ԃ̌���

#define MISS_X (30)
#define MISS_Y (70)


//�v���g�^�C�v�錾
void InitMiss(void);
void UninitMiss(void);
void UpdateMiss(void);
void DrawMiss(void);
void SetMiss(D3DXVECTOR3 pos);

#endif //_MISS_H_