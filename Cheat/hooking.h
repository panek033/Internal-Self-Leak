#pragma once
#include <Windows.h>
#include "settings.h"
#include "../XorString.h"
#include "../font/icons.h"
#include "../font/font.h"
#include "../XorString.h"
#include "../auth.hpp"


//KEY AUTH


using namespace KeyAuth;


std::string name = XorString(""); //Application name found in application settings
std::string ownerid = XorString(""); //Owner ID Found in user settings
std::string secret = XorString(""); //Application secret found in Application settings
std::string version = XorString("1.9"); // Version can be changed but is not really important
std::string url = "https://keyauth.win/api/1.1/"; // change if you're self-hosting
std::string sslPin = "ssl pin key (optional)"; // don't change unless you intend to pin public certificate key. you can get here in the "Pin SHA256" field https://www.ssllabs.com/ssltest/analyze.html?d=ke
api KeyAuthApp(name, ownerid, secret, version, url, sslPin);

char Licence[50] = "";

// MAIN

bool in_area(fvector2d target, fvector2d pos, fvector2d size)
{
	if (target.x > pos.x && target.y > pos.y)
		if (target.x < pos.x + size.x && target.y < pos.y + size.y)
			return true;

	return false;
}

inline ImFont* bigFont;
namespace husisudfr {

	namespace Styles {
		void InitStyle()
		{
			ImGuiStyle& style = ImGui::GetStyle();
			ImVec4* colors = ImGui::GetStyle().Colors;

			//	STYLE PROPERTIES
			style.WindowTitleAlign = ImVec2(0.5f, 0.5f);

			//  Base ImGui Styling , Aplying a custyom style is left up to you.
			ImGui::StyleColorsClassic();

			/// YOUR DEFAULT STYLE PROPERTIES HERE
			colors[ImGuiCol_Separator] = ImVec4(1.f, 1.f, 1.f, 1.f);
			colors[ImGuiCol_Text] = ImVec4(1.f, 1.f, 1.f, 1.f);
			colors[ImGuiCol_TitleBg] = ImVec4(0, 0, 0, 1.0f);
			colors[ImGuiCol_TitleBgActive] = ImVec4(0, 0, 0, 1.0f);
			colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0, 0, 0, 1.0f);


