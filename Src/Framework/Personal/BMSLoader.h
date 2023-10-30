#pragma once
#define BMS_RESOLUTION      9600            // 1小節のカウント値
#define BMS_MAXBUFFER       (16*16)         // 00～FFまでのバッファ数

// BMSチャンネル定義
#define BMS_BACKMUSIC       0x01            // その位置にきたら、自動的に再生されるWAVを指定します
#define BMS_STRETCH         0x02            // その小節の長さを定義したデータ倍します（１０進数、小数ともに使用可）
#define BMS_TEMPO           0x03            // 再生テンポ（ＢＰＭ / １分間の四分音符数）の途中変更（１６進数）
#define BMS_BACKANIME       0x04            // バックグラウンドアニメーション機能
#define BMS_EXTENEDOBJ      0x05            // 落下してくるオブジェを別のキャラクターナンバーのものにすり替える機能
#define BMS_CHGPOORANI      0x06            // POORを出したときに表示される画像を変更
#define BMS_LAYERANIME      0x07            // Ch.04で指定したBGAの上にかぶせるBMPを指定できます
#define BMS_BPMINDEX        0x08            // BPMのインデックス指定(新)

struct BMSHeader
{
    long        lPlayer;                    // プレイモード
    char        mGenre[256];                // データのジャンル
    char        mTitle[256];                // データのタイトル
    char        mArtist[256];               // データの製作者
    float       fBpm;                       // 初期テンポ(初期値は130)
    char        mMidifile[MAX_PATH];        // バックグラウンドで流すMIDIファイル
    long        lPlaylevel;                 // ゲームの難易度
    long        lRank;                      // 判定ランク
    long        lWavVol;                    // 音量を元の何%にするか
    long        lTotal;                     // ゲージの増量
    char        mStagePic[MAX_PATH];        // 曲開始字に表示する画像
    float       fBpmIndex[BMS_MAXBUFFER];   // テンポインデックス(初期値は120)

    long        lEndBar;                    // 終了小節
    long        lMaxCount;                  // 最大のカウント数
};

typedef struct BMSData
{
    LONG        lTime;                      // このデータの開始位置(BMSカウント値)
    LONG        lData;                      // 鳴らすデータ(0x01～0xFF)
    float       fData;                      // 小数値データ(テンポ用)
    BOOL        bFlag;                      // アプリが使用出来る任意の変数(ここでは判定に利用)
    Math::Color mColor;
};

struct BMSBar
{
    float       fScale;                     // この小節の長さ倍率
    LONG        lTime;                      // この小節の開始位置(BMSカウント値)
    LONG        lLength;                    // この小節の長さ(BMSカウント値)
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

    //inline const char* GetLastError(void) { return mLastError; }				// 最後のエラー文字列

    // ゲーム必須メソッド
    inline LONG GetMaxCount(void) { return m_header.lMaxCount; }			// ゲーム内の最大のカウント値
    inline int GetBarNum(void) { return m_header.lEndBar + 1; }			// 小節バーの個数(最後の小節も含むため+1する)
    inline const BMSBar* GetBar(int num) { return &m_bmsBar[num]; }			// 小節バーのデータ
    inline int GetObjeNum(int ch) { return mi_bmsData[ch]; }			// 指定チャネルのデータ数を返す
    inline BMSData* GetObje(int ch, int num) { return &mp_bmsData[ch][num]; }		// チャネルと配列番号でデータを取得する
    inline const BMSHeader* GetHeader(void) { return &m_header; }					// ヘッダ情報を返す
    inline const char* GetBmpFile(int num) { return m_bmpFile[num]; }			// 使用しているBMPファイル名
    inline const char* GetWavFile(int num) { return m_wavFile[num]; }			// 使用しているWAVファイル名

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

    int atoi1610(const char* s);							// 16進数文字列を数値に変換
    bool itoa1016(int num, char* dst, int keta = -1);			// 10進数を桁付きの16進数文字へ変換
    bool AddData(int ch, LONG cnt, LONG data);				// 1つのデータを追加（ソートはされない）
    int GetCommand(const char* s);						// コマンド番号を返す
    bool GetCommandString(const char* src, char* dst);		// パラメータ文字列を取得
    bool LoadBmsData(const char* file);					// BMSデータの読み込み
    bool LineCompact(const char* src, char* dst);			// データを最適化して返す

};