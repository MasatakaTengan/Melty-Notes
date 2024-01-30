#include "Numbers.h"

void Numbers::Draw()
{
	Math::Rectangle src = { mi_myNum * Constant::NUMBERS_WIDTH, 0,
			Constant::NUMBERS_WIDTH, Constant::NUMBERS_HEIGHT };
	KdShaderManager::Instance().m_spriteShader.DrawTex(
		msp_tex.get(),
		(int)m_pos.x,
		(int)m_pos.y,
		(int)(Constant::NUMBERS_HALFWIDTH * m_scale.x),
		(int)(Constant::NUMBERS_HALFHEIGHT * m_scale.y),
		&src, &m_color );
}
