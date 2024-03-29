#include "BMSLoader.h"

#define SAFE_FREE(x)		{ if(x) { free(x); x=NULL; } }

bool BMSLoader::Init()
{
	// ヘッダ初期化
	ZeroMemory(&m_header, sizeof(m_header));
	m_header.ml_player = 1;
	m_header.mf_bpm = 130;
	for (int i = 0; i < BMS_MAXBUFFER; i++) {
		m_header.mf_bpmIndex[i] = 120.0f;
	}

	// 実データ初期化
	for (int i = 0; i < BMS_MAXBUFFER; i++)
	{
		mp_bmsData[i] = nullptr;		// BMSデータ領域をクリア
		mi_bmsData[i] = 0;				// データの数をクリア
	}

	// ファイル名
	ZeroMemory(&m_wavFile, sizeof(m_wavFile));
	ZeroMemory(&m_bmpFile, sizeof(m_bmpFile));

	// 小節の長さを1.0で初期化
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
		//1行を読み込む
		buf.clear();
		std::getline( ifs, buf );
		if ( buf[0] == NULL && ifs.eof() )break;//ファイルの終端なら検索終わり

		//コマンド以外なら飛ばす
		if ( buf[0] != '#' )continue;

		//最後の改行を消去
		if ( buf[buf.length() - 1] == '\n' )
		{
			buf[buf.length() - 1] = NULL;
		}

		//コマンドの解析
		int cmd = GetCommand( buf );
		
		//不明なコマンドならスキップ
		if ( cmd <= -2 )
		{
			continue;
		}

		//パラメータの分割
		std::string str;
		str.clear();
		if ( !GetCommandString( buf, str ) )
		{
			//文字列の取得が失敗なら
			ifs.close();
			return false;
		}

		//パラメータの代入
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
					//基本コマンドなら
					m_header.mf_bpm = std::stof( str );
					AddData( BMS_TEMPO, 0, (LONG)m_header.mf_bpm );
				}
				else
				{
					//拡張コマンドなら
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
				//小節番号の取得 : #"111"xx:******
				tmp.clear();
				tmp = buf.substr( 1, 3 );
				line = std::stoi( tmp );
				//チャンネル番号の取得 : #111"xx":******
				tmp.clear();
				tmp = buf.substr( 4, 2 );
				ch = atoi1610( tmp );
				if ( ch == BMS_STRETCH )
				{
					//小節の倍率変更命令の場合
					m_bmsBar[line].mf_scale = (float)atof( str.data() );
				}
				//小節番号の最大値を記憶する
				if ( m_header.ml_endBar < line )
				{
					m_header.ml_endBar = line;
				}
				break;
		}
	}

	//最後の小節内にもデータが存在するため、その次の小節を終端小節とする
	m_header.ml_endBar++;

	//小節倍率データをもとにすべての小節情報を算出
	long cnt = 0;
	for ( int i = 0; i <= m_header.ml_endBar; i++ )
	{
		//小節リストを加算
		m_bmsBar[i].ml_time = cnt;	//現在の小節の開始カウントを記録
		m_bmsBar[i].ml_length = (long)(BMS_RESOLUTION * m_bmsBar[i].mf_scale);	//倍率からこの小節の長さカウント値を算出

		//この小節のカウント数を加算して次の小節の開始カウントとする
		cnt += m_bmsBar[i].ml_length;
	}

	m_header.ml_maxCount = cnt;

	ifs.close();

	return true;
}

