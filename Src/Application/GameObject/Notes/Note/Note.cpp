#include "Note.h"

Note::Note()
	:mb_enable(true),
	mb_hit(false),
	mi_key(0),
	ml_count(0),
	ml_nowCount(0),
	m_jadge(JADGE::MISS),
	m_color(kWhiteColor)
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
	mi_jadgeCnt = 0;

}

void Note::Update( LONG _nowCount )
{
	ml_nowCount = _nowCount;
	m_jadge = JADGE::MISS;								// ����l(0=POOR�A1=BAD�A2=GOOD�A3=GREAT�A4=PERFECT�Ȃ�)

	if ( ml_count < ( ml_nowCount - BAD_RANGE ) )
	{
		// BAD������߂�����S�Č����������Ƃ���
		mb_enable = FALSE;						// �I�u�W�F������
		mi_jadgeCnt = -MISS_RANGE;
		return;
	}
	//ImGui::Text( "%d, %s", mi_hitSubNum, m_jadgeString );
}

void Note::Draw( float _scrMulti )
{
	// �X�N���[�����W��ł̃X�N���[���ʂ��Z�o
	int scr_y = (int)((double)ml_nowCount / mf_bpm * _scrMulti);
	// �X�N���[�����l�����Ȃ��X�N���[�����W��ł̌��_����̍��W�l���Z�o
	int obj_y = (int)((double)ml_count / mf_bpm * _scrMulti);
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
	KdShaderManager::Instance().m_spriteShader.DrawTex( msp_tex.get(), (int)m_pos.x, (int)(BAR_Y + off_y), NOTE_WIDTH * 2, NOTE_HEIGHT, &rect, &m_color);
}
