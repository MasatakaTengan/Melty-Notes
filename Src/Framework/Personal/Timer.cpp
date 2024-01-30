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
	//�����̒l��FPS��Ή�
	i_fps = _fps;
	//�o��FPS��������
	i_cnt = 0;

	if (b_perf)
	{
		//���𑜓x�Ή��^�C�}�[
		QueryPerformanceCounter(&m_start);
	}
	else
	{
		//���𑜓x��Ή��^�C�}�[
		m_start.QuadPart = (LONGLONG)timeGetTime();
	}
	//��FPS�̎Z�o�Ɏg�p����
	li_nowTime = m_start.QuadPart;
	return true;
}

int Timer::Run(void)
{
	LARGE_INTEGER now;
	if (b_perf)
	{
		//���𑜓x�Ή��^�C�}�[
		QueryPerformanceCounter(&now);
	}
	else
	{
		//���𑜓x��Ή��^�C�}�[
		now.QuadPart = (LONGLONG)timeGetTime();
	}
	//���C�����[�v��1�x�ʂ邽�щ��Z
	mi_nowCnt++;
	//1�b�o�߂��Ƃ�
	if ( ( now.QuadPart - li_nowTime ) > m_freq.QuadPart )
	{
		//�X�^�[�g���̎��Ԃ�1�b���Z�������ɑΉ�������
		li_nowTime += m_freq.QuadPart;
		//FPS�l�X�V
		mi_nowFps = mi_nowCnt;
		mi_nowCnt = 0;
	}
	//�X�^�[�g������̌o�ߎ��Ԃ�1�t���[�������p�̎��ԂŊ���
	// ���J�n���Ԃ��牽�t���[���o�߂��������Z�o
	int cnt = (int)((now.QuadPart - m_start.QuadPart) / (m_freq.QuadPart / i_fps));
	//���ꂪ�O���[�v����ω����Ă��邩
	if (cnt != i_cnt)
	{
		//���t���[���o�߂�������Ԃ�
		int ret = cnt - i_cnt;
		i_cnt = cnt;
		return ret;

	}

	return 0;
}
