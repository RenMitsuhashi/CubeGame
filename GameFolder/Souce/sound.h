//Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{
// ÉTÉEÉìÉhèàóù [sound.h]
// Author : SAKAI KAZUTO
//Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{Å{
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

//=================================
// ÉTÉEÉìÉhÉtÉ@ÉCÉã
//=================================
typedef enum
{
	SOUND_LABEL_BGMTITLE = 0,		// BGMÉ^ÉCÉgÉã
	SOUND_LABEL_SETITLE,			// SEÉ^ÉCÉgÉã
	SOUND_LABEL_BGMTUTORIAL,		// BGMÉ`ÉÖÅ[ÉgÉäÉAÉã
	SOUND_LABEL_SECURSOR,			// SEÉJÅ[É\Éã
	SOUND_LABEL_SEJUMP,				// SEÉWÉÉÉìÉv
	SOUND_LABEL_SEENTER,			// SEÉGÉìÉ^Å[
	SOUND_LABEL_BGMGAME,			// BGMÉQÅ[ÉÄ
	SOUND_LABEL_SEDOOR,				// SEÉhÉA
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//=============================================================================
// ÉvÉçÉgÉ^ÉCÉvêÈåæ
//=============================================================================
HRESULT InitSound(HWND hWnd);
void UninitSound(void);
HRESULT PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void StopSound(void);

#endif
