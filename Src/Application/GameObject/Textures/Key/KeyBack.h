#pragma once
#include "../TextureBase.h"

class KeyBack : public TextureBase
{
public:

	KeyBack() {}
	~KeyBack() {}

	void Update()override;
	void Draw()override;

	void SetCount( int _cnt )
	{
		mi_count = _cnt;
	}
	int GetCount() { return mi_count; }

private:

	int mi_myKey = 0;
	int mi_count = 0;

};