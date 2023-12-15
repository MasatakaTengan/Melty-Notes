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
	}

	virtual Math::Rectangle GetRect()
	{
		return Math::Rectangle( (long)m_pos.x, (long)m_pos.y, msp_tex->GetInfo().Width, msp_tex->GetInfo().Height );
	}
	virtual Math::Vector2 GetPos() { return m_pos; }

protected:

	Math::Vector2 m_pos = {};

	std::shared_ptr<KdTexture> msp_tex = nullptr;

};