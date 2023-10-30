#pragma once
#include "../CameraBase.h"

class TPSCamera : public CameraBase
{
public:
	TPSCamera() {}
	virtual ~TPSCamera()		override {}

	void Init()					override;
	void Update()				override;

private:
	void UpdateRotateByMouse();

	POINT m_FixMousePos;
};