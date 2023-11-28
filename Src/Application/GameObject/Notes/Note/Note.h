#pragma once

using namespace Constant;

class Note
{
public:

	Note();
	~Note();

	void Init();
	void Update( LONG _nowCount );
	void Draw( float _scrMulti );

	void SetCount( LONG _count )
	{
		ml_count = _count;
	}
	void SetKey( int _key )
	{
		mi_key = _key;
	}
	void SetPos( Math::Vector2 _pos )
	{
		m_pos = _pos;
	}
	void SetTex( std::shared_ptr<KdTexture> _tex )
	{
		m_tex = _tex;
	}
	void SetEnable( bool _flg )
	{
		mb_enable = _flg;
	}
	void SetJadge( JADGE _jadge )
	{
		m_jadge = _jadge;
	}

	bool GetEnable()
	{
		return mb_enable;
	}
	const int GetKey()
	{
		return mi_key;
	}
	LONG GetCount()
	{
		return ml_count;
	}
	JADGE GetJadge()
	{
		return m_jadge;
	}

private:

	bool mb_enable;
	int mi_key;
	Math::Vector2 m_pos;

	LONG ml_count;
	LONG ml_nowCount;
	JADGE m_jadge;

	Math::Color m_color;
	std::shared_ptr<KdTexture> m_tex = nullptr;

	//debug
	int mi_hitSubNum;
	char m_jadgeString[100];

};