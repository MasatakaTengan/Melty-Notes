#include "BMSLoader.h"

#define SAFE_FREE(x)		{ if(x) { free(x); x=NULL; } }

bool BMSLoader::Init()
{
	// �w�b�_������
	ZeroMemory(&m_header, sizeof(m_header));
	m_header.ml_player = 1;
	m_header.mf_bpm = 130;
	for (int i = 0; i < BMS_MAXBUFFER; i++) {
		m_header.mf_bpmIndex[i] = 120.0f;
	}

	// ���f�[�^������
	for (int i = 0; i < BMS_MAXBUFFER; i++)
	{
		mp_bmsData[i] = nullptr;		// BMS�f�[�^�̈���N���A
		mi_bmsData[i] = 0;				// �f�[�^�̐����N���A
	}

	// �t�@�C����
	ZeroMemory(&m_wavFile, sizeof(m_wavFile));
	ZeroMemory(&m_bmpFile, sizeof(m_bmpFile));

	// ���߂̒�����1.0�ŏ�����
	ZeroMemory(&m_bmsBar, sizeof(m_bmsBar));
	for (int i = 0; i < 1001; i++) {
		m_bmsBar[i].mf_scale = 1.0f;
	}

	return TRUE;
}

bool BMSLoader::LoadHeader(const char* _file)
{
	std::ifstream ifs( _file );
	if ( ifs.fail() )return false;

	std::string buf;
	std::string tmp;
	int ch = 0;
	int line = 0;

	while ( 1 )
	{
		//1�s��ǂݍ���
		buf.clear();
		std::getline( ifs, buf );
		if ( buf[0] == NULL && ifs.eof() )break;//�t�@�C���̏I�[�Ȃ猟���I���

		//�R�}���h�ȊO�Ȃ��΂�
		if ( buf[0] != '#' )continue;

		//�Ō�̉��s������
		if ( buf[buf.length() - 1] == '\n' )
		{
			buf[buf.length() - 1] = NULL;
		}

		//�R�}���h�̉��
		int cmd = GetCommand( buf );
		
		//�s���ȃR�}���h�Ȃ�X�L�b�v
		if ( cmd <= -2 )
		{
			continue;
		}

		//�p�����[�^�̕���
		std::string str;
		str.clear();
		if ( !GetCommandString( buf, str ) )
		{
			//������̎擾�����s�Ȃ�
			ifs.close();
			return false;
		}

		//�p�����[�^�̑��
		switch ( cmd )
		{
			case 0://PLAYER
				break;
			case 1:// GENRE
				m_header.m_genre = str;
				break;
			case 2://TITLE
				m_header.m_title = str;
				break;
			case 3://ARTIST
				m_header.m_artist = str;
				break;
			case 4://BPM
				if ( buf[4] == ' ' || buf[4] == 0x09 )
				{
					//��{�R�}���h�Ȃ�
					m_header.mf_bpm = std::stof( str );
					AddData( BMS_TEMPO, 0, (LONG)m_header.mf_bpm );
				}
				else
				{
					//�g���R�}���h�Ȃ�
					tmp.clear();
					tmp = buf.substr( 4, 2 );
					ch = atoi1610( tmp );
					m_header.mf_bpmIndex[ch] = std::stof( str );
				}
				break;
			case 5://MIDIFILE
				m_header.m_midiFile = str;
				break;
			case 6://PLAYLEVEL
				m_header.ml_playLevel = std::stoi( str );
				break;
			case 7://RANK
				m_header.ml_rank = std::stoi( str );
				break;
			case 8://VOLWAV
				m_header.ml_wavVol = std::stoi( str );
				break;
			case 9://TOTAL
				m_header.ml_total = std::stoi( str );
				break;
			case 10://STAGEFILE
				m_header.m_stagePic = str;
				break;
			case 11://WAV
				tmp.clear();
				tmp = buf.substr( 4, 2 );
				ch = atoi1610( tmp );
				m_wavFile[ch] = str;
				break;
			case 12://BMP
				tmp.clear();
				tmp = buf.substr( 4, 2 );
				ch = atoi1610( tmp );
				m_bmpFile[ch] = str;
				break;
			default:
				//���ߔԍ��̎擾
				tmp.clear();
				tmp = buf.substr( 1, 3 );
				line = atoi( tmp.data() );
				//�`�����l���ԍ��̎擾
				tmp.clear();
				tmp = buf.substr( 4, 2 );
				ch = atoi1610( tmp );
				if ( ch == BMS_STRETCH )
				{
					//���߂̔{���ύX���߂̏ꍇ
					m_bmsBar[line].mf_scale = (float)atof( str.data() );
				}
				//���ߔԍ��̍ő�l���L������
				if ( m_header.ml_endBar < line )
				{
					m_header.ml_endBar = line;
				}
				break;
		}
	}

	//�Ō�̏��ߓ��ɂ��f�[�^�����݂��邽�߁A���̎��̏��߂��I�[���߂Ƃ���
	m_header.ml_endBar++;

	//���ߔ{���f�[�^�����Ƃɂ��ׂĂ̏��ߏ����Z�o
	long cnt = 0;
	for ( int i = 0; i <= m_header.ml_endBar; i++ )
	{
		//���߃��X�g�����Z
		m_bmsBar[i].ml_time = cnt;	//���݂̏��߂̊J�n�J�E���g���L�^
		m_bmsBar[i].ml_length = (long)(BMS_RESOLUTION * m_bmsBar[i].mf_scale);	//�{�����炱�̏��߂̒����J�E���g�l���Z�o

		//���̏��߂̃J�E���g�������Z���Ď��̏��߂̊J�n�J�E���g�Ƃ���
		cnt += m_bmsBar[i].ml_length;
	}

	m_header.ml_maxCount = cnt;

	ifs.close();

	return true;
}

