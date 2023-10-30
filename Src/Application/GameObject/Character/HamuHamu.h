#pragma once

class CameraBase;
class HamuHamu : public KdGameObject
{
public:
	HamuHamu() {}
	virtual ~HamuHamu()		override {}

	void Init()				override;
	void Update()			override;
	void DrawLit()			override;

	void SetCamera(const std::shared_ptr<CameraBase>& camera)
	{
		m_wpCamera = camera;
	}

private:

	void UpdateRotate(Math::Vector3& srcMoveVec);

	std::shared_ptr<KdSquarePolygon>	m_spPoly = nullptr;
	std::weak_ptr<CameraBase>			m_wpCamera;

	// ƒ[ƒ‹ƒhs—ñ‚ğ‘n‚éˆ×‚Ì‚»‚ê‚¼‚ê‚Ì‰ñ“]Šp“x
	Math::Vector3						m_worldRot;

	float m_movePerSecond = 3.f;
};