			colors[ImGuiCol_Button] = ImVec4(0, 0, 0, 240);
			colors[ImGuiCol_ButtonHovered] = ImVec4(0, 0, 0, 240);
			colors[ImGuiCol_ButtonActive] = ImVec4(0, 0, 0, 240);

		}
	}

	void Menu::Draw()
	{
		Menu::Waesfg();
		
		if (g_GameVariables->m_ShowMenu) {
			if(settings::spotify){ 
			Menu::BackgroundMenu(); 
			}

			MainMenu();
		}
	}

	void Menu::BackgroundMenu()
	{
		if (!g_GameVariables->m_ShowDemo)
			Styles::InitStyle();

		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg] = ImVec4(0, 0, 0, 0.275f);
		colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0.3f);

		ImGui::SetNextWindowSize(ImVec2(1920, 1080));
		ImGui::SetNextWindowPos(ImVec2(0, 0));
		if (!ImGui::Begin("Bg", &g_GameVariables->m_ShowMenu, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoInputs | ImGuiWindowFlags_NoMove))
		{
			ImGui::End();
			return;
		}
		ImDrawList* drawList = ImGui::GetWindowDrawList();

		//particle properties
		static const int numParticles = 275;
		static ImVec2 particlePositions[numParticles];
		static ImVec2 particleDistance;
		static ImVec2 particleVelocities[numParticles];
		//Particle code
		static bool initialized = false;
		if (!initialized) {
			for (int i = 0; i < numParticles; ++i) {
				particlePositions[i] = ImVec2(
					ImGui::GetWindowPos().x + ImGui::GetWindowSize().x * static_cast<float>(rand()) / RAND_MAX,
					ImGui::GetWindowPos().y + ImGui::GetWindowSize().y * static_cast<float>(rand()) / RAND_MAX);
				particleVelocities[i] = ImVec2(static_cast<float>((rand() % 1) - 19), static_cast<float>((rand() % 5) - 25));
			}
			initialized = true;
		}
		ImVec2 cursorPos = ImGui::GetIO().MousePos;
		for (int i = 0; i < numParticles; ++i)
		{
			for (int j = i + 1; j < numParticles; ++j) {
				float distance = std::hypotf(particlePositions[j].x - particlePositions[i].x, particlePositions[j].y - particlePositions[i].y);
				float opacity = 1.0f - (distance / 85.0f);  // opacity cahnge
				if (opacity > 0.0f) {
					ImU32 lineColor = ImGui::GetColorU32(ImVec4(0.0f, 0.0f, 0.0f, opacity));
					drawList->AddLine(particlePositions[i], particlePositions[j], lineColor);
				}
			}
			float distanceToCursor = std::hypotf(cursorPos.x - particlePositions[i].x, cursorPos.y - particlePositions[i].y);
			float opacityToCursor = 1.0f - (distanceToCursor / 52.0f);  // Adjust the divisor to control the opacity change
			if (opacityToCursor > 0.0f) {
				ImU32 lineColorToCursor = ImGui::GetColorU32(ImVec4(1.0f, 1.0f, 1.0f, opacityToCursor));
				drawList->AddLine(cursorPos, particlePositions[i], lineColorToCursor);
			}
		}
		float deltaTime = ImGui::GetIO().DeltaTime;
		for (int i = 0; i < numParticles; ++i)
		{
			particlePositions[i].x += particleVelocities[i].x * deltaTime;
			particlePositions[i].y += particleVelocities[i].y * deltaTime;

			if (particlePositions[i].x < ImGui::GetWindowPos().x)
				particlePositions[i].x = ImGui::GetWindowPos().x + ImGui::GetWindowSize().x;
			else if (particlePositions[i].x > ImGui::GetWindowPos().x + ImGui::GetWindowSize().x)
				particlePositions[i].x = ImGui::GetWindowPos().x;

			if (particlePositions[i].y < ImGui::GetWindowPos().y)
				particlePositions[i].y = ImGui::GetWindowPos().y + ImGui::GetWindowSize().y;
			else if (particlePositions[i].y > ImGui::GetWindowPos().y + ImGui::GetWindowSize().y)
				particlePositions[i].y = ImGui::GetWindowPos().y;

			ImU32 particleColour = ImGui::ColorConvertFloat4ToU32(ImVec4(g_Menu->dbg_RAINBOW));

			drawList->AddCircleFilled(particlePositions[i], 1.9f, particleColour);
		}ImGui::End();
	}

	inline bool gaymodelol = false;

	void Menu::Waesfg()
	{
		if (gaymodelol)
		{
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(10, 13), g_Menu->dbg_RAINBOW, XorString("Rive Public"));
		}
		else
		{
			ImGui::GetBackgroundDrawList()->AddText(ImVec2(10, 13), ImColor(255, 255, 255, 255), XorString("Rive Public"));
		}
	}


	//CHEAT MENU
	void Menu::MainMenu()
	{
		Styles::InitStyle();

		if (gaymodelol) {
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(g_Menu->dbg_RAINBOW));
			ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(g_Menu->dbg_RAINBOW));
			ImGui::PushStyleColor(ImGuiCol_BorderShadow, ImVec4(g_Menu->dbg_RAINBOW));
		}




		if (g_Menu->dbg_RAINBOW_THEME) {
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
			ImGui::PopStyleColor();
		}

		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0,0,0,240));

		ImGui::SetNextWindowSize(ImVec2(580, 490));
		ImGui::Begin(XorString("Spotify"), &g_GameVariables->m_ShowMenu, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoNavFocus);
		{
			static int log = 1;
			ImGui::BeginChild("Spotify Login", ImVec2(580, 477), false, ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoScrollbar);
			{
				ImGui::SetCursorPos(ImVec2(10, 455));
				ImGui::TextDisabled("Made by @hus");

				if (log == 1)
				{
					ImGui::SetCursorPos(ImVec2(258, 20));
					ImGui::TextDisabled(ICON_FA_HOME"");



					ImGui::SetCursorPos(ImVec2(187, 35));
					ImGui::Text("Log into your account");

					ImGui::PushItemWidth(260.f);
					{
						ImGui::SetCursorPos(ImVec2(142, 79));
						ImGui::TextDisabled("Key");

						ImGui::SetCursorPos(ImVec2(140, 100));
						ImGui::InputText("  ", Licence, IM_ARRAYSIZE(Licence));
					}
					ImGui::PopItemWidth();


					ImGui::SetCursorPos(ImVec2(229, 159));
					ImGui::Text("dsc.gg/yourp2c");

					ImGui::SetCursorPos(ImVec2(142, 210));
					ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 3.f);
					if (ImGui::Button("Login", ImVec2(260.f, 30.f)))
					{
						KeyAuthApp.init();
						Sleep(500);
						//KeyAuthApp.license(Licence);
						if (!KeyAuthApp.data.success)
						{
							exit(0);
						}
						if (KeyAuthApp.data.success)
						{
							MessageBox(NULL, TEXT("Logged In !"), TEXT("       "), MB_OK);
							log = 7;	
							
						}						
					}
					ImGui::PopStyleVar();
				}
				if (log == 7)
				{
					ImVec2 Buttonsize = ImVec2(115, 45);
					ImVec2 Childsize = ImVec2(435, 450);
					static int Tab = 1;
					static int vistab = 1;

					ImGui::SetCursorPos(ImVec2(15, 30));

					float oldfsize = ImGui::GetFont()->Scale;
					ImGui::GetFont()->Scale = 1.73;
					ImGui::PushFont(ImGui::GetFont());

					ImGui::TextColored(ImColor(255, 255, 255, 255), XorString("Rive"));

					ImGui::SetCursorPos(ImVec2(75, 30));
					ImGui::Text(XorString(".vip"));

					ImGui::GetFont()->Scale = oldfsize;
					ImGui::PopFont();

					ImGui::GetFont()->Scale = oldfsize;

					ImGui::SetCursorPos(ImVec2(10, 85));
					if (ImGui::Button(ICON_FA_CROSSHAIRS"", ImVec2(115, 45)))
					{
						Tab = 1;
					}
					ImGui::SetCursorPos(ImVec2(10, 140));
					if (ImGui::Button(ICON_FA_EYE" ", ImVec2(Buttonsize)))
					{
						Tab = 2;
					}
					ImGui::SetCursorPos(ImVec2(10, 190));
					if (ImGui::Button(ICON_FA_COGS"  ", ImVec2(Buttonsize)))
					{
						Tab = 3;
					}

					ImGui::SetCursorPos(ImVec2(135, 30));
					ImGui::BeginChild(XorString("Main"), ImVec2(Childsize));
					{
						if (Tab == 1)
						{
							ImGui::Checkbox(XorString("Aimbot"), &settings::aimbot::mouse);
							ImGui::SliderFloat(XorString("Smoothing"), &settings::aimbot::mouse_speed, 8, 25);
							ImGui::Checkbox(XorString("Visible Only"), &settings::aimbot::visible_only);
							ImGui::Checkbox(XorString("Draw Fov"), &settings::aimbot::draw_fov);
							ImGui::SliderFloat(XorString("FOV"), &settings::aimbot::field_of_view, 1, 50);
							ImGui::Text(XorString("Bone"));
							ImGui::Checkbox(XorString("Head"), &settings::bones::head);
							if (settings::bones::head == true)
							{
								settings::bones::neck = false;
							}
							ImGui::SameLine();
							ImGui::Checkbox(XorString("Neck"), &settings::bones::neck);
							if (settings::bones::neck == true)
							{
								settings::bones::head = false;
							}
							fvector2d size = fvector2d{ 150, 25 };

						}
						if (Tab == 2)
						{

							ImGui::Checkbox(XorString("Box"), &settings::player::normal_box);
							ImGui::Checkbox(XorString("Skeleton"), &settings::player::skeleton);
							ImGui::Checkbox(XorString("Distance"), &settings::player::display_distance);

							ImGui::SliderFloat(XorString("Render Distance"), &settings::player::max_distance, 5, 300, "%.1f");
						}
						if (Tab == 3)
						{
							ImGui::Checkbox(XorString("Rainbow mode"), &gaymodelol);
							ImGui::Checkbox(XorString("Show Particles"), &settings::spotify);
						}
					}ImGui::EndChild();
					
				}
			}ImGui::EndChild();
		}ImGui::End();
	}
}