bool BMSLoader::Load(const char* _file)
{
	// �w�b�_�����ߔ{���̓ǂݍ���
	if (!LoadHeader(_file))
	{
		return FALSE;
	}

	// ���f�[�^�̓ǂݍ���
	if (!LoadBmsData(_file))
	{
		return FALSE;
	}

	return TRUE;
}

bool BMSLoader::Sort(int _ch)
{
	if (_ch<0 || _ch>BMS_MAXBUFFER - 1)
		return FALSE;

	// �����ɕ��ёւ���
	int i, j;
	for (i = 0; i < mi_bmsData[_ch] - 1; i++) {
		for (j = i + 1; j < mi_bmsData[_ch]; j++) {
			if (mp_bmsData[_ch][i].ml_time > mp_bmsData[_ch][j].ml_time) {
				// �\���̂����ւ���
				BMSData dmy = mp_bmsData[_ch][i];		// �_�~�[�ɕۑ�
				mp_bmsData[_ch][i] = mp_bmsData[_ch][j];		// i��j������
				mp_bmsData[_ch][j] = dmy;					// j�Ƀ_�~�[������
			}
		}
	}
	return TRUE;
}

bool BMSLoader::Restart()
{
	int i, j;
	for (j = 0; j < BMS_MAXBUFFER; j++) {
		for (i = 0; i < mi_bmsData[j]; i++) {
			mp_bmsData[j][i].mb_flg = TRUE;
		}
	}
	return TRUE;
}

long BMSLoader::GetCountFromTime(double _sec)
{
	long cnt = 0;			// BMS�J�E���g
	double t = 0;			// BMS��̎���
	double bpm = 130;

	if (mi_bmsData[BMS_TEMPO] > 0)
	{
		bpm = mp_bmsData[BMS_TEMPO][0].mf_data;		// ����BPM
	}

	if (_sec < 0)
		return 0;


	// �w�莞�Ԃ��z����܂Ń^�C�������Z
	int i;
	for (i = 0; i < mi_bmsData[BMS_TEMPO]; i++) {

		// �P�O�̎��ԂƐV�������ԂƂ̌o�ߎ��Ԃ���b���Z�o
		double add = (double)(mp_bmsData[BMS_TEMPO][i].ml_time - cnt) / (bpm / 60) / (BMS_RESOLUTION / 4);

		// ���݂̃e���|�l�Ŏ��Ԃ��߂����甲����
		if (t + add > _sec) {
			break;
		}

		t += add;										// �o�ߎ��Ԃ����Z
		bpm = (double)mp_bmsData[BMS_TEMPO][i].mf_data;		// ����BPM���Z�b�g
		cnt = mp_bmsData[BMS_TEMPO][i].ml_time;			// �v�Z�ς݂̃J�E���g���Z�b�g
	}

		// �w�莞�Ԃ�1�O�܂ł̎��Ԃ̍���
	double sub = _sec - t;

		// ��������BMS�J�E���g�����Z�o
	long cnt2 = (long)(sub * (BMS_RESOLUTION / 4) * (bpm / 60));

	// BMS�J�E���g�l�ɉ��Z
	cnt += cnt2;

	return cnt;
}


