#include "NoteBreaker.h"
#include "../Note/Note.h"
#include "../NoteManager/NoteManager.h"

void NoteBreaker::Init()
{
	const std::list<std::shared_ptr<Note>>& noteList = mwp_noteManager.lock()->GetNoteList();
	auto& it = noteList.begin();
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

	for ( int i = 0; i < 4; i++ )	//それぞれのレーンについて
	{
		bool jadge = false;	//各レーンにおいて判定を行ったか
		long minDif = BMS_RESOLUTION;		//判定発生時に判定バーから一番近いノーツの距離
		//std::shared_ptr<Note>& nearNote = std::make_shared<Note>();	//一番近いノーツのデータ
		auto& it = mwp_noteList[i].begin();	//各レーンの全ノーツデータ
		auto& nearit = mwp_noteList[i].begin();	//一番近いノーツのイテレーター
		while ( it != mwp_noteList[i].end() )
		{
			if ( (*it).expired() )
			{
				it = mwp_noteList[i].erase( it );
				continue;
			}
			//判定範囲以上のノーツまで探索が進めばそれ以降は判定除外（ループから抜ける
			if ( (*it).lock()->GetCount() > (_nowCount + Constant::MISS_RANGE) )
			{
				//今ループで判定が行われているなら
				if ( jadge )
				{
					//判定されたノーツの結果を格納する
					if ( minDif <= PERFECT_RANGE )
					{
						(*nearit).lock()->SetJadge( PERFECT );
					}
					else if ( minDif <= GREAT_RANGE )
					{
						(*nearit).lock()->SetJadge( GREAT );
					}
					else if ( minDif <= GOOD_RANGE )
					{
						(*nearit).lock()->SetJadge( GOOD );
					}
					else if ( minDif <= BAD_RANGE )
					{
						(*nearit).lock()->SetJadge( BAD );
					}
					//判定されたノーツの生存フラグ無効化
					(*nearit).lock()->SetEnable( false );
				}

				break;
			}
			//判定範囲内↓

			// 判定未実行のノーツ(生存している)について判定する
			if ( !(*it).lock()->GetEnable() )	//判定されたノーツはスキップ
			{
				//it++;
				continue;
			}

			//判定範囲外までスクロールされたノーツについては消去する
			if ( (*it).lock()->GetCount() < (_nowCount - Constant::BAD_RANGE) )
			{
				(*it).lock()->SetEnable( false );
				//it = mwp_noteList[i].erase( it );
			}
			else
			{
				if ( INPUT.GetKeyStateToManager( Constant::KEYID_4K[i] ) == KEYSTATE::PRESS )
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
						nearit = it;
						//判定範囲外までループが進んだ際に処理を行う
						jadge = true;
					}
				}
			}
			it++;
		}
	}

}

void NoteBreaker::Draw()
{}
