#include "Application/main.h"
#include "GameScene.h"
#include "../SceneManager.h"
#include "../../GameObject/Notes/NoteManager/NoteManager.h"
#include "../../GameObject/Textures/Lane/Lane.h"
#include "../../GameObject/Textures/Key/KeyBack.h"
#include "../../GameObject/Textures/HitEffect/HitEffect.h"
#include "../../GameObject/Textures/Score/Score.h"

void GameScene::Init()
{
	std::shared_ptr<SceneManager> owner = mwp_owner.lock();

	m_noteTex.Load( "Asset/Textures/notes.png" );
	m_keyBackTex.Load( "Asset/Textures/keyback.png" );
	msp_hitEffectTex = std::make_shared<KdTexture>();
	msp_hitEffectTex->Load( "Asset/Textures/hit_effect_square.png" );
	for ( int i = 0; i < 4; i++ )
	{
		std::shared_ptr<KeyBack> keyback = std::make_shared<KeyBack>();
		keyback->LoadTex( "Asset/Textures/keyback.png" );
		keyback->SetPos( { (float)Constant::LANE_X[i], (float)Constant::KEYBACK_Y } );
		msp_keyBackList.push_back( keyback );
	}
	std::shared_ptr<TextureBase> back = std::make_shared<TextureBase>();
	back->LoadTex( "Asset/Textures/back.png" );
	back->SetPos( { 0, 0 } );
	msp_texList.push_back( back );
	std::shared_ptr<Lane> lane = std::make_shared<Lane>();
	lane->LoadTex( "Asset/Textures/back_lane (1).png" );
	lane->SetPos( { 0, 0 } );
	msp_texList.push_back( lane );
	std::shared_ptr<KdTexture> tex = std::make_shared<KdTexture>();
	tex->Load( "Asset/Textures/numbers.png" );
	msp_score = std::make_shared<Score>( Math::Vector2( WINDOW_HALFWIDTH, WINDOW_HALFHEIGHT ), Math::Vector2( 0.2f ), SCORE_NUM_GAMESIZEHALF );
	msp_score->SetTex( tex );
	msp_score->Init();

	m_hitSEFile = "Asset/Audio/drum-hitnormal.wav";
	m_hitSEInstance = KdAudioManager::Instance().Play( m_hitSEFile );
	m_hitSEInstance->Stop();
	m_hitSEInstance->SetVolume( owner->GetVolume() );

	mb_bgmStart = false;
	mb_bgmPlay = true;

	m_bgmFile = "Score/Shining_Star/shining_star.wav";
	//m_bgmFile = "Score/FREEDOM_DIVE/FREEDOM-DiVE��.wav";
	m_bgmInstance = KdAudioManager::Instance().Play( m_bgmFile );
	m_bgmInstance->Stop();
	m_bgmInstance->SetVolume( owner->GetVolume() );

	bms.Init();
	//bms.Load( "Score/FREEDOM_DIVE/freedomdive_easy.bms" );
	bms.Load( "Score/Shining_Star/ShiningStar_easy.bms" );

	mf_scrollMulti = owner->GetScrollSpeed();
	md_elapsedTime = 0;

	ZeroMemory( &mi_backKeyCount, sizeof( mi_backKeyCount ) );
	ZeroMemory( &mi_hitEffectCount, sizeof( mi_hitEffectCount ) );

	// �}�V���̎��g�����擾
	LARGE_INTEGER freq;
	QueryPerformanceFrequency( &freq );
	mll_globalFreq = freq.QuadPart;

	// ���݂̎��Ԃ��J�n���ԂƂ���
	LARGE_INTEGER li;
	QueryPerformanceCounter( &li );
	mll_startTime = li.QuadPart;
	mll_beforeTime = li.QuadPart;

	mll_offsetTime = 0;

	msp_noteManager = std::make_shared<NoteManager>();
	msp_noteManager->Init( bms );
	msp_noteManager->SetOwner( shared_from_this() );

	mi_jadgeOffset = 00;

	//debug
	mi_hitSubNum = 0;
	mi_pressNum = 0;
}

