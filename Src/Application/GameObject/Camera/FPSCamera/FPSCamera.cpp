#include "FPSCamera.h"

void FPSCamera::Init()
{
	// 基準点(ターゲット)の目線の位置
	m_LocalPos = Math::Matrix::CreateTranslation(0, 1.5f, 0.0f);

	m_FixMousePos.x = 640;
	m_FixMousePos.y = 360;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	CameraBase::Init();
}

void FPSCamera::Update()
{
	// ターゲットの行列(有効な場合利用する)
	Math::Matrix targetMat = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		const std::shared_ptr<const KdGameObject> spTarget = m_wpTarget.lock();
		if (spTarget)
		{
			targetMat = spTarget->GetMatrix();
		}
	}

	// カメラの回転
	UpdateRotateByMouse();
	m_Rotation = GetRotationMatrix();
	m_mWorld = m_Rotation * m_LocalPos * targetMat;

	CameraBase::Update();
}

void FPSCamera::UpdateRotateByMouse()
{
	// マウス位置の差分を得る
	POINT nowPos;
	GetCursorPos(&nowPos);

	POINT mouseMove;
	mouseMove.x = nowPos.x - m_FixMousePos.x;
	mouseMove.y = nowPos.y - m_FixMousePos.y;

	SetCursorPos(m_FixMousePos.x, m_FixMousePos.y);

	// カメラを回転させる為に各軸の回転角度を設定する
	m_DegAng.x += mouseMove.y * 0.15f;
	m_DegAng.y += mouseMove.x * 0.15f;

	// 回転制御
	m_DegAng.x = std::clamp(m_DegAng.x, -FLT_MAX, FLT_MAX);
}
