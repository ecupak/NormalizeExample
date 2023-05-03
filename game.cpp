#include <cstdio> //printf
#include <vector>
#include <string>
#include <math.h>

#include "game.h"

// -----------------------------------------------------------
// Constructor
// -----------------------------------------------------------
Game::Game(Tmpl8::Surface* surface) 
	: screen{ surface }
{	}


// -----------------------------------------------------------
// Initialize the application
// -----------------------------------------------------------	
void Game::Init()
{
		
}


// -----------------------------------------------------------
// Close down application
// -----------------------------------------------------------
void Game::Shutdown()
{
	
}


// -----------------------------------------------------------
// Main application tick function
// -----------------------------------------------------------	
void Game::Tick(float deltaTime)
{
	screen->Clear(0x2a2a2a);

	deltaTime = Tmpl8::Clamp(deltaTime * 0.001f, 0.0f, 0.033f);

	// Get user IO.
	m_mouse_manager.Update();
	m_keyboard_manager.Update();

	// Check if Y mode was changed.
	YModeCheck();

	// Draw diagrams.
	Tmpl8::vec2 center{ 200, 300 };
	m_player_pos = center;

	DrawPartA(center);

	screen->Line(400, 0, 400, ScreenHeight, 0xFFFFFF);
	center.x += 400;
	center.y -= 200;

	DrawPartB(center);
}


void Game::YModeCheck()
{
	if (m_keyboard_manager.IsJustPressed(SDL_SCANCODE_W))
	{
		m_mode = m_mode == Mode::Y_ADD ? Mode::Y_SUB : Mode::Y_ADD;
	}
}


void Game::DrawPartA(Tmpl8::vec2& center)
{	
	// Player.
	screen->Box(m_player_pos.x - 1, m_player_pos.y - 1, m_player_pos.x + 1, m_player_pos.y + 1, 0x00FFFF);

	// 100 width circle.
	int radius{ 100 };
	for (int angle{ 0 }; angle < 360; ++angle)
	{
		float radian{ 180.0f / angle };
		screen->Plot(
			center.x + radius * cos(angle),
			center.y - radius * sin(angle),
			0xFF0000
		);
	}

	// Vector.
	Tmpl8::vec2& pos{ m_mouse_manager.GetPosition() };
	screen->Line(m_player_pos.x, m_player_pos.y, pos.x, m_player_pos.y, m_x_color);
	screen->Line(pos.x, m_player_pos.y, pos.x, pos.y, m_y_color);
	screen->Line(m_player_pos.x, m_player_pos.y, pos.x, pos.y, m_h_color);
	

	// Output.
	int line_gap{ 10 };
	int line_y{ line_gap };
	
	screen->Print("Player", 10, line_y, 0x00FFFF);
	std::string display = std::to_string(static_cast<int>(m_player_pos.x));
	display += ", ";
	display += std::to_string(static_cast<int>(m_player_pos.y));
	screen->Print(&display[0], 50, line_y, 0xFFFFFF);

	line_y += line_gap;
	line_y += line_gap;
	screen->Print("X", 10, line_y, m_x_color);
	display = std::to_string(static_cast<int>(pos.x));	
	screen->Print(&display[0], 20, line_y, 0xFFFFFF);

	line_y += line_gap;
	screen->Print("Y", 10, line_y, m_y_color);
	display = std::to_string(static_cast<int>(pos.y));
	screen->Print(&display[0], 20, line_y, 0xFFFFFF);
}


void Game::DrawPartB(Tmpl8::vec2& center)
{
	// Axis.
	screen->Line(center.x, 0, center.x, ScreenHeight, 0x080808);
	screen->Line(401, center.y, ScreenWidth, center.y, 0x080808);

	// 100 width circle.
	int radius{ 100 };
	for (int angle{ 0 }; angle < 360; ++angle)
	{
		float radian{ 180.0f / angle };
		screen->Plot(
			center.x + radius * cos(angle),
			center.y - radius * sin(angle),
			0x210000
		);
	}

	// Non-normalized vector.
	Tmpl8::vec2& pos{ m_mouse_manager.GetPosition() };
	float x_comp{ pos.x - m_player_pos.x };
	float y_comp{ pos.y };
	std::string mode_name{ "W: " };

	if (m_mode == Mode::Y_ADD)
	{
		y_comp += m_player_pos.y;
		mode_name += "Y ADDED";
	}
	else
	{
		y_comp -= m_player_pos.y;
		mode_name += "Y SUBTRACTED";
	}

	// Vector.
	screen->Line(center.x, center.y, center.x + x_comp, center.y + y_comp, m_h_color);
	screen->Line(center.x, center.y, center.x + x_comp, center.y, m_x_color);
	screen->Line(center.x + x_comp, center.y, center.x + x_comp, center.y + y_comp, m_y_color);

	// Find normalized vector components manually.
	float h{ sqrtf((x_comp * x_comp) + (y_comp * y_comp)) };
	float x_h{ 0.0f };
	float y_h{ 0.0f };
	if (h != 0.0f)
	{
		x_h = x_comp / h;
		y_h = y_comp / h;
	}


	// Output.
	int line_gap{ 10 };
	int line_y{ line_gap };
	
	screen->Print(&mode_name[0], 410, line_y, 0x00FFFF);

	line_y += line_gap;
	line_y += line_gap;
	screen->Print("Magnitude", 410, line_y, m_h_color);
	std::string display = std::to_string(h);
	screen->Print(&display[0], 470, line_y, 0xFFFFFF);

	line_y += line_gap;
	screen->Print("Non-normalized X", 410, line_y, m_x_color);
	display = std::to_string(static_cast<int>(x_comp));
	screen->Print(&display[0], 510, line_y, 0xFFFFFF);

	line_y += line_gap;
	screen->Print("Non-normalized Y", 410, line_y, m_y_color);
	display = std::to_string(static_cast<int>(y_comp));
	screen->Print(&display[0], 510, line_y, 0xFFFFFF);

	line_y += line_gap;
	line_y += line_gap;
	screen->Print("Normalized X", 410, line_y, m_x_color);
	display = std::to_string(x_h);
	screen->Print(&display[0], 490, line_y, 0xFFFFFF);

	line_y += line_gap;
	screen->Print("Normalized Y", 410, line_y, m_y_color);
	display = std::to_string(y_h);
	screen->Print(&display[0], 490, line_y, 0xFFFFFF);
}


// -----------------------------------------------------------
// Mouse events
// -----------------------------------------------------------
void Game::MouseDown(int button)
{
	m_mouse_manager.Pressed(button);
}


void Game::MouseUp(int button)
{
	m_mouse_manager.Released(button);
}	


void Game::MouseMove(int x, int y)
{
	m_mouse_manager.SetPosition(x, y);
}


// -----------------------------------------------------------
// Keyboard events
// -----------------------------------------------------------

/* Register key press only if the key was not already being pressed
	(must register key release first). */

void Game::KeyDown(int key_code)
{
	m_keyboard_manager.Pressed(key_code);
}

void Game::KeyUp(int key_code)
{
	m_keyboard_manager.Released(key_code);
}