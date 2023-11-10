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
	{		// �L�[�̃��X�g
		'D',							// ch11�Ɋ��蓖�Ă�L�[
		'F',							// ch12�Ɋ��蓖�Ă�L�[
		' ',							// ch13�Ɋ��蓖�Ă�L�[
		'J',							// ch14�Ɋ��蓖�Ă�L�[
		'K',							// ch15�Ɋ��蓖�Ă�L�[
	};
	//BMSLoader& bms;

};