int BMSLoader::atoi1610( const std::string& str )
{
	int ret = 0;			// 10�i���ɕϊ������l
	int i = 0;				// �Q�Ƃ��镶���z��
	while (str[i]) {
		if (!(str[i] >= '0' && str[i] <= '9') &&
			!(str[i] >= 'A' && str[i] <= 'Z') &&
			!(str[i] >= 'a' && str[i] <= 'z'))
			return 0;

		ret *= 16;				// 16�{
		int n = str[i] - '0';
		if (n > 9)
			n -= 7;
		if (n > 15)
			n -= 0x20;
		ret += n;
		i++;
	}
	return ret;
}

bool BMSLoader::itoa1016(int num, char* dst, int keta)
{
	if (num < 0)
		return FALSE;

	itoa(num, dst, 16);

	// �������`�F�b�N
	int i;
	for (i = 0; i < (int)strlen(dst); i++) {
		if (dst[i] >= 'a' && dst[i] <= 'z') {
			dst[i] -= 0x20;
		}
	}

	// �����w��̏ꍇ
	if (keta >= 0) {
		// ����������Ȃ��ꍇ
		if ((int)strlen(dst) < keta) {
			char dmy[1024];
			ZeroMemory(dmy, sizeof(dmy));
			for (i = 0; i < keta - (int)strlen(dst); i++) {
				dmy[i] = '0';			// �擪�ɑ���Ȃ�����0��ǉ�
			}
			strcpy(&dmy[i], dst);		// ���̎��ɎZ�o���ꂽ��������Ȃ���
			strcpy(dst, dmy);			// �o�͌��ʂɃ_�~�[��������R�s�[		
		}
	}

	return TRUE;
}

bool BMSLoader::AddData(int ch, LONG cnt, LONG data)
{
	// �`�����l���ԍ����`�F�b�N
	if (ch < 0 || ch>255)
		return FALSE;

	// ���ߒ��ύX�R�}���h�Ȃ牽�����Ȃ�
	if (ch == BMS_STRETCH)
		return FALSE;

	// �f�[�^��������Ή������Ȃ�
	if (data == 0)
		return TRUE;

	switch (ch)
	{
		case BMS_BPMINDEX:
			// BPM�̃C���f�b�N�X�w��(�V)
			mi_bmsData[BMS_TEMPO]++;
			mp_bmsData[BMS_TEMPO] = (BMSData*)realloc(mp_bmsData[BMS_TEMPO], mi_bmsData[BMS_TEMPO] * sizeof(BMSData));
			ZeroMemory(&mp_bmsData[BMS_TEMPO][mi_bmsData[BMS_TEMPO] - 1], sizeof(BMSData));	// �ǉ������z����N���A
			mp_bmsData[BMS_TEMPO][mi_bmsData[BMS_TEMPO] - 1].mb_flg = TRUE;
			mp_bmsData[BMS_TEMPO][mi_bmsData[BMS_TEMPO] - 1].ml_time = cnt;
			mp_bmsData[BMS_TEMPO][mi_bmsData[BMS_TEMPO] - 1].ml_data = (LONG)m_header.mf_bpmIndex[data];	// �e���|���X�g�ɓ����Ă���e���|�l��o�^(LONG�^�ɂ��ۑ�)
			mp_bmsData[BMS_TEMPO][mi_bmsData[BMS_TEMPO] - 1].mf_data = m_header.mf_bpmIndex[data];			// �e���|���X�g�ɓ����Ă���e���|�l��o�^
			break;
		default:
			// �f�[�^��ǉ�
			mi_bmsData[ch]++;
			mp_bmsData[ch] = (BMSData*)realloc(mp_bmsData[ch], mi_bmsData[ch] * sizeof(BMSData));
			ZeroMemory(&mp_bmsData[ch][mi_bmsData[ch] - 1], sizeof(BMSData));					// �ǉ������z����N���A
			mp_bmsData[ch][mi_bmsData[ch] - 1].mb_flg = TRUE;
			mp_bmsData[ch][mi_bmsData[ch] - 1].m_color = kWhiteColor;
			mp_bmsData[ch][mi_bmsData[ch] - 1].ml_time = cnt;
			mp_bmsData[ch][mi_bmsData[ch] - 1].ml_data = data;
			mp_bmsData[ch][mi_bmsData[ch] - 1].mf_data = (float)data;								// float�^�ɂ��ۑ�
			break;
	}

	return TRUE;
}

