#ifndef _MAIN_H_
#define _MAIN_H_
#define DIRECTINPUT_VERSION (0x0800)

#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include "d3dx9.h"
#include "dinput.h"
#include "xaudio2.h"

//ライブラリのリンク
#pragma comment(lib,"d3d9.lib")			//描画に必要
#pragma comment(lib,"d3dx9.lib")		//[d3d9.lib]の拡張ライブラリ
#pragma comment(lib,"dxguid.lib")		//DirectXコンポーネント(部品のコト)使用に必要
#pragma comment(lib,"dinput8.lib")		//入力処理に必要
#pragma comment(lib,"winmm.lib")

//マクロ定義
#define SCREEN_WIDTH (1280)												//ウィンドウの幅
#define SCREEN_HEIGHT (720)												//ウィンドウの高さ
#define FVF_VERTEX_2D (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)	//頂点フォーマット

//構造体定義
typedef struct
{
	D3DXVECTOR3 pos;	//頂点座標
	float rhw;			//1.0f固定
	D3DCOLOR col;		//頂点カラー
	D3DXVECTOR2 tex;	//テクスチャ座標
}VERTEX_2D;

typedef enum
{
	MODE_TITLE = 0,
	MODE_GAME,
	MODE_RESULT,
	MODE_RANK,
	MODE_MAX
} MODE;

//プロトタイプ宣言
LPDIRECT3DDEVICE9 GetDevice(void);
void SetMode(MODE mode);
MODE GetMode(void);

#endif //_MAIN_H_