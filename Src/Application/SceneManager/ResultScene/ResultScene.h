#pragma once

#include "../BaseScene/BaseScene.h"

class ResultScene : public BaseScene
{
public:

	ResultScene() { Init(); }
	~ResultScene() {}

	void Init()override;
	void Event()override;
	void PreUpdate()override;
	void Update()override;
	void PostUpdate()override;

	void PreDraw()override;
	void DrawLit()override;
	void DrawUnLit()override;
	void PostDraw()override;

	void DrawSprite()override;

private:

};