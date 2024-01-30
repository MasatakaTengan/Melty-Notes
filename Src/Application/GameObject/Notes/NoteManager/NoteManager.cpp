#include "NoteManager.h"
#include "../Note/Note.h"
#include "../NoteBreaker/NoteBreaker.h"
#include "../../../SceneManager/GameScene/GameScene.h"

void NoteManager::Init( BMSLoader& _bms )
{
	m_tex = std::make_shared<KdTexture>();
	m_tex->Load( "Asset/Textures/notes.png" );

	//static const float Constant::LANE_X[4] = { -192,-64,64,192 };			// �I�u�W�F�\��X���W
	//�m�[�c�f�[�^�擾���m�[�c���X�g����
	for ( int j = 0; j < 4; j++ )
	{
		// ����Ώۂ̃`�����l���̃I�u�W�F���`�F�b�N
		for ( int i = 0; i < _bms.GetObjeNum( j + Constant::BMS_MAINCHANNEL ); i++ )
		{
			BMSData* b = _bms.GetObje( j + Constant::BMS_MAINCHANNEL, i );
			std::shared_ptr<Note> n = std::make_shared<Note>();
			n->SetCount( b->ml_time );
			n->SetKey( Constant::KEYID_4K[j] );
			n->SetPos( Math::Vector2( Constant::LANE_X[j], 0 ) );
			n->SetTex( m_tex );
			n->SetBPM( _bms.GetBPM() );
			msp_noteList.push_back( n );
		}
		mi_noteNum += _bms.GetObjeNum( j + Constant::BMS_MAINCHANNEL );
	}
	md_noteScore = (double)MAX_SCORE / mi_noteNum;

	//NoteBreaker������
	msp_noteBreaker = std::make_shared<NoteBreaker>();
	msp_noteBreaker->SetNoteManager( shared_from_this() );
	msp_noteBreaker->Init();

	//�f�o�b�O�p
	ml_noteCnt = 0;
	mi_jadgeCnt = 0;
	mi_jadgeNum = 0;
}

void NoteManager::PreUpdate()
{
	//�m�[�c���X�g���Ŕ��肳�ꂽ���̂〈�����ꂽ���̂𔻒�ۑ����X�g�̍X�V�ƂƂ��ɏ�������
	auto it = msp_noteList.begin();
	while ( it != msp_noteList.end() )
	{
		//�����t���O�m�F
		if ( !(*it)->GetEnable() )
		{
			m_jadge = (*it)->GetJadge();
			ml_noteCnt = (*it)->GetCount();
			m_jadgeList.push_back( { (*it)->GetJadge(), (*it)->GetJadgeCnt() } );
			if ( (*it)->GetHit() )
			{
				mi_jadgeCnt += (*it)->GetJadgeCnt();
				mi_jadgeNum++;
			}
			md_score += (md_noteScore * ((*it)->GetJadge()) / 4.0);
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

	/*if ( ImGui::Begin( "NoteJadge" ) )
	{
		ImGui::Text( "%d", mi_noteNum );
		ImGui::Text( "%d", (int)md_score );
		for ( auto jadge = m_jadgeList.rbegin(); jadge != m_jadgeList.rend(); jadge++ )
		{
			ImGui::Text( "%d:%d", (*jadge).first, (*jadge).second );
		}
	}
	ImGui::End();*/
}

void NoteManager::Draw( float _scrMulti )
{
	for ( auto& note : msp_noteList )
	{
		note->Draw( _scrMulti );
	}
}

void NoteManager::SetOwner( std::shared_ptr<GameScene> _owner )
{
	mwp_owner = _owner;
}

void NoteManager::AddHitEffect( Math::Vector2 _pos, JADGE _jadge )
{
	mwp_owner.lock()->AddHitEffect( _pos, _jadge );
}
