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
	m_hitSEFile = "Asset/Audio/drum-hitnormal.wav";
	//KdAudioManager::Instance().Instance().Play(m_bgmFile);

	bms.Init();
	bms.Load( "Score/FREEDOM_DIVE/freedom_dive.bms" );

	mf_scrollMulti = 4.0f;
	md_elapsedTime = 0;
	ZeroMemory( &mi_startNum, sizeof( mi_startNum ) );

	//ZeroMemory( &mb_onKey, sizeof( mb_onKey ) );
	ZeroMemory( &mi_flashIndex, sizeof( mi_flashIndex ) );
	ZeroMemory( &mi_flashCount, sizeof( mi_flashCount ) );
	ZeroMemory( &mi_backKeyCount, sizeof( mi_backKeyCount ) );

	// マシンの周波数を取得
	LARGE_INTEGER freq;
	QueryPerformanceFrequency( &freq );
	mll_globalFreq = freq.QuadPart;

	// 現在の時間を開始時間とする
	LARGE_INTEGER li;
	QueryPerformanceCounter( &li );
	mll_startTime = li.QuadPart;

	m_noteManager = std::make_shared<NoteManager>();
	m_noteManager->Init( bms );

	//debug
	mi_hitSubNum = 0;
	mi_pressNum = 0;
}

