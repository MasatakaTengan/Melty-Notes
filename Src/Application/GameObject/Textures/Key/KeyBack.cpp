#include "KeyBack.h"

void KeyBack::Update()
{}

void KeyBack::Draw()
{
	KdShaderManager::Instance().ChangeBlendState( KdBlendState::Add );
	Math::Color color = Math::Color( 0.5f, 0.5f, 0.5f, (float)mi_count / Constant::KEYBACK_COUNT );
	KdShaderManager::Instance().m_spriteShader.DrawTex(
		msp_tex.get(),
		m_pos.x,
		m_pos.y,
		nullptr,
		&color )
		;
	KdShaderManager::Instance().ChangeBlendState( KdBlendState::Alpha );
}
