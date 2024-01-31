#include "HitEffect.h"

void HitEffect::Init( std::shared_ptr<KdTexture> _tex )
{
	SetTex( _tex );
}

void HitEffect::Draw()
{
	float scale = (float)sin( ((float)(-md_count + Constant::HITEFFECT_COUNT) / Constant::HITEFFECT_COUNT) * M_PI / 2 ) / 2 + 1.0f;
	float alpha = ((float)md_count / Constant::HITEFFECT_COUNT);
	Math::Rectangle src = { 0, m_jadge * Constant::HITEFFECT_HEIGHT, Constant::HITEFFECT_WIDTH, Constant::HITEFFECT_HEIGHT };
	Math::Color color = { 1, 1, 1, alpha };
	KdShaderManager::Instance().m_spriteShader.DrawTex(
		msp_tex.get(),
		(int)m_pos.x, (int)m_pos.y,
		(int)(Constant::HITEFFECT_WIDTH * scale),
		(int)(Constant::HITEFFECT_HEIGHT * scale),
		&src, &color );
}