void GameScene::Event()
{
	// テンポラリ変数
	int i, j, k;
	// フラッシュ部
	for ( j = 0; j < 6; j++ )
	{
		for ( i = 0; i < 3; i++ )
		{
			if ( mi_flashCount[j][i] > 0 )
				mi_flashCount[j][i] -= 2;
		}
	}

	// 後ろのバックライト演出
	for ( i = 0; i < 6; i++ )
	{
		if ( mi_backKeyCount[i] > 0 )
			mi_backKeyCount[i]--;
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
	static const int index[6] = { 0,2,4,1,3,5 };				// インデックスリスト
	static const int obj_kind[6] = { 0,1,0,1,0,2 };				// オブジェの種類
	static const float obj_x[6] = { 1,18,30,46,59,92 };			// オブジェ表示X座標
	static const float obj_fx[6] = { 14,29,44,58,72,109 };		// フラッシュ表示X座標

	// 開始時から経過した時間を算出
	LARGE_INTEGER li;
	QueryPerformanceCounter( &li );
	md_elapsedTime = (double)( li.QuadPart - mll_startTime ) / mll_globalFreq;

	// 経過した時間から進んだBMSカウント値を算出
	LONG now_count = bms.GetCountFromTime( md_elapsedTime );

	// BMSカウンタが曲の最大カウント+1小節を超えたら終了
	if ( bms.GetMaxCount() + BMS_RESOLUTION <= now_count )Application::Instance().End();

	//add::::
	// BMSカウンタが曲の開始位置に来たら再生する
	LONG bmsStart = bms.GetObje( BMS_BACKMUSIC, 0 )->ml_time;
	if ( now_count >= bmsStart )
	{
		if ( !mb_bgm )
		{
			mb_bgm = true;
			KdAudioManager::Instance().Play( m_bgmFile );
		}
	}

	// テンポラリ変数
	int i, j, k;
	/////////////////////////////////////////////////////////////////////////////////////
		// 人間プレイ
		/////////////////////////////////////////////////////////////////////////////////////

		// 仮想入力ハードウェア(押された瞬間だけTRUEとなる配列)
	//BOOL press[6];						// 5Key+スクラッチ
	//ZeroMemory( &press, sizeof( press ) );

	// 鍵盤の処理
	for ( i = 0; i < 5; i++ )
	{
		if ( INPUT.GetKeyStateToManager( KEYID[i] ) == KEYSTATE::PRESS )
		{
			KdAudioManager::Instance().Play( m_hitSEFile );
		}
	}

	// 入力判定

	//add::note類
	m_noteManager->PreUpdate();
	m_noteManager->Update( now_count );

	/*
	// 全チャンネル分を処理
	for ( j = 0; j < 5; j++ )
	{
		// 判定対象のチャンネルのオブジェをチェック
		for ( i = mi_startNum[j + 0x11 + 0x20]; i < bms.GetObjeNum( 0x11 + j ); i++ )
		{
			BMSData* b = bms.GetObje( 0x11 + j, i );
			if ( b->mb_flg )
			{
				// まだ未判定のオブジェなら

				int jadge = JADGE::POOR;								// 判定値(0=POOR、1=BAD、2=GOOD、3=GREAT、4=PERFECTなど)

				if ( b->ml_time < ( now_count - BAD_RANGE ) )
				{
					// BAD判定を過ぎたら全て見逃し扱いとする
					b->mb_flg = FALSE;						// オブジェを消す
					sprintf_s( m_jadge, sizeof( m_jadge ), "MISS" );

					// 判定オブジェをその次からに変更
					mi_startNum[j + 0x11 + 0x20] = i + 1;
					// 次のオブジェをチェック
					continue;
				}

				// オブジェが判定外なら抜ける
				if ( ( now_count + POOR_RANGE ) <= b->ml_time )break;

				// オブジェが判定内ならキーが押された瞬間かをチェック
				if ( m_keyState[j] == KEYSTATE::PRESS )
				{
					// キーを押した瞬間なら精度判定
					mi_hitSubNum = now_count - b->ml_time;
					LONG sub = abs( mi_hitSubNum );		// オブジェとの差を絶対値で取得

					if ( sub <= PERFECT_RANGE )
					{
						jadge = PERFECT;
						sprintf_s( m_jadge, sizeof( m_jadge ), "PERFECT" );
					}
					else if ( sub <= GREAT_RANGE )
					{
						jadge = GREAT;
						sprintf_s( m_jadge, sizeof( m_jadge ), "GREAT" );
					}
					else if ( sub <= GOOD_RANGE )
					{
						jadge = GOOD;
						sprintf_s( m_jadge, sizeof( m_jadge ), "GOOD" );
					}
					else if ( sub <= BAD_RANGE )
					{
						jadge = BAD;
						sprintf_s( m_jadge, sizeof( m_jadge ), "BAD" );
					}

					switch ( jadge )
					{
					case POOR:
						b->m_color = kRedColor;
						break;
					case BAD:
						b->m_color = kBlueColor;
						break;
					case GOOD:
						b->m_color = kGreenColor;
						break;
					case GREAT:
						b->m_color = kRedColor + kGreenColor;
						break;
					case PERFECT:
						b->m_color = kWhiteColor;
						break;
					}

					if ( jadge >= BAD )
					{
						// BAD以上ならオブジェを処理
						b->mb_flg = FALSE;						// オブジェを消す
						// そのオブジェの音を再生
						//ds.Reset(b->lData);
						//ds.Play(b->lData);
						// 判定オブジェをその次からに変更
						mi_startNum[j + 0x11 + 0x20] = i + 1;
						// フラッシュ画像の定義
						mi_flashCount[j][mi_flashIndex[j]] = 45;
						mi_flashIndex[j]++;
						if ( mi_flashIndex[j] > 2 )mi_flashIndex[j] = 0;
						// 判定オブジェをその次からに変更
						mi_startNum[j + 0x11 + 0x20] = i + 1;
						break;
					}
				}
			}
		}
	}
	//*/
	// 鍵盤のバックライト
	for ( i = 0; i < 5; i++ )
	{
		if ( INPUT.GetKeyStateToManager( KEYID[i] ) == KEYSTATE::HOLD )
		{
			// キーが押された状態ならカウンタをリセット
			mi_backKeyCount[i] = 30;
		}
	}

	// スクラッチのバックライト
	//if (iScratchStatus != 0) {
	//	// 右か左に回している状態ならカウンタをリセット
	//	iBackKeyCount[5] = 30;
	//}
	ImGui::Text( "%ld", mi_hitSubNum );
	ImGui::Text( "%ld", mi_pressNum );
	ImGui::Text( "%s", m_noteManager->GetJadge() );
	ImGui::Text( "%ld", m_noteManager->GetScore() );
	ImGui::Text( "%s", m_jadge );
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

void GameScene::PostDraw()
{
	for ( auto& obj : msp_objList )
	{

	}
}

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

		static const float obj_x[6] = { -256,-128,0,128,256,512 };			// オブジェ表示X座標

		//add:note類
		m_noteManager->Draw( mf_scrollMulti );

		for ( int j = 0; j < 6; j++ )
		{
			KdShaderManager::Instance().ChangeBlendState( KdBlendState::Add );

			Math::Color color = { 0.4f, 0.4f, 0.4f, (float)mi_backKeyCount[j] / 30 };
			KdShaderManager::Instance().m_spriteShader.DrawTex(
				&m_keyBackTex,
				obj_x[j], KEYBACK_Y,
				nullptr, &color );

			float scale = sin( ( (float)( -mi_hitEffectCount[j] + 10 ) / 10 ) * M_PI / 2 ) / 2 + 1.0f;
			float alpha = ( (float)mi_hitEffectCount[j] / 10 );
			color = { 1, 1, 1, alpha };
			KdShaderManager::Instance().m_spriteShader.DrawTex(
				&m_hitEffectTex,
				obj_x[j], BAR_Y,
				HITEFFECT_HALFWIDTH * scale, HITEFFECT_HALFHEIGHT * scale,
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
