#pragma once

#include "template.h"
#include "surface.h"

#include "mouse_manager.h"
#include "keyboard_manager.h"

enum class Mode
{
	Y_ADD,
	Y_SUB,
};

class Game
{
public:
	Game(Tmpl8::Surface* surface);
		
	void Init();
	void Shutdown();
	void Tick(float deltaTime);
	void MouseUp(int button);
	void MouseDown(int button);
	void MouseMove(int x, int y);
	void KeyUp(int key_code);
	void KeyDown(int key_code);

private:
	void YModeCheck();
	void DrawPartA(Tmpl8::vec2& center);
	void DrawPartB(Tmpl8::vec2& center);

	KeyboardManager m_keyboard_manager;
	MouseManager m_mouse_manager;
	Tmpl8::Surface* screen{ nullptr };

	Tmpl8::vec2 m_player_pos{ 0, 0 };
	int m_x_color{ 0xFFFF00 };
	int m_y_color{ 0x00FF00 };
	int m_h_color{ 0xAAAAFF };
	Mode m_mode{ Mode::Y_ADD };
};