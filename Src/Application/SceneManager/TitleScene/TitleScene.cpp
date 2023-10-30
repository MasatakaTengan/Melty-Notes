#include "Application/main.h"
#include "TitleScene.h"
#include "../SceneManager.h"
#include "../../GameObject/UI/ToGame/ToGame.h"

void TitleScene::Init()
{
	std::shared_ptr<ToGame> sp_toGame = std::make_shared<ToGame>();
	sp_toGame->LoadTex("Asset/Textures/UI/ToGame.png");
	sp_toGame->SetPos({ 0, 0 });
	msp_uiList.push_back(sp_toGame);
}

void TitleScene::Event()
{
	POINT mousePos = mwp_inputManager.lock()->GetPosition();

	//クリックされた
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		for (auto& ui : msp_uiList)
		{
			//UIが有効か
			if (ui->GetEnable())continue;		//有効になっているUIはスキップする
			//UIが押されていない状態
			if (!ui->GetPush())
			{
				//かつ範囲内でクリックされたら
				if (ui->IsRange(mousePos))
				{
					ui->SetPush(true);
				}
			}
			//UIが押されている状態
			else
			{
				//かつ範囲内でクリックされたら
				if (ui->IsRange(mousePos))
				{
					//押し続けられている状態
				}
			}
		}
	}
	//クリックされていない
	else
	{
		for (auto& ui : msp_uiList)
		{
			//UIが有効か
			if (ui->GetEnable())continue;		//有効になっているUIはスキップする
			//UIが押されていない状態
			if (!ui->GetPush())
			{
				//かつ範囲内でクリックが離されたら
				if (ui->IsRange(mousePos))
				{
					//なんも起きてないよね
				}
			}
			//UIが押されている状態
			else
			{
				//かつ範囲内でクリックが離されたら
				if (ui->IsRange(mousePos))
				{
					//UIの効果発動
					ui->SetEnable(true);
					ui->SetPush(false);
				}
				else
				{
					ui->SetPush(false);
				}
			}
		}
	}
}

void TitleScene::PreUpdate()
{
	UIID id = UIID_NONE;
	for (auto& ui : msp_uiList)
	{
		if (ui->GetEnable())
		{
			id = ui->GetID();
			break;
		}
	}
	std::shared_ptr<SceneManager> owner = mwp_owner.lock();
	owner->SetNextScene(id);
}

void TitleScene::Update()
{
}

void TitleScene::PostUpdate()
{
}

void TitleScene::PreDraw()
{
}

void TitleScene::DrawLit()
{
}

void TitleScene::DrawUnLit()
{
}

void TitleScene::PostDraw()
{
}

void TitleScene::DrawSprite()
{
	for (auto& ui : msp_uiList)
	{
		ui->DrawSprite();
	}
}
