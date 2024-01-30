#include "Timer.h"

Timer::Timer()
{
	if (QueryPerformanceFrequency(&m_freq))
	{
		//高解像度カウンターに対応
		b_perf = true;
	}
	else
	{
		//高解像度カウンターに非対応
		m_freq.QuadPart = 1000ll;		//timeGetTime()で代用
		b_perf = false;
	}
	i_fps = 60;
	mi_nowCnt = 0;
	mi_nowFps = 0;
}

bool Timer::Start(int _fps)
{
	//引数の値にFPSを対応
	i_fps = _fps;
	//経過FPSを初期化
	i_cnt = 0;

	if (b_perf)
	{
		//高解像度対応タイマー
		QueryPerformanceCounter(&m_start);
	}
	else
	{
		//高解像度非対応タイマー
		m_start.QuadPart = (LONGLONG)timeGetTime();
	}
	//可変FPSの算出に使用する
	li_nowTime = m_start.QuadPart;
	return true;
}

int Timer::Run(void)
{
	LARGE_INTEGER now;
	if (b_perf)
	{
		//高解像度対応タイマー
		QueryPerformanceCounter(&now);
	}
	else
	{
		//高解像度非対応タイマー
		now.QuadPart = (LONGLONG)timeGetTime();
	}
	//メインループを1度通るたび加算
	mi_nowCnt++;
	//1秒経過ごとに
	if ( ( now.QuadPart - li_nowTime ) > m_freq.QuadPart )
	{
		//スタート時の時間に1秒加算し処理に対応させる
		li_nowTime += m_freq.QuadPart;
		//FPS値更新
		mi_nowFps = mi_nowCnt;
		mi_nowCnt = 0;
	}
	//スタート時からの経過時間を1フレーム処理用の時間で割る
	// ＝開始時間から何フレーム経過したかを算出
	int cnt = (int)((now.QuadPart - m_start.QuadPart) / (m_freq.QuadPart / i_fps));
	//これが前ループから変化しているか
	if (cnt != i_cnt)
	{
		//何フレーム経過したかを返す
		int ret = cnt - i_cnt;
		i_cnt = cnt;
		return ret;

	}

	return 0;
}
