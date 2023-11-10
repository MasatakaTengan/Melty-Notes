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
	{		// キーのリスト
		'D',							// ch11に割り当てるキー
		'F',							// ch12に割り当てるキー
		' ',							// ch13に割り当てるキー
		'J',							// ch14に割り当てるキー
		'K',							// ch15に割り当てるキー
	};
	//BMSLoader& bms;

};