int BMSLoader::GetCommand(const char* s)
{
	static const char* command[13] = {
		"PLAYER",
		"GENRE",
		"TITLE",
		"ARTIST",
		"BPM",
		"MIDIFILE",
		"PLAYLEVEL",
		"RANK",
		"VOLWAV",
		"TOTAL",
		"StageFile",
		"WAV",
		"BMP",
	};

	// �������[�`��
	int i;
	for (i = 0; i < 13; i++) {
		if (strnicmp(s + 1, command[i], strlen(command[i])) == 0)
			return i;	// �R�}���h�Ȃ炻�̔ԍ���Ԃ�
	}

	// �擪��'#nnncc'�`����
	BOOL obj = TRUE;
	for (i = 0; i < 5; i++) {
		if (s[i + 1] < '0' || s[i + 1]>'9') {
			obj = FALSE;
			break;
		}
	}

	// �I�u�W�F�z�u�Ȃ� -1
	if (obj) {
		return -1;
	}

	// �����s�\������Ȃ�
	return -2;
}

int BMSLoader::GetCommand( const std::string& _str )
{
	std::string command[13] = {
		"PLAYER",
		"GENRE",
		"TITLE",
		"ARTIST",
		"BPM",
		"MIDIFILE",
		"PLAYLEVEL",
		"RANK",
		"VOLWAV",
		"TOTAL",
		"StageFile",
		"WAV",
		"BMP",
	};

	std::string tmp;
	int i = 0;
	while ( 1 )
	{
		if ( _str[i] == ' ' || _str[i] == 0x09 || _str[i] == ':' )
		{
			break;
		}
		if ( _str[i] == '\n' || _str[i] == NULL )
		{
			return FALSE;
		}
		i++;
	}
	tmp = _str.substr( 1, i - 1 );

	// �������[�`��
	for ( i = 0; i < 13; i++ )
	{
		if ( tmp == command[i] )
			return i;	// �R�}���h�Ȃ炻�̔ԍ���Ԃ�
	}

	// �擪��'#nnncc'�`����
	BOOL obj = TRUE;
	for ( i = 0; i < 5; i++ )
	{
		if ( tmp[i] < '0' || tmp[i]>'9' )
		{
			obj = FALSE;
			break;
		}
	}

	// �I�u�W�F�z�u�Ȃ� -1
	if ( obj )
	{
		return -1;
	}

	// �����s�\������Ȃ�
	return -2;
}

bool BMSLoader::GetCommandString(const char* src, char* dst)
{
	int i = 0;
	int j = 0;

	// �܂��\�[�X�f�[�^����f�[�^�����܂ł̃|�C���^���Z�o
	while (1) {
		if (src[i] == ' ' || src[i] == 0x09 || src[i] == ':') {
			i++;
			break;
		}
		if (src[i] == '\n' || src[i] == NULL) {
			return FALSE;
		}
		i++;
	}

	// �I�[�܂ł��R�s�[
	while (1) {
		if (src[i] == '\n' || src[i] == NULL)
			break;
		dst[j] = src[i];
		i++;
		j++;
	}
	dst[j] = NULL;
	return TRUE;
}

