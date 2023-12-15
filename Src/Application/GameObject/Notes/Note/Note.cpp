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
	m_jadge = JADGE::MISS;								// ����l(0=POOR�A1=BAD�A2=GOOD�A3=GREAT�A4=PERFECT�Ȃ�)

	/*
	if ( ml_count < ( ml_nowCount - BAD_RANGE ) )
	{
		// BAD������߂�����S�Č����������Ƃ���
		mb_enable = FALSE;						// �I�u�W�F������
		sprintf_s( m_jadgeString, sizeof( m_jadgeString ), "MISS" );
		// ���̃I�u�W�F���`�F�b�N
		return;
	}
	// �I�u�W�F��������Ȃ�L�[�������ꂽ�u�Ԃ����`�F�b�N
	if ( INPUT.GetKeyStateToManager(mi_key) == KEYSTATE::PRESS )
	{
		// �L�[���������u�ԂȂ琸�x����
		mi_hitSubNum = ml_nowCount - ml_count;
		LONG sub = abs( mi_hitSubNum );		// �I�u�W�F�Ƃ̍����Βl�Ŏ擾

		if ( sub <= PERFECT_RANGE )
		{
			m_jadge = PERFECT;
			sprintf_s( m_jadgeString, sizeof( m_jadgeString ), "PERFECT" );
			mb_enable = FALSE;						// �I�u�W�F������
		}
		else if ( sub <= GREAT_RANGE )
		{
			m_jadge = GREAT;
			sprintf_s( m_jadgeString, sizeof( m_jadgeString ), "GREAT" );
			mb_enable = FALSE;						// �I�u�W�F������
		}
		else if ( sub <= GOOD_RANGE )
		{
			m_jadge = GOOD;
			sprintf_s( m_jadgeString, sizeof( m_jadgeString ), "GOOD" );
			mb_enable = FALSE;						// �I�u�W�F������
		}
		else if ( sub <= BAD_RANGE )
		{
			m_jadge = BAD;
			sprintf_s( m_jadgeString, sizeof( m_jadgeString ), "BAD" );
			mb_enable = FALSE;						// �I�u�W�F������
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
	// �X�N���[�����W��ł̃X�N���[���ʂ��Z�o
	int scr_y = (int)( (double)ml_nowCount / ( BMS_RESOLUTION / ( _scrMulti * 192 ) ) );
	// �X�N���[�����l�����Ȃ��X�N���[�����W��ł̌��_����̍��W�l���Z�o
	int obj_y = (int)( (double)ml_count / ( BMS_RESOLUTION / ( _scrMulti * 192 ) ) );
	// �X�N���[�����l���������݂�Y���W���Z�o
	int off_y = obj_y - scr_y;

	// ���胉�C����艺�Ȃ�����\���͂����A���񂩂炻�̎��̃I�u�W�F����Q�Ƃ���
	if ( off_y < BAR_Y )
	{
		return;
	}

	// ��ʓ��Ȃ�`��
	Math::Rectangle rect = Math::Rectangle( 0, 0, NOTE_WIDTH, NOTE_HEIGHT );
	//Math::Color color = { 1, (float)b->bFlag, 1, 1 };
	KdShaderManager::Instance().m_spriteShader.DrawTex( msp_tex.get(), (int)m_pos.x, BAR_Y + off_y, NOTE_WIDTH * 2, NOTE_HEIGHT, &rect, &m_color);
}
