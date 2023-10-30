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
	// �L�����N�^�[�̈ړ����x(�^��������_���ł���)
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

	// �L�����N�^�[�̉�]�s���n��
	UpdateRotate(moveVec);

	// �L�����N�^�[�̃��[���h�s���n�鏈��
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
	// �������͂������ꍇ�͏������Ȃ�
	if (srcMoveVec.LengthSquared() == 0.0f) { return; }

	// �ړ������̃x�N�g��
	Math::Vector3 targetDir = srcMoveVec;

	// �L�����̐��ʕ����̃x�N�g��
	Math::Vector3 nowDir = GetMatrix().Backward();

	targetDir.Normalize();
	nowDir.Normalize();

	float nowAng = atan2(nowDir.x, nowDir.z);
	nowAng = DirectX::XMConvertToDegrees(nowAng);

	float targetAng = atan2(targetDir.x, targetDir.z);
	targetAng = DirectX::XMConvertToDegrees(targetAng);

	// �Ԃ̊p�x
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
