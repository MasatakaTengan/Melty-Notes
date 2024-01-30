#pragma once

class Note;
class NoteBreaker;
class GameScene;

class NoteManager : public std::enable_shared_from_this<NoteManager>
{
public:

	NoteManager() {}
	~NoteManager() {}

	void Init( BMSLoader& _bms );
	void PreUpdate();
	void Update( LONG _nowCount );
	void Draw( float _scrMulti );
	
	int GetScore()
	{
		if ( (md_score - (int)md_score) > 0 )
		{
			return md_score > Constant::MAX_SCORE ? Constant::MAX_SCORE : md_score + 1;
		}
		else
		{
			return md_score;
		}
	}
	float GetBPM() { return mf_bpm; }

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
	long GetNoteCnt()
	{
		return ml_noteCnt;
	}
	int GetJadgeCnt()
	{ 
		if ( mi_jadgeNum == 0 )
		{
			return mi_jadgeCnt;
		}
		else
		{
			return mi_jadgeCnt / mi_jadgeNum;
		}
	}

	const std::list<std::shared_ptr<Note>>& GetNoteList() const
	{
		return msp_noteList;
	}

	void SetOwner( std::shared_ptr<GameScene> _owner );
	void AddHitEffect( Math::Vector2 _pos, JADGE _jadge );

private:

	std::weak_ptr<GameScene> mwp_owner;

	std::list<std::shared_ptr<Note>> msp_noteList;
	std::shared_ptr<KdTexture> m_tex = nullptr;

	std::shared_ptr<NoteBreaker> msp_noteBreaker = nullptr;

	float mf_bpm = 0.f;
	int mi_noteNum = 0;
	double md_noteScore = 0.0;
	double md_score = 0.0;

	const int KEYID[4] =
	{		// キーのリスト
		'D',							// ch11に割り当てるキー
		'F',							// ch12に割り当てるキー
		'J',							// ch13に割り当てるキー
		'K',							// ch14に割り当てるキー
	};
	//BMSLoader& bms;
	std::list<std::pair<JADGE, int>> m_jadgeList;
	JADGE m_jadge;
	long ml_noteCnt;
	int mi_jadgeCnt;
	int mi_jadgeNum;

};