//�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{
// �T�E���h���� [sound.h]
// Author : SAKAI KAZUTO
//�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{�{
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//=================================
// �T�E���h�t�@�C��
//=================================
typedef enum
{
	SOUND_LABEL_BGMTITLE = 0,		// BGM�^�C�g��
	SOUND_LABEL_SETITLE,			// SE�^�C�g��
	SOUND_LABEL_BGMTUTORIAL,		// BGM�`���[�g���A��
	SOUND_LABEL_SECURSOR,			// SE�J�[�\��
	SOUND_LABEL_SEJUMP,				// SE�W�����v
	SOUND_LABEL_SEENTER,			// SE�G���^�[
	SOUND_LABEL_BGMGAME,			// BGM�Q�[��
	SOUND_LABEL_SEDOOR,				// SE�h�A
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//=============================================================================
// �v���g�^�C�v�錾
//=============================================================================
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
