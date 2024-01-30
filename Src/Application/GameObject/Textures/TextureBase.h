#pragma once

class TextureBase
{
public:

	TextureBase() {}
	virtual ~TextureBase() {}

	virtual void LoadTex( std::string_view _filename );

	virtual void Update();
	virtual void Draw();

	virtual void SetPos( Math::Vector2 _pos ) { m_pos = _pos; }
	virtual void SetTex( std::shared_ptr<KdTexture> _tex )
	{
		msp_tex = std::make_shared<KdTexture>();
		msp_tex = _tex;
		m_srcRect = { 0l, 0l, (long)msp_tex->GetInfo().Width, (long)msp_tex->GetInfo().Height };
	}
	virtual void SetScale( Math::Vector2 _scale ) { m_scale = _scale; }
	virtual void SetSourceRectangle( Math::Rectangle _rect ) { m_srcRect = _rect; }
	virtual void SetColor( Math::Color _color ) { m_color = _color; }

	virtual Math::Vector2 GetPos() { return m_pos; }

protected:

	Math::Vector2 m_pos = {};
	Math::Vector2 m_scale = { 1.f, 1.f };
	Math::Rectangle m_srcRect = { 0l, 0l, 0l, 0l };
	Math::Color m_color = kWhiteColor;

	std::shared_ptr<KdTexture> msp_tex = nullptr;

};