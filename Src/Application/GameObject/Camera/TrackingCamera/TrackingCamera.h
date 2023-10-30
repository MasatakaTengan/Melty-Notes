#pragma once
#include "../CameraBase.h"

class TrackingCamera : public CameraBase
{
public:
	TrackingCamera() {}
	virtual ~TrackingCamera()	override {}

	void Init()					override;
	void Update()				override;
};