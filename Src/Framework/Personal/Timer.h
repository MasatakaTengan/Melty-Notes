#pragma once

class Timer
{
public:

	Timer();
    ~Timer() {}

    bool Start(int _fps = 60);       // �w���FPS�Ŋ��荞�݃X�^�[�g
    int Run(void);                // ���荞�݂������1�ȏ��Ԃ��i���荞�݉񐔁j

    int GetNowFPS()
    {
        //return i_nowFps;
        return i_cnt;
    }

    LONGLONG GetNowTime() { return li_nowTime; }

private:

    bool b_perf;          // ���𑜓x�^�C�}�[���T�|�[�g����Ă��邩
    LARGE_INTEGER m_freq;          // �V�X�e�����g��
    LARGE_INTEGER m_start;         // �J�n����
    int i_fps;           // ���荞�݃^�C�~���O
    int i_cnt;         // ���荞�݃J�E���g��

    LONGLONG li_nowTime;

};