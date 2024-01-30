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
	UIID_TORESULT,
	UIID_VOL_UP,
	UIID_VOL_DOWN,
	UIID_SPD_UP,
	UIID_SPD_DOWN
};

enum NumbersID
{
	NID_NONE,
	NID_GAMESCORE,
	NID_RESULTSCORE,
	NID_OPTIONVOL,
	NID_OPTIONSPD,
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

	static const int HITEFFECT_WIDTH = 128;
	static const int HITEFFECT_HALFWIDTH = HITEFFECT_WIDTH / 2;
	static const int HITEFFECT_HEIGHT = 32;
	static const int HITEFFECT_HALFHEIGHT = HITEFFECT_HEIGHT / 2;

	static const int LANE_X[4] = { -195,-65,65,195 };
	static const int BAR_Y = -300;
	static const int KEYBACK_Y = -200;
	static const int KEYBACK_COUNT = 15;
	static const float HITEFFECT_COUNT = 0.2f;

	const int KEYID_4K[4] = { 'D', 'F', 'J', 'K' };

	static const int BMS_MAINCHANNEL = 0x11;

	static const int PERFECT_RANGE = BMS_RESOLUTION / 32;		// PERFECTと判定する中心からの範囲(前後合わせて16分音符内)
	static const int GREAT_RANGE = BMS_RESOLUTION / 24;		// GREATと判定する中心からの範囲(前後合わせて12分音符内)
	static const int GOOD_RANGE = BMS_RESOLUTION / 16;		// GOODと判定する中心からの範囲(前後合わせて8分音符内)
	static const int BAD_RANGE = BMS_RESOLUTION / 12;		// BADと判定する中心からの範囲(前後合わせて6分音符内)
	static const int MISS_RANGE = BMS_RESOLUTION / 8;		// POOR判定する中心からの範囲(前後合わせて4分音符内)

	static const int MAX_SCORE = 1000000;

	static const int NUMBERS_WIDTH = 320;
	static const int NUMBERS_HALFWIDTH = NUMBERS_WIDTH / 2;
	static const int NUMBERS_HEIGHT = 320;
	static const int NUMBERS_HALFHEIGHT = NUMBERS_HEIGHT / 2;

	static const int SCORE_NUM_GAMESIZE = 64;
	static const int SCORE_NUM_GAMESIZEHALF = SCORE_NUM_GAMESIZE / 2;
	static const int SCORE_NUM_RESULTSIZE = 160;
	static const int SCORE_NUM_RESULTSIZEHALF = SCORE_NUM_RESULTSIZE / 2;
	static const int SCORE_NUM_OPTIONSIZE = 160;
	static const int SCORE_NUM_OPTIONSIZEHALF = SCORE_NUM_OPTIONSIZE / 2;

	const int OPTIONBUTTON_X[2] = { 100, 460 };
	const int OPTIONBUTTON_Y[2] = { 70, -180 };
	static const int OPTION_VOLUME_X = 440;
	static const int OPTION_VOLUME_Y = 150;
	static const int OPTION_SCROLLSPD_X = 440;
	static const int OPTION_SCROLLSPD_Y = -100;

	static const float MAX_SCROLLSPEED = 20.f;
	static const float MIN_SCROLLSPEED = 5.f;
	static const float DEFAULT_SCROLLSPEED = 10.f;
	static const float VALIATION_SCROLLSPEED = 0.5f;
	static const float MAX_VOLUME = 1.f;
	static const float MIN_VOLUME = 0.f;
	static const float DEFAULT_VOLUME = 0.5f;
	static const float VALIATION_VOLUME = 0.05f;
}