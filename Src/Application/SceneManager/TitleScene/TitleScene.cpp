#include "Application/main.h"
#include "TitleScene.h"
#include "../SceneManager.h"
#include "../../GameObject/UI/ToGame/ToGame.h"
#include "../../GameObject/Textures/Title/Title.h"

void TitleScene::Init()
{
	std::shared_ptr<ToGame> toGame = std::make_shared<ToGame>();
	toGame->LoadTex( "Asset/Textures/UI/ToGame.png" );
	toGame->SetPos( { 0, -100 } );
	msp_uiList.push_back( toGame );

	std::shared_ptr<Title> title = std::make_shared<Title>();
	title->LoadTex( "Asset/Textures/Title.png" );
	title->SetPos( { 0, 0 } );
	msp_texList.push_back( title );
}

void TitleScene::Event()
{
}

void TitleScene::PreUpdate()
{
	POINT mousePos = INPUT.GetPosition( Application::Instance().GetWindowHandle() );

	//全てのUIについて
	for ( auto& ui : msp_uiList )
	{
		//有効でないUIだけ調べる
		if ( ui->GetEnable() )continue;
		//カーソルの状態が
		switch ( INPUT.GetKeyStateToManager( VK_LBUTTON ) )
		{
			//前フレームから放されて続けている状態
			case KEYSTATE::IDLING:
				break;
				//今フレームで放された状態
			case KEYSTATE::RELEASE:
				if ( ui->GetPush() )
				{
					//カーソル座標がUI上にあるか
					if ( ui->IsRange( mousePos ) )
					{
						ui->SetEnable( true );
					}
					ui->SetPush( false );
				}
				break;
				//今フレームで押された状態
			case KEYSTATE::PRESS:
				if ( !ui->GetPush() )
				{
					//カーソル座標がUI上にあるか
					if ( ui->IsRange( mousePos ) )
					{
						ui->SetPush( true );
					}
				}
				break;
				//前フレームから押され続けている状態
			case KEYSTATE::HOLD:
				/*if ( !ui->GetPush() )
				{
					ui->SetPush( true );
				}*/
				break;
		}
	}
}

void TitleScene::Update()
{}

void TitleScene::PostUpdate()
{
	UIID id = UIID_NONE;
	for ( auto& ui : msp_uiList )
	{
		if ( ui->GetEnable() )
		{
			id = ui->GetID();
			break;
		}
	}
	std::shared_ptr<SceneManager> owner = mwp_owner.lock();
	owner->SetNextScene( id );
}

void TitleScene::PreDraw()
{}

void TitleScene::DrawLit()
{}

void TitleScene::DrawUnLit()
{}

void TitleScene::PostDraw()
{}

void TitleScene::DrawSprite()
{
	for ( auto& tex : msp_texList )
	{
		tex->Draw();
	}
	for ( auto& ui : msp_uiList )
	{
		ui->Draw();
	}
}
