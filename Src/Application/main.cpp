#include "main.h"

#include "SceneManager/SceneManager.h"

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// エントリーポイント
// アプリケーションはこの関数から進行する
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// メモリリークを知らせる
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// COM初期化
	if (FAILED(CoInitializeEx(nullptr, COINIT_MULTITHREADED)))
	{
		CoUninitialize();

		return 0;
	}

	// mbstowcs_s関数で日本語対応にするために呼ぶ
	setlocale(LC_ALL, "japanese");

	//===================================================================
	// 実行
	//===================================================================
	Application::Instance().Execute();

	// COM解放
	CoUninitialize();

	return 0;
}

void Application::Event()
{
	msp_scene->Event();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション更新の前処理
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::PreUpdate()
{
	// 入力状況の更新
	KdInputManager::Instance().Update();

	KdShaderManager::Instance().WorkAmbientController().PreUpdate();

	msp_scene->PreUpdate();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション更新
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::Update()
{
	msp_scene->Update();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション更新の後処理
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::PostUpdate()
{
	// 3DSoundListnerの行列を更新
	KdAudioManager::Instance().SetListnerMatrix(KdShaderManager::Instance().GetCameraCB().mView.Invert());

	msp_scene->PostUpdate();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション描画の前処理
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::PreDraw()
{
	KdDirect3D::Instance().ClearBackBuffer();

	KdShaderManager::Instance().WorkAmbientController().PreDraw();

	KdShaderManager::Instance().m_postProcessShader.PreDraw();

	msp_scene->PreDraw();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション描画
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::Draw()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光を遮るオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_HD2DShader.BeginGenerateDepthMapFromLight();
	{
	}
	KdShaderManager::Instance().m_HD2DShader.EndGenerateDepthMapFromLight();

	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のあるオブジェクト(不透明な物体や2Dキャラ)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_HD2DShader.BeginLit();
	{
		msp_scene->DrawLit();
	}
	KdShaderManager::Instance().m_HD2DShader.EndLit();


	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 陰影のないオブジェクト(透明な部分を含む物体やエフェクト)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_HD2DShader.BeginUnLit();
	{
	}
	KdShaderManager::Instance().m_HD2DShader.EndUnLit();


	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 光源オブジェクト(自ら光るオブジェクトやエフェクト)はBeginとEndの間にまとめてDrawする
	KdShaderManager::Instance().m_postProcessShader.BeginBright();
	{
	}
	KdShaderManager::Instance().m_postProcessShader.EndBright();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション描画の後処理
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::PostDraw()
{
	// 画面のぼかしや被写界深度処理の実施
	KdShaderManager::Instance().m_postProcessShader.PostEffectProcess();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// 2Dスプライトの描画
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::DrawSprite()
{
	// ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== ===== =====
	// 2Dの描画はこの間で行う
	KdShaderManager::Instance().m_spriteShader.Begin();
	{
		msp_scene->DrawSprite();
	}
	KdShaderManager::Instance().m_spriteShader.End();
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション初期設定
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
bool Application::Init(int w, int h)
{
	//===================================================================
	// ウィンドウ作成
	//===================================================================
	if (m_window.Create(w, h, "Melty Notes", "Window") == false) {
		MessageBoxA(nullptr, "ウィンドウ作成に失敗", "エラー", MB_OK);
		return false;
	}

	//===================================================================
	// フルスクリーン確認
	//===================================================================
	bool bFullScreen = false;
	/*if (MessageBoxA(m_window.GetWndHandle(), "フルスクリーンにしますか？", "確認", MB_YESNO | MB_ICONQUESTION | MB_DEFBUTTON2) == IDYES) {
		bFullScreen = true;
	}*/

	//===================================================================
	// Direct3D初期化
	//===================================================================

	// デバイスのデバッグモードを有効にする
	bool deviceDebugMode = false;
#ifdef _DEBUG
	deviceDebugMode = true;
#endif

	// Direct3D初期化
	std::string errorMsg;
	if (KdDirect3D::Instance().Init(m_window.GetWndHandle(), w, h, deviceDebugMode, errorMsg) == false) {
		MessageBoxA(m_window.GetWndHandle(), errorMsg.c_str(), "Direct3D初期化失敗", MB_OK | MB_ICONSTOP);
		return false;
	}

	// フルスクリーン設定
	if (bFullScreen) {
		KdDirect3D::Instance().WorkSwapChain()->SetFullscreenState(TRUE, 0);
	}

	//::::::::::::::::::::::::::::::::::::::::::::
	//ImGui
	//::::::::::::::::::::::::::::::::::::::::::::
	//Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	//Setup Dear ImGui style
	ImGui::StyleColorsDark();
	//ImGui::StyleColorsClassic();
	//Setup Platform/Renderer bindings
	ImGui_ImplWin32_Init(m_window.GetWndHandle());
	ImGui_ImplDX11_Init(KdDirect3D::Instance().WorkDev(), KdDirect3D::Instance().WorkDevContext());

#include "ImGui/ja_glyph_ranges.h"
	ImGuiIO& io = ImGui::GetIO();
	ImFontConfig config;
	config.MergeMode = true;
	io.Fonts->AddFontDefault();
	//日本語対応
	io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\msgothic.ttc", 13.0f, &config, glyphRangesJapanese);

	//===================================================================
	// シェーダー初期化
	//===================================================================
	KdShaderManager::Instance().Init();

	//===================================================================
	// サウンド初期化
	//===================================================================
	KdAudioManager::Instance().Init();

	//SceneManager
	msp_scene = std::make_shared<SceneManager>();
	msp_scene->Init();

	return true;
}

// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
// アプリケーション実行
// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// ///// /////
void Application::Execute()
{
	//KdCSVData windowData("Asset/Data/WindowSettings.csv");
	//const std::vector<std::string>& sizeData = windowData.GetLine(0);

	////===================================================================
	//// 初期設定(ウィンドウ作成、Direct3D初期化など)
	////===================================================================
	//if (Application::Instance().Init(atoi(sizeData[0].c_str()), atoi(sizeData[1].c_str())) == false) {
	//	return;
	//}
	if (Application::Instance().Init(Constant::WINDOW_WIDTH, Constant::WINDOW_HEIGHT) == false) {
		return;
	}

	//===================================================================
	// ゲームループ
	//===================================================================

	// 時間
	//m_fpsController.Init();
	m_timer.Start(mi_fps);	//mi_fpsフレームで動くようにする

	// ループ
	while (1)
	{
		// 処理開始時間Get
		//m_fpsController.UpdateStartTime();

		// ゲーム終了指定があるときはループ終了
		if (m_endFlag)
		{
			break;
		}

		//=========================================
		//
		// ウィンドウ関係の処理
		//
		//=========================================

		// ウィンドウのメッセージを処理する
		m_window.ProcessMessage();

		// ウィンドウが破棄されてるならループ終了
		if (m_window.IsCreated() == false)
		{
			break;
		}
		//前ループからの経過時間と設定FPS値を計算して何回描画ループを行うかを計算する
		int loop = m_timer.Run();
		for (int i = 0; i < loop; i++)
		{
			Event();
		}

		//ImGui開始
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		//ImGui Demo ウィンドウ表示　※すごく参考になるウィンドウらしい imgui_demo.cpp参照
		//ImGui::ShowDemoWindow(nullptr);

		//ウィンドウ開始 "WindowName"の部分はほかのウィンドウと被らないようにすること
/*		
		if (ImGui::Begin("MainDebug"))
		{
			//ウィンドウを折りたたんでいない状態だと、この中が実行される。
			//ここにボタン等を配置
			if (ImGui::Button(u8"おっけー"))
			{
				//ボタンを押したとき
			}
			if (ImGui::Button("abc"))
			{
				//ボタンを押したとき
			}
			ImGui::SameLine();	//次のものを同じ行にしたいとき
			if (ImGui::Button("kobe"))
			{
				//ボタンを押したとき
			}

			//文字表示のみ
			ImGui::Text("kobe denshi !");

			//ラベル付き文字表示
			ImGui::LabelText("FPS", "FPS:%d", mi_fps);
			ImGui::LabelText("NowFPS", "Now:%d", m_timer.GetNowFPS());

			//入力ボックス
			std::string str = "";	//ここに文字列が入る
			ImGui::InputText("Name", &str);
			//その他、色付きの文字列用のコントロールもあります。
		}
		ImGui::End();
		//*/
				//=========================================
				//
				// アプリケーション更新処理
				//
				//=========================================
		PreUpdate();

		Update();

		PostUpdate();

		//=========================================
		//
		// アプリケーション描画処理
		//
		//=========================================
		PreDraw();

		Draw();

		PostDraw();

		DrawSprite();

		//ImGuiのレンダリング：ここより上にImGuiの描画はすること
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		// BackBuffer -> 画面表示
		KdDirect3D::Instance().WorkSwapChain()->Present(0, 0);

		//=========================================
		//
		// フレームレート制御
		//
		//=========================================



	//m_fpsController.Update();
		//タイトルバーにFPSを表示
		std::string title = "Melty Notes  FPS : " + std::to_string( m_timer.GetNowFPS() );
		SetWindowTextA( m_window.GetWndHandle(), title.c_str() );
	}

	//===================================================================
	// アプリケーション解放
	//===================================================================
	Release();
}

// アプリケーション終了
void Application::Release()
{
	KdInputManager::Instance().Release();

	KdShaderManager::Instance().Release();

	KdAudioManager::Instance().Release();

	//ImGui解放
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();

	KdDirect3D::Instance().Release();

	// ウィンドウ削除
	m_window.Release();
}