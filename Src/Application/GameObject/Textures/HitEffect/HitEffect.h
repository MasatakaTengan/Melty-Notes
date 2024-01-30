#pragma once
#include "../TextureBase.h"

class HitEffect : public TextureBase
{
public:

	HitEffect() {}
	//HitEffect(  ) { SetTex( _tex ); }
	~HitEffect() {}

	void Init( std::shared_ptr<KdTexture> _tex );
	void Draw()override;

	void SetCount( double _cnt ) { md_count = _cnt; }
	double GetCount() { return md_count; }

	void SetJadge( JADGE _jadge ) { m_jadge = _jadge; }

private:

	double md_count = 0;
	JADGE m_jadge = JADGE::MISS;

};