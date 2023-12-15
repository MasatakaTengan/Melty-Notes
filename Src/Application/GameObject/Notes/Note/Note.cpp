#include "Note.h"

Note::Note()
	:mb_enable(true),
	mi_key(0),
	ml_count(0),
	ml_nowCount(0),
	m_jadge(JADGE::MISS),
	m_color(kRedColor)
{}

Note::~Note()
{}

void Note::Init()
{
	mb_enable = true;
	ml_nowCount = 0;
	m_jadge = JADGE::MISS;

	//debug
	mi_hitSubNum = 0;
	sprintf_s( m_jadgeString, sizeof( m_jadgeString ), "" );

}

void Note::Update( LONG _nowCount )
{
	ml_nowCount = _nowCount;
	m_jadge = JADGE::MISS;								// 判定値(0=POOR、1=BAD、2=GOOD、3=GREAT、4=PERFECTなど)

	/*
	if ( ml_count < ( ml_nowCount - BAD_RANGE ) )
	{
		// BAD判定を過ぎたら全て見逃し扱いとする
		mb_enable = FALSE;						// オブジェを消す
		sprintf_s( m_jadgeString, sizeof( m_jadgeString ), "MISS" );
		// 次のオブジェをチェック
		return;
	}
	// オブジェが判定内ならキーが押された瞬間かをチェック
	if ( INPUT.GetKeyStateToManager(mi_key) == KEYSTATE::PRESS )
	{
		// キーを押した瞬間なら精度判定
		mi_hitSubNum = ml_nowCount - ml_count;
		LONG sub = abs( mi_hitSubNum );		// オブジェとの差を絶対値で取得

		if ( sub <= PERFECT_RANGE )
		{
			m_jadge = PERFECT;
			sprintf_s( m_jadgeString, sizeof( m_jadgeString ), "PERFECT" );
			mb_enable = FALSE;						// オブジェを消す
		}
		else if ( sub <= GREAT_RANGE )
		{
			m_jadge = GREAT;
			sprintf_s( m_jadgeString, sizeof( m_jadgeString ), "GREAT" );
			mb_enable = FALSE;						// オブジェを消す
		}
		else if ( sub <= GOOD_RANGE )
		{
			m_jadge = GOOD;
			sprintf_s( m_jadgeString, sizeof( m_jadgeString ), "GOOD" );
			mb_enable = FALSE;						// オブジェを消す
		}
		else if ( sub <= BAD_RANGE )
		{
			m_jadge = BAD;
			sprintf_s( m_jadgeString, sizeof( m_jadgeString ), "BAD" );
			mb_enable = FALSE;						// オブジェを消す
		}

		switch ( m_jadge )
		{
		case MISS:
			m_color = kRedColor;
			break;
		case BAD:
			m_color = kBlueColor;
			break;
		case GOOD:
			m_color = kGreenColor;
			break;
		case GREAT:
			m_color = kRedColor + kGreenColor;
			break;
		case PERFECT:
			m_color = kWhiteColor;
			break;
		}
	}
	//ImGui::Text( "%d, %s", mi_hitSubNum, m_jadgeString );*/
}

void Note::Draw( float _scrMulti )
{
	// スクリーン座標上でのスクロール量を算出
	int scr_y = (int)( (double)ml_nowCount / ( BMS_RESOLUTION / ( _scrMulti * 192 ) ) );
	// スクロールを考慮しないスクリーン座標上での原点からの座標値を算出
	int obj_y = (int)( (double)ml_count / ( BMS_RESOLUTION / ( _scrMulti * 192 ) ) );
	// スクロールを考慮した現在のY座標を算出
	int off_y = obj_y - scr_y;

	// 判定ラインより下ならもう表示はせず、次回からその次のオブジェから参照する
	if ( off_y < BAR_Y )
	{
		return;
	}

	// 画面内なら描画
	Math::Rectangle rect = Math::Rectangle( 0, 0, NOTE_WIDTH, NOTE_HEIGHT );
	//Math::Color color = { 1, (float)b->bFlag, 1, 1 };
	KdShaderManager::Instance().m_spriteShader.DrawTex( msp_tex.get(), (int)m_pos.x, BAR_Y + off_y, NOTE_WIDTH * 2, NOTE_HEIGHT, &rect, &m_color);
}