#define M_PI   3.01159265358979323846264338327950288

float degree_to_radian(float degree) {
	return degree * (M_PI / 180);
}

void angle_rotation(const frotator& angles, fvector* forward)
{
	float	sp, sy, cp, cy;

	sy = sin(degree_to_radian(angles.yaw));
	cy = cos(degree_to_radian(angles.yaw));

	sp = sin(degree_to_radian(angles.pitch));
	cp = cos(degree_to_radian(angles.pitch));

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

namespace Spotify {
	bool visible;
	void set_aim_mouse(aplayercontroller* controller, fvector aim_location, float width, float height, float smooth) {
		float ScreenCenterX = (width / 2);
		float ScreenCenterY = (height / 2);
		float TargetX = 0;
		float TargetY = 0;
		auto aim_screen = fvector2d();
		if (controller->w2s(aim_location, &aim_screen) && aim_screen) {
			if (aim_screen.x != 0)
			{
				if (aim_screen.x > ScreenCenterX)
				{
					TargetX = -(ScreenCenterX - aim_screen.x);
					TargetX /= smooth;
					if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = -0;
				}

				if (aim_screen.x < ScreenCenterX)
				{
					TargetX = aim_screen.x - ScreenCenterX;
					TargetX /= smooth;
					if (TargetX + ScreenCenterX < 0) TargetX = -0;
				}
			}

			if (aim_screen.y != 0)
			{
				if (aim_screen.y > ScreenCenterY)
				{
					TargetY = -(ScreenCenterY - aim_screen.y);
					TargetY /= smooth;
					if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = -0;
				}

				if (aim_screen.y < ScreenCenterY)
				{
					TargetY = aim_screen.y - ScreenCenterY;
					TargetY /= smooth;
					if (TargetY + ScreenCenterY < 0) TargetY = -0;
				}
			}

			INPUT input = {};
			input.type = INPUT_MOUSE;
			input.mi.dx = static_cast<LONG>(TargetX);
			input.mi.dy = static_cast<LONG>(TargetY);
			input.mi.dwFlags = MOUSEEVENTF_MOVE;
			SendInput(1, &input, sizeof(INPUT));
		}
	}

	

	void set_aim(uworld* world, aplayercontroller* controller, fvector camera_location, frotator camera_rotation, fvector aim_location, bool interpolate, bool constant, double speed) {
		auto aim_rotation = defines::math->find_look_at_rotation(camera_location, aim_location);
		auto world_delta_seconds = defines::game_statics->get_world_delta_seconds(world);

		auto result = aim_rotation;
		if (interpolate) {
			if (constant)
				result = defines::math->interp_to_constant(camera_rotation, aim_rotation, world_delta_seconds, speed);
			else
				result = defines::math->interp_to(camera_rotation, aim_rotation, world_delta_seconds, speed);
		}

		result.pitch = (result.pitch - camera_rotation.pitch) / controller->input_pitch_scale();
		result.yaw = (result.yaw - camera_rotation.yaw) / controller->input_yaw_scale();

		controller->add_pitch_input((float)result.pitch);
		controller->add_yaw_input((float)result.yaw);
	}

	fvector bottom, bottom_2, head, head_2, neck, chest, pelvis, rshoulder, relbow, rhand, rthigh, rknee, rfoot, rfootout, lshoulder, lelbow, lhand, lthigh, lknee, lfoot, lfootout;
	fvector2d s_bottom, s_bottom_2, s_head, s_head_2, s_neck, s_chest, s_pelvis, s_rshoulder, s_relbow, s_rhand, s_rthigh, s_rknee, s_rfoot, s_rfootout, s_lshoulder, s_lelbow, s_lhand, s_lthigh, s_lknee, s_lfoot, s_lfootout;

	bool get_performance_bones(aplayercontroller* controller, mesh* mesh) {
		bool success = true;

		bottom = mesh->get_bone_location(0);
		if (success && !controller->w2s(bottom, &s_bottom)) success = false;

	
		head = mesh->get_bone_location(67);
		if (success && !controller->w2s(head, &s_head)) success = false;

		if (!s_bottom || !s_head) 
			return false;


		return success;
	}	

	bool get_all_bones(aplayercontroller* controller, mesh* mesh) {
		bool success = true;

		bottom = mesh->get_bone_location(0);
		if (success && !controller->w2s(bottom, &s_bottom)) success = false;

		bottom_2 = bottom; bottom_2.z -= 10;
		if (success && !controller->w2s(bottom_2, &s_bottom_2)) success = false;

		head = mesh->get_bone_location(67);
		if (success && !controller->w2s(head, &s_head)) success = false;

		head_2 = head; head_2.z += 13;
		if (success && !controller->w2s(head_2, &s_head_2)) success = false;

		neck = mesh->get_bone_location(67);
		if (success && !controller->w2s(neck, &s_neck)) success = false;

		chest = mesh->get_bone_location(7); chest.z += 7;
		if (success && !controller->w2s(chest, &s_chest)) success = false;

		pelvis = mesh->get_bone_location(2);
		if (success && !controller->w2s(pelvis, &s_pelvis)) success = false;

		rshoulder = mesh->get_bone_location(38);
		if (success && !controller->w2s(rshoulder, &s_rshoulder)) success = false;

		relbow = mesh->get_bone_location(39);
		if (success && !controller->w2s(relbow, &s_relbow)) success = false;

		rhand = mesh->get_bone_location(40);
		if (success && !controller->w2s(rhand, &s_rhand)) success = false;

		rthigh = mesh->get_bone_location(78);
		if (success && !controller->w2s(rthigh, &s_rthigh)) success = false;

		rknee = mesh->get_bone_location(79);
		if (success && !controller->w2s(rknee, &s_rknee)) success = false;

		rfoot = mesh->get_bone_location(82);
		if (success && !controller->w2s(rfoot, &s_rfoot)) success = false;

		rfootout = mesh->get_bone_location(83);
		if (success && !controller->w2s(rfootout, &s_rfootout)) success = false;

		lshoulder = mesh->get_bone_location(9);
		if (success && !controller->w2s(lshoulder, &s_lshoulder)) success = false;

		lelbow = mesh->get_bone_location(10);
		if (success && !controller->w2s(lelbow, &s_lelbow)) success = false;

		lhand = mesh->get_bone_location(11);
		if (success && !controller->w2s(lhand, &s_lhand)) success = false;

		lthigh = mesh->get_bone_location(71);
		if (success && !controller->w2s(lthigh, &s_lthigh)) success = false;

		lknee = mesh->get_bone_location(72);
		if (success && !controller->w2s(lknee, &s_lknee)) success = false;

		lfoot = mesh->get_bone_location(75);
		if (success && !controller->w2s(lfoot, &s_lfoot)) success = false;

		lfootout = mesh->get_bone_location(76);
		if (success && !controller->w2s(lfootout, &s_lfootout)) success = false;

		if (!s_bottom || !s_head || !s_neck || !s_chest || !s_rshoulder || !s_relbow || !s_rhand || !s_lshoulder || !s_lelbow || !s_lhand || !s_rthigh || !s_rknee || !s_rfoot || !s_rfootout || !s_lthigh || !s_lknee || !s_lfoot || !s_lfootout)
			return false;

		return success;
	}
}

namespace essential_hooks {
	fvector location_o = fvector();
	frotator rotation_o = frotator();

	ulocalplayer* local_player_old = 0;
	aplayercontroller* player_controller_old = 0;

	using get_view_point_sim = void(*)(ulocalplayer* local_player, fminimalviewinfo* out_viewinfo, BYTE stereo_pass);
	get_view_point_sim get_view_point_o = 0;

	void get_view_point(ulocalplayer* local_player, fminimalviewinfo* out_viewinfo, BYTE stereo_pass) {
		get_view_point_o(local_player, out_viewinfo, stereo_pass);
	}

	using get_player_view_point_sim = void(*)(aplayercontroller* controller, fvector* location, frotator* rotation);
	get_player_view_point_sim get_player_view_point_o = 0;

	void get_player_view_point(aplayercontroller* controller, fvector* location, frotator* rotation) {
		get_player_view_point_o(controller, location, rotation);

		location_o = *location;
		rotation_o = *rotation;
	}

	void run(aplayercontroller* controller, ulocalplayer* local_player) {
		if (local_player != essential_hooks::local_player_old) {
 			void** LocalPlayer_VTable = *(void***)(local_player);
			DWORD OldProtection;
			(VirtualProtect)(&LocalPlayer_VTable[88], 8, PAGE_EXECUTE_READWRITE, &OldProtection);
			essential_hooks::get_view_point_o = decltype(essential_hooks::get_view_point_o)(LocalPlayer_VTable[88]);
			LocalPlayer_VTable[88] = &essential_hooks::get_view_point;
			(VirtualProtect)(&LocalPlayer_VTable[88], 8, OldProtection, &OldProtection);

			essential_hooks::local_player_old = local_player;
		}

		if (controller != essential_hooks::player_controller_old) {
			void** PlayerController_VTable = *(void***)(controller);
			DWORD OldProtection;
			(VirtualProtect)(&PlayerController_VTable[245], 8, PAGE_EXECUTE_READWRITE, &OldProtection);
			essential_hooks::get_player_view_point_o = decltype(essential_hooks::get_player_view_point_o)(PlayerController_VTable[245]);
			PlayerController_VTable[245] = &essential_hooks::get_player_view_point;
			(VirtualProtect)(&PlayerController_VTable[245], 8, OldProtection, &OldProtection);
			essential_hooks::player_controller_old = controller;
		}
	}
}



void mytektocfel(ugameviewportclient* viewport, ucanvas* canvas) {
	auto screen_size = fvector2d((double)canvas->clip_x(), (double)canvas->clip_y());
	auto center = fvector2d((double)screen_size.x / 2, (double)screen_size.y / 2);
	bool player_silent = false;

	auto world = viewport->get_world();
	if (!world) return;

	auto game_instance = defines::game_statics->get_game_instance(world);
	if (!game_instance) return;

	auto local_player = game_instance->get_local_players()[0];
	if (!game_instance) return;

	auto controller = defines::game_statics->get_player_controller(world, std::int32_t(0));
	if (!controller) return;

	auto camera_manager = defines::game_statics->get_player_camera_manager(world, std::int32_t(0));
	if (!camera_manager) return;

	render::world = world;
	render::canvas = canvas;
	render::controller = controller;
	render::screen_center = center;
	render::screen_size = screen_size;



	auto camera_location = camera_manager->get_camera_location();
	auto camera_rotation = camera_manager->get_camera_rotation();
	auto camera_fov = camera_manager->get_fov_angle();


	auto radius = (settings::aimbot::field_of_view * center.x / (double)camera_fov) / 2;

	auto local_pawn_holding_pickaxe = false;
	auto local_pawn = controller->get_pawn();

	if (settings::player::enable) {
		double closest_distance = DBL_MAX;
		player_pawn* target_player = 0;

		tarray<uobject*> player_array = defines::game_statics->get_all_actors_of_class(world, defines::actor_player_class);
		for (int i = 0; i < player_array.size(); i++) {
			if (!player_array.valid(i)) continue;

			auto player = (player_pawn*)player_array[i];
			if (player == local_pawn) continue;
			if (!player) continue;

			if (player->is_dead()) continue;

			auto object_name = defines::system->get_object_name(player);

			if (!defines::string->contains(object_name.c_str(), L"PlayerPawn", false, false)) continue;

			auto skeleton_color = flinearcolor(0.f, 1.f, 1.f, 1.f);
			auto esp_color = flinearcolor(1.f, 1.f, 1.f, 1.f);
			auto black_color = flinearcolor(0.f, 0.f, 0.f, 1.f);

			auto world_location = player->get_actor_location();


			auto max_distance = settings::player::max_distance;
			double distance = defines::math->vector_distance(world_location, camera_location) * 0.01;
			if (distance > max_distance) continue;

			bool visible = player->was_recently_rendered(0.f);
			if (visible) {
				Spotify::visible = true;
				skeleton_color = flinearcolor(0.1f, 1.f, 0.1f, 1.f);
			}
			else {
				Spotify::visible = false;
				skeleton_color = flinearcolor(1.f, 0.f, 0.f, 1.f);
			}







			auto mesh = player->get_pawn_mesh();
			if (!mesh) continue;

			if (!settings::style::performance) {
				if (!Spotify::get_all_bones(controller, mesh)) continue;
			}
			else {
				if (!Spotify::get_performance_bones(controller, mesh)) continue;
			}
			if (settings::style::performance) {
				if (!in_area(Spotify::s_bottom, fvector2d(50, 50), fvector2d(screen_size.x - (50 * 2), screen_size.y - (50 * 2)))) continue;
			}

			if (settings::player::skeleton) {
				render::line_with_outline(Spotify::s_head, Spotify::s_neck, skeleton_color, 1.f);
				render::line_with_outline(Spotify::s_neck, Spotify::s_chest, skeleton_color, 1.f);
				render::line_with_outline(Spotify::s_chest, Spotify::s_pelvis, skeleton_color, 1.f);

				render::line_with_outline(Spotify::s_chest, Spotify::s_rshoulder, skeleton_color, 1.f);
				render::line_with_outline(Spotify::s_rshoulder, Spotify::s_relbow, skeleton_color, 1.f);
				render::line_with_outline(Spotify::s_relbow, Spotify::s_rhand, skeleton_color, 1.f);
				render::line_with_outline(Spotify::s_pelvis, Spotify::s_rthigh, skeleton_color, 1.f);
				render::line_with_outline(Spotify::s_rthigh, Spotify::s_rknee, skeleton_color, 1.f);
				render::line_with_outline(Spotify::s_rknee, Spotify::s_rfoot, skeleton_color, 1.f);
				//render::line_with_outline(Spotify::s_rfoot, Spotify::s_rfootout, skeleton_color, 1.f);

				render::line_with_outline(Spotify::s_chest, Spotify::s_lshoulder, skeleton_color, 1.f);
				render::line_with_outline(Spotify::s_lshoulder, Spotify::s_lelbow, skeleton_color, 1.f);
				render::line_with_outline(Spotify::s_lelbow, Spotify::s_lhand, skeleton_color, 1.f);
				render::line_with_outline(Spotify::s_pelvis, Spotify::s_lthigh, skeleton_color, 1.f);
				render::line_with_outline(Spotify::s_lthigh, Spotify::s_lknee, skeleton_color, 1.f);
				render::line_with_outline(Spotify::s_lknee, Spotify::s_lfoot, skeleton_color, 1.f);
				//render::line_with_outline(Spotify::s_lfoot, Spotify::s_lfootout, skeleton_color, 1.f);
			}
			auto most_left = DBL_MAX;
			auto most_right = DBL_MIN;
			auto most_top = DBL_MAX;
			auto most_bottom = DBL_MIN;


			if (!settings::style::performance) {
				fvector2d bones_to_check[] = { Spotify::s_bottom, Spotify::s_head, Spotify::s_head_2, Spotify::s_neck, Spotify::s_chest, Spotify::s_pelvis, Spotify::s_rshoulder, Spotify::s_relbow, Spotify::s_rhand, Spotify::s_rthigh, Spotify::s_rknee, Spotify::s_rfoot, Spotify::s_rfootout, Spotify::s_lshoulder, Spotify::s_lelbow,Spotify::s_lhand, Spotify::s_lthigh, Spotify::s_lknee, Spotify::s_lfoot, Spotify::s_lfootout };

				for (int i = 0; i < 20; i++) {
					auto bone = bones_to_check[i];

					if (bone.x < most_left)
						most_left = bone.x;

					if (bone.x > most_right)
						most_right = bone.x;

					if (bone.y < most_top)
						most_top = bone.y;

					if (bone.y > most_bottom)
						most_bottom = bone.y;
				}
			}
			else {
				fvector2d bones_to_check[] = { Spotify::s_bottom, Spotify::s_head };

				for (int i = 0; i < 2; i++) {
					auto bone = bones_to_check[i];

					if (bone.x < most_left)
						most_left = bone.x;

					if (bone.x > most_right)
						most_right = bone.x;

					if (bone.y < most_top)
						most_top = bone.y;

					if (bone.y > most_bottom)
						most_bottom = bone.y;
				}
			}
			auto actor_height = most_bottom - most_top;
			auto actor_width = most_right - most_left;

			auto calculated_distance = max_distance - distance;
			auto offset = calculated_distance * 0.035;
			auto corner_wide = actor_width / 3;
			auto corner_heit = actor_height / 3;

			auto text_offset = double(0);
			fvector2d bottom_middle;
			fvector2d top_middle;
			if (!settings::style::performance) {
				bottom_middle = fvector2d(most_left + actor_width / 2, Spotify::s_bottom_2.y);
			}
			else {
				bottom_middle = fvector2d(most_left + actor_width / 2, Spotify::s_bottom_2.y);
			}

			if (settings::player::normal_box) {
				render::line_with_outline(fvector2d(most_left - offset, most_top - offset), fvector2d(most_left - offset, most_bottom + offset), esp_color, 1.f);

				// right line
				render::line_with_outline(fvector2d(most_right + offset, most_top - offset), fvector2d(most_right + offset, most_bottom + offset), esp_color, 1.f);

				// top line
				render::line_with_outline(fvector2d(most_left - offset, most_top - offset), fvector2d(most_right + offset, most_top - offset), esp_color, 1.f);

				// bottom line
				render::line_with_outline(fvector2d(most_left - offset, most_bottom + offset), fvector2d(most_right + offset, most_bottom + offset), esp_color, 1.f);
			}

			if (settings::player::player_name) {
				if (player && defines::actor_player_nameprivate && local_pawn) {
					auto playername = defines::lib->get_player_name_safe(player, defines::actor_player_nameprivate);
					render::text(playername.c_str(), fvector2d(bottom_middle.x, bottom_middle.y + text_offset), flinearcolor(1.f, 1.f, 1.f, 1.f), true, false, settings::style::text_outlined);
					text_offset += 15;
				}
			}

			if (settings::player::display_distance) {
				auto final = defines::string->build_string_double(L"", L"", defines::math->round(distance), L"m");
				render::text(final.c_str(), fvector2d(bottom_middle.x, bottom_middle.y + text_offset), flinearcolor(1.f, 1.f, 1.f, 1.f), true, false, settings::style::text_outlined);
				text_offset += 15;
			}

			if (settings::player::snapline) {
				render::line_with_outline(fvector2d(center.x, screen_size.y - 15), fvector2d(bottom_middle.x, bottom_middle.y + text_offset), flinearcolor(1.f, 1.f, 1.f, 1.f), 1.f);
			}



			if (!local_pawn_holding_pickaxe && ((settings::aimbot::visible_only && visible) || (!settings::aimbot::visible_only)) && local_pawn && render::in_circle(center.x, center.y, radius, Spotify::s_head.x, Spotify::s_head.y)) {
				double distance = defines::math->distance2d(Spotify::s_head, center);
				if (distance < closest_distance) {
					target_player = player;
					closest_distance = distance;
				}
			}
		}

		if (settings::aimbot::enable) {
			auto temp_silent_active = false;
			auto temp_silent_rotation = frotator();

			auto temp_sticky_silent_active = false;
			auto temp_sticky_silent_rotation = frotator();

			if (settings::aimbot::draw_fov) {
				render::circle(center, radius, 900, flinearcolor(0.f, 0.f, 0.f, 1.f));
			}



			if (target_player && local_pawn) {
				if (auto mesh = target_player->get_pawn_mesh()) {

					int32_t bone = 67;

					if (settings::bones::head)
						bone = 67;
					else if (settings::bones::neck)
						bone = 67;


					auto aim_location = mesh->get_bone_location(bone);
					auto skeleton_color = flinearcolor(0.f, 1.f, 1.f, 1.f);
					auto esp_color = flinearcolor(1.f, 0.f, 0.f, 1.f);
					auto visible = target_player->was_recently_rendered(0.f);
					if (visible) {
						skeleton_color = flinearcolor(0.f, 1.f, 0.f, 1.f);
					}
					else {
						skeleton_color = flinearcolor(1.f, 0.f, 0.f, 1.f);
					}	

					if (settings::aimbot::mouse) {
						if (GetAsyncKeyState(settings::aimbot::aim_key) & 0x8000) {
							float width = ((double)canvas->clip_x());
							float height = ((double)canvas->clip_y());

							if (visible) {
								Spotify::set_aim_mouse(controller, aim_location, width, height, settings::aimbot::mouse_speed);
							}
							else
							{
								return;
							}

						}
					}
				}
			}
		}
	}
}

namespace husint {
	using draw_transition_sim = void(*)(ugameviewportclient* viewport, ucanvas* canvas);
	draw_transition_sim draw_transition_o = 0;


	void draw_transition(ugameviewportclient* viewport, ucanvas* canvas) {
		mytektocfel(viewport, canvas);
		return draw_transition_o(viewport, canvas);
	}




	static void mytekgej() {
		fn = get_module(L"FortniteClient-Win64-Shipping.exe");

		if (!fn)
			return;		

		defines::init();


		if (auto world = uobject::find_object(L"Frontend", reinterpret_cast<uobject*>(-1))) {
			auto game_instance = defines::game_statics->get_game_instance(world);
			auto local_player = game_instance->get_local_players()[0];


			auto controller = defines::game_statics->get_player_controller(world, std::int32_t(0));
			auto viewport = local_player->get_viewport();

			auto engine = (uengine*)defines::system->get_outer_object(game_instance);
			render::font = engine->get_font();



			draw_transition_o = vmt<decltype(draw_transition_o)>(std::uintptr_t(viewport), std::uintptr_t(draw_transition), offsets::drawtransition);
		}

	}
}
						
						