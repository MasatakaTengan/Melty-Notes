#pragma once

#include "../BaseScene/BaseScene.h"

using namespace Constant;

class BMSLoader;

class GameScene : public BaseScene
{
public:

	GameScene() { Init(); }
	~GameScene() {}

	void Init()override;
	void Event()override;
	void PreUpdate()override;
	void Update()override;
	void PostUpdate()override;

	void PreDraw()override;
	void DrawLit()override;
	void DrawUnLit()override;
	void PostDraw()override;

	void DrawSprite()override;

private:

	KdTexture m_noteTex;
	KdTexture m_keyBackTex;
	KdTexture m_hitEffectTex;

	bool mb_bgm;
	std::string_view m_bgmFile;
	std::string_view m_hitSEFile;

	BMSLoader bms;

	float			fScrMulti;			// 小節間の幅の倍率
	LONGLONG		llStartTime;		// ゲーム開始時の時間(高解像度タイマー)
	LONGLONG		llGlobalFreq;		// マシンの１秒間のカウント数(高解像度タイマー)
	double			dElapsedTime;		// 開始からの経過時間(秒)
	BOOL			bOnKey[5];			// 鍵盤キーが押されているか
	int				iStartNum[256];		// BMS演奏で計算開始する配列番号(処理を軽くするため)

	int				iFlashIndex[6];		// 次に使用されるフラッシュカウンタのインデックス
	int				iFlashCount[6][3];	// フラッシュ６ｘ３個分のカウンタ
	int				iBackKeyCount[6];	// キーを離した時の後ろのバックライトの演出用カウンタ

	//debug
	int m_hitSubNum;
	char m_jadge[100];

};