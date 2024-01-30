#pragma once
#include "../../Textures/TextureBase.h"

using namespace Constant;

class Note : public TextureBase
{
public:

	Note();
	~Note();

	void Init();
	void Update( LONG _nowCount );
	void Draw( float _scrMulti );

	void SetCount( LONG _count ) { ml_count = _count; }
	void SetKey( int _key ) { mi_key = _key; }
	void SetEnable( bool _flg ) { mb_enable = _flg; }
	void SetJadge( JADGE _jadge ) { m_jadge = _jadge; }
	void SetHit( bool _flg ) { mb_hit = _flg; }
	void SetBPM( float _bpm ) { mf_bpm = _bpm; }

	bool GetEnable() { return mb_enable; }
	bool GetHit() { return mb_hit; }
	int GetKey() { return mi_key; }
	LONG GetCount() { return ml_count; }
	JADGE GetJadge() { return m_jadge; }

	int GetJadgeCnt() { return mi_jadgeCnt; }
	void SetJadgeCnt( int _cnt ) { mi_jadgeCnt = _cnt; }

private:

	bool mb_enable;
	bool mb_hit;
	int mi_key;
	float mf_bpm;

	LONG ml_count;
	LONG ml_nowCount;
	JADGE m_jadge;

	Math::Color m_color;

	//debug
	int mi_hitSubNum;
	char m_jadgeString[100];
	int mi_jadgeCnt;

};