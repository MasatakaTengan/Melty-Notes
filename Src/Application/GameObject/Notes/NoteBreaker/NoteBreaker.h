#pragma once

class Note;
class NoteManager;

class NoteBreaker
{
public:

	NoteBreaker() {}
	~NoteBreaker() {}

	void Init();
	void Update( LONG _nowCount );
	void Draw();

	void SetNoteManager( std::shared_ptr<NoteManager> _manager )
	{
		mwp_noteManager = _manager;
	}

private:

	std::weak_ptr<NoteManager> mwp_noteManager;
	std::array<std::list<std::weak_ptr<Note>>, 4> mwp_noteList;

};