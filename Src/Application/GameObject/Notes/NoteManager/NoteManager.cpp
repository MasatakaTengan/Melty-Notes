#include "NoteManager.h"
#include "../Note/Note.h"
#include "../NoteBreaker/NoteBreaker.h"

void NoteManager::Init( BMSLoader& _bms )
{
	m_tex = std::make_shared<KdTexture>();
	m_tex->Load( "Asset/Textures/notes.png" );
	//static const float obj_x[5] = { -256,-128,0,128,256 };			// オブジェ表示X座標
	static const float obj_x[4] = { -192,-64,64,192 };			// オブジェ表示X座標
	// 全チャンネル分を処理
	for ( int j = 0; j < 4; j++ )
	{
		// 判定対象のチャンネルのオブジェをチェック
		for ( int i = 0; i < _bms.GetObjeNum( 0x11 + j ); i++ )
		{
			BMSData* b = _bms.GetObje( 0x11 + j, i );
			std::shared_ptr<Note> n = std::make_shared<Note>();
			n->SetCount( b->ml_time );
			n->SetKey( Constant::KEYID_4K[j] );
			n->SetPos( Math::Vector2( obj_x[j], 0 ) );
			n->SetTex( m_tex );
			msp_noteList.push_back( n );
		}
		mi_noteNum += _bms.GetObjeNum( 0x11 + j );
		md_noteScore = (double)MAX_SCORE / mi_noteNum;
	}
	msp_noteBreaker = std::make_shared<NoteBreaker>();
	msp_noteBreaker->SetNoteManager( shared_from_this() );
	msp_noteBreaker->Init();
}

void NoteManager::PreUpdate()
{
	auto it = msp_noteList.begin();
	while ( it != msp_noteList.end() )
	{
		if ( !(*it)->GetEnable() )
		{
			m_jadgeList.push_back( (*it)->GetJadge() );
			m_jadge = (*it)->GetJadge();
			m_jadgeCnt = (*it)->GetCount();
			mi_score += (int)(md_noteScore * ((*it)->GetJadge()) / 4.0);
			it = msp_noteList.erase( it );
		}
		else
		{
			++it;
		}
	}
}

void NoteManager::Update( LONG _nowCount )
{
	for ( auto& note : msp_noteList )
	{
		note->Update( _nowCount );
	}
	msp_noteBreaker->Update( _nowCount );

	if ( ImGui::Begin( "NoteJadge" ) )
	{
		ImGui::Text( "%d", mi_noteNum );
		ImGui::Text( "%d", mi_score );
		for ( auto jadge = m_jadgeList.rbegin(); jadge != m_jadgeList.rend(); jadge++ )
		{
			ImGui::Text( "%d", *jadge );
		}
	}
	ImGui::End();
}

void NoteManager::Draw( float _scrMulti )
{
	for ( auto& note : msp_noteList )
	{
		note->Draw( _scrMulti );
	}
}
