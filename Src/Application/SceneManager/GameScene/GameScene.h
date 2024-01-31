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

	void DrawSprite()override;

	void AddHitEffect( Math::Vector2 _pos, JADGE _jadge );

private:

	KdTexture m_noteTex;
	KdTexture m_keyBackTex;
	std::shared_ptr<KdTexture> msp_hitEffectTex;

	bool mb_bgmStart;		//�Đ��J�n������
	bool mb_bgmPlay;		//�Đ�����
	std::string_view m_bgmFile;
	std::shared_ptr<KdSoundInstance> m_bgmInstance = nullptr;
	std::string_view m_hitSEFile;
	std::shared_ptr<KdSoundInstance> m_hitSEInstance = nullptr;

	std::shared_ptr<Score> msp_score = nullptr;
	std::vector<std::shared_ptr<KeyBack>> msp_keyBackList = {};
	std::vector<std::shared_ptr<HitEffect>> msp_hitEffectList = {};

	BMSLoader bms;

	float			mf_scrollMulti;			// ���ߊԂ̕��̔{��
	LONGLONG		mll_startTime;		// �Q�[���J�n���̎���[��s]
	LONGLONG		mll_globalFreq;		// �}�V���̂P�b�Ԃ̃J�E���g��[��s]
	LONGLONG		mll_beforeTime;		// �O���[�v�̎���[��s]
	LONGLONG		mll_offsetTime;		// ���蒲�ߗp[��s]
	double			md_elapsedTime;		// �J�n����̌o�ߎ���[s]
	double			md_difTime;				// �O���[�v����̌o�ߎ���[s]

	int mi_jadgeOffset;

	//debug
	int mi_pressNum;
	int mi_hitSubNum;

	std::shared_ptr<NoteManager> msp_noteManager = nullptr;

};