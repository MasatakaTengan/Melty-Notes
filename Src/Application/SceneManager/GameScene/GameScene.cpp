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
	lane->LoadTex( "Asset/Textures/back_lane.png" );
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
	m_hitSEInstance->SetVolume( SceneManager::Instance().GetVolume() );
	
	mb_bgmStart = false;
	mb_bgmPlay = true;

	m_bgmFile = "Score/Shining_Star/shining_star.wav";
	//m_bgmFile = "Score/FREEDOM_DIVE/FREEDOM-DiVE��.wav";
	m_bgmInstance = KdAudioManager::Instance().Play( m_bgmFile );
	m_bgmInstance->Stop();
	m_bgmInstance->SetVolume( SceneManager::Instance().GetVolume() );

	bms.Init();
	//bms.Load( "Score/FREEDOM_DIVE/freedomdive_easy.bms" );
	bms.Load( "Score/Shining_Star/ShiningStar_easy.bms" );

	mf_scrollMulti = SceneManager::Instance().GetScrollSpeed();
	md_elapsedTime = 0;

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

	for ( int i = 0; i < (int)msp_keyBackList.size(); i++ )
	{
		if ( msp_keyBackList[i]->GetCount() > 0 )
		{
			msp_keyBackList[i]->SetCount( msp_keyBackList[i]->GetCount() - 1 );
		}
	}

	// �X�N���[�����ύX
	if ( INPUT.GetKeyStateToManager( VK_UP ) == KEYSTATE::PRESS )
	{
		// ����L�[
		mf_scrollMulti += 0.5f;
		if ( mf_scrollMulti > MAX_SCROLLSPEED )mf_scrollMulti = MAX_SCROLLSPEED;
		SceneManager::Instance().SetScrollSpeed( mf_scrollMulti );
	}
	if ( INPUT.GetKeyStateToManager( VK_DOWN ) == KEYSTATE::PRESS )
	{
		// �����L�[
		mf_scrollMulti -= 0.5f;
		if ( mf_scrollMulti < MIN_SCROLLSPEED )mf_scrollMulti = MIN_SCROLLSPEED;
		SceneManager::Instance().SetScrollSpeed( mf_scrollMulti );
	}
}

void GameScene::PreUpdate()
{}

void GameScene::Update()
{

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
		SceneManager::Instance().SetScore( msp_noteManager->GetScore() );
		SceneManager::Instance().SetNextScene( UIID::UIID_TORESULT );
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

}

void GameScene::PostUpdate()
{}

void GameScene::PreDraw()
{}

void GameScene::DrawLit()
{}

void GameScene::DrawUnLit()
{}

void GameScene::DrawSprite()
{

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
