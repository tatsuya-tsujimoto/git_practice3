#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include <Windows.h>
#include "Engine/Engine.h"
#include "Common/Vec.h"

#include<math.h>

Vec2 g_Position = Vec2(0.0f, 0.0f);
Vec2 g_Scale = Vec2(1.0f, 1.0f);
float g_Angle = 0.0f;

static float player_pos_x = 100.0f;
static float player_pos_y = 100.0f;
static float enemy_pos_x = 300.0f;
static float enemy_pos_y = 200.0f;
const float X_posmax = 640.0f;
const float Y_posmax = 480.0f;
const float X_posmin = 0.0f;
const float Y_posmin = 0.0f;
const float pmove = 3.0f;
const float emove = 1.0f;
static float radian;

// ゲーム処理
void GameProcessing();
// 描画処理
void DrawProcessing();

float Pai();

float Angle(float r);



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


	Engine::LoadTexture("Enemy", "Res/Enemies/EA1.PNG");
	Engine::LoadTexture("Robo", "Res/Robot/Robot_idle 1.PNG");

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

	if (Engine::IsKeyboardKeyHeld(DIK_UP) == true)
	{
		player_pos_y -= pmove;
		if (player_pos_y < Y_posmin)
			player_pos_y = Y_posmin;
	}

	if (Engine::IsKeyboardKeyHeld(DIK_DOWN) == true)
	{
		player_pos_y += pmove;
		if (player_pos_y > Y_posmax - Engine::GetTexture("Robo")->Height)
			player_pos_y = Y_posmax - Engine::GetTexture("Robo")->Height;
	}

	if (Engine::IsKeyboardKeyHeld(DIK_RIGHT) == true)
	{
		player_pos_x += pmove;
		if (player_pos_x > X_posmax - Engine::GetTexture("Robo")->Width)
			player_pos_x = X_posmax - Engine::GetTexture("Robo")->Width;
	}

	if (Engine::IsKeyboardKeyHeld(DIK_LEFT) == true)
	{
		player_pos_x -= pmove;
		if (player_pos_x < X_posmin)
			player_pos_x = X_posmin;
	}


	///////////////////////////

	
	float tmp_wid = (player_pos_x - enemy_pos_x) * (player_pos_x - enemy_pos_x);
	float tmp_hei = (player_pos_y - enemy_pos_y) * (player_pos_y - enemy_pos_y);
	float y = sqrt(tmp_hei);
	float x = sqrt(tmp_wid);
	float tmp_dia = tmp_hei + tmp_wid;
	float diagonal = sqrt(tmp_dia);
	radian = asin(y / diagonal);
	float Y_increment = y / diagonal;
	float X_increment = x / diagonal;



	if (player_pos_x > enemy_pos_x)
		enemy_pos_x += emove * X_increment;
	else
		enemy_pos_x -= emove * X_increment;


	if (player_pos_y > enemy_pos_y)
		enemy_pos_y += emove * Y_increment;
	else
		enemy_pos_y -= emove * Y_increment;
	


}

void DrawProcessing()
{
	// 描画開始
	// 描画処理を実行する場合、必ず最初実行する
	Engine::StartDrawing(0);


	Engine::DrawTexture(player_pos_x, player_pos_y, "Robo", 255, 0.0f,  1.0f,  1.0f);
	Engine::DrawTexture(enemy_pos_x, enemy_pos_y, "Enemy", 255, Angle(radian), 1.0f, 1.0f);



	// 描画終了
	// 描画処理を終了する場合、必ず最後に実行する
	Engine::FinishDrawing();
}




float Pai()
{
	return atan(1.0) * 4.0f;
}

float Angle(float r)		//enemyの角度
{
	
	if (player_pos_x > enemy_pos_x)
	{

		if (player_pos_y == enemy_pos_y)				//y = 0
		{
			return 180.0f;
		}
		else if (player_pos_y > enemy_pos_y)			//Enemyが第二象限
		{
			return (r * 180.0f / Pai()) + 180.0f;
		}
		else											//Enemyが第三象限
		{
			return ((Pai() / 2 - r) * 180.0f / Pai()) + 90.0f;
			
		}
	}
	else if(player_pos_x < enemy_pos_x)
	{
		if (player_pos_y == enemy_pos_y)				//y = 0
		{
			return 0.0f;
		}
		else if (player_pos_y > enemy_pos_y)			//Enemyが第一象限
		{
			return ((Pai() / 2 - r) * 180.0f / Pai()) + 270.0f;
		}
		else											//Enemyが第四象限
		{
			return r * 180.0f / Pai();
		}
	}
	else if (player_pos_x == enemy_pos_x)				//x = 0
	{
		if (player_pos_y > enemy_pos_y)
		{
			return 90.0f;
		}
		else if (player_pos_y < enemy_pos_y)
		{
			return 270.0f;
		}
		else											//判定重なり
			return 0.0f;
	}
}