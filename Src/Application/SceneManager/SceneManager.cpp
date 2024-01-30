#include "Application/main.h"
#include "SceneManager.h"

#include "BaseScene/BaseScene.h"
#include "TitleScene/TitleScene.h"
#include "SettingScene/OptionScene.h"
#include "SelectScene/SelectScene.h"
#include "GameScene/GameScene.h"
#include "ResultScene/ResultScene.h"

void SceneManager::Init()
{

	//msp_inputManager = std::make_shared<InputManager>(Application::Instance().GetWindowHandle());

	ChangeScene( SID_TITLE );

	msp_optionChanger = std::make_shared<OptionChanger>();
	msp_optionChanger->LoadOption();

}

void SceneManager::Event()
{
	msp_nowScene->Event();
}

void SceneManager::PreUpdate()
{
	INPUT.KeyStateUpdate();
	msp_nowScene->PreUpdate();
}

void SceneManager::Update()
{
		msp_nowScene->Update();
	/*if ( ImGui::Begin( "SceneUpdateDebug" ) )
	{
	}
	ImGui::End();*/
}

void SceneManager::PostUpdate()
{
	msp_nowScene->PostUpdate();
	ChangeScene( m_nextID );
}

void SceneManager::PreDraw()
{
	msp_nowScene->PreDraw();
}

void SceneManager::DrawLit()
{
	msp_nowScene->DrawLit();
}

void SceneManager::DrawUnLit()
{
	msp_nowScene->DrawUnLit();
}

void SceneManager::PostDraw()
{
	msp_nowScene->PostDraw();
}

void SceneManager::DrawSprite()
{
		msp_nowScene->DrawSprite();
	/*if ( ImGui::Begin( "SceneDrawSpriteDebug" ) )
	{

		ImGui::Text( "vol:%f", msp_optionChanger->GetVolume() );
		ImGui::Text( "spd:%f", msp_optionChanger->GetScrollSpeed() );
	}
	ImGui::End();*/
}

void SceneManager::SetNextScene( UIID _id )
{
	switch ( _id )
	{
		case UIID::UIID_NONE:
			break;
		case UIID::UIID_TOTITLE:
			SetNextScene( SID_TITLE );
			break;
		case UIID::UIID_TOSELECT:
			SetNextScene( SID_SELECT );
			break;
		case UIID::UIID_TOOPTION:
			SetNextScene( SID_OPTION );
			break;
		case UIID::UIID_TOGAME:
			SetNextScene( SID_GAME );
			break;
		case UIID::UIID_TORESULT:
			SetNextScene( SID_RESULT );
			break;
		default:
			break;
	}
}

const float& SceneManager::GetVolume()
{
	return msp_optionChanger->GetVolume();
}

const float& SceneManager::GetScrollSpeed()
{
	return msp_optionChanger->GetScrollSpeed();
}

void SceneManager::SetVolume( float _vol )
{
	msp_optionChanger->SetVolume( _vol );
}

void SceneManager::SetScrollSpeed( float _speed )
{
	msp_optionChanger->SetScrollSpeed( _speed );
}

void SceneManager::ChangeScene( SceneID _id )
{
	if ( m_nowID == _id )return;

	switch ( _id )
	{
		case SID_NONE:
			break;
		case SID_TITLE:
			msp_nowScene = std::make_shared<TitleScene>();
			break;
		case SID_SELECT:
			msp_nowScene = std::make_shared<SelectScene>();
			break;
		case SID_OPTION:
			msp_nowScene = std::make_shared<OptionScene>();
			break;
		case SID_GAME:
			msp_nowScene = std::make_shared<GameScene>();
			break;
		case SID_RESULT:
			msp_nowScene = std::make_shared<ResultScene>();
			break;
	}
	msp_nowScene->SetOwner( shared_from_this() );
	msp_nowScene->Init();
	//msp_nowScene->SetInputManager(msp_inputManager);

	m_nowID = _id;
}

void OptionChanger::LoadOption()
{
	std::ifstream ifs;
	ifs.open( "Data/option.txt", std::ios::in );
	if ( ifs.fail() )
	{
		std::ofstream ofs( "Data/option.txt" );
		ofs << 0.1f << std::endl;
		ofs << 10.0f << std::endl;
		ifs.open( "Data/option.txt" );
	}

	std::string buf;
	int i = 0;
	while ( !ifs.eof() )
	{
		buf.clear();
		std::getline( ifs, buf );
		//if ( buf[0] == NULL && ifs.eof() )break;

		if ( i == 0 )
		{
			m_volume = (float)std::atof( buf.data() );
			i++;
			continue;
		}
		if ( i == 1 )
		{
			m_scrollSpeed = (float)std::atof( buf.data() );
			i++;
			continue;
		}
		//if ( i >= 2 )break;
	}
}

void OptionChanger::SaveOption()
{
	std::ofstream ofs( "Data/option.txt" );

	ofs << m_volume << std::endl;
	ofs << m_scrollSpeed << std::endl;
}
