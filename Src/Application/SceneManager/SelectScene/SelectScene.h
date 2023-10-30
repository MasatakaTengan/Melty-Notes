#pragma once

#include "../BaseScene/BaseScene.h"

class SelectScene : public BaseScene
{
public:

	SelectScene() { Init(); }
	~SelectScene() {}

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