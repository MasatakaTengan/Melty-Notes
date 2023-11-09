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

	float			mf_scrollMulti;			// ���ߊԂ̕��̔{��
	LONGLONG		mll_startTime;		// �Q�[���J�n���̎���(���𑜓x�^�C�}�[)
	LONGLONG		mll_globalFreq;		// �}�V���̂P�b�Ԃ̃J�E���g��(���𑜓x�^�C�}�[)
	double			md_elapsedTime;		// �J�n����̌o�ߎ���(�b)
	KEYSTATE	m_keyState[5];			// ���ՃL�[��������Ă��邩
	int				mi_startNum[256];		// BMS���t�Ōv�Z�J�n����z��ԍ�(�������y�����邽��)

	int				mi_flashIndex[6];		// ���Ɏg�p�����t���b�V���J�E���^�̃C���f�b�N�X
	int				mi_flashCount[6][3];	// �t���b�V���U���R���̃J�E���^
	int				mi_backKeyCount[6];	// �L�[�𗣂������̌��̃o�b�N���C�g�̉��o�p�J�E���^
	int mi_hitEffectCount[6];

	//debug
	int mi_pressNum;
	int mi_hitSubNum;
	char m_jadge[100];

	const int KEYID[5] =
	{		// �L�[�̃��X�g
		'D',							// ch11�Ɋ��蓖�Ă�L�[
		'F',							// ch12�Ɋ��蓖�Ă�L�[
		' ',							// ch13�Ɋ��蓖�Ă�L�[
		'J',							// ch14�Ɋ��蓖�Ă�L�[
		'K',							// ch15�Ɋ��蓖�Ă�L�[
	};

	std::shared_ptr<NoteManager> m_noteManager = nullptr;

};