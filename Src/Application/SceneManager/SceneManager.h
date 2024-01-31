#pragma once

class BaseScene;
class OptionChanger;

class OptionChanger
{
public:

	OptionChanger() {}
	~OptionChanger() {}

	const float& GetVolume()
	{
		return m_volume;
	}
	const float& GetScrollSpeed() { return m_scrollSpeed; }

	void SetVolume( float _vol )
	{
		m_volume = _vol;
	}
	void SetScrollSpeed( float _speed )
	{
		m_scrollSpeed = _speed;
	}

	void LoadOption();
	void SaveOption();

private:

	float m_volume;
	float m_scrollSpeed;

};

class SceneManager : public std::enable_shared_from_this<SceneManager>
{
public:

	void Init();

	void Event();

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void DrawLit();
	void DrawUnLit();
	void PostDraw();

	void DrawSprite();

	void SetNextScene( SceneID _id ) { m_nextID = _id; }
	void SetNextScene( UIID _id );

	void SetScore( int _score )
	{
		mi_score = _score;
	}
	int GetScore() { return mi_score; }

	std::shared_ptr<OptionChanger> msp_optionChanger;

	const float& GetVolume();
	const float& GetScrollSpeed();

	void SetVolume( float _vol );
	void SetScrollSpeed( float _speed );

private:

	void ChangeScene( SceneID _id );

	std::shared_ptr<BaseScene> msp_nowScene = nullptr;
	SceneID m_beforeID = SID_NONE;
	SceneID m_nowID = SID_NONE;
	SceneID m_nextID = SID_NONE;

	int mi_score = 0;

public:

	SceneManager() {}
	~SceneManager() { msp_optionChanger->SaveOption(); }

public:
	static SceneManager& Instance()
	{
		static SceneManager Instance;
		return Instance;
	}
};