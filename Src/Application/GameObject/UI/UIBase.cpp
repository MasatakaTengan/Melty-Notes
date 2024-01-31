#include "UIBase.h"
#include "Application/main.h"

void UIBase::Update()
{
	POINT mousePos = INPUT.GetPosition( Application::Instance().GetWindowHandle() );

	//有効でないUIだけ調べる
	if ( GetEnable() ) return;
	//カーソルの状態が
	switch ( INPUT.GetKeyStateToManager( VK_LBUTTON ) )
	{
		//前フレームから放されて続けている状態
		case KEYSTATE::IDLING:
			break;
			//今フレームで放された状態
		case KEYSTATE::RELEASE:
			if ( GetPush() )
			{
				//カーソル座標がUI上にあるか
				if ( IsRange( mousePos ) )
				{
					SetEnable( true );
				}
				SetPush( false );
			}
			break;
			//今フレームで押された状態
		case KEYSTATE::PRESS:
			if ( !GetPush() )
			{
				//カーソル座標がUI上にあるか
				if ( IsRange( mousePos ) )
				{
					SetPush( true );
				}
			}
			break;
			//前フレームから押され続けている状態
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