void GameScene::Event()
{

	for ( int i = 0; i < msp_keyBackList.size(); i++ )
	{
		if ( msp_keyBackList[i]->GetCount() > 0 )
		{
			msp_keyBackList[i]->SetCount( msp_keyBackList[i]->GetCount() - 1 );
		}
	}
	// ���̃o�b�N���C�g���o
	for ( int i = 0; i < 6; i++ )
	{
		if ( mi_backKeyCount[i] > 0 )
			mi_backKeyCount[i]--;
		if ( mi_hitEffectCount[i] > 0 )
			mi_hitEffectCount[i]--;
	}

	// �X�N���[�����ύX
	if ( GetAsyncKeyState( VK_UP ) & 0x8000 )
	{
		// �e���L�[�́{
		mf_scrollMulti += 0.05f;
	}
	if ( GetAsyncKeyState( VK_DOWN ) & 0x8000 )
	{
		// �e���L�[�́|
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

	// �J�n������o�߂������Ԃ��Z�o
	LARGE_INTEGER li;
	//�����[�v�̎��Ԃ��擾!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	QueryPerformanceCounter( &li );
	md_elapsedTime = (double)(li.QuadPart - mll_startTime) / mll_globalFreq;

	//�����Ȃ��Đ����Ȃ�
	if ( !mb_bgmPlay )
	{
		//�O���[�v����o�߂������Ԃ��Z�o���A���v�̌o�ߎ��Ԃɉ��Z����
		mll_offsetTime += li.QuadPart - mll_beforeTime;
	}
	//�Ȃ��Đ����łȂ��Ȃ�o�ߎ��Ԃ̉��Z�����Ȃ����߁A�|�[�Y��ʒ��̏��������s����͂�

	//�O���[�v����̌o�ߎ���
	md_difTime = (double)(li.QuadPart - mll_beforeTime) / mll_globalFreq;

	//�O���[�v���̎��Ԃ��X�V
	mll_beforeTime = li.QuadPart;

	// �o�߂������Ԃ���i��BMS�J�E���g�l���Z�o
	LONG now_count = bms.GetCountFromTime( md_elapsedTime - ((double)mll_offsetTime / mll_globalFreq) );

	// BMS�J�E���^���Ȃ̍ő�J�E���g+1���߂𒴂�����I��
	if ( bms.GetMaxCount() + (BMS_RESOLUTION * 1) <= now_count )
	{
		std::shared_ptr<SceneManager> owner = mwp_owner.lock();
		owner->SetScore( msp_noteManager->GetScore() );
		owner->SetNextScene( UIID::UIID_TORESULT );
		//Application::Instance().End();
	}
	// BMS�J�E���^���Ȃ̊J�n�ʒu�ɗ�����Đ�����
	LONG bmsStart = bms.GetObje( BMS_BACKMUSIC, 0 )->ml_time;
	if ( now_count >= (bmsStart + mi_jadgeOffset) )
	{
		if ( !mb_bgmStart )
		{
			mb_bgmStart = true;
			m_bgmInstance->Play();
		}
	}

	// ���Ղ̏���
	for ( int i = 0; i < 4; i++ )
	{
		if ( INPUT.GetKeyStateToManager( Constant::KEYID_4K[i] ) == KEYSTATE::PRESS )
		{
			m_hitSEInstance->Play();
			//mi_hitEffectCount[i] = 10;
		}
	}

	if ( msp_hitEffectList.size() > 0 )
	{
		for ( auto it = msp_hitEffectList.begin();
			it != msp_hitEffectList.end(); )
		{
			//�O���[�v����̌o�ߎ���[s]��ݒ肳��Ă���FPS�l�ɏ�Z���Čo�߃t���[����(��:0.1f)���Z�o�����̌�GetCount���猸�Z
			(*it)->SetCount( (*it)->GetCount() - md_difTime );
			if ( (*it)->GetCount() < 0 )
			{
				it = msp_hitEffectList.erase( it );
			}
			else
			{
				it++;
			}
		}
	}

	// ���͔���

	// note��
	msp_noteManager->PreUpdate();
	msp_noteManager->Update( now_count );

	// ���Ղ̃o�b�N���C�g
	for ( int i = 0; i < 4; i++ )
	{
		if ( INPUT.GetKeyStateToManager( Constant::KEYID_4K[i] ) == KEYSTATE::HOLD )
		{
			// �L�[�������ꂽ��ԂȂ�J�E���^�����Z�b�g
			msp_keyBackList[i]->SetCount( Constant::KEYBACK_COUNT );
		}
	}

	msp_score->Update( msp_noteManager->GetScore() );

	/*ImGui::Text( "%f", bms.GetBPM() );
	ImGui::Text( "%s", msp_noteManager->GetJadge() );
	ImGui::Text( "%ld", msp_noteManager->GetScore() );
	ImGui::Text( "%ld", msp_noteManager->GetJadgeCnt() );
	ImGui::Text( "%ld", now_count );*/
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
	/*if ( ImGui::Begin( "GameSceneDebug" ) )
	{*/
		for ( auto& obj : msp_objList )
		{
			obj->DrawSprite();
		}
		//ImGui::Text( "ScreenScale : %.2f", mf_scrollMulti );

		for ( auto& tex : msp_texList )
		{
			tex->Draw();
		}

		msp_score->Draw();

		for ( auto& keyback : msp_keyBackList )
		{
			keyback->Draw();
		}
		for ( auto& effect : msp_hitEffectList )
		{
			effect->Draw();
		}

		//add:note��
		msp_noteManager->Draw( mf_scrollMulti );

		for ( int j = 0; j < 4; j++ )
		{

			KdShaderManager::Instance().ChangeBlendState( KdBlendState::Add );

			/*float scale = (float)sin( ((float)(-mi_hitEffectCount[j] + 10) / 10) * M_PI / 2 ) / 2 + 1.0f;
			float alpha = ((float)mi_hitEffectCount[j] / 10);
			Math::Color color = { 1, 1, 1, alpha };
			KdShaderManager::Instance().m_spriteShader.DrawTex(
				msp_hitEffectTex.get(),
				Constant::LANE_X[j], BAR_Y,
				(int)(HITEFFECT_HALFWIDTH * scale), (int)(HITEFFECT_HALFHEIGHT * scale),
				nullptr, &color );*/

			KdShaderManager::Instance().ChangeBlendState( KdBlendState::Alpha );
		}

		/*KdShaderManager::Instance().m_spriteShader.DrawLine(
			-WINDOW_HALFWIDTH / 2, BAR_Y + NOTE_HALFHEIGHT,
			WINDOW_HALFWIDTH / 2, BAR_Y + NOTE_HALFHEIGHT );
		KdShaderManager::Instance().m_spriteShader.DrawLine(
			-WINDOW_HALFWIDTH / 2, BAR_Y - NOTE_HALFHEIGHT,
			WINDOW_HALFWIDTH / 2, BAR_Y - NOTE_HALFHEIGHT );*/

	//}

	////ImGui::End();
}

void GameScene::AddHitEffect( Math::Vector2 _pos, JADGE _jadge )
{
	std::shared_ptr<HitEffect> effect = std::make_shared<HitEffect>();
	effect->Init( msp_hitEffectTex );
	effect->SetCount( Constant::HITEFFECT_COUNT );
	effect->SetPos( _pos );
	effect->SetJadge( _jadge );
	msp_hitEffectList.push_back( effect );
}
