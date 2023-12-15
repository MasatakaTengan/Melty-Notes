#include "Application/main.h"
#include "GameScene.h"
#include "../../GameObject/Notes/NoteManager/NoteManager.h"

void GameScene::Init()
{
	m_noteTex.Load( "Asset/Textures/notes.png" );
	m_keyBackTex.Load( "Asset/Textures/note_effect.png" );
	m_hitEffectTex.Load( "Asset/Textures/hit_effect.png" );
	mb_bgm = false;
	//m_bgmFile = "Score/SAMPLE/SAMPLE.WAV";
	m_bgmFile = "Score/FREEDOM_DIVE/FREEDOM-DiVE↓fast.wav";
	m_bgmInstance = KdAudioManager::Instance().Play( m_bgmFile );
	m_bgmInstance->Stop();
	m_hitSEFile = "Asset/Audio/drum-hitnormal.wav";
	//KdAudioManager::Instance().Instance().Play(m_bgmFile);

	bms.Init();
	bms.Load( "Score/FREEDOM_DIVE/freedom_dive.bms" );

	mf_scrollMulti = 4.0f;
	md_elapsedTime = 0;
	//ZeroMemory( &mi_startNum, sizeof( mi_startNum ) );

	//ZeroMemory( &mb_onKey, sizeof( mb_onKey ) );
	//ZeroMemory( &mi_flashIndex, sizeof( mi_flashIndex ) );
	//ZeroMemory( &mi_flashCount, sizeof( mi_flashCount ) );
	ZeroMemory( &mi_backKeyCount, sizeof( mi_backKeyCount ) );
	ZeroMemory( &mi_hitEffectCount, sizeof( mi_hitEffectCount ) );

	// マシンの周波数を取得
	LARGE_INTEGER freq;
	QueryPerformanceFrequency( &freq );
	mll_globalFreq = freq.QuadPart;

	// 現在の時間を開始時間とする
	LARGE_INTEGER li;
	QueryPerformanceCounter( &li );
	mll_startTime = li.QuadPart;
	mll_beforeTime = li.QuadPart;

	mll_elapsedTime = 0;

	m_noteManager = std::make_shared<NoteManager>();
	m_noteManager->Init( bms );

	//debug
	mi_hitSubNum = 0;
	mi_pressNum = 0;
}

void GameScene::Event()
{
	// フラッシュ部
	/*for ( j = 0; j < 6; j++ )
	{
		for ( i = 0; i < 3; i++ )
		{
			if ( mi_flashCount[j][i] > 0 )
				mi_flashCount[j][i] -= 2;
		}
	}*/

	// 後ろのバックライト演出
	for ( int i = 0; i < 6; i++ )
	{
		if ( mi_backKeyCount[i] > 0 )
			mi_backKeyCount[i]--;
		if ( mi_hitEffectCount[i] > 0 )
			mi_hitEffectCount[i]--;
	}

	// スクロール幅変更
	if ( GetAsyncKeyState( VK_UP ) & 0x8000 )
	{
		// テンキーの＋
		mf_scrollMulti += 0.05f;
	}
	if ( GetAsyncKeyState( VK_DOWN ) & 0x8000 )
	{
		// テンキーの－
		mf_scrollMulti -= 0.05f;
		if ( mf_scrollMulti < 0.05f )
			mf_scrollMulti = 0.05f;
	}
}

void GameScene::PreUpdate()
{
	for ( auto& obj : msp_objList )
	{
		obj->PreUpdate();
	}
}

