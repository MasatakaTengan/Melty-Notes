#include "BaseScene.h"
#include "../../GameObject/UI/UIBase.h"
#include "../../GameObject/Textures/TextureBase.h"
#include "../SceneManager.h"

void BaseScene::Init()
{
}

void BaseScene::Event()
{
}

void BaseScene::PreUpdate()
{
	for (auto& obj : msp_objList)
	{
		obj->PreUpdate();
	}
}

void BaseScene::Update()
{
	for (auto& obj : msp_objList)
	{
		obj->Update();
	}
}

void BaseScene::PostUpdate()
{
	for (auto& obj : msp_objList)
	{
		obj->PostUpdate();
	}
}

void BaseScene::PreDraw()
{
	for (auto& obj : msp_objList)
	{
		obj->PreDraw();
	}
}

void BaseScene::DrawLit()
{
	for (auto& obj : msp_objList)
	{
		obj->DrawLit();
	}
}

void BaseScene::DrawUnLit()
{
	for (auto& obj : msp_objList)
	{
		obj->DrawUnLit();
	}
}

void BaseScene::PostDraw()
{
}

void BaseScene::DrawSprite()
{
	for (auto& obj : msp_objList)
	{
		obj->DrawSprite();
	}
}
