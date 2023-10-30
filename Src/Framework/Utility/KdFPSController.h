#pragma once

//============================================================
// アプリケーションのFPS制御 + 測定
//============================================================
struct KdFPSController
{

	void Init();

	void UpdateStartTime();

	void Update();

	int GetNowFPS() const { return m_nowFps; }
	int GetMaxFPS() const { return m_maxFps; }
	double GetDeltaTime() const { return m_deltaTime; }

private:

	void Control();

	void Monitoring();

	// FPS制御
	int		m_nowFps = 0;		// 現在のFPS値
	int		m_maxFps = 60;		// 最大FPS

	int			m_fpsCnt = 0;				// FPS計測用カウント

	DWORD		m_frameStartTime = 0;		// フレームの開始時間
	DWORD		m_fpsMonitorBeginTime = 0;	// FPS計測開始時間

	const int	kSecond = 1000;				// １秒のミリ秒
	double m_deltaTime = 0.0;
};