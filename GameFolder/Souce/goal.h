#ifndef _GOAL_H_
#define _GOAL_H_

#include "main.h"

#define BLOCK_GOAL (5)

typedef struct
{
	D3DXVECTOR3 pos;
	bool bHit;
	bool bUse;
}GOAL;

//�v���g�^�C�v�錾
void Initgoal(void);	//������
void Uninitgoal(void);	//�I��
void Updategoal(void);	//�X�V
void Drawgoal(void);	//�`��
void Setgoal(D3DXVECTOR3 pos);

#endif //_GOAL_H_