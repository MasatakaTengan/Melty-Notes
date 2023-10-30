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
    long ml_player;                    // プレイモード
    char m_genre[256];                // データのジャンル
    char m_title[256];                // データのタイトル
    char m_artist[256];               // データの製作者
    float mf_bpm;                       // 初期テンポ(初期値は130)
    char m_midiFile[MAX_PATH];        // バックグラウンドで流すMIDIファイル
    long ml_playLevel;                 // ゲームの難易度
    long ml_rank;                      // 判定ランク
    long ml_wavVol;                    // 音量を元の何%にするか
    long ml_total;                     // ゲージの増量
    char m_stagePic[MAX_PATH];        // 曲開始字に表示する画像
    float mf_bpmIndex[BMS_MAXBUFFER];   // テンポインデックス(初期値は120)

    long ml_endBar;                    // 終了小節
    long ml_maxCount;                  // 最大のカウント数
};

typedef struct BMSData
{
    LONG ml_time;                      // このデータの開始位置(BMSカウント値)
    LONG ml_data;                      // 鳴らすデータ(0x01～0xFF)
    float mf_data;                      // 小数値データ(テンポ用)
    BOOL mb_flg;                      // アプリが使用出来る任意の変数(ここでは判定に利用)
    Math::Color m_color;
};

struct BMSBar
{
    float mf_scale;                     // この小節の長さ倍率
    LONG ml_time;                      // この小節の開始位置(BMSカウント値)
    LONG ml_length;                    // この小節の長さ(BMSカウント値)
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
    inline LONG GetMaxCount(void) { return m_header.ml_maxCount; }			// ゲーム内の最大のカウント値
    inline int GetBarNum(void) { return m_header.ml_endBar + 1; }			// 小節バーの個数(最後の小節も含むため+1する)
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