bool BMSLoader::Load(const char* _file)
{
	// ヘッダ＆小節倍率の読み込み
	if (!LoadHeader(_file))
	{
		return FALSE;
	}

	// 実データの読み込み
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

	// 昇順に並び替える
	int i, j;
	for (i = 0; i < mi_bmsData[_ch] - 1; i++) {
		for (j = i + 1; j < mi_bmsData[_ch]; j++) {
			if (mp_bmsData[_ch][i].ml_time > mp_bmsData[_ch][j].ml_time) {
				// 構造体を入れ替える
				BMSData dmy = mp_bmsData[_ch][i];		// ダミーに保存
				mp_bmsData[_ch][i] = mp_bmsData[_ch][j];		// iにjを入れる
				mp_bmsData[_ch][j] = dmy;					// jにダミーを入れる
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
	long cnt = 0;			// BMSカウント
	double t = 0;			// BMS上の時間
	double bpm = 130;

	if (mi_bmsData[BMS_TEMPO] > 0)
	{
		bpm = mp_bmsData[BMS_TEMPO][0].mf_data;		// 初期BPM
	}

	if (_sec < 0)
		return 0;


	// 指定時間を越えるまでタイムを加算
	int i;
	for (i = 0; i < mi_bmsData[BMS_TEMPO]; i++) {

		// １つ前の時間と新しい時間との経過時間から秒を算出
		double add = (double)(mp_bmsData[BMS_TEMPO][i].ml_time - cnt) / (bpm / 60) / (BMS_RESOLUTION / 4);

		// 現在のテンポ値で時間が過ぎたら抜ける
		if (t + add > _sec) {
			break;
		}

		t += add;										// 経過時間を加算
		bpm = (double)mp_bmsData[BMS_TEMPO][i].mf_data;		// 次のBPMをセット
		cnt = mp_bmsData[BMS_TEMPO][i].ml_time;			// 計算済みのカウントをセット
	}

		// 指定時間と1つ前までの時間の差分
	double sub = _sec - t;

		// 差分からBMSカウント数を算出
	long cnt2 = (long)(sub * (BMS_RESOLUTION / 4) * (bpm / 60));

	// BMSカウント値に加算
	cnt += cnt2;

	return cnt;
}


int BMSLoader::atoi1610( const std::string& str )
{
	int ret = 0;			// 10進数に変換した値
	int i = 0;				// 参照する文字配列
	while (str[i]) {
		if (!(str[i] >= '0' && str[i] <= '9') &&
			!(str[i] >= 'A' && str[i] <= 'Z') &&
			!(str[i] >= 'a' && str[i] <= 'z'))
			return 0;

		ret *= 16;				// 16倍
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

bool BMSLoader::itoa1016( int num, std::string& dst, int size )
{
	if ( num < 0 )
		return FALSE;

	std::string tmp;

	for ( int i = 0; i < size; i++ )
	{
		int digit = num % (int)pow( 16, (i + 1) );
		std::stringstream ss;
		ss << std::hex << digit;
		tmp = ss.str() + tmp;
	}

	dst = tmp;

	return TRUE;
}

bool BMSLoader::AddData(int ch, LONG cnt, LONG data)
{
	// チャンネル番号をチェック
	if (ch < 0 || ch>255)
		return FALSE;

	// 小節長変更コマンドなら何もしない
	if (ch == BMS_STRETCH)
		return FALSE;

	// データが無ければ何もしない
	if (data == 0)
		return TRUE;

	switch (ch)
	{
		case BMS_BPMINDEX:
			// BPMのインデックス指定(新)
			mi_bmsData[BMS_TEMPO]++;
			mp_bmsData[BMS_TEMPO] = (BMSData*)realloc(mp_bmsData[BMS_TEMPO], mi_bmsData[BMS_TEMPO] * sizeof(BMSData));
			ZeroMemory(&mp_bmsData[BMS_TEMPO][mi_bmsData[BMS_TEMPO] - 1], sizeof(BMSData));	// 追加した配列をクリア
			mp_bmsData[BMS_TEMPO][mi_bmsData[BMS_TEMPO] - 1].mb_flg = TRUE;
			mp_bmsData[BMS_TEMPO][mi_bmsData[BMS_TEMPO] - 1].ml_time = cnt;
			mp_bmsData[BMS_TEMPO][mi_bmsData[BMS_TEMPO] - 1].ml_data = (LONG)m_header.mf_bpmIndex[data];	// テンポリストに入っているテンポ値を登録(LONG型にも保存)
			mp_bmsData[BMS_TEMPO][mi_bmsData[BMS_TEMPO] - 1].mf_data = m_header.mf_bpmIndex[data];			// テンポリストに入っているテンポ値を登録
			break;
		default:
			// データを追加
			mi_bmsData[ch]++;
			mp_bmsData[ch] = (BMSData*)realloc(mp_bmsData[ch], mi_bmsData[ch] * sizeof(BMSData));
			ZeroMemory(&mp_bmsData[ch][mi_bmsData[ch] - 1], sizeof(BMSData));					// 追加した配列をクリア
			mp_bmsData[ch][mi_bmsData[ch] - 1].mb_flg = TRUE;
			mp_bmsData[ch][mi_bmsData[ch] - 1].m_color = kWhiteColor;
			mp_bmsData[ch][mi_bmsData[ch] - 1].ml_time = cnt;
			mp_bmsData[ch][mi_bmsData[ch] - 1].ml_data = data;
			mp_bmsData[ch][mi_bmsData[ch] - 1].mf_data = (float)data;								// float型にも保存
			break;
	}

	return TRUE;
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

	// 検索ルーチン
	for ( i = 0; i < 13; i++ )
	{
		if ( tmp == command[i] )
			return i;	// コマンドならその番号を返す
	}

	// 先頭が'#nnncc'形式か
	BOOL obj = TRUE;
	for ( i = 0; i < 5; i++ )
	{
		if ( tmp[i] < '0' || tmp[i]>'9' )
		{
			obj = FALSE;
			break;
		}
	}

	// オブジェ配置なら -1
	if ( obj )
	{
		return -1;
	}

	// 処理不可能文字列なら
	return -2;
}

bool BMSLoader::GetCommandString( const std::string& src, std::string& dst )
{
	int i = 0;

	std::string tmp;

	// まずソースデータからデータ部分までのポインタを算出
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

	std::ifstream ifs( file );
	if ( ifs.fail() )return false;

	int i;
	std::string data;
	int cmd;			// コマンド番号
	int line;			// 現在の小節番号
	int ch;				// 現在のチャンネル番号
	int len;			// 文字列の長さ

	std::string buf;

	while (1) {
		// １行を読みこむ
		buf.clear();
		std::getline( ifs, buf );
		if ( buf[0] == NULL && ifs.eof() )break;//ファイルの終端なら検索終わり

		// コマンド以外なら次の行へ
		if (buf[0] != '#')
			continue;

		// 最後の改行を消去
		if ( buf[buf.size() - 1] == '\n' )
		{
			buf[buf.size() - 1] = NULL;
		}

		// コマンドの種類を取得
		cmd = GetCommand(buf);

		// オブジェ以外は全て無視
		if (cmd != -1)
			continue;

		// パラメータ文字列を取得
		data.clear();
		if ( !GetCommandString( buf, data ) )
		{
			ifs.close();
			return false;
		}

		// データであれば解析

		//// チャンネル番号の取得
		std::string tmp;

		//小節番号の取得 : #"111"xx:******
		tmp.clear();
		tmp = buf.substr( 1, 3 );
		line = std::stoi( tmp );
		//チャンネル番号の取得 : #111"xx":******
		tmp.clear();
		tmp = buf.substr( 4, 2 );
		ch = atoi1610( tmp );

		// 小節の倍率変更命令の場合はキャンセル
		if (ch == BMS_STRETCH)
			continue;

		// データが存在するかチェック
		if ((int)data.length() < 1) {
			continue;
		}

		// データが偶数かチェック
		if ((int)data.size() % 2 == 1 ) {
			ifs.close();
			return FALSE;
		}

		// データ数
		len = (int)data.length() / 2;

		// 現在の小節の長さ(カウント値)からこの小節内での最短音符1音符分のカウント値を算出
		LONG tick = m_bmsBar[line].ml_length / len;

		// 実データを追加
		tmp.clear();
		for (i = 0; i < len; i++) {
			//i番目のデータを取得
			tmp = data.substr( i * 2, 2 );
			int idata = atoi1610(tmp);			// 16進数
			// データが存在する場合
			if (idata > 0) {
				//探索中のチャンネル番号・小説番号から求めたこの音符データのカウント値でデータ追加
				AddData(ch, m_bmsBar[line].ml_time + (tick * i), idata);
			}
		}
	}

	ifs.close();

	// ソート
	for (i = 0; i < BMS_MAXBUFFER; i++)
		Sort(i);

	return TRUE;
}

/*
bool BMSLoader::LineCompact(const char* src, char* dst)
{
	int i, j, k;
	char buf[1024];		// ワークバッファ
	strcpy(buf, src);	// 元データをコピー
	*dst = NULL;		// 出力先初期化

	// 010000020000030000040000010000020000030000040000010000020000030000040000010000020000030000040000
	//   ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  ^^  i=1  (i+1)=2で割り切れる文字数か→○
	//   ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  ^^^^  i=2  (i+1)=3で割り切れる文字数か→○
	//   ^^^^^^  ^^^^^^  ^^^^^^  ^^^^^^  ^^^^^^  ^^^^^^  ^^^^^^  ^^^^^^  ^^^^^^  ^^^^^^  ^^^^^^  ^^^^^^  i=3  (i+1)=4で割り切れる文字数か→○
	//   ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^^^^^  ^^^^  i=4  (i+1)=5で割り切れる文字数か→×
	//   ^^^^^^^^^^  ^^^^^^^^^^  ^^^^^^^^^^  ^^^^^^^^^^  ^^^^^^^^^^  ^^^^^^^^^^  ^^^^^^^^^^  ^^^^^^^^^^  i=5  (i+1)=6で割り切れる文字数か→○
	//   ^^^^^^^^^^^^  ^^^^^^^^^^^^  ^^^^^^^^^^^^  ^^^^^^^^^^^^  ^^^^^^^^^^^^  ^^^^^^^^^^^^  ^^^^^^^^^^  i=6  (i+1)=7で割り切れる文字数か→×
	//                                              ：
	//   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^      i=45 (i+1)=46で割り切れる文字数か→×
	//   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^    i=46 (i+1)=47で割り切れる文字数か→×
	//   ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^  i=47 (i+1)=48で割り切れる文字数か→○
	while (1) {

		// データ数を算出
		int l = (int)strlen(buf) / 2;

		// 空文字ならデータ無し
		if (l == 0) {
			return FALSE;
		}

		// 1個のデータなら
		if (l == 1) {
			if (buf[0] == '0' && buf[1] == '0') {
				// 00ならデータ無し
				return FALSE;
			}
			// それ以外なら確定
			break;
		}


		// 2個以上なら最適化処理
		BOOL div = FALSE;					// 分割出来たか
		for (i = 1; i < l; i++) {

			// 分割する文字数
			int fetch_size = 1 + i;

			// 割り切れるか
			if ((l % fetch_size) != 0)
				continue;					// 割り切れないならスキップ

			// 分割文字数内の2番目以降に00以外が含まれるか
			BOOL zero = TRUE;		// 00以外が存在するか
			for (j = 0; j < l; j += fetch_size) {
				for (k = 1; k < fetch_size; k++) {
					int p = (j + k) * 2;
					if (buf[p] != '0' || buf[p + 1] != '0') {
						// 00以外が存在するなら
						zero = FALSE;
						j = l;				// 上位ループも終わらせる
						break;
					}
				}
			}

			// 00のみなら現座時の分割サイズで分割
			if (zero) {
				k = 1;
				for (j = 0 + fetch_size; j < l; j += fetch_size) {			// 1文字目は常に確定なので2つ目からコピー
					//int src = j * 2;
					//int dst = k * 2;
					buf[k * 2 + 0] = buf[j * 2 + 0];
					buf[k * 2 + 1] = buf[j * 2 + 1];
					// 次のコピー先へ
					k++;
				}
				buf[k * 2] = NULL;
				l = k;
				div = TRUE;

				//				DEBUG( "分割後 : %s\n",buf );
				break;
			}
		}

		// これ以上分割出来なかったら終わる
		if (!div)
			break;
	}

	// 結果を返す
	strcpy(dst, buf);

	return TRUE;
}
*/