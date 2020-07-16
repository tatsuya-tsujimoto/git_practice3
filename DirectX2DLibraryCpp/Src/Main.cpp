#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"
#include<time.h>

Vec2 g_Position = Vec2(0.0f, 0.0f);
Vec2 g_Scale = Vec2(1.0f, 1.0f);
float g_Angle = 0.0f;

//DWORD g_StartTime = timeGetTime();
//DWORD g_ElapsedTime = 0;

int g_hand = -1;
int g_enemyhand;
int g_timeframe = 0;
int g_CheatCount = 0;
const int hand_rock = 0;
const int hand_scisors = 1;
const int hand_paper = 2;
const int RPSwin = 0;						//勝敗判定：勝利
const int RPSdraw = 1;						//			引き分け
const int RPSlose = 2;						//			敗北
const int CheatCountFrame = 300;			//裏技実行Frame
const int ContinueAdjustment = 5;			//相手の手の連続描画調整
const int Texture_transparent = 255;		//描画処理透過
const float Texture_x_scale = 0.3f;			//		　X軸拡縮
const float Texture_y_scale = 0.3f;			//		　Y軸拡縮
const float Texture_rotate = 0.0f;			//		　回転
const float Phand_x_position = 3.0f;		//		　X座標（Player）
const float Phand_y_position = 20.0f;		//		　Y座標（Player）
const float Ehand_x_position = 0.0f;		//		　X座標（CPU）
const float Ehand_y_position = 20.0f;		//		　Y座標（CPU）
const float Font_x_position = 0.0f;			//		　X座標（Font）
const float Font_y_position = 30.0f;		//		　Y座標（Font）
//const float Result_x_position;
//const float Result_y_position;


// ゲーム処理
void GameProcessing();
// 描画処理
void DrawProcessing();

int RPSjudge(int g_hand,int g_enemyhand);		//じゃんけんの勝敗判定

void EnemyHandDraw();							//相手の手の描画

void PlayerHandDraw();							//自分の手の描画

void ResultDraw(int RPSjudge);					//結果の描画

void InEnemyHandDraw(int count);				//相手の手の描画の簡素化

void JankenPhandDraw(const char* keyword);

void JankenEhandDraw(const char* keyword);

int WINAPI WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmpLine,
	_In_ INT       nCmdShow)
{
	// エンジンの初期化
	// ゲームループ開始前に1度だけ実行する
	if (Engine::Initialize(640, 480, "Sample") == false)
	{
		return 0;
	}

	Engine::LoadTexture("rock", "Res/janken_gu.png");
	Engine::LoadTexture("scisors", "Res/janken_choki.png");
	Engine::LoadTexture("paper", "Res/janken_pa.png");
	Engine::LoadTexture("question", "Res/mark_question.png");

	srand((unsigned)time(NULL));
	g_enemyhand = rand() % 3;

	while (true)
	{
		bool message_ret = false;
		MSG msg;

		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else 
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			//g_ElapsedTime = timeGetTime() - g_StartTime;
			g_timeframe++;
			// ゲーム処理
			GameProcessing();

			// 描画開始
			DrawProcessing();
		}
	}

	// エンジン終了
	// ゲームループ終了後に1度だけ実行する
	// テクスチャとサウンドファイルも全て解放する
	Engine::Release();

	// メモリリーク検出
	_CrtDumpMemoryLeaks();

	return 0;
}
void GameProcessing()
{
	// 入力データの更新
	Engine::Update();


	//========================================================
	// キーボードの入力取得
	//========================================================
	
	if ((Engine::IsKeyboardKeyHeld(DIK_0) == true || Engine::IsKeyboardKeyHeld(DIK_1) == true || Engine::IsKeyboardKeyHeld(DIK_2)) == true)
	{
		g_CheatCount++;
	}

	//通常時の押下判定
	if (Engine::IsKeyboardKeyReleased(DIK_0) == true && g_CheatCount <= CheatCountFrame)
	{
		g_hand = hand_rock;
	}
	else if (Engine::IsKeyboardKeyReleased(DIK_1) == true && g_CheatCount <= CheatCountFrame)
	{
		g_hand = hand_scisors;
	}
	else if (Engine::IsKeyboardKeyReleased(DIK_2) == true && g_CheatCount <= CheatCountFrame)
	{
		g_hand = hand_paper;
	}
	
	//裏技時の押下判定
	if (Engine::IsKeyboardKeyPushed(DIK_0) == true && g_CheatCount > CheatCountFrame)
	{
		g_hand = hand_rock;
	}
	else if (Engine::IsKeyboardKeyPushed(DIK_1) == true && g_CheatCount > CheatCountFrame)
	{
		g_hand = hand_scisors;
	}
	else if (Engine::IsKeyboardKeyPushed(DIK_2) == true && g_CheatCount > CheatCountFrame)
	{
		g_hand = hand_paper;
	}


	/*
	if (Engine::IsKeyboardKeyPushed(DIK_0) == true)
	{
		g_hand = 0;
	}
	else if (Engine::IsKeyboardKeyPushed(DIK_1) == true)
	{
		g_hand = 1;
	}
	else if (Engine::IsKeyboardKeyPushed(DIK_2) == true)
	{
		g_hand = 2;
	}*/
	//RPSjudge(g_hand,g_enemyhand);

	//if (Engine::IsKeyboardKeyHeld(DIK_LEFT) == true)
	//{
	//	// キーが押されている間の処理
	//}

	//if (Engine::IsKeyboardKeyReleased(DIK_A))
	//{
	//	// キーが離された瞬間の処理
	//}
}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);
	
	PlayerHandDraw();
	EnemyHandDraw();
	

	/*
	// フォント描画
	Engine::DrawFont(0.0f, 0.0f, "FontSize:Small", FontSize::Small, FontColor::White);
	Engine::DrawFont(0.0f, 30.0f, "FontSize:Regular", FontSize::Regular, FontColor::White);
	Engine::DrawFont(0.0f, 60.0f, "FontSize:Large", FontSize::Large, FontColor::White);
	*/
	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}


