#pragma once

class Note;

class NoteManager
{
public:

	NoteManager() {}
	~NoteManager() {}

	void Init( BMSLoader& _bms );
	void PreUpdate();
	void Update( LONG _nowCount );
	void Draw( float _scrMulti );
	
	void AddScore( JADGE _jadge );
	int GetScore() { return mi_score; }

	std::string_view GetJadge()
	{
		switch ( m_jadge )
		{
			case MISS:
				return "MISS";
				break;
			case BAD:
				return "BAD";
				break;
			case GOOD:
				return "GOOD";
				break;
			case GREAT:
				return "GREAT";
				break;
			case PERFECT:
				return "PERFECT";
				break;
			default:
				return "ERROR";
				break;
		}
	}

	void SetBMS(  )
	{
		//bms = _bms;
	}

private:

	std::shared_ptr<KdTexture> m_tex = nullptr;
	std::vector<std::shared_ptr<Note>> msp_noteList;
	std::vector<JADGE> m_jadgeList;
	JADGE m_jadge;

	int mi_noteNum = 0;
	double md_noteScore = 0.0;
	int mi_score = 0;

	const int KEYID[5] =
	{		// �L�[�̃��X�g
		'D',							// ch11�Ɋ��蓖�Ă�L�[
		'F',							// ch12�Ɋ��蓖�Ă�L�[
		' ',							// ch13�Ɋ��蓖�Ă�L�[
		'J',							// ch14�Ɋ��蓖�Ă�L�[
		'K',							// ch15�Ɋ��蓖�Ă�L�[
	};
	//BMSLoader& bms;

};