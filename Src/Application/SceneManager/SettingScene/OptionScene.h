#pragma once

#include "../BaseScene/BaseScene.h"

class OptionScene : public BaseScene
{
public:

	OptionScene() { Init(); }
	~OptionScene() {}

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