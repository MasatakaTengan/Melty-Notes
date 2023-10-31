#include "Application/main.h"
#include "GameScene.h"

void GameScene::Init()
{
	m_noteTex.Load("Asset/Textures/notes.png");
	m_keyBackTex.Load("Asset/Textures/note_effect.png");
	mb_bgm = false;
	m_bgmFile = "Asset/Audio/SAMPLE.WAV";
	m_hitSEFile = "Asset/Audio/drum-hitnormal.wav";
	//KdAudioManager::Instance().Instance().Play(m_bgmFile);

	bms.Init();
	bms.Load("SAMPLE/SAMPLE.BMS");

	fScrMulti = 4.0f;
	dElapsedTime = 0;
	ZeroMemory(&iStartNum, sizeof(iStartNum));

	ZeroMemory(&bOnKey, sizeof(bOnKey));
	ZeroMemory(&iFlashIndex, sizeof(iFlashIndex));
	ZeroMemory(&iFlashCount, sizeof(iFlashCount));
	ZeroMemory(&iBackKeyCount, sizeof(iBackKeyCount));

	// �}�V���̎��g�����擾
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	llGlobalFreq = freq.QuadPart;

	// ���݂̎��Ԃ��J�n���ԂƂ���
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	llStartTime = li.QuadPart;

	//debug
	m_hitSubNum = 0;
}

void GameScene::Event()
{
	// �e���|�����ϐ�
	int i, j, k;
	// �t���b�V����
	for (j = 0; j < 6; j++) {
		for (i = 0; i < 3; i++) {
			if (iFlashCount[j][i] > 0)
				iFlashCount[j][i] -= 2;
		}
	}

	// ���̃o�b�N���C�g���o
	for (i = 0; i < 6; i++) {
		if (iBackKeyCount[i] > 0)
			iBackKeyCount[i]--;
	}

	// �X�N���[�����ύX
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		// �e���L�[�́{
		fScrMulti += 0.05f;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		// �e���L�[�́|
		fScrMulti -= 0.05f;
		if (fScrMulti < 0.05f)
			fScrMulti = 0.05f;
	}
}

void GameScene::PreUpdate()
{
	for (auto& obj : msp_objList)
	{
		obj->PreUpdate();
	}
}