bool BMSLoader::GetCommandString( const std::string& src, std::string& dst )
{
	int i = 0;
	int j = 0;

	std::string tmp;

	// �܂��\�[�X�f�[�^����f�[�^�����܂ł̃|�C���^���Z�o
	while ( 1 )
	{
		if ( src[i] == ' ' || src[i] == 0x09 || src[i] == ':' )
		{
			i++;
			break;
		}
		if ( src[i] == '\n' || src[i] == NULL )
		{
			return FALSE;
		}
		i++;
	}

	dst = src.substr( i );
	return TRUE;
}

bool BMSLoader::LoadBmsData(const char* file)
{
	int i;
	char data[1024];
	int cmd;			// �R�}���h�ԍ�
	int line;			// ���݂̏��ߔԍ�
	int ch;				// ���݂̃`�����l���ԍ�
	int len;			// ������̒���


	FILE* fp;
	fp = fopen(file, "r");
	if (!fp) {
		//sprintf(mLastError, "[%s] �t�@�C���I�[�v���G���[", file);
		return FALSE;
	}

	char buf[1024];
	while (1) {
		// �P�s��ǂ݂���
		ZeroMemory(buf, 1024);
		fgets(buf, 1024, fp);
		if (buf[0] == NULL && feof(fp))	// �t�@�C���̏I�[�Ȃ猟���I���
			break;

		//		DEBUG( "%s",buf );

				// �R�}���h�ȊO�Ȃ玟�̍s��
		if (buf[0] != '#')
			continue;

		// �Ō�̉��s������
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = NULL;

		// �R�}���h�̎�ނ��擾
		cmd = GetCommand(buf);

		// �I�u�W�F�ȊO�͑S�Ė���
		if (cmd != -1)
			continue;

		// �p�����[�^��������擾
		ZeroMemory(data, 1024);
		if (!GetCommandString(buf, data)) {
			//DEBUG("�p�����[�^������擾�G���[\n");
			fclose(fp);
			//sprintf(mLastError, "[%s] �p�����[�^������擾�G���[", buf);
			return FALSE;
		}

		// �f�[�^�ł���Ή��

		// �`�����l���ԍ��̎擾
		char tmp[4];						// �ėp�o�b�t�@
		ZeroMemory(&tmp, sizeof(tmp));
		tmp[0] = buf[4];					// �`�����l���ԍ�
		tmp[1] = buf[5];					// #001xx:******* ��xx����
		ch = atoi1610(tmp);				// 16�i��

		// ���߂̔{���ύX���߂̏ꍇ�̓L�����Z��
		if (ch == BMS_STRETCH)
			continue;

		// ���ߔԍ����擾
		ZeroMemory(&tmp, sizeof(tmp));
		memcpy(tmp, buf + 1, 3);		// ���ߕ������擾
		line = std::stoi( tmp );			// ������

		// �f�[�^�����݂��邩�`�F�b�N
		if (strlen(data) < 1) {
			//DEBUG("�f�[�^����`����Ă��Ȃ��ӏ�������\n%s", buf);
			continue;
		}

		// �f�[�^���������`�F�b�N
		if (strlen(data) % 2 == 1) {
			//DEBUG("�f�[�^�������ł͂Ȃ� [%s]\n", buf);
			fclose(fp);
			//sprintf(mLastError, "[%s] �f�[�^�������ł͂Ȃ�", buf);
			return FALSE;
		}

		// �f�[�^��
		len = (int)strlen(data) / 2;

		// ���݂̏��߂̃J�E���g�l����1�������̃J�E���g�l���Z�o
		LONG tick = m_bmsBar[line].ml_length / len;

		// ���f�[�^��ǉ�
		ZeroMemory(&tmp, sizeof(tmp));
		for (i = 0; i < len; i++) {
			tmp[0] = data[i * 2];
			tmp[1] = data[i * 2 + 1];
			int idata = atoi1610(tmp);			// 16�i��
			if (idata > 0) {
				// �f�[�^�����݂���ꍇ
				AddData(ch, m_bmsBar[line].ml_time + (tick * i), idata);
			}
		}
	}

	fclose(fp);

	// �\�[�g
	for (i = 0; i < BMS_MAXBUFFER; i++)
		Sort(i);

	/*	for( i=0;i<mi_bmsData[BMS_TEMPO];i++ ) {
			char s[256];
			sprintf( s,"[%08d] %f",(int)pBmsData[BMS_TEMPO][i].lTime,pBmsData[BMS_TEMPO][i].fData );
			DEBUG( "%s\n",s )
		}/**/

	return TRUE;
}

