#include "TrackingCamera.h"

void TrackingCamera::Init()
{
	// ��_(�^�[�Q�b�g)����ǂꂾ������Ă��邩
	m_LocalPos = Math::Matrix::CreateTranslation(0, 6.0f, -5.0f);

	// �ǂꂾ���X���Ă��邩
	m_Rotation = Math::Matrix::CreateRotationX(DirectX::XMConvertToRadians(45));

	CameraBase::Init();
}

void TrackingCamera::Update()
{
	// �^�[�Q�b�g�̍s��(�L���ȏꍇ���p����)
	Math::Matrix targetMat = Math::Matrix::Identity;
	if (!m_wpTarget.expired())
	{
		const std::shared_ptr<const KdGameObject> spTarget = m_wpTarget.lock();
		if (spTarget)
		{
			targetMat = spTarget->GetMatrix();
		}
	}

	m_mWorld = m_Rotation * m_LocalPos * targetMat;

	CameraBase::Update();
}
