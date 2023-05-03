#pragma once

#include "template.h"


struct MousePacket
{
	void Reset()
	{			
		m_was_pressed = false;
		m_was_released = false;
	}
				
	Tmpl8::vec2 m_position{ 0.0f, 0.0f };

	bool m_was_pressed{ false };
	bool m_was_released{ false };

	bool m_is_pressed{ false };
	bool m_is_just_pressed{ false };
	bool m_is_just_released{ false };
};


class MouseManager
{
public:
	MouseManager() { };

	void Update();

	void Pressed(int SDL_code);
	void Released(int SDL_code);
	void SetPosition(int x, int y);

	bool IsPressed();
	bool IsJustPressed();
	bool IsJustReleased();
	Tmpl8::vec2& GetPosition();

private:
	MousePacket m_mouse_packet;

};