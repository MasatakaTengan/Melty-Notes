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
	
	void AddNote();

	void SetBMS(  )
	{
		//bms = _bms;
	}

private:

	std::shared_ptr<KdTexture> m_tex = nullptr;
	std::vector<std::shared_ptr<Note>> msp_noteList;
	std::vector<JADGE> m_jadgeList;

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