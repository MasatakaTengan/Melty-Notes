#include "TitleScene.h"
#include "../SceneManager.h"
#include "../../GameObject/UI/ToGame/ToGame.h"
#include "../../GameObject/UI/ToOption/ToOption.h"
#include "../../GameObject/Textures/Title/Title.h"

void TitleScene::Init()
{
	std::shared_ptr<ToGame> toGame = std::make_shared<ToGame>();
	toGame->LoadTex( "Asset/Textures/Play.png" );
	toGame->SetPos( { -200, -100 } );
	toGame->SetIsScene( true );
	msp_uiList.push_back( toGame );
	std::shared_ptr<ToOption> toOption = std::make_shared<ToOption>();
	toOption->LoadTex( "Asset/Textures/Option.png" );
	toOption->SetPos( { 200, -100 } );
	toOption->SetIsScene( true );
	msp_uiList.push_back( toOption );

	std::shared_ptr<TextureBase> back = std::make_shared<TextureBase>();
	back->LoadTex( "Asset/Textures/back.png" );
	back->SetPos( { 0, 0 } );
	msp_texList.push_back( back );
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
	//‘S‚Ä‚ÌUI‚É‚Â‚¢‚Ä
	for ( auto& ui : msp_uiList )
	{
		ui->Update();
	}
}

void TitleScene::Update()
{}

void TitleScene::PostUpdate()
{
	UIID id = UIID::UIID_NONE;
	bool isScene = false;
	for ( auto& ui : msp_uiList )
	{
		if ( ui->GetEnable() )
		{
			id = ui->GetID();
			isScene = ui->GetIsScene();
			break;
		}
	}
	if ( isScene )
	{
		SceneManager::Instance().SetNextScene( id );
	}
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
