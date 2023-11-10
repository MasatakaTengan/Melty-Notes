#include "Timer.h"

Timer::Timer()
{
	if (QueryPerformanceFrequency(&m_freq))
	{
		//���𑜓x�J�E���^�[�ɑΉ�
		b_perf = true;
	}
	else
	{
		//���𑜓x�J�E���^�[�ɔ�Ή�
		m_freq.QuadPart = 1000ll;		//timeGetTime()�ő�p
		b_perf = false;
	}
	i_fps = 60;
	mi_nowCnt = 0;
	mi_nowFps = 0;
}

bool Timer::Start(int _fps)
{
	i_fps = _fps;
	i_cnt = 0;

	if (b_perf)
	{
		QueryPerformanceCounter(&m_start);
	}
	else
	{
		m_start.QuadPart = (LONGLONG)timeGetTime();
	}
	li_nowTime = m_start.QuadPart;
	return true;
}

int Timer::Run(void)
{
	LARGE_INTEGER now;
	if (b_perf)
	{
		QueryPerformanceCounter(&now);
	}
	else
	{
		now.QuadPart = (LONGLONG)timeGetTime();
	}

	mi_nowCnt++;
	if ( ( now.QuadPart - li_nowTime ) > m_freq.QuadPart )
	{
		li_nowTime += m_freq.QuadPart;
		mi_nowFps = mi_nowCnt;
		mi_nowCnt = 0;
	}

	int cnt = (int)((now.QuadPart - m_start.QuadPart) / (m_freq.QuadPart / i_fps));
	if (cnt != i_cnt)
	{
		int ret = cnt - i_cnt;
		i_cnt = cnt;
		return ret;
	}

	return 0;
}
