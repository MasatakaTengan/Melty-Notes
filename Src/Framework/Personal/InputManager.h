#pragma once

class InputManager
{
public:

	struct Mouse : public POINT
	{
		Mouse()
			:POINT({ 0, 0 })
		{}
		Mouse(const POINT& _pos)
			:POINT(_pos)
		{}
		Mouse(const Mouse& _pos)
			:POINT(_pos)
		{}
		const Mouse(long _x, long _y)
			: POINT({ _x, _y })
		{}

		Mouse& operator=(const POINT& _pos)
		{
			x = _pos.x;
			y = _pos.y;
			return *this;
		}
		Mouse& operator=(const Mouse& _pos)
		{
			x = _pos.x;
			y = _pos.y;
			return *this;
		}
		Mouse& operator+=(const Mouse& _pos)
		{
			x += _pos.x;
			y += _pos.y;
			return *this;
		}
		Mouse& operator-=(const Mouse& _pos)
		{
			x -= _pos.x;
			y -= _pos.y;
			return *this;
		}
		Mouse& operator*=(const Mouse& _pos)
		{
			x *= _pos.x;
			y *= _pos.y;
			return *this;
		}
		Mouse& operator/=(const Mouse& _pos)
		{
			x /= _pos.x;
			y /= _pos.y;
			return *this;
		}
	};
	friend Mouse operator+(const Mouse& _pos1, const Mouse& _pos2)
	{
		return Mouse(_pos1) += _pos2;
	}
	friend Mouse operator-(const Mouse& _pos1, const Mouse& _pos2)
	{
		return Mouse(_pos1) -= _pos2;
	}
	friend Mouse operator*(const Mouse& _pos1, const Mouse& _pos2)
	{
		return Mouse(_pos1) *= _pos2;
	}
	friend Mouse operator/(const Mouse& _pos1, const Mouse& _pos2)
	{
		return Mouse(_pos1) /= _pos2;
	}

	InputManager(HWND hwnd = 0)
		:m_hwnd(hwnd),
		m_pos(),
		m_beforePos()
	{}

	Mouse GetPosition()
	{
		m_beforePos = m_pos;
		return m_pos = GetPosition(m_hwnd);
	}

	Mouse GetDifference() const
	{
		return m_beforePos - m_pos;
	}

	Mouse GetCenterPosition() const
	{
		WINDOWINFO wi{};
		SecureZeroMemory(&wi, sizeof(wi));
		GetWindowInfo(m_hwnd, &wi);
		LONG width = wi.rcClient.right - wi.rcClient.left;
		LONG height = wi.rcClient.bottom - wi.rcClient.top;
		return { wi.rcClient.left + width / 2, wi.rcClient.top + height / 2 };
	}

	Mouse GetPositionFromCenter() const
	{
		return Mouse(GetPosition(0)) - GetCenterPosition();
	}
	Mouse GetPositionFromCenter(const Mouse& limit) const
	{
		Mouse relative = GetPositionFromCenter();
		if (relative.x > limit.x) {
			relative.x = limit.x;
		}
		else if (relative.x < -limit.x) {
			relative.x = -limit.x;
		}
		if (relative.y > limit.y) {
			relative.y = limit.y;
		}
		else if (relative.y < -limit.y) {
			relative.y = -limit.y;
		}
		return relative;
	}

	void LockInCenter()
	{
		SetPosition(GetCenterPosition());
	}

	static POINT GetPosition(HWND hwnd)
	{
		POINT point;
		GetCursorPos(&point);
		if (hwnd) {
			ScreenToClient(hwnd, &point);
		}
		return point;
	}

	static void SetPosition(POINT point)
	{
		SetCursorPos(point.x, point.y);
	}

	static void ShowCursor(bool show)
	{
		CURSORINFO ci{};
		SecureZeroMemory(&ci, sizeof(ci));
		ci.cbSize = sizeof(ci);
		GetCursorInfo(&ci);
		if (show) {
			// When the cursor is hidden
			if (!ci.flags) {
				::ShowCursor(TRUE);
			}
		}
		else {
			// When the cursor is showing
			if (ci.flags || CURSOR_SHOWING) {
				::ShowCursor(FALSE);
			}
		}
	}

	Mouse m_pos;
	Mouse m_beforePos;

private:

	const HWND m_hwnd;

public:
	static InputManager& Instance()
	{
		static InputManager Instance;
		return Instance;
	}
};

#define INPUT InputManager::Instance()