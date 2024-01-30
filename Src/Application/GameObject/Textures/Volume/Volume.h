#pragma once

class Numbers;

class Volume
{
public:

	Volume() {}
	~Volume() {}

	void Init();
	void Update( int _vol );
	void Draw();

	void SetTex( std::shared_ptr<KdTexture> _tex )
	{
		msp_numTex = _tex;
	}

private:

	std::shared_ptr<KdTexture> msp_numTex = nullptr;

	std::vector<std::shared_ptr<Numbers>> msp_numbersList = {};

};