void GameScene::Update()
{
	for (auto& obj : msp_objList)
	{
		obj->Update();
	}
	static const int index[6] = { 0,2,4,1,3,5 };				// �C���f�b�N�X���X�g
	static const int obj_kind[6] = { 0,1,0,1,0,2 };				// �I�u�W�F�̎��
	static const float obj_x[6] = { 1,18,30,46,59,92 };			// �I�u�W�F�\��X���W
	static const float obj_fx[6] = { 14,29,44,58,72,109 };		// �t���b�V���\��X���W

	// �J�n������o�߂������Ԃ��Z�o
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	dElapsedTime = (double)(li.QuadPart - llStartTime) / llGlobalFreq;

	// �o�߂������Ԃ���i��BMS�J�E���g�l���Z�o
	LONG now_count = bms.GetCountFromTime(dElapsedTime);

	// BMS�J�E���^���Ȃ̍ő�J�E���g+1���߂𒴂�����I��
	if (bms.GetMaxCount() + BMS_RESOLUTION <= now_count)Application::Instance().End();

	//add::::
	// BMS�J�E���^���Ȃ̊J�n�ʒu�ɗ�����Đ�����
	LONG bmsStart = bms.GetObje(BMS_BACKMUSIC, 0)->ml_time;
	if (now_count >= bmsStart)
	{
		if (!mb_bgm)
		{
			mb_bgm = true;
			KdAudioManager::Instance().Play(m_bgmFile);
		}
	}

	// �e���|�����ϐ�
	int i, j, k;
	/////////////////////////////////////////////////////////////////////////////////////
		// �l�ԃv���C
		/////////////////////////////////////////////////////////////////////////////////////

		// ���z���̓n�[�h�E�F�A(�����ꂽ�u�Ԃ���TRUE�ƂȂ�z��)
	BOOL press[6];						// 5Key+�X�N���b�`
	ZeroMemory(&press, sizeof(press));

	// ���Ղ̏���
	static const int KEYID[5] = {		// �L�[�̃��X�g
		'D',							// ch11�Ɋ��蓖�Ă�L�[
		'F',							// ch12�Ɋ��蓖�Ă�L�[
		' ',							// ch13�Ɋ��蓖�Ă�L�[
		'J',							// ch14�Ɋ��蓖�Ă�L�[
		'K',							// ch15�Ɋ��蓖�Ă�L�[
	};
	for (i = 0; i < 5; i++) {
		if ((GetAsyncKeyState(KEYID[i]) & 0x8000)) {
			// �L�[�{�[�h���W���C�X�e�B�b�N�̓��͂��������ꍇ
			if (!bOnKey[i]) {
				// �܂�������Ă��Ȃ���Ή����ꂽ�u�ԂƂ���
				press[i] = TRUE;
				bOnKey[i] = TRUE;
			}
		}
		else {
			// ������Ă��Ȃ���΃t���O�����Z�b�g
			bOnKey[i] = FALSE;
		}
	}

	// ���͔���
	const LONG PERFECT_RANGE = BMS_RESOLUTION / 48;		// GREAT�Ɣ��肷�钆�S����͈̔�(�O�㍇�킹��24��������)
	const LONG GREAT_RANGE = BMS_RESOLUTION / 32;		// GREAT�Ɣ��肷�钆�S����͈̔�(�O�㍇�킹��16��������)
	const LONG GOOD_RANGE = BMS_RESOLUTION / 16;		// GOOD�Ɣ��肷�钆�S����͈̔�(�O�㍇�킹��8��������)
	const LONG BAD_RANGE = BMS_RESOLUTION / 8;		// BAD�Ɣ��肷�钆�S����͈̔�(�O�㍇�킹��5��������)
	const LONG POOR_RANGE = BMS_RESOLUTION / 2;		// POOR���肷�钆�S����͈̔�(�O�㍇�킹��1���ߓ�)

	// �S�`�����l����������
	for (j = 0; j < 6; j++) {
		// ����Ώۂ̃`�����l���̃I�u�W�F���`�F�b�N
		for (i = iStartNum[j + 0x11 + 0x20]; i < bms.GetObjeNum(0x11 + j); i++) {
			BMSData* b = /*(BMSData*)*/(bms.GetObje(0x11 + j, i));
			if (b->mb_flg) {
				// �܂�������̃I�u�W�F�Ȃ�
				if (b->ml_time < (now_count - GOOD_RANGE)) {
					// �ǔ�����߂�����S�Č����������Ƃ���
					b->mb_flg = FALSE;						// �I�u�W�F������
					// ����I�u�W�F�����̎�����ɕύX
					iStartNum[j + 0x11 + 0x20] = i + 1;
					// ���̃I�u�W�F���`�F�b�N
					continue;
				}

				// �I�u�W�F������O�Ȃ甲����
				if ((now_count + POOR_RANGE) <= b->ml_time)
					break;

				// �I�u�W�F��������Ȃ�L�[�������ꂽ�u�Ԃ����`�F�b�N
				if (press[j]) {
					// �L�[���������u�ԂȂ琸�x����
					LONG sub = abs(now_count - b->ml_time);		// �I�u�W�F�Ƃ̍����Βl�Ŏ擾
					KdAudioManager::Instance().Play(m_hitSEFile);
					m_hitSubNum = sub;

					int jadge = 0;								// ����l(0=POOR�A1=BAD�A2=GOOD�A3=GREAT�A4=PKGREAT�Ȃ�)
					if (sub <= PERFECT_RANGE) {
						jadge = 4;
						sprintf_s(m_jadge, sizeof(m_jadge), "PERFECT");
					}
					else if (sub <= GREAT_RANGE) {
						jadge = 3;
						sprintf_s(m_jadge, sizeof(m_jadge), "GREAT");
					}
					else if (sub <= GOOD_RANGE) {
						jadge = 2;
						sprintf_s(m_jadge, sizeof(m_jadge), "GOOD");
					}
					else if (sub <= BAD_RANGE) {
						jadge = 1;
						sprintf_s(m_jadge, sizeof(m_jadge), "BAD");
					}

					switch (jadge)
					{
						case 0:
							b->m_color = kRedColor;
							break;
						case 1:
							b->m_color = kBlueColor;
							break;
						case 2:
							b->m_color = kGreenColor;
							break;
						case 3:
							b->m_color = kRedColor + kGreenColor;
							break;
						case 4:
							b->m_color = kWhiteColor;
							break;
					}

					if (jadge >= 1) {
						// BAD�ȏ�Ȃ�I�u�W�F������
						b->mb_flg = FALSE;						// �I�u�W�F������
						// ���̃I�u�W�F�̉����Đ�
						/*ds.Reset(b->lData);
						ds.Play(b->lData);*/
						// ����I�u�W�F�����̎�����ɕύX
						iStartNum[j + 0x11 + 0x20] = i + 1;
						// �t���b�V���摜�̒�`
						iFlashCount[j][iFlashIndex[j]] = 45;
						iFlashIndex[j]++;
						if (iFlashIndex[j] > 2)
							iFlashIndex[j] = 0;
						// ����I�u�W�F�����̎�����ɕύX
						iStartNum[j + 0x11 + 0x20] = i + 1;
						break;
					}
				}
			}
		}
	}

	// ���Ղ̃o�b�N���C�g
	for (i = 0; i < 5; i++) {
		if (bOnKey[i]) {
			// �L�[�������ꂽ��ԂȂ�J�E���^�����Z�b�g
			iBackKeyCount[i] = 30;
		}
	}

	// �X�N���b�`�̃o�b�N���C�g
	//if (iScratchStatus != 0) {
	//	// �E�����ɉ񂵂Ă����ԂȂ�J�E���^�����Z�b�g
	//	iBackKeyCount[5] = 30;
	//}
	ImGui::Text("%ld", m_hitSubNum);
	ImGui::Text("%s", m_jadge);
}

