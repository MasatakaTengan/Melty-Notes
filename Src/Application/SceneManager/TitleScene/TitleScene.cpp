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

	//�N���b�N���ꂽ
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		for (auto& ui : msp_uiList)
		{
			//UI���L����
			if (ui->GetEnable())continue;		//�L���ɂȂ��Ă���UI�̓X�L�b�v����
			//UI��������Ă��Ȃ����
			if (!ui->GetPush())
			{
				//���͈͓��ŃN���b�N���ꂽ��
				if (ui->IsRange(mousePos))
				{
					ui->SetPush(true);
				}
			}
			//UI��������Ă�����
			else
			{
				//���͈͓��ŃN���b�N���ꂽ��
				if (ui->IsRange(mousePos))
				{
					//�����������Ă�����
				}
			}
		}
	}
	//�N���b�N����Ă��Ȃ�
	else
	{
		for (auto& ui : msp_uiList)
		{
			//UI���L����
			if (ui->GetEnable())continue;		//�L���ɂȂ��Ă���UI�̓X�L�b�v����
			//UI��������Ă��Ȃ����
			if (!ui->GetPush())
			{
				//���͈͓��ŃN���b�N�������ꂽ��
				if (ui->IsRange(mousePos))
				{
					//�Ȃ���N���ĂȂ����
				}
			}
			//UI��������Ă�����
			else
			{
				//���͈͓��ŃN���b�N�������ꂽ��
				if (ui->IsRange(mousePos))
				{
					//UI�̌��ʔ���
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
