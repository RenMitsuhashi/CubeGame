#ifndef _BLOCK_H_
#define _BLOCK_H_

#include "main.h"

#define MAX_BLOCK (1024)

//�u���b�N�̍\����
typedef struct
{
	D3DXVECTOR3 pos;		//�ʒu
	D3DXVECTOR3 move;		//�ړ���
	float fWidth;			//��
	float fHeight;			//����
	bool bUse;				//�g�p����
	int nType;				//�u���b�N�̎��
	float fMove;			//�v�ړ���
	float fMoveMax;			//�ړ��ʂ̌��E
	int UDMoveScan;			//UDMove�ł�����
	int LRMoveScan; 		//LRMove�ł�����
	bool bSwitch;			//(Door)�X�C�b�`�������ꂽ��
	int nHitP;				//�ǂ��ɓ�������������(�v���C���[)
	D3DCOLOR col;			//�F
} BLOCK;

//�u���b�N�̓�����
typedef enum
{
	BLOCK_NORMAL = 1,		//�Î~
	BLOCK_LR_MOVE,			//���E�ړ�
	BLOCK_UD_MOVE,			//�㉺�ړ�
	BLOCK_DOOR,				//�X�C�b�`�ɔ������ĊJ�����
	//BLOCK_GOAL,
	BLOCK_TOGE = 6,			//�㓥�񂾂炢�������
	BLOCK_BUTTON,			//�h�A�p�̃{�^��
	BLOCK_MAX
} BLOCKTYPE;

//�ǂ��ɓ���������
typedef enum
{
	HIT_NONE = 0,
	HIT_LEFT,
	HIT_RIGHT,
	HIT_DOWN,
	HIT_UP,
	HIT_MAX
} HITBLOCK;

//�v���g�^�C�v�錾
void InitBlock(void);		//������
void UninitBlock(void);		//�I��
void UpdateBlock(void);		//�X�V
void DrawBlock(void);		//�`��
void SetBlock(D3DXVECTOR3 pos, D3DXVECTOR3 move, float fWidth, float fHeight, int nType, float movemax);
BLOCK *GetBlock(void);
void ColisionBlock(D3DXVECTOR3 *pos, D3DXVECTOR3 *lastpos, D3DXVECTOR3 *move, int *nJump, int nCnt);

//�u���b�N�̎�ޕʂ̍s��
void UDMove(int nBlockCount, D3DXVECTOR3 *pos, D3DXVECTOR3 *lastpos, D3DXVECTOR3 *move, int *nJump, int nCnt);
void LRMove(int nBlockCount, D3DXVECTOR3 *pos, D3DXVECTOR3 *lastpos, D3DXVECTOR3 *move, int *nJump, int nCnt);
void Door(int nBlockCount);

#endif //_BLOCK_H_