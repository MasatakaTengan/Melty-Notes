#pragma once

class Numbers;

class Score
{
public:

	Score(Math::Vector2 _pos, Math::Vector2 _scale, float _size)
	{
		m_pos = _pos;
		m_scale = _scale;
		mf_size = _size;
	}
	~Score() {}

	void Init();
	void Update( int _score );
	void Draw();

	void SetTex( std::shared_ptr<KdTexture> _tex )
	{
		msp_numTex = _tex;
	}

private:

	std::shared_ptr<KdTexture> msp_numTex = nullptr;

	std::vector<std::shared_ptr<Numbers>> msp_numbersList = {};

	Math::Vector2 m_pos = {};
	Math::Vector2 m_scale = {};
	float mf_size = 0;

	bool mb_max = false;

};