void GameScene::PostUpdate()
{
	for (auto& obj : msp_objList)
	{
		obj->PostUpdate();
	}
}

void GameScene::PreDraw()
{
	for (auto& obj : msp_objList)
	{
		obj->PreDraw();
	}
}

void GameScene::DrawLit()
{
	for (auto& obj : msp_objList)
	{
		obj->DrawLit();
	}
}

void GameScene::DrawUnLit()
{
	for (auto& obj : msp_objList)
	{
		obj->DrawUnLit();
	}
}

void GameScene::PostDraw()
{
	for (auto& obj : msp_objList)
	{

	}
}

void GameScene::DrawSprite()
{
	if (ImGui::Begin("GameSceneDebug"))
	{
		for (auto& obj : msp_objList)
		{
			obj->DrawSprite();
		}
		ImGui::Text("ScreenScale : %.2f", fScrMulti);
		
		KdShaderManager::Instance().m_spriteShader.DrawBox(0, 0, 640, 360, &kBlackColor);

		static const int index[6] = { 0,2,4,1,3,5 };				// �C���f�b�N�X���X�g
		static const int obj_kind[6] = { 0,1,0,1,0,2 };				// �I�u�W�F�̎��
		static const float obj_x[6] = { 1,18,30,46,59,92 };			// �I�u�W�F�\��X���W
		static const float obj_fx[6] = { 14,29,44,58,72,109 };		// �t���b�V���\��X���W
		// �o�߂������Ԃ���i��BMS�J�E���g�l���Z�o
		LONG now_count = bms.GetCountFromTime(dElapsedTime);
		// �X�N���[�����W��ł̃X�N���[���ʂ��Z�o
		int scr_y = (int)((double)now_count / (BMS_RESOLUTION / (fScrMulti * 192)));

		for (int j = 0; j < 6; j++) {
			for (int i = iStartNum[0x11 + index[j]]; i < bms.GetObjeNum(0x11 + index[j]); i++) {
				BMSData* b = bms.GetObje(0x11 + index[j], i);
				// �X�N���[�����l�����Ȃ��X�N���[�����W��ł̌��_����̍��W�l���Z�o
				int obj_y = (int)((double)b->ml_time / (BMS_RESOLUTION / (fScrMulti * 192)));
				// �X�N���[�����l���������݂�Y���W���Z�o
				int off_y = obj_y - scr_y;
				if (i == iStartNum[0x11 + index[j]] && j == 0)
				{
					ImGui::Text("obj_y : %d", obj_y);
					ImGui::Text("scr_y : %d", scr_y);
					ImGui::Text("off_y : %d", off_y);
				}
				// ���胉�C����艺�Ȃ�����\���͂����A���񂩂炻�̎��̃I�u�W�F����Q�Ƃ���
				if (off_y < -360) {
					iStartNum[index[j] + 0x11] = i + 1;
					continue;
				}
				// ��ʂ̏���O�Ȃ�΂��̐�͑S�ĕ`��X�L�b�v
				/*if (off_y > 413 + 6)
					break;*/
					// ��ʓ��Ȃ�`��
					//dd.Put(15 + obj_kind[index[j]], obj_x[index[j]], (float)(413 - off_y));
				Math::Rectangle rect = Math::Rectangle(0, obj_kind[index[j]] * 16, 64, 16);
				//Math::Color color = { 1, (float)b->bFlag, 1, 1 };
				KdShaderManager::Instance().m_spriteShader.DrawTex(&m_noteTex, obj_x[index[j]] * 5, (float)(BAR_Y + off_y), NOTE_WIDTH, NOTE_HEIGHT, &rect, &b->m_color);
			}
			KdShaderManager::Instance().ChangeBlendState(KdBlendState::Add);
			Math::Color color = { 0.4f, 0.4f, 0.4f, (float)iBackKeyCount[j] / 30 };
			KdShaderManager::Instance().m_spriteShader.DrawTex(&m_keyBackTex, obj_x[j] * 5, KEYBACK_Y, nullptr, &color);
			KdShaderManager::Instance().ChangeBlendState(KdBlendState::Alpha);

			//KdShaderManager::Instance().m_spriteShader.DrawLine(0, -192, 640, -192);
			////KdShaderManager::Instance().m_spriteShader.DrawLine(0, 0, 640, 0);
			KdShaderManager::Instance().m_spriteShader.DrawLine(0, BAR_Y + NOTE_HALFHEIGHT, WINDOW_HALFWIDTH, BAR_Y + NOTE_HALFHEIGHT);
			KdShaderManager::Instance().m_spriteShader.DrawLine(0, BAR_Y - NOTE_HALFHEIGHT, WINDOW_HALFWIDTH, BAR_Y - NOTE_HALFHEIGHT);
		}


		for (int i = 0; i < 5; i++) {
			if (bOnKey[i]) {
				// �L�[�������ꂽ��ԂȂ�J�E���^�����Z�b�g
				iBackKeyCount[i] = 30;
			}
		}
	}

	ImGui::End();
}
