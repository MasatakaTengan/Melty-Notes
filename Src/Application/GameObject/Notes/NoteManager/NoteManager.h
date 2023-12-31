#pragma once

class Note;
class NoteBreaker;

class NoteManager : public std::enable_shared_from_this<NoteManager>
{
public:

	NoteManager() {}
	~NoteManager() {}

	void Init( BMSLoader& _bms );
	void PreUpdate();
	void Update( LONG _nowCount );
	void Draw( float _scrMulti );
	
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
	long GetJadgeCnt()
	{
		return m_jadgeCnt;
	}

	const std::list<std::shared_ptr<Note>>& GetNoteList() const
	{
		return msp_noteList;
	}

private:

	std::list<std::shared_ptr<Note>> msp_noteList;
	std::shared_ptr<KdTexture> m_tex = nullptr;

	std::shared_ptr<NoteBreaker> msp_noteBreaker = nullptr;

	int mi_noteNum = 0;
	double md_noteScore = 0.0;
	int mi_score = 0;

	const int KEYID[4] =
	{		// キーのリスト
		'D',							// ch11に割り当てるキー
		'F',							// ch12に割り当てるキー
		'J',							// ch13に割り当てるキー
		'K',							// ch14に割り当てるキー
	};
	//BMSLoader& bms;
	std::list<JADGE> m_jadgeList;
	JADGE m_jadge;
	long m_jadgeCnt;

};