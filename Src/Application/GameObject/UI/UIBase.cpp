#include "UIBase.h"
#include "Application/main.h"

void UIBase::Update()
{
	POINT mousePos = INPUT.GetPosition( Application::Instance().GetWindowHandle() );

	//�L���łȂ�UI�������ׂ�
	if ( GetEnable() ) return;
	//�J�[�\���̏�Ԃ�
	switch ( INPUT.GetKeyStateToManager( VK_LBUTTON ) )
	{
		//�O�t���[�����������đ����Ă�����
		case KEYSTATE::IDLING:
			break;
			//���t���[���ŕ����ꂽ���
		case KEYSTATE::RELEASE:
			if ( GetPush() )
			{
				//�J�[�\�����W��UI��ɂ��邩
				if ( IsRange( mousePos ) )
				{
					SetEnable( true );
				}
				SetPush( false );
			}
			break;
			//���t���[���ŉ����ꂽ���
		case KEYSTATE::PRESS:
			if ( !GetPush() )
			{
				//�J�[�\�����W��UI��ɂ��邩
				if ( IsRange( mousePos ) )
				{
					SetPush( true );
				}
			}
			break;
			//�O�t���[�����牟���ꑱ���Ă�����
		case KEYSTATE::HOLD:
			break;
	}
}

void UIBase::Draw()
{
	Math::Rectangle rect = {};
	Math::Color color = b_push ? Math::Color( 0.5f, 0.5f, 0.5f, 1.0f ) : kWhiteColor;
	KdShaderManager::Instance().m_spriteShader.DrawTex( msp_tex.get(), (int)m_pos.x, (int)m_pos.y, 
		(int)(msp_tex.get()->GetInfo().Width * m_scale.x), (int)(msp_tex.get()->GetInfo().Height * m_scale.y),
		nullptr, &color);
}

bool UIBase::IsRange(POINT _pos)
{
	_pos.x -= Constant::WINDOW_HALFWIDTH;
	_pos.y -= Constant::WINDOW_HALFHEIGHT;
	_pos.y *= -1;

	if (((m_pos.x - (msp_tex->GetInfo().Width / 2.0f)) < _pos.x && _pos.x < (m_pos.x + (msp_tex->GetInfo().Width / 2.0f))) &&
		((m_pos.y - (msp_tex->GetInfo().Height / 2.0f)) < _pos.y && _pos.y < (m_pos.y + (msp_tex->GetInfo().Height / 2.0f))))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void UIBase::Release()
{
	msp_tex->Release();
}
