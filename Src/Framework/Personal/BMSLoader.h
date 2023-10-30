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
    long        lPlayer;                    // �v���C���[�h
    char        mGenre[256];                // �f�[�^�̃W������
    char        mTitle[256];                // �f�[�^�̃^�C�g��
    char        mArtist[256];               // �f�[�^�̐����
    float       fBpm;                       // �����e���|(�����l��130)
    char        mMidifile[MAX_PATH];        // �o�b�N�O���E���h�ŗ���MIDI�t�@�C��
    long        lPlaylevel;                 // �Q�[���̓�Փx
    long        lRank;                      // ���胉���N
    long        lWavVol;                    // ���ʂ����̉�%�ɂ��邩
    long        lTotal;                     // �Q�[�W�̑���
    char        mStagePic[MAX_PATH];        // �ȊJ�n���ɕ\������摜
    float       fBpmIndex[BMS_MAXBUFFER];   // �e���|�C���f�b�N�X(�����l��120)

    long        lEndBar;                    // �I������
    long        lMaxCount;                  // �ő�̃J�E���g��
};

typedef struct BMSData
{
    LONG        lTime;                      // ���̃f�[�^�̊J�n�ʒu(BMS�J�E���g�l)
    LONG        lData;                      // �炷�f�[�^(0x01�`0xFF)
    float       fData;                      // �����l�f�[�^(�e���|�p)
    BOOL        bFlag;                      // �A�v�����g�p�o����C�ӂ̕ϐ�(�����ł͔���ɗ��p)
    Math::Color mColor;
};

struct BMSBar
{
    float       fScale;                     // ���̏��߂̒����{��
    LONG        lTime;                      // ���̏��߂̊J�n�ʒu(BMS�J�E���g�l)
    LONG        lLength;                    // ���̏��߂̒���(BMS�J�E���g�l)
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
    inline LONG GetMaxCount(void) { return m_header.lMaxCount; }			// �Q�[�����̍ő�̃J�E���g�l
    inline int GetBarNum(void) { return m_header.lEndBar + 1; }			// ���߃o�[�̌�(�Ō�̏��߂��܂ނ���+1����)
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