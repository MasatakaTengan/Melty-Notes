#include "ResultScene.h"
#include "../SceneManager.h"
#include "../../GameObject/UI/ToTitle/ToTitle.h"
#include "../../GameObject/Textures/Score/Score.h"

void ResultScene::Init()
{
	std::shared_ptr<TextureBase> back = std::make_shared<TextureBase>();
	back->LoadTex( "Asset/Textures/back.png" );
	back->SetPos( { 0, 0 } );
	msp_texList.push_back( back );

	std::shared_ptr<ToTitle> toTitle = std::make_shared<ToTitle>();
	toTitle->LoadTex( "Asset/Textures/ToTitle.png" );
	toTitle->SetPos( { 0, -200 } );
	toTitle->SetIsScene( true );
	msp_uiList.push_back( toTitle );

	std::shared_ptr<KdTexture> tex = std::make_shared<KdTexture>();
	tex->Load( "Asset/Textures/numbers.png" );
	msp_score = std::make_shared<Score>( Math::Vector2( 280, 100 ), Math::Vector2( 0.5f ), Constant::SCORE_NUM_RESULTSIZEHALF );
	msp_score->SetTex( tex );
	msp_score->Init();

	mi_score = SceneManager::Instance().GetScore();
}

void ResultScene::Event()
{
}

void ResultScene::PreUpdate()
{
	//‘S‚Ä‚ÌUI‚É‚Â‚¢‚Ä
	for ( auto& ui : msp_uiList )
	{
		ui->Update();
	}
}

void ResultScene::Update()
{
	msp_score->Update( mi_score );
}

void ResultScene::PostUpdate()
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
		}
	}
}

void ResultScene::PreDraw()
{
}

void ResultScene::DrawLit()
{
}

void ResultScene::DrawUnLit()
{
}

void ResultScene::PostDraw()
{
}

void ResultScene::DrawSprite()
{
	for ( auto& tex : msp_texList )
	{
		tex->Draw();
	}
	for ( auto& ui : msp_uiList )
	{
		ui->Draw();
	}
	msp_score->Draw();
}
