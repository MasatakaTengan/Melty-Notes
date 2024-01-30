#include "Score.h"
#include "../Numbers/Numbers.h"

void Score::Init()
{
	for ( int i = 0; i < (log10( Constant::MAX_SCORE ) + 1); i++ )
	{
		std::shared_ptr<Numbers> num = std::make_shared<Numbers>();
		num->SetTex( msp_numTex );
		num->SetScale( m_scale );
		num->SetPos(
			Math::Vector2(
				m_pos.x - mi_size - i * mi_size,
				m_pos.y - mi_size
			)
		);
		num->SetMyNum( 0 );
		msp_numbersList.push_back( num );
	}
}

void Score::Update( int _score )
{
	if ( _score >= Constant::MAX_SCORE )
	{
		mb_max = true;
	}

	for ( int i = 0; i < (int)msp_numbersList.size(); i++ )
	{
		msp_numbersList[i]->SetMyNum( (_score % (int)std::pow( 10, (i + 1) )) / (int)std::pow( 10, i ) );
	}
}

void Score::Draw()
{
	if ( mb_max )
	{
		for ( int i = 0; i < (int)msp_numbersList.size(); i++ )
		{
			msp_numbersList[i]->Draw();
		}
	}
	else
	{
		for ( int i = 0; i < ((int)msp_numbersList.size() - 1); i++ )
		{
			msp_numbersList[i]->Draw();
		}
	}
}
