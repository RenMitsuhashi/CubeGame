#ifndef _SCORE_H_
#define _SCORE_H_

#include <math.h>
#include "main.h"

//�}�N����`
#define MAX_SCORE (999999)			//�X�R�A�̍ő吔
#define MAX_SCOREPORYGON (6)		//�X�R�A�̌���

//�v���g�^�C�v�錾
void InitScore(void);
void UninitScore(void);
void UpdateScore(void);
void DrawScore(void);
void AddScore(int nValue);
void SetScore(D3DXVECTOR3 pos);

#endif //_SCORE_H_