#pragma once

enum KEYSTATE
{
	IDLING,
	RELEASE,
	PRESS,
	HOLD
};

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

enum JADGE
{
	MISS,
	BAD,
	GOOD,
	GREAT,
	PERFECT
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

	static const int HITEFFECT_WIDTH = 320;
	static const int HITEFFECT_HALFWIDTH = HITEFFECT_WIDTH / 2;
	static const int HITEFFECT_HEIGHT = 320;
	static const int HITEFFECT_HALFHEIGHT = HITEFFECT_HEIGHT / 2;

	static const int BAR_Y = -200;
	static const int KEYBACK_Y = -200;

	static const int PERFECT_RANGE = BMS_RESOLUTION / 48;		// PERFECTと判定する中心からの範囲(前後合わせて24分音符内)
	static const int GREAT_RANGE = BMS_RESOLUTION / 32;		// GREATと判定する中心からの範囲(前後合わせて16分音符内)
	static const int GOOD_RANGE = BMS_RESOLUTION / 16;		// GOODと判定する中心からの範囲(前後合わせて8分音符内)
	static const int BAD_RANGE = BMS_RESOLUTION / 8;		// BADと判定する中心からの範囲(前後合わせて5分音符内)
	static const int POOR_RANGE = BMS_RESOLUTION / 2;		// POOR判定する中心からの範囲(前後合わせて1小節内)
}