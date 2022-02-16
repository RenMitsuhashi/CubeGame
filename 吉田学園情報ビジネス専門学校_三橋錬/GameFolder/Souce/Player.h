#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"

#define COUNTER_ANIM (8)
#define MAX_PLAYER (3)
#define JUMP_MAX (1)					//�W�����v�̍ő吔
#define MOVE_MAX (5.0f)					//�ő�ړ���
#define MAX_GRAVITY (18.0f)				//�����̍ō����x
#define PLAYER_X (40.0f)
#define PLAYER_Y (80.0f)

typedef struct
{
	D3DXVECTOR3 pos;					//�ʒu
	D3DXVECTOR3 lastpos;				//�O��ʒu
	D3DXVECTOR3 move;					//�ړ���
	bool bUse;							//�g�p����
	int nJump = JUMP_MAX;				//�W�����v�ł���ő吔
} PLAYER;

typedef enum
{
	TYPE_RED = 0,
	TYPE_BLUE,
	TYPE_YELLOW,
	TYPE_MAX
} TYPE;

typedef enum
{
	BLOCK_RED = 10,
	BLOCK_BLUE,
	BLOCK_YELLOW,
	SET_MAX
} SETPLAYER;

//�v���g�^�C�v�錾
void InitPlayer(void);							//������
void UninitPlayer(void);						//�I��
void UpdatePlayer(void);						//�X�V
void DrawPlayer(void);							//�`��
void SetPlayerRed(D3DXVECTOR3 pos);				//�Z�b�g
void SetPlayerBlue(D3DXVECTOR3 pos);			//�Z�b�g
void SetPlayerYellow(D3DXVECTOR3 pos);			//�Z�b�g
PLAYER *GetPlayer(void);						//���擾
void ColisionPlayer(int nCnt, int nCnt2);		//�v���C���[���m�̓����蔻��

#endif //_PLAYER_H_