#pragma once

enum SceneID
{
	SID_NONE,
	SID_TITLE,
	SID_SELECT,
	SID_OPTION,
	SID_GAME,
	SID_RESULT
};

enum UIID
{
	UIID_NONE,
	UIID_TOTITLE,
	UIID_TOSELECT,
	UIID_TOOPTION,
	UIID_TOGAME,
	UIID_TORESULT
};

namespace Constant
{
	static const int WINDOW_WIDTH = 1280;
	static const int WINDOW_HALFWIDTH = WINDOW_WIDTH / 2;
	static const int WINDOW_HEIGHT = 720;
	static const int WINDOW_HALFHEIGHT = WINDOW_HEIGHT / 2;

	static const int NOTE_WIDTH = 64;
	static const int NOTE_HALFWIDTH = NOTE_WIDTH / 2;
	static const int NOTE_HEIGHT = 16;
	static const int NOTE_HALFHEIGHT = NOTE_HEIGHT / 2;

	static const int BAR_Y = -200;
	static const int KEYBACK_Y = -200;
}