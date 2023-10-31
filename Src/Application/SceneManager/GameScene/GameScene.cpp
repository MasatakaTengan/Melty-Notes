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

	// マシンの周波数を取得
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	llGlobalFreq = freq.QuadPart;

	// 現在の時間を開始時間とする
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	llStartTime = li.QuadPart;

	//debug
	m_hitSubNum = 0;
}

void GameScene::Event()
{
	// テンポラリ変数
	int i, j, k;
	// フラッシュ部
	for (j = 0; j < 6; j++) {
		for (i = 0; i < 3; i++) {
			if (iFlashCount[j][i] > 0)
				iFlashCount[j][i] -= 2;
		}
	}

	// 後ろのバックライト演出
	for (i = 0; i < 6; i++) {
		if (iBackKeyCount[i] > 0)
			iBackKeyCount[i]--;
	}

	// スクロール幅変更
	if (GetAsyncKeyState(VK_UP) & 0x8000) {
		// テンキーの＋
		fScrMulti += 0.05f;
	}
	if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
		// テンキーの－
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
	static const int index[6] = { 0,2,4,1,3,5 };				// インデックスリスト
	static const int obj_kind[6] = { 0,1,0,1,0,2 };				// オブジェの種類
	static const float obj_x[6] = { 1,18,30,46,59,92 };			// オブジェ表示X座標
	static const float obj_fx[6] = { 14,29,44,58,72,109 };		// フラッシュ表示X座標

	// 開始時から経過した時間を算出
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	dElapsedTime = (double)(li.QuadPart - llStartTime) / llGlobalFreq;

	// 経過した時間から進んだBMSカウント値を算出
	LONG now_count = bms.GetCountFromTime(dElapsedTime);

	// BMSカウンタが曲の最大カウント+1小節を超えたら終了
	if (bms.GetMaxCount() + BMS_RESOLUTION <= now_count)Application::Instance().End();

	//add::::
	// BMSカウンタが曲の開始位置に来たら再生する
	LONG bmsStart = bms.GetObje(BMS_BACKMUSIC, 0)->ml_time;
	if (now_count >= bmsStart)
	{
		if (!mb_bgm)
		{
			mb_bgm = true;
			KdAudioManager::Instance().Play(m_bgmFile);
		}
	}

	// テンポラリ変数
	int i, j, k;
	/////////////////////////////////////////////////////////////////////////////////////
		// 人間プレイ
		/////////////////////////////////////////////////////////////////////////////////////

		// 仮想入力ハードウェア(押された瞬間だけTRUEとなる配列)
	BOOL press[6];						// 5Key+スクラッチ
	ZeroMemory(&press, sizeof(press));

	// 鍵盤の処理
	static const int KEYID[5] = {		// キーのリスト
		'D',							// ch11に割り当てるキー
		'F',							// ch12に割り当てるキー
		' ',							// ch13に割り当てるキー
		'J',							// ch14に割り当てるキー
		'K',							// ch15に割り当てるキー
	};
	for (i = 0; i < 5; i++) {
		if ((GetAsyncKeyState(KEYID[i]) & 0x8000)) {
			// キーボードかジョイスティックの入力があった場合
			if (!bOnKey[i]) {
				// まだ押されていなければ押された瞬間とする
				press[i] = TRUE;
				bOnKey[i] = TRUE;
			}
		}
		else {
			// 押されていなければフラグをリセット
			bOnKey[i] = FALSE;
		}
	}

	// 入力判定
	const LONG PERFECT_RANGE = BMS_RESOLUTION / 48;		// GREATと判定する中心からの範囲(前後合わせて24分音符内)
	const LONG GREAT_RANGE = BMS_RESOLUTION / 32;		// GREATと判定する中心からの範囲(前後合わせて16分音符内)
	const LONG GOOD_RANGE = BMS_RESOLUTION / 16;		// GOODと判定する中心からの範囲(前後合わせて8分音符内)
	const LONG BAD_RANGE = BMS_RESOLUTION / 8;		// BADと判定する中心からの範囲(前後合わせて5分音符内)
	const LONG POOR_RANGE = BMS_RESOLUTION / 2;		// POOR判定する中心からの範囲(前後合わせて1小節内)

	// 全チャンネル分を処理
	for (j = 0; j < 6; j++) {
		// 判定対象のチャンネルのオブジェをチェック
		for (i = iStartNum[j + 0x11 + 0x20]; i < bms.GetObjeNum(0x11 + j); i++) {
			BMSData* b = /*(BMSData*)*/(bms.GetObje(0x11 + j, i));
			if (b->mb_flg) {
				// まだ未判定のオブジェなら
				if (b->ml_time < (now_count - GOOD_RANGE)) {
					// 良判定を過ぎたら全て見逃し扱いとする
					b->mb_flg = FALSE;						// オブジェを消す
					// 判定オブジェをその次からに変更
					iStartNum[j + 0x11 + 0x20] = i + 1;
					// 次のオブジェをチェック
					continue;
				}

				// オブジェが判定外なら抜ける
				if ((now_count + POOR_RANGE) <= b->ml_time)
					break;

				// オブジェが判定内ならキーが押された瞬間かをチェック
				if (press[j]) {
					// キーを押した瞬間なら精度判定
					LONG sub = abs(now_count - b->ml_time);		// オブジェとの差を絶対値で取得
					KdAudioManager::Instance().Play(m_hitSEFile);
					m_hitSubNum = sub;

					int jadge = 0;								// 判定値(0=POOR、1=BAD、2=GOOD、3=GREAT、4=PKGREATなど)
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
						// BAD以上ならオブジェを処理
						b->mb_flg = FALSE;						// オブジェを消す
						// そのオブジェの音を再生
						/*ds.Reset(b->lData);
						ds.Play(b->lData);*/
						// 判定オブジェをその次からに変更
						iStartNum[j + 0x11 + 0x20] = i + 1;
						// フラッシュ画像の定義
						iFlashCount[j][iFlashIndex[j]] = 45;
						iFlashIndex[j]++;
						if (iFlashIndex[j] > 2)
							iFlashIndex[j] = 0;
						// 判定オブジェをその次からに変更
						iStartNum[j + 0x11 + 0x20] = i + 1;
						break;
					}
				}
			}
		}
	}

	// 鍵盤のバックライト
	for (i = 0; i < 5; i++) {
		if (bOnKey[i]) {
			// キーが押された状態ならカウンタをリセット
			iBackKeyCount[i] = 30;
		}
	}

	// スクラッチのバックライト
	//if (iScratchStatus != 0) {
	//	// 右か左に回している状態ならカウンタをリセット
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

		static const int index[6] = { 0,2,4,1,3,5 };				// インデックスリスト
		static const int obj_kind[6] = { 0,1,0,1,0,2 };				// オブジェの種類
		static const float obj_x[6] = { 1,18,30,46,59,92 };			// オブジェ表示X座標
		static const float obj_fx[6] = { 14,29,44,58,72,109 };		// フラッシュ表示X座標
		// 経過した時間から進んだBMSカウント値を算出
		LONG now_count = bms.GetCountFromTime(dElapsedTime);
		// スクリーン座標上でのスクロール量を算出
		int scr_y = (int)((double)now_count / (BMS_RESOLUTION / (fScrMulti * 192)));

		for (int j = 0; j < 6; j++) {
			for (int i = iStartNum[0x11 + index[j]]; i < bms.GetObjeNum(0x11 + index[j]); i++) {
				BMSData* b = bms.GetObje(0x11 + index[j], i);
				// スクロールを考慮しないスクリーン座標上での原点からの座標値を算出
				int obj_y = (int)((double)b->ml_time / (BMS_RESOLUTION / (fScrMulti * 192)));
				// スクロールを考慮した現在のY座標を算出
				int off_y = obj_y - scr_y;
				if (i == iStartNum[0x11 + index[j]] && j == 0)
				{
					ImGui::Text("obj_y : %d", obj_y);
					ImGui::Text("scr_y : %d", scr_y);
					ImGui::Text("off_y : %d", off_y);
				}
				// 判定ラインより下ならもう表示はせず、次回からその次のオブジェから参照する
				if (off_y < -360) {
					iStartNum[index[j] + 0x11] = i + 1;
					continue;
				}
				// 画面の上より外ならばその先は全て描画スキップ
				/*if (off_y > 413 + 6)
					break;*/
					// 画面内なら描画
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
				// キーが押された状態ならカウンタをリセット
				iBackKeyCount[i] = 30;
			}
		}
	}

	ImGui::End();
}
