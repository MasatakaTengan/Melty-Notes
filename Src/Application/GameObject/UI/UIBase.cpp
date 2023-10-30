#include "UIBase.h"

void UIBase::Init()
{
}

void UIBase::LoadTex(std::string_view _filename)
{
	m_tex.Load(_filename);
}

void UIBase::DrawSprite()
{
	Math::Rectangle rect = {};
	Math::Color color = b_push ? kWhiteColor * 0.8f : kWhiteColor;
	KdShaderManager::Instance().m_spriteShader.DrawTex(&m_tex, m_pos.x, m_pos.y, nullptr, &color);/*
	KdShaderManager::Instance().m_spriteShader.DrawLine(-160, 90, 160, -90);
	KdShaderManager::Instance().m_spriteShader.DrawLine(-160, -90, 160, 90);*/
}

bool UIBase::IsRange(POINT _pos)
{
	_pos.x -= Constant::WINDOW_HALFWIDTH;
	_pos.y -= Constant::WINDOW_HALFHEIGHT;
	_pos.y *= -1;

	if (((m_pos.x - (m_tex.GetInfo().Width / 2.0f)) < _pos.x && _pos.x < (m_pos.x + (m_tex.GetInfo().Width / 2.0f))) &&
		((m_pos.y - (m_tex.GetInfo().Height / 2.0f)) < _pos.y && _pos.y < (m_pos.y + (m_tex.GetInfo().Height / 2.0f))))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UIBase::Release()
{
	m_tex.Release();
}
