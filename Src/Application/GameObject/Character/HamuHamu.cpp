#include "HamuHamu.h"
#include "../Camera/CameraBase.h"
#include "Application/main.h"

void HamuHamu::Init()
{
	if (!m_spPoly)
	{
		m_spPoly = std::make_shared<KdSquarePolygon>();
		m_spPoly->SetMaterial("Asset/Data/Sample/Character/SampleChara.png");
		m_spPoly->SetPivot(KdSquarePolygon::PivotType::Center_Bottom);
	}
}

void HamuHamu::Update()
{
	// キャラクターの移動速度(真似しちゃダメですよ)
	float moveSpd			= m_movePerSecond / Application::Instance().GetNowFPS();
	Math::Vector3 nowPos	= GetPos();

	Math::Vector3 moveVec = Math::Vector3::Zero;
	/*if (GetAsyncKeyState('D')) { moveVec.x = 1.0f;	}
	if (GetAsyncKeyState('A')) { moveVec.x = -1.0f; }
	if (GetAsyncKeyState('W')) { moveVec.z = 1.0f;	}
	if (GetAsyncKeyState('S')) { moveVec.z = -1.0f; }*/

	std::shared_ptr<CameraBase> spCamera = m_wpCamera.lock();
	if (spCamera)
	{
		moveVec = moveVec.TransformNormal
		(moveVec, spCamera->GetRotationYMatrix());
	}

	moveVec.Normalize();
	moveVec *= moveSpd;

	nowPos.x += moveVec.x;
	nowPos.z += moveVec.z;

	// キャラクターの回転行列を創る
	UpdateRotate(moveVec);

	// キャラクターのワールド行列を創る処理
	Math::Matrix rotation =
		Math::Matrix::CreateRotationY(
			DirectX::XMConvertToRadians(m_worldRot.y));
	m_mWorld = rotation * Math::Matrix::CreateTranslation(nowPos);
}

void HamuHamu::DrawLit()
{
	if (!m_spPoly) return;

	KdShaderManager::Instance().m_HD2DShader.DrawPolygon(*m_spPoly, m_mWorld);
}

void HamuHamu::UpdateRotate(Math::Vector3& srcMoveVec)
{
	// 何も入力が無い場合は処理しない
	if (srcMoveVec.LengthSquared() == 0.0f) { return; }

	// 移動方向のベクトル
	Math::Vector3 targetDir = srcMoveVec;

	// キャラの正面方向のベクトル
	Math::Vector3 nowDir = GetMatrix().Backward();

	targetDir.Normalize();
	nowDir.Normalize();

	float nowAng = atan2(nowDir.x, nowDir.z);
	nowAng = DirectX::XMConvertToDegrees(nowAng);

	float targetAng = atan2(targetDir.x, targetDir.z);
	targetAng = DirectX::XMConvertToDegrees(targetAng);

	// 間の角度
	float betweenAng = targetAng - nowAng;
	if (betweenAng > 180)
	{
		betweenAng -= 360;
	}
	else if (betweenAng < -180)
	{
		betweenAng += 360;
	}

	float rotateAng = std::clamp(betweenAng, -8.0f, 8.0f);
	m_worldRot.y += rotateAng;
}
