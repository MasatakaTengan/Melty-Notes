#pragma once

class Timer
{
public:

	Timer();
    ~Timer() {}

    bool Start(int _fps = 60);       // 指定のFPSで割り込みスタート
    int Run(void);                // 割り込みがあれば1以上を返す（割り込み回数）

    int GetNowFPS()
    {
        //return i_nowFps;
        return i_cnt;
    }

    LONGLONG GetNowTime() { return li_nowTime; }

private:

    bool b_perf;          // 高解像度タイマーがサポートされているか
    LARGE_INTEGER m_freq;          // システム周波数
    LARGE_INTEGER m_start;         // 開始時間
    int i_fps;           // 割り込みタイミング
    int i_cnt;         // 割り込みカウント数

    LONGLONG li_nowTime;

};