int RPSjudge(int g_hand, int g_enemyhand)
{
	if (g_hand == g_enemyhand)
	{
		return RPSdraw;//Draw
	}
	if (((g_hand + 3) - g_enemyhand) % 3 == 1)
	{
		return RPSlose;//CPUwin
	}
	else 
	{
		return RPSwin;//Playerwin
	}
}

void EnemyHandDraw()
{
	if (g_CheatCount > CheatCountFrame)
	{
		InEnemyHandDraw(g_enemyhand);
	}
	if (g_hand == -1 &&g_CheatCount<= CheatCountFrame) {
		InEnemyHandDraw((g_timeframe / ContinueAdjustment) % 3);
	}
	else if (g_hand >= 0)
	{
		InEnemyHandDraw(g_enemyhand);
	}

}


void PlayerHandDraw()
{
	if (g_hand == -1) {
		
		Engine::DrawTexture(Engine::GetTexture("question")->Width - Phand_x_position, Engine::GetTexture("question")->Height / Phand_y_position, "question", Texture_transparent, Texture_rotate, Texture_x_scale, Texture_y_scale);
		Engine::DrawFont(Font_x_position, 30.0f, "0:グー,1:チョキ,2:パーから出す手を選択してください", FontSize::Regular, FontColor::White);
	}
	else if (g_hand >= 0)
	{
		switch (g_hand)
		{
		case hand_rock   :JankenPhandDraw("rock"); break;
		case hand_scisors:JankenPhandDraw("scisors"); break;
		case hand_paper  :JankenPhandDraw("paper"); break;
		}
		ResultDraw(RPSjudge(g_hand, g_enemyhand));
	}
}

void ResultDraw(int RPSjudge)
{
	switch (RPSjudge)
	{
	case RPSwin:
		Engine::DrawFont(Font_x_position, Font_y_position, "勝利", FontSize::Regular, FontColor::White);
		break;
	case RPSdraw:
		Engine::DrawFont(Font_x_position, Font_y_position, "引き分け", FontSize::Regular, FontColor::White);
		break;
	case RPSlose:
		Engine::DrawFont(Font_x_position, Font_y_position, "敗北", FontSize::Regular, FontColor::White);
		break;
	default:break;
	}
}

void InEnemyHandDraw(int count)
{
	switch (count)
	{
	case hand_rock   :JankenEhandDraw("rock"); break;
	case hand_scisors:JankenEhandDraw("scisors"); break;
	case hand_paper:JankenEhandDraw("paper"); break;
	}
}

void JankenPhandDraw(const char* keyword)
{
	Engine::DrawTexture(Engine::GetTexture(keyword)->Width / Phand_x_position, Engine::GetTexture(keyword)->Height / Phand_y_position, keyword, Texture_transparent, Texture_rotate, Texture_x_scale, Texture_y_scale);
}

void JankenEhandDraw(const char* keyword)
{
	Engine::DrawTexture(Engine::GetTexture(keyword)->Width * Ehand_x_position, Engine::GetTexture(keyword)->Height / Ehand_y_position, keyword, Texture_transparent, Texture_rotate, Texture_x_scale, Texture_y_scale);

}