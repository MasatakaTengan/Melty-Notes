#include "OptionScene.h"
#include "../SceneManager.h"
#include "../../GameObject/UI/ToTitle/ToTitle.h"
#include "../../GameObject/Textures/Option/Option.h"
#include "../../GameObject/Textures/Option/OptionButton.h"
#include "../../GameObject/Textures/ScrollSpeed/ScrollSpeed.h"
#include "../../GameObject/Textures/Volume/Volume.h"

void OptionScene::Init()
{
	std::shared_ptr<ToTitle> toTitle = std::make_shared<ToTitle>();
	toTitle->LoadTex( "Asset/Textures/ToTitle.png" );
	toTitle->SetPos( { -480, 270 } );
	toTitle->SetIsScene( true );
	msp_uiList.push_back( toTitle );

	std::shared_ptr<KdTexture> tex = std::make_shared<KdTexture>();
	tex->Load( "Asset/Textures/numbers.png" );
	msp_scrollSpeed = std::make_shared<ScrollSpeed>();
	msp_scrollSpeed->SetTex( tex );
	msp_scrollSpeed->Init();
	msp_volume = std::make_shared<Volume>();
	msp_volume->SetTex( tex );
	msp_volume->Init();

	for ( int i = 0; i < 2; i++ )
	{
		for ( int j = 0; j < 2; j++ )
		{
			std::shared_ptr<OptionButton> button = std::make_shared<OptionButton>();
			button->LoadTex( "Asset/Textures/OptionButton.png" );
			button->SetPos( { (float)Constant::OPTIONBUTTON_X[i], (float)Constant::OPTIONBUTTON_Y[j] } );
			if ( i == 0 )
			{
				if ( j == 0 )button->SetID( UIID::UIID_VOL_DOWN );
				else button->SetID( UIID::UIID_SPD_DOWN );
			}
			else
			{
				button->SetScale( { -1.f,1.f } );
				if ( j == 0 )button->SetID( UIID::UIID_VOL_UP );
				else button->SetID( UIID::UIID_SPD_UP );
			}
			msp_uiList.push_back( button );
		}
	}

	std::shared_ptr<TextureBase> back = std::make_shared<TextureBase>();
	back->LoadTex( "Asset/Textures/back.png" );
	back->SetPos( { 0, 0 } );
	msp_texList.push_back( back );
	std::shared_ptr<Option> option = std::make_shared<Option>();
	option->LoadTex( "Asset/Textures/OptionScene.png" );
	option->SetPos( { 0, 0 } );
	msp_texList.push_back( option );
}

void OptionScene::Event()
{}

void OptionScene::PreUpdate()
{
	//‘S‚Ä‚ÌUI‚É‚Â‚¢‚Ä
	for ( auto& ui : msp_uiList )
	{
		ui->Update();
	}
}

void OptionScene::Update()
{}

void OptionScene::PostUpdate()
{

	UIID id = UIID::UIID_NONE;
	bool isScene = false;
	for ( auto& ui : msp_uiList )
	{
		if ( ui->GetEnable() )
		{
			id = ui->GetID();
			isScene = ui->GetIsScene();
			if ( isScene )
			{
				SceneManager::Instance().SetNextScene( id );
				break;
			}
			else
			{
				switch ( id )
				{
					case UIID_VOL_UP:
						SceneManager::Instance().SetVolume(
							SceneManager::Instance().GetVolume() + Constant::VALIATION_VOLUME );
						if ( SceneManager::Instance().GetVolume() > Constant::MAX_VOLUME )
						{
							SceneManager::Instance().SetVolume( Constant::MAX_VOLUME );
						}
						break;
					case UIID_VOL_DOWN:
						SceneManager::Instance().SetVolume(
							SceneManager::Instance().GetVolume() - Constant::VALIATION_VOLUME );
						if ( SceneManager::Instance().GetVolume() < Constant::MIN_VOLUME )
						{
							SceneManager::Instance().SetVolume( Constant::MIN_VOLUME );
						}
						break;
					case UIID_SPD_UP:
						SceneManager::Instance().SetScrollSpeed(
							SceneManager::Instance().GetScrollSpeed() + Constant::VALIATION_SCROLLSPEED );
						if ( SceneManager::Instance().GetScrollSpeed() > Constant::MAX_SCROLLSPEED )
						{
							SceneManager::Instance().SetScrollSpeed( Constant::MAX_SCROLLSPEED );
						}
						break;
					case UIID_SPD_DOWN:
						SceneManager::Instance().SetScrollSpeed(
							SceneManager::Instance().GetScrollSpeed() - Constant::VALIATION_SCROLLSPEED );
						if ( SceneManager::Instance().GetScrollSpeed() < Constant::MIN_SCROLLSPEED )
						{
							SceneManager::Instance().SetScrollSpeed( Constant::MIN_SCROLLSPEED );
						}
						break;
					default:
						break;
				}
				ui->SetEnable( false );
			}
		}
	}
	msp_scrollSpeed->Update( (int)((SceneManager::Instance().GetScrollSpeed()) * 10) );
	msp_volume->Update( (int)((SceneManager::Instance().GetVolume()) * 100) );
}

void OptionScene::PreDraw()
{}

void OptionScene::DrawLit()
{}

void OptionScene::DrawUnLit()
{}

void OptionScene::PostDraw()
{}

void OptionScene::DrawSprite()
{
	for ( auto& tex : msp_texList )
	{
		tex->Draw();
	}
	for ( auto& ui : msp_uiList )
	{
		ui->Draw();
	}
	msp_scrollSpeed->Draw();
	msp_volume->Draw();
}
