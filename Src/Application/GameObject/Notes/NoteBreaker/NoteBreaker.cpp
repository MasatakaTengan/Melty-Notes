#include "NoteBreaker.h"
#include "../Note/Note.h"
#include "../NoteManager/NoteManager.h"

void NoteBreaker::Init()
{
	const std::list<std::shared_ptr<Note>>& noteList = mwp_noteManager.lock()->GetNoteList();
	auto it = noteList.begin();
	while ( it != noteList.end() )
	{
		/*switch ( ( *it )->GetKey() )
		{
			case Constant::KEYID_4K[0]:
				break;
		}*/
		for ( int i = 0; i < 4; i++ )
		{
			if ( (*it)->GetKey() == Constant::KEYID_4K[i] )
			{
				mwp_noteList[i].push_back( (*it) );
			}
		}
		it++;
	}
}

void NoteBreaker::Update( LONG _nowCount )
{
	//入力判定のループ
	//判定用のデータリストを作成
	//判定用の関数JADGEで専用リストのみで判定する

	for ( int i = 0; i < 4; i++ )
	{
		if ( INPUT.GetKeyStateToManager( Constant::KEYID_4K[i] ) == KEYSTATE::PRESS )
		{
			//キーが押された時にそのキーに対応したノーツリストから判定用のリストを作成する
			AddJadgeList( i, _nowCount );
			//判定用のリストを使用して判定を行う
			JadgeNote( _nowCount );
		}
	}

}

void NoteBreaker::AddJadgeList( int i, LONG _nowCount )
{
	auto it = mwp_noteList[i].begin();
	while ( it != mwp_noteList[i].end() )
	{
		//Managerの持つ元リストが削除されている部分は削除する
		if ( (*it).expired() )
		{
			it = mwp_noteList[i].erase( it );
			continue;
		}

		//判定範囲以上のノーツまで探索が進めばそれ以降は判定除外（ループから抜ける
		if ( (*it).lock()->GetCount() > (_nowCount + Constant::MISS_RANGE) )
		{
			break;
		}
		//判定範囲以下のノーツが残っている場合は自クラス内のノーツリストから除外する
		if ( (*it).lock()->GetCount() < (_nowCount - Constant::BAD_RANGE) )
		{
			(*it).lock()->SetEnable( false );
		}
		else
		{
			//判定範囲内のノーツを判定用リストに格納
			mwp_jadgeList.push_back( *it );
		}

		it++;

	}
}

void NoteBreaker::JadgeNote( LONG _nowCount )
{
	if ( mwp_jadgeList.size() <= 0 )return;

	auto it = mwp_jadgeList.begin();
	auto jadgeit = mwp_jadgeList.begin();
	long minDif = BMS_RESOLUTION;
	while ( it != mwp_jadgeList.end() )
	{
		//判定範囲内で一番下のノーツから判定を優先するか
		//判定発生時に判定ラインから一番近いノーツを優先するか←採用!!!!!!!!!!!!

		//一番判定ラインに近いノーツを探索

		//判定発生時のカウント値とノーツ自身のカウント値との絶対値
		//カウント絶対値
		long dif = abs( (*it).lock()->GetCount() - _nowCount );
		if ( dif < minDif )		//これまでのノーツのカウント絶対値より小さい場合
		{
			//カウント絶対値をより小さいものに更新
			minDif = dif;
			//このノーツを一番近いノーツとして登録
			jadgeit = it;
			//判定範囲外までループが進んだ際に処理を行う
			//jadge = true;
		}
		it++;
	}

	//判定されたノーツの結果を格納する
	if ( minDif <= PERFECT_RANGE )
	{
		(*jadgeit).lock()->SetJadge( PERFECT );
	}
	else if ( minDif <= GREAT_RANGE )
	{
		(*jadgeit).lock()->SetJadge( GREAT );
	}
	else if ( minDif <= GOOD_RANGE )
	{
		(*jadgeit).lock()->SetJadge( GOOD );
	}
	else if ( minDif <= BAD_RANGE )
	{
		(*jadgeit).lock()->SetJadge( BAD );
	}
	//判定されたノーツの生存フラグ無効化
	(*jadgeit).lock()->SetEnable( false );

	mwp_jadgeList.clear();
}

void NoteBreaker::Draw()
{}
