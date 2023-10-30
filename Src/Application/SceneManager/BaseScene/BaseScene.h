#pragma once

class UIBase;
class SceneManager;

class BaseScene
{
public:

	BaseScene() { Init(); }
	~BaseScene() {}

	virtual void Init();
	virtual void Event();

	virtual void PreUpdate();
	virtual void Update();
	virtual void PostUpdate();

	virtual void PreDraw();
	virtual void DrawLit();
	virtual void DrawUnLit();
	virtual void PostDraw();

	virtual void DrawSprite();

	void SetSceneID(SceneID _nowID, SceneID _beforeID)
	{
		m_nowID = _nowID;
		m_beforeID = _beforeID;
	}

	void SetOwner(std::shared_ptr<SceneManager> _owner)
	{
		mwp_owner = _owner;
	}
	void SetInputManager(std::shared_ptr<InputManager> _manager)
	{
		mwp_inputManager = _manager;
	}

protected:

	SceneID m_nowID;
	SceneID m_beforeID;

	std::weak_ptr<SceneManager> mwp_owner;
	std::weak_ptr<InputManager> mwp_inputManager;

	std::vector<std::shared_ptr<KdGameObject>> msp_objList;
	std::vector<std::shared_ptr<UIBase>> msp_uiList;

};