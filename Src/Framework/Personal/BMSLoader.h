#pragma once
#define BMS_RESOLUTION      9600            // 1���߂̃J�E���g�l
#define BMS_MAXBUFFER       (16*16)         // 00�`FF�܂ł̃o�b�t�@��

// BMS�`�����l����`
#define BMS_BACKMUSIC       0x01            // ���̈ʒu�ɂ�����A�����I�ɍĐ������WAV���w�肵�܂�
#define BMS_STRETCH         0x02            // ���̏��߂̒������`�����f�[�^�{���܂��i�P�O�i���A�����Ƃ��Ɏg�p�j
#define BMS_TEMPO           0x03            // �Đ��e���|�i�a�o�l / �P���Ԃ̎l���������j�̓r���ύX�i�P�U�i���j
#define BMS_BACKANIME       0x04            // �o�b�N�O���E���h�A�j���[�V�����@�\
#define BMS_EXTENEDOBJ      0x05            // �������Ă���I�u�W�F��ʂ̃L�����N�^�[�i���o�[�̂��̂ɂ���ւ���@�\
#define BMS_CHGPOORANI      0x06            // POOR���o�����Ƃ��ɕ\�������摜��ύX
#define BMS_LAYERANIME      0x07            // Ch.04�Ŏw�肵��BGA�̏�ɂ��Ԃ���BMP���w��ł��܂�
#define BMS_BPMINDEX        0x08            // BPM�̃C���f�b�N�X�w��(�V)

struct BMSHeader
{
    long ml_player;                    // �v���C���[�h
    char m_genre[256];                // �f�[�^�̃W������
    char m_title[256];                // �f�[�^�̃^�C�g��
    char m_artist[256];               // �f�[�^�̐����
    float mf_bpm;                       // �����e���|(�����l��130)
    char m_midiFile[MAX_PATH];        // �o�b�N�O���E���h�ŗ���MIDI�t�@�C��
    long ml_playLevel;                 // �Q�[���̓�Փx
    long ml_rank;                      // ���胉���N
    long ml_wavVol;                    // ���ʂ����̉�%�ɂ��邩
    long ml_total;                     // �Q�[�W�̑���
    char m_stagePic[MAX_PATH];        // �ȊJ�n���ɕ\������摜
    float mf_bpmIndex[BMS_MAXBUFFER];   // �e���|�C���f�b�N�X(�����l��120)

    long ml_endBar;                    // �I������
    long ml_maxCount;                  // �ő�̃J�E���g��
};

typedef struct BMSData
{
    LONG ml_time;                      // ���̃f�[�^�̊J�n�ʒu(BMS�J�E���g�l)
    LONG ml_data;                      // �炷�f�[�^(0x01�`0xFF)
    float mf_data;                      // �����l�f�[�^(�e���|�p)
    BOOL mb_flg;                      // �A�v�����g�p�o����C�ӂ̕ϐ�(�����ł͔���ɗ��p)
    Math::Color m_color;
};

struct BMSBar
{
    float mf_scale;                     // ���̏��߂̒����{��
    LONG ml_time;                      // ���̏��߂̊J�n�ʒu(BMS�J�E���g�l)
    LONG ml_length;                    // ���̏��߂̒���(BMS�J�E���g�l)
};

class BMSLoader
{
public:

    BMSLoader() {}
    ~BMSLoader() {}

    bool Init();
    bool LoadHeader(const char* _file);
    bool Load(const char* _file);
    bool Save(const char* _file);
    bool Sort(int _ch);

    bool Restart();
    long GetCountFromTime(double _sec);

    //inline const char* GetLastError(void) { return mLastError; }				// �Ō�̃G���[������

    // �Q�[���K�{���\�b�h
    inline LONG GetMaxCount(void) { return m_header.ml_maxCount; }			// �Q�[�����̍ő�̃J�E���g�l
    inline int GetBarNum(void) { return m_header.ml_endBar + 1; }			// ���߃o�[�̌�(�Ō�̏��߂��܂ނ���+1����)
    inline const BMSBar* GetBar(int num) { return &m_bmsBar[num]; }			// ���߃o�[�̃f�[�^
    inline int GetObjeNum(int ch) { return mi_bmsData[ch]; }			// �w��`���l���̃f�[�^����Ԃ�
    inline BMSData* GetObje(int ch, int num) { return &mp_bmsData[ch][num]; }		// �`���l���Ɣz��ԍ��Ńf�[�^���擾����
    inline const BMSHeader* GetHeader(void) { return &m_header; }					// �w�b�_����Ԃ�
    inline const char* GetBmpFile(int num) { return m_bmpFile[num]; }			// �g�p���Ă���BMP�t�@�C����
    inline const char* GetWavFile(int num) { return m_wavFile[num]; }			// �g�p���Ă���WAV�t�@�C����

protected:

    BMSHeader m_header;
    //std::vector<BMSData> m_bmsData[BMS_MAXBUFFER];
    BMSData* mp_bmsData[BMS_MAXBUFFER];
    int mi_bmsData[BMS_MAXBUFFER];

    char m_wavFile[BMS_MAXBUFFER][MAX_PATH];
    char m_bmpFile[BMS_MAXBUFFER][MAX_PATH];

    BMSBar m_bmsBar[1000 + 1];

    //char m_lastError[1024];

private:

    int atoi1610(const char* s);							// 16�i��������𐔒l�ɕϊ�
    bool itoa1016(int num, char* dst, int keta = -1);			// 10�i�������t����16�i�������֕ϊ�
    bool AddData(int ch, LONG cnt, LONG data);				// 1�̃f�[�^��ǉ��i�\�[�g�͂���Ȃ��j
    int GetCommand(const char* s);						// �R�}���h�ԍ���Ԃ�
    bool GetCommandString(const char* src, char* dst);		// �p�����[�^��������擾
    bool LoadBmsData(const char* file);					// BMS�f�[�^�̓ǂݍ���
    bool LineCompact(const char* src, char* dst);			// �f�[�^���œK�����ĕԂ�

};