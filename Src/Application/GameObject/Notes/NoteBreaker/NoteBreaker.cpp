#include "NoteBreaker.h"
#include "../Note/Note.h"
#include "../NoteManager/NoteManager.h"

void NoteBreaker::Init()
{
	const std::list<std::shared_ptr<Note>>& noteList = mwp_noteManager.lock()->GetNoteList();
	auto it = noteList.begin();
	while ( it != noteList.end() )
	{
		/*switch ( ( *it )->GetKey() )
		{
			case Constant::KEYID_4K[0]:
				break;
		}*/
		for ( int i = 0; i < 4; i++ )
		{
			if ( (*it)->GetKey() == Constant::KEYID_4K[i] )
			{
				mwp_noteList[i].push_back( (*it) );
			}
		}
		it++;
	}
}

void NoteBreaker::Update( LONG _nowCount )
{
	//���͔���̃��[�v
	//����p�̃f�[�^���X�g���쐬
	//����p�̊֐�JADGE�Ő�p���X�g�݂̂Ŕ��肷��

	for ( int i = 0; i < 4; i++ )
	{
		if ( INPUT.GetKeyStateToManager( Constant::KEYID_4K[i] ) == KEYSTATE::PRESS )
		{
			//�L�[�������ꂽ���ɂ��̃L�[�ɑΉ������m�[�c���X�g���画��p�̃��X�g���쐬����
			AddJadgeList( i, _nowCount );
			//����p�̃��X�g���g�p���Ĕ�����s��
			JadgeNote( _nowCount );
		}
	}

}

void NoteBreaker::AddJadgeList( int i, LONG _nowCount )
{
	auto it = mwp_noteList[i].begin();
	while ( it != mwp_noteList[i].end() )
	{
		//Manager�̎������X�g���폜����Ă��镔���͍폜����
		if ( (*it).expired() )
		{
			it = mwp_noteList[i].erase( it );
			continue;
		}

		//����͈͈ȏ�̃m�[�c�܂ŒT�����i�߂΂���ȍ~�͔��菜�O�i���[�v���甲����
		if ( (*it).lock()->GetCount() > (_nowCount + Constant::MISS_RANGE) )
		{
			break;
		}
		//����͈͈ȉ��̃m�[�c���c���Ă���ꍇ�͎��N���X���̃m�[�c���X�g���珜�O����
		if ( (*it).lock()->GetCount() < (_nowCount - Constant::BAD_RANGE) )
		{
			(*it).lock()->SetEnable( false );
		}
		else
		{
			//����͈͓��̃m�[�c�𔻒�p���X�g�Ɋi�[
			mwp_jadgeList.push_back( *it );
		}

		it++;

	}
}

void NoteBreaker::JadgeNote( LONG _nowCount )
{
	if ( mwp_jadgeList.size() <= 0 )return;

	auto it = mwp_jadgeList.begin();
	auto jadgeit = mwp_jadgeList.begin();
	long minDif = BMS_RESOLUTION;
	while ( it != mwp_jadgeList.end() )
	{
		//����͈͓��ň�ԉ��̃m�[�c���画���D�悷�邩
		//���蔭�����ɔ��胉�C�������ԋ߂��m�[�c��D�悷�邩���̗p!!!!!!!!!!!!

		//��Ԕ��胉�C���ɋ߂��m�[�c��T��

		//���蔭�����̃J�E���g�l�ƃm�[�c���g�̃J�E���g�l�Ƃ̐�Βl
		//�J�E���g��Βl
		long dif = abs( (*it).lock()->GetCount() - _nowCount );
		if ( dif < minDif )		//����܂ł̃m�[�c�̃J�E���g��Βl��菬�����ꍇ
		{
			//�J�E���g��Βl����菬�������̂ɍX�V
			minDif = dif;
			//���̃m�[�c����ԋ߂��m�[�c�Ƃ��ēo�^
			jadgeit = it;
			//����͈͊O�܂Ń��[�v���i�񂾍ۂɏ������s��
			//jadge = true;
		}
		it++;
	}

	//���肳�ꂽ�m�[�c�̌��ʂ��i�[����
	if ( minDif <= PERFECT_RANGE )
	{
		(*jadgeit).lock()->SetJadge( PERFECT );
	}
	else if ( minDif <= GREAT_RANGE )
	{
		(*jadgeit).lock()->SetJadge( GREAT );
	}
	else if ( minDif <= GOOD_RANGE )
	{
		(*jadgeit).lock()->SetJadge( GOOD );
	}
	else if ( minDif <= BAD_RANGE )
	{
		(*jadgeit).lock()->SetJadge( BAD );
	}
	//���肳�ꂽ�m�[�c�̐����t���O������
	(*jadgeit).lock()->SetEnable( false );

	mwp_jadgeList.clear();
}

void NoteBreaker::Draw()
{}
