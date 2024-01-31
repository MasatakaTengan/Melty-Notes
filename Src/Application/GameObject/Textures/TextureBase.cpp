#include "TextureBase.h"

void TextureBase::LoadTex( std::string_view _filename )
{
	msp_tex = std::make_shared<KdTexture>();
	msp_tex->Load( _filename );
	m_srcRect = { 0l, 0l, (long)msp_tex->GetInfo().Width, (long)msp_tex->GetInfo().Height };
}

void TextureBase::Update()
{}

void TextureBase::Draw()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex( msp_tex.get(), (int)m_pos.x, (int)m_pos.y,
		(int)(msp_tex.get()->GetInfo().Width * m_scale.x), (int)(msp_tex.get()->GetInfo().Height * m_scale.y) );
}
