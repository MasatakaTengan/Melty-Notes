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

	float			fScrMulti;			// ���ߊԂ̕��̔{��
	LONGLONG		llStartTime;		// �Q�[���J�n���̎���(���𑜓x�^�C�}�[)
	LONGLONG		llGlobalFreq;		// �}�V���̂P�b�Ԃ̃J�E���g��(���𑜓x�^�C�}�[)
	double			dElapsedTime;		// �J�n����̌o�ߎ���(�b)
	BOOL			bOnKey[5];			// ���ՃL�[��������Ă��邩
	int				iStartNum[256];		// BMS���t�Ōv�Z�J�n����z��ԍ�(�������y�����邽��)

	int				iFlashIndex[6];		// ���Ɏg�p�����t���b�V���J�E���^�̃C���f�b�N�X
	int				iFlashCount[6][3];	// �t���b�V���U���R���̃J�E���^
	int				iBackKeyCount[6];	// �L�[�𗣂������̌��̃o�b�N���C�g�̉��o�p�J�E���^

	//debug
	int m_hitSubNum;
	char m_jadge[100];

};