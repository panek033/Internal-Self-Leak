#pragma once
#include "vector.h"

namespace settings {
	namespace bones
	{
		bool head = false;
		bool neck = true;
	}

	static bool spotify = false;

	namespace aimbot {

		bool enable = true;
		int aim_key = VK_MENU;
		bool interpolate = false;
		bool mouse = false;
		bool constant = false;
		bool draw_fov = false;
		bool visible_only = false;
		float mouse_speed = 15;
		float interpolate_speed = 5;
		float field_of_view = 10;
	}

	namespace player {
		bool enable = true;
		bool skeleton = false;
		bool box = true;
		bool normal_box = false;
		bool cornered_box = false;
		bool snapline = false;
		bool player_name = false;
		bool active_weapon = false;
		bool display_distance = false;
		float max_distance = 300;
	}

	namespace style {
		bool text_outlined = true;
		bool performance = false;  //false


	}
}