bool BMSLoader::LineCompact(const char* src, char* dst)
{
	int i, j, k;
	char buf[1024];		// ���[�N�o�b�t�@
	strcpy(buf, src);	// ���f�[�^���R�s�[
	*dst = NULL;		// �o�͐揉����

	// 010000020000030000040000010000020000030000040000010000020000030000040000010000020000030000040000
	//   ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  i=1  (i+1)=2�Ŋ���؂�镶����������
	//   ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  i=2  (i+1)=3�Ŋ���؂�镶����������
	//   ^^^^^^  ^^^^^^  ^^^^^^  ^^^^^^  ^^^^^^  ^^^^^^  ^^^^^^  ^^^^^^  ^^^^^^  ^^^^^^  ^^^^^^  ^^^^^^  i=3  (i+1)=4�Ŋ���؂�镶����������
	//   ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^  i=4  (i+1)=5�Ŋ���؂�镶���������~
	//   ^^^^^^^^^^  ^^^^^^^^^^  ^^^^^^^^^^  ^^^^^^^^^^  ^^^^^^^^^^  ^^^^^^^^^^  ^^^^^^^^^^  ^^^^^^^^^^  i=5  (i+1)=6�Ŋ���؂�镶����������
	//   ^^^^^^^^^^^^  ^^^^^^^^^^^^  ^^^^^^^^^^^^  ^^^^^^^^^^^^  ^^^^^^^^^^^^  ^^^^^^^^^^^^  ^^^^^^^^^^  i=6  (i+1)=7�Ŋ���؂�镶���������~
	//                                              �F
	//   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^      i=45 (i+1)=46�Ŋ���؂�镶���������~
	//   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^    i=46 (i+1)=47�Ŋ���؂�镶���������~
	//   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  i=47 (i+1)=48�Ŋ���؂�镶����������
	while (1) {

		// �f�[�^�����Z�o
		int l = (int)strlen(buf) / 2;

		// �󕶎��Ȃ�f�[�^����
		if (l == 0) {
			return FALSE;
		}

		// 1�̃f�[�^�Ȃ�
		if (l == 1) {
			if (buf[0] == '0' && buf[1] == '0') {
				// 00�Ȃ�f�[�^����
				return FALSE;
			}
			// ����ȊO�Ȃ�m��
			break;
		}


		// 2�ȏ�Ȃ�œK������
		BOOL div = FALSE;					// �����o������
		for (i = 1; i < l; i++) {

			// �������镶����
			int fetch_size = 1 + i;

			// ����؂�邩
			if ((l % fetch_size) != 0)
				continue;					// ����؂�Ȃ��Ȃ�X�L�b�v

			// ��������������2�Ԗڈȍ~��00�ȊO���܂܂�邩
			BOOL zero = TRUE;		// 00�ȊO�����݂��邩
			for (j = 0; j < l; j += fetch_size) {
				for (k = 1; k < fetch_size; k++) {
					int p = (j + k) * 2;
					if (buf[p] != '0' || buf[p + 1] != '0') {
						// 00�ȊO�����݂���Ȃ�
						zero = FALSE;
						j = l;				// ��ʃ��[�v���I��点��
						break;
					}
				}
			}

			// 00�݂̂Ȃ猻�����̕����T�C�Y�ŕ���
			if (zero) {
				k = 1;
				for (j = 0 + fetch_size; j < l; j += fetch_size) {			// 1�����ڂ͏�Ɋm��Ȃ̂�2�ڂ���R�s�[
					//int src = j * 2;
					//int dst = k * 2;
					buf[k * 2 + 0] = buf[j * 2 + 0];
					buf[k * 2 + 1] = buf[j * 2 + 1];
					// ���̃R�s�[���
					k++;
				}
				buf[k * 2] = NULL;
				l = k;
				div = TRUE;

				//				DEBUG( "������ : %s\n",buf );
				break;
			}
		}

		// ����ȏ㕪���o���Ȃ�������I���
		if (!div)
			break;
	}

	// ���ʂ�Ԃ�
	strcpy(dst, buf);

	return TRUE;
}
