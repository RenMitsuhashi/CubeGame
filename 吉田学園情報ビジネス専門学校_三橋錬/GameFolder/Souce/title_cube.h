#ifndef _TITLE_CUBE_H_
#define _TITLE_CUBE_H_

#include "main.h"

typedef struct
{
	D3DXVECTOR3 pos;					//�ʒu
	D3DXVECTOR3 move;					//�ړ���
	bool bUse;							//�g�p����
	bool bHit;
	int nCoolTime;						//�Ďg�p�܂ł̎���
	int nCounterAnim;
	float fTex;
	int nRBY;
	int nTimer;
} CUBE;

//�v���g�^�C�v�錾
void InitCube(void);							//������
void UninitCube(void);							//�I��
void UpdateCube(void);							//�X�V
void DrawCube(void);							//�`��
void SetCubeRed(D3DXVECTOR3 pos, float move, int cnt);		//�Z�b�g
void SetCubeBlue(D3DXVECTOR3 pos, float move, int cnt);		//�Z�b�g
void SetCubeYellow(D3DXVECTOR3 pos, float move, int cnt);	//�Z�b�g

#endif //_PLAYER_H_