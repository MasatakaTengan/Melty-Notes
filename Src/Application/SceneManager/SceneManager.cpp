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

	msp_inputManager = std::make_shared<InputManager>(Application::Instance().GetWindowHandle());

	ChangeScene(SID_TITLE);


}

void SceneManager::Event()
{
	msp_nowScene->Event();
}

void SceneManager::PreUpdate()
{
	msp_nowScene->PreUpdate();
	ChangeScene(m_nextID);
}

void SceneManager::Update()
{
	if (ImGui::Begin("SceneUpdateDebug"))
	{
		msp_nowScene->Update();
	}
	ImGui::End();
}

void SceneManager::PostUpdate()
{
	msp_nowScene->PostUpdate();
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
	if (ImGui::Begin("SceneDrawSpriteDebug"))
	{
		msp_nowScene->DrawSprite();
	}
	ImGui::End();
}

void SceneManager::SetNextScene(UIID _id)
{
	switch (_id)
	{
		case UIID_NONE:
			break;
		case UIID_TOTITLE:
			SetNextScene(SID_TITLE);
			break;
		case UIID_TOSELECT:
			SetNextScene(SID_SELECT);
			break;
		case UIID_TOOPTION:
			SetNextScene(SID_OPTION);
			break;
		case UIID_TOGAME:
			SetNextScene(SID_GAME);
			break;
		case UIID_TORESULT:
			SetNextScene(SID_RESULT);
			break;
		default:
			break;
	}
}

void SceneManager::ChangeScene(SceneID _id)
{
	if (m_nowID == _id)return;

	switch (_id)
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
	msp_nowScene->SetOwner(shared_from_this());
	msp_nowScene->SetInputManager(msp_inputManager);

	m_nowID = _id;
}
