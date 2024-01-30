#pragma once
#include "../TextureBase.h"

class Numbers : public TextureBase
{
public:

	Numbers() {}
	~Numbers() {}

	void Draw()override;

	void SetScale(Math::Vector2 _scale)
	{
		m_scale = _scale;
	}
	void SetMyNum( int _num )
	{
		mi_myNum = _num;
	}
	void SetColor( Math::Color _color )
	{
		m_color = _color;
	}

private:

	Math::Color m_color = kWhiteColor;

	Math::Vector2 m_scale = {};
	int mi_myNum = 0;

};