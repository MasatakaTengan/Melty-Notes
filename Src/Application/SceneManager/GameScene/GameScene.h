#pragma once

#include "../BaseScene/BaseScene.h"

using namespace Constant;

class BMSLoader;
class NoteManager;

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
	//void PostDraw()override;

	void DrawSprite()override;

private:

	KdTexture m_noteTex;
	KdTexture m_keyBackTex;
	KdTexture m_hitEffectTex;

	bool mb_bgm;		//再生中か
	std::string_view m_bgmFile;
	std::shared_ptr<KdSoundInstance> m_bgmInstance = nullptr;
	std::string_view m_hitSEFile;

	BMSLoader bms;

	float			mf_scrollMulti;			// 小節間の幅の倍率
	LONGLONG		mll_startTime;		// ゲーム開始時の時間(高解像度タイマー)
	LONGLONG		mll_beforeTime;		// 前ループの時間(高解像度タイマー)
	LONGLONG		mll_elapsedTime;		// 経過時間(高解像度タイマー[ms])
	LONGLONG		mll_globalFreq;		// マシンの１秒間のカウント数(高解像度タイマー)
	double			md_elapsedTime;		// 開始からの経過時間(秒)
	KEYSTATE	m_keyState[5];			// 鍵盤キーが押されているか
	//int				mi_startNum[256];		// BMS演奏で計算開始する配列番号(処理を軽くするため)

	//int				mi_flashIndex[6];		// 次に使用されるフラッシュカウンタのインデックス
	//int				mi_flashCount[6][3];	// フラッシュ６ｘ３個分のカウンタ
	int mi_backKeyCount[4];			// キーを離した時の後ろのバックライトの演出用カウンタ
	int mi_hitEffectCount[4];

	//debug
	int mi_pressNum;
	int mi_hitSubNum;
	char m_jadge[100];

	std::shared_ptr<NoteManager> m_noteManager = nullptr;

};