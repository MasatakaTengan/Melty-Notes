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

	std::string sbuf;
	std::string stmp;
	int inum = 0;
	int ich = 0;
	int iline = 0;

	while ( 1 )
	{
		//1行を読み込む
		sbuf.clear();
		std::getline( ifs, sbuf );
		if ( sbuf[0] == NULL && ifs.eof() )break;//ファイルの終端なら検索終わり

		//コマンド以外なら飛ばす
		if ( sbuf[0] != '#' )continue;

		//最後の改行を消去
		if ( sbuf[sbuf.length() - 1] == '\n' )
		{
			sbuf[sbuf.length() - 1] = NULL;
		}

		//コマンドの解析
		int cmd = GetCommandStd( sbuf );
		
		//不明なコマンドならスキップ
		if ( cmd <= -2 )
		{
			continue;
		}

		//パラメータの分割
		std::string sstr;
		sstr.clear();
		if ( !GetCommandStringStd( sbuf, sstr ) )
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
				m_header.m_genre = sstr;
				break;
			case 2://TITLE
				m_header.m_title = sstr;
				break;
			case 3://ARTIST
				m_header.m_artist = sstr;
				break;
			case 4://BPM
				if ( sbuf[4] == ' ' || sbuf[4] == 0x09 )
				{
					//基本コマンドなら
					m_header.mf_bpm = (float)atof( sstr.data() );
					AddData( BMS_TEMPO, 0, (LONG)m_header.mf_bpm );
				}
				else
				{
					//拡張コマンドなら
					stmp.clear();
					stmp = sbuf.substr( 4, 2 );
					ich = atoi1610( stmp.data() );
					m_header.mf_bpmIndex[ich] = (float)atof( sstr.data() );
				}
				break;
			case 5://MIDIFILE
				m_header.m_midiFile = sstr;
				break;
			case 6://PLAYLEVEL
				m_header.ml_playLevel = atoi( sstr.data() );
				break;
			case 7://RANK
				m_header.ml_rank = atoi( sstr.data() );
				break;
			case 8://VOLWAV
				m_header.ml_wavVol = atoi( sstr.data() );
				break;
			case 9://TOTAL
				m_header.ml_total = atoi( sstr.data() );
				break;
			case 10://STAGEFILE
				m_header.m_stagePic = sstr;
				break;
			case 11://WAV
				stmp.clear();
				stmp = sbuf.substr( 4, 2 );
				ich = atoi1610( stmp.data() );
				m_wavFile[ich] = sstr;
				break;
			case 12://BMP
				stmp.clear();
				stmp = sbuf.substr( 4, 2 );
				ich = atoi1610( stmp.data() );
				m_bmpFile[ich] = sstr;
				break;
			default:
				//小節番号の取得
				stmp.clear();
				stmp = sbuf.substr( 1, 3 );
				iline = atoi( stmp.data() );
				//チャンネル番号の取得
				stmp.clear();
				stmp = sbuf.substr( 4, 2 );
				ich = atoi1610( stmp.data() );
				if ( ich == BMS_STRETCH )
				{
					//小節の倍率変更命令の場合
					m_bmsBar[iline].mf_scale = (float)atof( sstr.data() );
				}
				//小節番号の最大値を記憶する
				if ( m_header.ml_endBar < iline )
				{
					m_header.ml_endBar = iline;
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

bool BMSLoader::Save(const char* _file)
{
	return false;
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

	if (mi_bmsData[BMS_TEMPO] > 0) {
		bpm = mp_bmsData[BMS_TEMPO][0].mf_data;		// 初期BPM
	}

	if (_sec < 0)
		return 0;

	//	DEBUG( "■指定時間 (%.2f秒)\n",_sec );

		// 指定時間を越えるまでタイムを加算
	int i;
	for (i = 0; i < mi_bmsData[BMS_TEMPO]; i++) {

		// １つ前の時間と新しい時間との経過時間から秒を算出
		double add = (double)(mp_bmsData[BMS_TEMPO][i].ml_time - cnt) / (bpm / 60) / (BMS_RESOLUTION / 4);
		//		DEBUG( "  [%d] 経過時間 %f秒\n",i,t+add );

				// 現在のテンポ値で時間が過ぎたら抜ける
		if (t + add > _sec) {
			//			DEBUG( "                  └時間が過ぎた\n" );
			break;
		}

		t += add;										// 経過時間を加算
		bpm = (double)mp_bmsData[BMS_TEMPO][i].mf_data;		// 次のBPMをセット
		cnt = mp_bmsData[BMS_TEMPO][i].ml_time;			// 計算済みのカウントをセット
	}

	//	DEBUG( "  BPM %f\n",bpm );
	//	DEBUG( "  CNT %I64d\n",cnt );

		// 指定時間と1つ前までの時間の差分
	double sub = _sec - t;
	//	DEBUG( "  差分 %f秒\n",sub );

		// 差分からBMSカウント数を算出
	long cnt2 = (long)(sub * (BMS_RESOLUTION / 4) * (bpm / 60));

	// BMSカウント値に加算
	cnt += cnt2;
	//	DEBUG( "  結果のカウント値 %I64d\n",cnt );

	return cnt;
}


int BMSLoader::atoi1610(const char* s)
{
	int ret = 0;			// 10進数に変換した値
	int i = 0;				// 参照する文字配列
	while (s[i]) {
		if (!(s[i] >= '0' && s[i] <= '9') &&
			!(s[i] >= 'A' && s[i] <= 'Z') &&
			!(s[i] >= 'a' && s[i] <= 'z'))
			return 0;

		ret *= 16;				// 16倍
		int n = s[i] - '0';
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

	// 小文字チェック
	int i;
	for (i = 0; i < (int)strlen(dst); i++) {
		if (dst[i] >= 'a' && dst[i] <= 'z') {
			dst[i] -= 0x20;
		}
	}

	// 桁数指定の場合
	if (keta >= 0) {
		// 桁数が足りない場合
		if ((int)strlen(dst) < keta) {
			char dmy[1024];
			ZeroMemory(dmy, sizeof(dmy));
			for (i = 0; i < keta - (int)strlen(dst); i++) {
				dmy[i] = '0';			// 先頭に足りない分の0を追加
			}
			strcpy(&dmy[i], dst);		// その次に算出された文字列をつなげる
			strcpy(dst, dmy);			// 出力結果にダミー文字列をコピー		
		}
	}

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
			mp_bmsData[ch][mi_bmsData[ch] - 1].m_color = kRedColor;
			mp_bmsData[ch][mi_bmsData[ch] - 1].ml_time = cnt;
			mp_bmsData[ch][mi_bmsData[ch] - 1].ml_data = data;
			mp_bmsData[ch][mi_bmsData[ch] - 1].mf_data = (float)data;								// float型にも保存
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

	// 検索ルーチン
	int i;
	for (i = 0; i < 13; i++) {
		if (strnicmp(s + 1, command[i], strlen(command[i])) == 0)
			return i;	// コマンドならその番号を返す
	}

	// 先頭が'#nnncc'形式か
	BOOL obj = TRUE;
	for (i = 0; i < 5; i++) {
		if (s[i + 1] < '0' || s[i + 1]>'9') {
			obj = FALSE;
			break;
		}
	}

	// オブジェ配置なら -1
	if (obj) {
		return -1;
	}

	// 処理不可能文字列なら
	return -2;
}

int BMSLoader::GetCommandStd( std::string _str )
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

	// 検索ルーチン
	int i;
	for ( i = 0; i < 13; i++ )
	{
		if ( strnicmp( _str.c_str() + 1, command[i], strlen(command[i])) == 0 )
			return i;	// コマンドならその番号を返す
	}

	// 先頭が'#nnncc'形式か
	BOOL obj = TRUE;
	for ( i = 0; i < 5; i++ )
	{
		if ( _str[i + 1] < '0' || _str[i + 1]>'9' )
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

bool BMSLoader::GetCommandString(const char* src, char* dst)
{
	int i = 0;
	int j = 0;

	// まずソースデータからデータ部分までのポインタを算出
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

	// 終端までをコピー
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

bool BMSLoader::GetCommandStringStd( const std::string& src, std::string& dst )
{
	int i = 0;
	int j = 0;

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
	int i;
	char data[1024];
	int cmd;			// コマンド番号
	int line;			// 現在の小節番号
	int ch;				// 現在のチャンネル番号
	int len;			// 文字列の長さ


	FILE* fp;
	fp = fopen(file, "r");
	if (!fp) {
		//sprintf(mLastError, "[%s] ファイルオープンエラー", file);
		return FALSE;
	}

	char buf[1024];
	while (1) {
		// １行を読みこむ
		ZeroMemory(buf, 1024);
		fgets(buf, 1024, fp);
		if (buf[0] == NULL && feof(fp))	// ファイルの終端なら検索終わり
			break;

		//		DEBUG( "%s",buf );

				// コマンド以外なら次の行へ
		if (buf[0] != '#')
			continue;

		// 最後の改行を消去
		if (buf[strlen(buf) - 1] == '\n')
			buf[strlen(buf) - 1] = NULL;

		// コマンドの種類を取得
		cmd = GetCommand(buf);

		// オブジェ以外は全て無視
		if (cmd != -1)
			continue;

		// パラメータ文字列を取得
		ZeroMemory(data, 1024);
		if (!GetCommandString(buf, data)) {
			//DEBUG("パラメータ文字列取得エラー\n");
			fclose(fp);
			//sprintf(mLastError, "[%s] パラメータ文字列取得エラー", buf);
			return FALSE;
		}

		// データであれば解析

		// チャンネル番号の取得
		char tmp[4];						// 汎用バッファ
		ZeroMemory(&tmp, sizeof(tmp));
		tmp[0] = buf[4];					// チャンネル番号
		tmp[1] = buf[5];					// #001xx:******* のxx部分
		ch = atoi1610(tmp);				// 16進数

		// 小節の倍率変更命令の場合はキャンセル
		if (ch == BMS_STRETCH)
			continue;

		// 小節番号を取得
		ZeroMemory(&tmp, sizeof(tmp));
		memcpy(tmp, buf + 1, 3);		// 小節部分を取得
		line = atoi(tmp);			// 数字化

		// データが存在するかチェック
		if (strlen(data) < 1) {
			//DEBUG("データが定義されていない箇所が存在\n%s", buf);
			continue;
		}

		// データが偶数かチェック
		if (strlen(data) % 2 == 1) {
			//DEBUG("データが偶数個ではない [%s]\n", buf);
			fclose(fp);
			//sprintf(mLastError, "[%s] データが偶数個ではない", buf);
			return FALSE;
		}

		// データ数
		len = (int)strlen(data) / 2;

		// 現在の小節のカウント値から1音符分のカウント値を算出
		LONG tick = m_bmsBar[line].ml_length / len;

		// 実データを追加
		ZeroMemory(&tmp, sizeof(tmp));
		for (i = 0; i < len; i++) {
			tmp[0] = data[i * 2];
			tmp[1] = data[i * 2 + 1];
			int idata = atoi1610(tmp);			// 16進数
			if (idata > 0) {
				// データが存在する場合
				AddData(ch, m_bmsBar[line].ml_time + (tick * i), idata);
			}
		}
	}

	fclose(fp);

	// ソート
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
