#pragma once

#include "../BaseScene/BaseScene.h"

class ScrollSpeed;
class Volume;

class OptionScene : public BaseScene
{
public:

	OptionScene() {}
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

	std::shared_ptr<ScrollSpeed> msp_scrollSpeed = nullptr;
	std::shared_ptr<Volume> msp_volume = nullptr;

};