void GameScene::Update()
{
	for ( auto& obj : msp_objList )
	{
		obj->Update();
	}

	// 開始時から経過した時間を算出
	LARGE_INTEGER li;
	QueryPerformanceCounter( &li );
	md_elapsedTime = (double)(li.QuadPart - mll_startTime) / mll_globalFreq;

	//もし曲が再生中なら
	if ( mb_bgm )
	{
		//前ループから経過した時間を算出し、合計の経過時間に加算する
		mll_elapsedTime += li.QuadPart - mll_beforeTime;
	}
	//曲が再生中でないなら経過時間の加算をしないため、ポーズ画面中の処理等を行えるはず

	//前ループ時の時間を更新
	mll_beforeTime = li.QuadPart;

	// 経過した時間から進んだBMSカウント値を算出
	LONG now_count = bms.GetCountFromTime( (double)mll_elapsedTime / mll_globalFreq );

	// BMSカウンタが曲の最大カウント+1小節を超えたら終了
	if ( bms.GetMaxCount() + (BMS_RESOLUTION * 1) <= now_count )Application::Instance().End();

	// BMSカウンタが曲の開始位置に来たら再生する
	LONG bmsStart = bms.GetObje( BMS_BACKMUSIC, 0 )->ml_time;
	if ( now_count >= bmsStart )
	{
		if ( !mb_bgm )
		{
			mb_bgm = true;
			m_bgmInstance->Play();
		}
	}

	// 鍵盤の処理
	for ( int i = 0; i < 4; i++ )
	{
		if ( INPUT.GetKeyStateToManager( Constant::KEYID_4K[i] ) == KEYSTATE::PRESS )
		{
			KdAudioManager::Instance().Play( m_hitSEFile );
			mi_hitEffectCount[i] = 10;
		}
	}

	// 入力判定

	// note類
	m_noteManager->PreUpdate();
	m_noteManager->Update( now_count );

	// 鍵盤のバックライト
	for ( int i = 0; i < 4; i++ )
	{
		if ( INPUT.GetKeyStateToManager( Constant::KEYID_4K[i] ) == KEYSTATE::HOLD )
		{
			// キーが押された状態ならカウンタをリセット
			mi_backKeyCount[i] = 15;
		}
	}

	ImGui::Text( "%s", m_noteManager->GetJadge() );
	ImGui::Text( "%ld", m_noteManager->GetScore() );
	ImGui::Text( "%ld", m_noteManager->GetJadgeCnt() );
	ImGui::Text( "%ld", now_count );
}

void GameScene::PostUpdate()
{
	for ( auto& obj : msp_objList )
	{
		obj->PostUpdate();
	}
}

void GameScene::PreDraw()
{
	for ( auto& obj : msp_objList )
	{
		obj->PreDraw();
	}
}

void GameScene::DrawLit()
{
	for ( auto& obj : msp_objList )
	{
		obj->DrawLit();
	}
}

void GameScene::DrawUnLit()
{
	for ( auto& obj : msp_objList )
	{
		obj->DrawUnLit();
	}
}

//void GameScene::PostDraw()
//{
//	for ( auto& obj : msp_objList )
//	{
//
//	}
//}

void GameScene::DrawSprite()
{
	if ( ImGui::Begin( "GameSceneDebug" ) )
	{
		for ( auto& obj : msp_objList )
		{
			obj->DrawSprite();
		}
		ImGui::Text( "ScreenScale : %.2f", mf_scrollMulti );

		KdShaderManager::Instance().m_spriteShader.DrawBox( 0, 0, WINDOW_HALFWIDTH, WINDOW_HALFHEIGHT, &kBlackColor );

		//static const float obj_x[6] = { -256,-128,0,128,256,512 };			// オブジェ表示X座標
		static const int obj_x[4] = { -192,-64,64,192 };			// オブジェ表示X座標

		//add:note類
		m_noteManager->Draw( mf_scrollMulti );

		for ( int j = 0; j < 4; j++ )
		{
			KdShaderManager::Instance().ChangeBlendState( KdBlendState::Add );

			Math::Color color = { 0.4f, 0.4f, 0.4f, (float)mi_backKeyCount[j] / 30 };
			KdShaderManager::Instance().m_spriteShader.DrawTex(
				&m_keyBackTex,
				obj_x[j], KEYBACK_Y,
				nullptr, &color );

			float scale = (float)sin( ((float)(-mi_hitEffectCount[j] + 10) / 10) * M_PI / 2 ) / 2 + 1.0f;
			float alpha = ((float)mi_hitEffectCount[j] / 10);
			color = { 1, 1, 1, alpha };
			KdShaderManager::Instance().m_spriteShader.DrawTex(
				&m_hitEffectTex,
				obj_x[j], BAR_Y,
				(int)(HITEFFECT_HALFWIDTH * scale), (int)(HITEFFECT_HALFHEIGHT * scale),
				nullptr, &color );

			KdShaderManager::Instance().ChangeBlendState( KdBlendState::Alpha );
		}

		KdShaderManager::Instance().m_spriteShader.DrawLine(
			-WINDOW_HALFWIDTH / 2, BAR_Y + NOTE_HALFHEIGHT,
			WINDOW_HALFWIDTH / 2, BAR_Y + NOTE_HALFHEIGHT );
		KdShaderManager::Instance().m_spriteShader.DrawLine(
			-WINDOW_HALFWIDTH / 2, BAR_Y - NOTE_HALFHEIGHT,
			WINDOW_HALFWIDTH / 2, BAR_Y - NOTE_HALFHEIGHT );

	}

	ImGui::End();
}
