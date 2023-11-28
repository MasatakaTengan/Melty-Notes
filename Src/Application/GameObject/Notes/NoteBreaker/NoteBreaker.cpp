#include "NoteBreaker.h"
#include "../Note/Note.h"
#include "../NoteManager/NoteManager.h"

void NoteBreaker::Init()
{
	const std::list<std::shared_ptr<Note>>& noteList = mwp_noteManager.lock()->GetNoteList();
	auto& it = noteList.begin();
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

	for ( int i = 0; i < 4; i++ )	//���ꂼ��̃��[���ɂ���
	{
		bool jadge = false;	//�e���[���ɂ����Ĕ�����s������
		long minDif = BMS_RESOLUTION;		//���蔭�����ɔ���o�[�����ԋ߂��m�[�c�̋���
		//std::shared_ptr<Note>& nearNote = std::make_shared<Note>();	//��ԋ߂��m�[�c�̃f�[�^
		auto& it = mwp_noteList[i].begin();	//�e���[���̑S�m�[�c�f�[�^
		auto& nearit = mwp_noteList[i].begin();	//��ԋ߂��m�[�c�̃C�e���[�^�[
		while ( it != mwp_noteList[i].end() )
		{
			if ( (*it).expired() )
			{
				it = mwp_noteList[i].erase( it );
				continue;
			}
			//����͈͈ȏ�̃m�[�c�܂ŒT�����i�߂΂���ȍ~�͔��菜�O�i���[�v���甲����
			if ( (*it).lock()->GetCount() > (_nowCount + Constant::MISS_RANGE) )
			{
				//�����[�v�Ŕ��肪�s���Ă���Ȃ�
				if ( jadge )
				{
					//���肳�ꂽ�m�[�c�̌��ʂ��i�[����
					if ( minDif <= PERFECT_RANGE )
					{
						(*nearit).lock()->SetJadge( PERFECT );
					}
					else if ( minDif <= GREAT_RANGE )
					{
						(*nearit).lock()->SetJadge( GREAT );
					}
					else if ( minDif <= GOOD_RANGE )
					{
						(*nearit).lock()->SetJadge( GOOD );
					}
					else if ( minDif <= BAD_RANGE )
					{
						(*nearit).lock()->SetJadge( BAD );
					}
					//���肳�ꂽ�m�[�c�̐����t���O������
					(*nearit).lock()->SetEnable( false );
				}

				break;
			}
			//����͈͓���

			// ���薢���s�̃m�[�c(�������Ă���)�ɂ��Ĕ��肷��
			if ( !(*it).lock()->GetEnable() )	//���肳�ꂽ�m�[�c�̓X�L�b�v
			{
				//it++;
				continue;
			}

			//����͈͊O�܂ŃX�N���[�����ꂽ�m�[�c�ɂ��Ă͏�������
			if ( (*it).lock()->GetCount() < (_nowCount - Constant::BAD_RANGE) )
			{
				(*it).lock()->SetEnable( false );
				//it = mwp_noteList[i].erase( it );
			}
			else
			{
				if ( INPUT.GetKeyStateToManager( Constant::KEYID_4K[i] ) == KEYSTATE::PRESS )
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
						nearit = it;
						//����͈͊O�܂Ń��[�v���i�񂾍ۂɏ������s��
						jadge = true;
					}
				}
			}
			it++;
		}
	}

}

void NoteBreaker::Draw()
{}
