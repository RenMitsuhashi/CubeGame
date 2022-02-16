#ifndef _TITLE_CUBE_H_
#define _TITLE_CUBE_H_

#include "main.h"

typedef struct
{
	D3DXVECTOR3 pos;					//位置
	D3DXVECTOR3 move;					//移動量
	bool bUse;							//使用判定
	bool bHit;
	int nCoolTime;						//再使用までの時間
	int nCounterAnim;
	float fTex;
	int nRBY;
	int nTimer;
} CUBE;

//プロトタイプ宣言
void InitCube(void);							//初期化
void UninitCube(void);							//終了
void UpdateCube(void);							//更新
void DrawCube(void);							//描画
void SetCubeRed(D3DXVECTOR3 pos, float move, int cnt);		//セット
void SetCubeBlue(D3DXVECTOR3 pos, float move, int cnt);		//セット
void SetCubeYellow(D3DXVECTOR3 pos, float move, int cnt);	//セット

#endif //_PLAYER_H_