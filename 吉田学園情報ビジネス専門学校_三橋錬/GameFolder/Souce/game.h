#ifndef _GAME_H_
#define _GAME_H_

#include "main.h"

#define MAX_LINE (37)
#define MAX_ROW (65)

typedef enum
{
	STAGE_TUTORIAL = 0,
	STAGE_1,
	STAGE_2,
	STAGE_MAX
} STAGETYPE;

typedef struct
{
	int nStageType;
	bool bSetScan;
	int nTime;
	int nMiss;
	int nScore;
	bool bPause;
	bool bClear;
	bool bLock;
} STAGE;

//�v���g�^�C�v�錾
void InitGame(void);	//������
void UninitGame(void);	//�I��
void UpdateGame(void);	//�X�V
void DrawGame(void);	//�`��
void LoadStage(void);	//�X�e�[�W�f�[�^�̃��[�h
STAGE *GetStage(void);

#endif //_GAME_H_