#pragma once
#include "../Textures/TextureBase.h"

class UIBase : public TextureBase
{
public:

	UIBase() {}
	~UIBase() {}


	virtual void Update()override;
	virtual void Draw()override;

	virtual bool GetEnable() { return b_enable; }
	virtual bool GetPush() { return b_push; }
	virtual bool GetIsScene() { return b_isScene; }
	virtual UIID GetID() { return m_id; }

	virtual void SetEnable(bool _flg) { b_enable = _flg; }
	virtual void SetPush(bool _flg) { b_push = _flg; }
	virtual void SetIsScene(bool _flg) { b_isScene = _flg; }
	virtual void SetID(UIID _id) { m_id = _id; }

	virtual bool IsRange(POINT _pos);

protected:

	virtual void Release();

	bool b_enable = false;
	bool b_push = false;
	bool b_isScene = false;

	UIID m_id = UIID::UIID_NONE;

};