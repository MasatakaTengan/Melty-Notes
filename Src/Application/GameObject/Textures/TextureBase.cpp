#include "TextureBase.h"

void TextureBase::LoadTex( std::string_view _filename )
{
	msp_tex = std::make_shared<KdTexture>();
	msp_tex->Load( _filename );
}

void TextureBase::Update()
{}

void TextureBase::Draw()
{
	KdShaderManager::Instance().m_spriteShader.DrawTex( msp_tex.get(), (int)m_pos.x, (int)m_pos.y);
}
