#pragma once

class BaseScene;

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

	void SetNextScene(SceneID _id) { m_nextID = _id; }
	void SetNextScene(UIID _id);

private:

	void ChangeScene(SceneID _id);

	//std::shared_ptr<InputManager> msp_inputManager = nullptr;

	std::shared_ptr<BaseScene> msp_nowScene = nullptr;
	SceneID m_beforeID = SID_NONE;
	SceneID m_nowID = SID_NONE;
	SceneID m_nextID = SID_NONE;

public:

	SceneManager() {}
	~SceneManager() {}

public:
	static SceneManager& Instance()
	{
		static SceneManager Instance;
		return Instance;
	}
};