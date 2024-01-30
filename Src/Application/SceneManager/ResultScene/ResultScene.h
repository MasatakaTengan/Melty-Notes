#pragma once

#include "../BaseScene/BaseScene.h"

class Score;

class ResultScene : public BaseScene
{
public:

	ResultScene() {}
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

	std::shared_ptr<Score> msp_score = nullptr;

	int mi_score = 0;

};