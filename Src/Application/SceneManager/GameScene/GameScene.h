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

	bool mb_bgm;		//�Đ�����
	std::string_view m_bgmFile;
	std::shared_ptr<KdSoundInstance> m_bgmInstance = nullptr;
	std::string_view m_hitSEFile;

	BMSLoader bms;

	float			mf_scrollMulti;			// ���ߊԂ̕��̔{��
	LONGLONG		mll_startTime;		// �Q�[���J�n���̎���(���𑜓x�^�C�}�[)
	LONGLONG		mll_beforeTime;		// �O���[�v�̎���(���𑜓x�^�C�}�[)
	LONGLONG		mll_elapsedTime;		// �o�ߎ���(���𑜓x�^�C�}�[[ms])
	LONGLONG		mll_globalFreq;		// �}�V���̂P�b�Ԃ̃J�E���g��(���𑜓x�^�C�}�[)
	double			md_elapsedTime;		// �J�n����̌o�ߎ���(�b)
	KEYSTATE	m_keyState[5];			// ���ՃL�[��������Ă��邩
	//int				mi_startNum[256];		// BMS���t�Ōv�Z�J�n����z��ԍ�(�������y�����邽��)

	//int				mi_flashIndex[6];		// ���Ɏg�p�����t���b�V���J�E���^�̃C���f�b�N�X
	//int				mi_flashCount[6][3];	// �t���b�V���U���R���̃J�E���^
	int mi_backKeyCount[4];			// �L�[�𗣂������̌��̃o�b�N���C�g�̉��o�p�J�E���^
	int mi_hitEffectCount[4];

	//debug
	int mi_pressNum;
	int mi_hitSubNum;
	char m_jadge[100];

	std::shared_ptr<NoteManager> m_noteManager = nullptr;

};