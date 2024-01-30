#pragma once

#include "../BaseScene/BaseScene.h"

using namespace Constant;

class BMSLoader;
class NoteManager;
class Score;
class KeyBack;
class HitEffect;

class GameScene : public BaseScene, public std::enable_shared_from_this<GameScene>
{
public:

	GameScene() {}
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

	void AddHitEffect( Math::Vector2 _pos, JADGE _jadge );

private:

	KdTexture m_noteTex;
	KdTexture m_keyBackTex;
	std::shared_ptr<KdTexture> msp_hitEffectTex;

	bool mb_bgmStart;		//再生開始したか
	bool mb_bgmPlay;		//再生中か
	std::string_view m_bgmFile;
	std::shared_ptr<KdSoundInstance> m_bgmInstance = nullptr;
	std::string_view m_hitSEFile;
	std::shared_ptr<KdSoundInstance> m_hitSEInstance = nullptr;

	std::shared_ptr<Score> msp_score = nullptr;
	std::vector<std::shared_ptr<KeyBack>> msp_keyBackList = {};
	std::vector<std::shared_ptr<HitEffect>> msp_hitEffectList = {};

	BMSLoader bms;

	float			mf_scrollMulti;			// 小節間の幅の倍率
	LONGLONG		mll_startTime;		// ゲーム開始時の時間[μs]
	LONGLONG		mll_globalFreq;		// マシンの１秒間のカウント数[μs]
	LONGLONG		mll_beforeTime;		// 前ループの時間[μs]
	LONGLONG		mll_offsetTime;		// 判定調節用[μs]
	double			md_elapsedTime;		// 開始からの経過時間[s]
	double			md_difTime;				// 前ループからの経過時間[s]
	KEYSTATE	m_keyState[5];			// 鍵盤キーが押されているか
	//int				mi_startNum[256];		// BMS演奏で計算開始する配列番号(処理を軽くするため)

	//int				mi_flashIndex[6];		// 次に使用されるフラッシュカウンタのインデックス
	//int				mi_flashCount[6][3];	// フラッシュ６ｘ３個分のカウンタ
	int mi_backKeyCount[4];			// キーを離した時の後ろのバックライトの演出用カウンタ
	int mi_hitEffectCount[4];

	int mi_jadgeOffset;

	//debug
	int mi_pressNum;
	int mi_hitSubNum;
	char m_jadge[100];

	std::shared_ptr<NoteManager> msp_noteManager = nullptr;

};