#include "NoteManager.h"
#include "../Note/Note.h"

void NoteManager::Init( BMSLoader& _bms )
{
	m_tex = std::make_shared<KdTexture>();
	m_tex->Load( "Asset/Textures/notes.png" );
	static const int index[6] = { 0,2,4,1,3,5 };				// インデックスリスト
	static const float obj_x[6] = { -256,-128,0,128,256,512 };			// オブジェ表示X座標
	// 全チャンネル分を処理
	for ( int j = 0; j < 5; j++ )
	{
		// 判定対象のチャンネルのオブジェをチェック
		for ( int i = 0; i < _bms.GetObjeNum( 0x11 + j ); i++ )
		{
			BMSData* b = _bms.GetObje( 0x11 + j, i );
			std::shared_ptr<Note> n = std::make_shared<Note>();
			n->SetCount( b->ml_time );
			n->SetKey( KEYID[j] );
			n->SetPos( Math::Vector2( obj_x[j], 0 ) );
			n->SetTex( m_tex );
			msp_noteList.push_back( n );
		}
	}
}

void NoteManager::PreUpdate()
{
	auto it = msp_noteList.begin();
	while ( it != msp_noteList.end() )
	{
		if ( !( *it )->GetEnable() )
		{
			m_jadgeList.push_back( ( *it )->GetJadge() );
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
	if ( ImGui::Begin( "NoteJadge" ) )
	{
		for ( auto& note : msp_noteList )
		{
			note->Update( _nowCount );
		}
		for ( auto jadge : m_jadgeList )
		{
			ImGui::Text( "%d", jadge );
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

void NoteManager::AddNote()
{}
