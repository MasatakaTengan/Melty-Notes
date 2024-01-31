#pragma once

class UIBase;
class TextureBase;
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

protected:

	SceneID m_nowID;
	SceneID m_beforeID;

	bool mb_nowChanging;

	std::list<std::shared_ptr<UIBase>> msp_uiList;
	std::list<std::shared_ptr<TextureBase>> msp_texList;

};