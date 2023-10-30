#pragma once

class UIBase
{
public:

	UIBase() { Init(); }
	~UIBase() {}

	virtual void Init();
	virtual void LoadTex(std::string_view _filename);

	virtual void DrawSprite();

	virtual Math::Rectangle GetRect()
	{
		return Math::Rectangle(m_pos.x, m_pos.y, m_tex.GetInfo().Width, m_tex.GetInfo().Height);
	}

	virtual bool GetEnable() { return b_enable; }
	virtual bool GetPush() { return b_push; }
	virtual UIID GetID() { return m_id; }

	virtual void SetEnable(bool _flg) { b_enable = _flg; }
	virtual void SetPush(bool _flg) { b_push = _flg; }

	virtual void SetPos(Math::Vector2 _pos) { m_pos = _pos; }

	virtual bool IsRange(POINT _pos);

protected:

	void Release();

	bool b_enable = false;
	bool b_push = false;

	UIID m_id = UIID_NONE;

	Math::Vector2 m_pos = {};

	KdTexture m_tex;

};