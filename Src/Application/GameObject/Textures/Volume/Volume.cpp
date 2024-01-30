#include "Volume.h"
#include "../Numbers/Numbers.h"

void Volume::Init()
{
	for ( int i = 0; i < (log10( Constant::MAX_VOLUME * 100 ) + 1); i++ )
	{
		std::shared_ptr<Numbers> num = std::make_shared<Numbers>();
		num->SetTex( msp_numTex );
		num->SetScale( Math::Vector2( 0.5f ) );
		num->SetPos(
			Math::Vector2(
				Constant::OPTION_VOLUME_X - Constant::SCORE_NUM_OPTIONSIZEHALF - i * Constant::SCORE_NUM_OPTIONSIZEHALF,
				Constant::OPTION_VOLUME_Y - Constant::SCORE_NUM_OPTIONSIZEHALF
			)
		);
		num->SetMyNum( 0 );
		num->SetColor( kWhiteColor );
		msp_numbersList.push_back( num );
	}
}

void Volume::Update( int _vol )
{
	for ( int i = 0; i < (int)msp_numbersList.size(); i++ )
	{
		msp_numbersList[i]->SetMyNum( (_vol % (int)std::pow( 10, (i + 1) )) / (int)std::pow( 10, i ) );
	}
}

void Volume::Draw()
{
	for ( int i = 0; i < ((int)msp_numbersList.size()); i++ )
	{
		msp_numbersList[i]->Draw();
	}
}
