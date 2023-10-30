#pragma once

class SceneManager;

//============================================================
// アプリケーションクラス
//	APP.～ でどこからでもアクセス可能
//============================================================
class Application
{
// メンバ
public:

	// アプリケーション実行
	void Execute();

	// アプリケーション終了
	void End() { m_endFlag = true; }

	HWND GetWindowHandle() const { return m_window.GetWndHandle(); }
	int GetMouseWheelValue() const { return m_window.GetMouseWheelVal(); }

	int GetNowFPS() { return mi_fps; }
	//int GetMaxFPS() const { return m_fpsController.GetMaxFPS(); }
	//double GetDeltaTime() const { return m_fpsController.GetDeltaTime(); }

private:

	void Event();

	void PreUpdate();
	void Update();
	void PostUpdate();

	void PreDraw();
	void Draw();
	void PostDraw();

	void DrawSprite();

	// アプリケーション初期化
	bool Init(int w, int h);

	// アプリケーション解放
	void Release();

	// ゲームウィンドウクラス
	KdWindow		m_window;

	// ゲーム終了フラグ trueで終了する
	bool		m_endFlag = false;

	// FPSコントローラー
	//KdFPSController	m_fpsController;

	//Timer
	Timer m_timer;
	int mi_fps = 60;

	// ゲームオブジェクトリスト
	//std::vector<std::shared_ptr<KdGameObject>>	m_spGameObjectList;

	//SceneManager
	std::shared_ptr<SceneManager> msp_scene = nullptr;

//=====================================================
// シングルトンパターン
//=====================================================
private:
	// 
	Application() {}

public:
	static Application &Instance(){
		static Application Instance;
		return Instance;
	}
};