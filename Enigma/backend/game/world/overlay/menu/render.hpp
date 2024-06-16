#pragma once
#ifndef DRAW
#define DRAW

#include "../../../../../includes.hpp"
#include "../../sdk/engine/worldtoscreen.hpp"
#include <iostream>
#include <string>
#include "../../../../framework/imgui/etc_element.h"
#include "../../../../framework/imgui/imgui_setting.h"
#include "../helper/binder.hpp"
#include "../helper/config.hpp"
#include "../../sdk/engine/decrypt.hpp"
bool bTheme = true;
#define ALPHA    ( ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_InputRGB | ImGuiColorEditFlags_Float | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_PickerHueBar | ImGuiColorEditFlags_NoBorder )

namespace v
{
	
	class draw
	{
	public:
		__forceinline auto setup_colors() -> void
		{
			ImGuiStyle& s = ImGui::GetStyle();
			ImGuiContext& g = *GImGui;
			s.WindowPadding = ImVec2(0, 0);
			s.ChildRounding = 4.f;
			s.WindowRounding = 5.f;
			s.Colors[ImGuiCol_WindowBg] = ImLerp(s.Colors[ImGuiCol_WindowBg], bTheme ? ImColor(0.06f, 0.06f, 0.06f, 1.f) : ImColor(5.90f, 5.90f, 5.90f, 1.f), g.IO.DeltaTime * 5.f);
			settings::tabs::text = ImColor(255, 255, 255, 255);
			s.Colors[ImGuiCol_ChildBg] = ImLerp(s.Colors[ImGuiCol_ChildBg], bTheme ? ImColor(0.06f, 0.06f, 0.06f, 1.f) : ImColor(5.90f, 5.90f, 5.90f, 1.f), g.IO.DeltaTime * 5.f);
			s.Colors[ImGuiCol_Text] = ImLerp(s.Colors[ImGuiCol_Text], bTheme ? ImColor(1.f, 1.f, 1.f, 1.f) : ImColor(0.00f, 0.00f, 0.00f, 1.00f), g.IO.DeltaTime * 5.f);
		}
		__forceinline auto menu() -> void
		{
			setup_colors();
			ImGui::SetNextWindowSize(ImVec2(890, 620));
			ImGui::Begin("", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoBringToFrontOnFocus);
			{
				
				ImGui::SetCursorPos(ImVec2(10, 160));
				BeginGroup();
				{

					if (ImGui::Tabs(0 == globals->tabs, _("F"), _("Aimbot"), ImVec2(90, 40))) globals->tabs = 0;

					if (ImGui::Tabs(1 == globals->tabs, _("A"), _("Visuals"), ImVec2(90, 40))) globals->tabs = 1;

					if (ImGui::Tabs(2 == globals->tabs, _("X"), _("Misc"), ImVec2(90, 40))) globals->tabs = 2;

				}
				EndGroup();
				if (globals->tabs == 0)
				{

					ImGui::SetCursorPos(ImVec2(200, 60));
					e_elements::begin_child(_("General"), ImVec2(335, 400));
					{
						ImGui::SetCursorPos(ImVec2(10, 45));

						ImGui::BeginGroup();
						{
							ImGui::Checkbox(_("Enable Aimbot"), &aimbot->enable);
							ImGui::SameLine(150);
							HotkeyButton(aimbot->key, ChangeKey, keystatus);
							ImGui::Checkbox(_("Enable Snipers Prediction"), &aimbot->prediction);
							ImGui::Checkbox(_("Enable Shotguns Triggerbot"), &aimbot->triggerbot);
							HotkeyButton(aimbot->triggerkey, ChangeKey2, keystatus2);
							ImGui::SliderInt(_("Triggerbot Speed"), &aimbot->triggerspeed, 1, 5);
							ImGui::SliderFloat(_("Aimbot Smoothing"), &aimbot->smoothing, 1, 10);
							ImGui::SliderFloat(_("FOV Size"), &aimbot->fovsize, 50, 250);
							ImGui::Checkbox(_("Show FOV"), &aimbot->fov);
							//ImGui::Checkbox(_("Player Freeze"), &aimbot->playerfreeze);
						}
						ImGui::EndGroup();

					}
					e_elements::end_child();

					ImGui::SetCursorPos(ImVec2(550, 60));
					e_elements::begin_child(_("Settings"), ImVec2(335, 400));
					{
						ImGui::SetCursorPos(ImVec2(10, 45));

						ImGui::BeginGroup();
						{
							ImGui::Text("Target Bones");
							if (ImGui::Checkbox("Head", &aimbot->head))
							{
								aimbot->bone = 0;
								aimbot->chest = false;
								aimbot->neck = false;
								aimbot->closest = false;
							}
							if (ImGui::Checkbox("Neck", &aimbot->neck))
							{
								aimbot->bone = 1;
								aimbot->chest = false;
								aimbot->head = false;
								aimbot->closest = false;
							}
							if (ImGui::Checkbox("Chest", &aimbot->chest))
							{
								aimbot->bone = 2;
								aimbot->neck = false;
								aimbot->head = false;
								aimbot->closest = false;
							}
							/*if (ImGui::Checkbox("Closest", &aimbot->closest))
							{
								aimbot->bone = 3;
								aimbot->neck = false;
								aimbot->head = false;
								aimbot->chest = false;
							}*/
						}
						ImGui::EndGroup();

					}
					e_elements::end_child();
					

				}
				else if (globals->tabs == 1)
				{
					ImGui::SetCursorPos(ImVec2(200, 60));
					e_elements::begin_child(_("Esp"), ImVec2(335, 500));
					{
						ImGui::SetCursorPos(ImVec2(10, 45));

						ImGui::BeginGroup();
						{
							ImGui::Checkbox(_("Enable ESP"), &visuals->enable);


							ImGui::Checkbox(_("Enable Skeleton"), &visuals->skeleton);
							ImGui::Checkbox(_("Outlined Skeleton"), &visuals->skeleton_outlined);
							ImGui::Checkbox(_("Enable Head ESP"), &visuals->head);
							//ImGui::Checkbox(_("Enable Username ESP"), &visuals->username);
							//ImGui::Checkbox(_("Enable Weapon ESP"), &visuals->weapon);
							//ImGui::Checkbox(_("Enable Platform ESP"), &visuals->platform);
							ImGui::Checkbox(_("Enable Boxes"), &visuals->box);
							
							if (visuals->box)
							{
								

								
								if (ImGui::Checkbox(_("2D Box"), &visuals->regular)) {
									visuals->cornered = false;
									visuals->rounded = false;
								}
								
								if (ImGui::Checkbox(_("Cornered Box"), &visuals->cornered))
								{
									visuals->regular = false;
									visuals->rounded = false;
								}
								if (ImGui::Checkbox(_("Rounded Box"), &visuals->rounded))
								{
									visuals->regular = false;
									visuals->cornered = false;
								}
								
								
								ImGui::Checkbox(_("Outlined Box"), &visuals->outlinedbox);
							}
							ImGui::Checkbox(_("Enable Distance"), &visuals->distance);
							

						}
						ImGui::EndGroup();

					}e_elements::end_child();
					

					ImGui::SetCursorPos(ImVec2(500, 60));
					e_elements::begin_child(_("Customization"), ImVec2(335, 500));
					{
						ImGui::SetCursorPos(ImVec2(10, 45));
						ImGui::SliderFloat(_("Box Thickness"), &visuals->box_thickness, 1, 5);
						ImGui::SliderFloat(_("Skeleton Thickness"), &visuals->skeleton_thickness, 1, 5);
						ImGui::Text(_("Visible Color"));
						ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 101, ImGui::GetCursorPos().y - 27));
						ImGui::ColorEdit4(_("Visible Color"), colours->visible_color_float, ALPHA);

						ImGui::Text(_("Invisible Color"));
						ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPos().x + 110, ImGui::GetCursorPos().y - 27));
						ImGui::ColorEdit4(_("Invisible Color"), colours->invisible_color_float, ALPHA);

					}e_elements::end_child();
				}
				else if (globals->tabs == 2)
				{
					ImGui::SetCursorPos(ImVec2(200, 60));
					e_elements::begin_child(_("Misc"), ImVec2(335, 300));
					{
						ImGui::SetCursorPos(ImVec2(10, 45));
						ImGui::Checkbox(_("Show FPS"), &misc->fps);
						//ImGui::Checkbox(_("Show Console"), &misc->show_console);
						//ImGui::Checkbox(_("Show Radar"), &misc->radar);
						if (ImGui::Button(_("Exit"), ImVec2(60, 60)))
						{
							system(_("taskkill /F /IM FXSCOVER.exe"));
							ExitProcess(0);
						}
						ImGui::Checkbox("Bone IDs", &misc->bones);
						if (ImGui::Checkbox(_("Enable Loot ESP"), &loot->enable))
						{
							//loot_cache->LevelDrawing();
						}
						ImGui::Checkbox(_("Enable pickups"), &loot->pickups);

						

					}e_elements::end_child();


					ImGui::SetCursorPos(ImVec2(500, 60));
					e_elements::begin_child(_("Config"), ImVec2(335, 500));
					{
						ImGui::SetCursorPos(ImVec2(10, 45));
						if (ImGui::Button(_("Save\nConfig"), ImVec2(100, 60)))
						{

							configs->savecfg(TEXT(_("C:\\cfg")));
						}
						ImGui::SameLine();
						if (ImGui::Button(_("Load\nConfig"), ImVec2(100, 60)))
						{
							configs->loadcfg(TEXT(_("C:\\cfg")));
						}

					}e_elements::end_child();
				}

			} 
			ImGui::End();
		}
	};



	class vis
	{
	public:
		
		ImDrawList* draw_list = ImGui::GetForegroundDrawList();
		__forceinline void watermark()
		{
			draw_list->AddText(ImVec2(35, 25), ImColor(255, 255, 255), _("Enigga Public"));
		}


		__forceinline void TwoD(int x, int y, int w, int h, ImColor color, int thickness) {

			draw_list->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), color, 0.0f, 0, thickness);
		}
		__forceinline void rounded(Vector2 head_box, float box_width, float box_height, ImColor visible_color, float thickness)
		{
			if (visuals->outlinedbox)
			{
				draw_list->AddRect(ImVec2(head_box.x - box_width / 2, head_box.y), ImVec2((head_box.x - box_width / 2) + box_width, head_box.y + box_height), ImColor(0, 0, 0), 10, ImDrawCornerFlags_All, thickness + 2.5);
			}

			draw_list->AddRect(ImVec2(head_box.x - box_width / 2, head_box.y), ImVec2((head_box.x - box_width / 2) + box_width, head_box.y + box_height), visible_color, 10, ImDrawCornerFlags_All, thickness);

		}
		__forceinline void fps()
		{
			float fps = ImGui::GetIO().Framerate;
			std::string fpsText = std::to_string(static_cast<int>(fps)) + _(" fps");
			draw_list->AddText(ImVec2(35, 45), ImColor(255, 255, 255), fpsText.c_str());
		}
		__forceinline void draw_head(Vector3 head_location, float fov, Vector3 location, uintptr_t pawn, ImColor color)
		{
			if (!pawn)
			{
				Vector2 head_2d = uengine->w2s(Vector3(head_location.x, head_location.y, head_location.z + 13));

				Vector3 delta = head_location - location;
				float distance = delta.Length();

				const float constant_circle_size = 4;

				float circle_radius = constant_circle_size * (globals->height / (2.0f * distance * tanf(fov * (float)M_PI / 360.f)));

				float y_offset = +70.0f;
				head_2d.y += y_offset;

				int segments = 50;
				float thickness = 2.0f;

				if (visuals->skeleton_outlined)
				{
					draw_list->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, ImColor(0, 0, 0, 255), segments, visuals->skeleton_thickness + 2);
				}
				draw_list->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, color, segments, visuals->skeleton_thickness);

			}
			else
			{
				Vector2 head_2d = uengine->w2s(Vector3(head_location.x, head_location.y, head_location.z));

				Vector3 delta = head_location - location;
				float distance = delta.Length();

				const float constant_circle_size = 8;

				float circle_radius = constant_circle_size * (globals->height / (2.0f * distance * tanf(fov * (float)M_PI / 360.f)));

				float y_offset = +12.0f;



				int segments = 50;
				float thickness = 2.0f;

				if (visuals->skeleton_outlined)
				{
					draw_list->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, ImColor(0, 0, 0, 255), segments, visuals->skeleton_thickness + 2);
				}
				draw_list->AddCircle(ImVec2(head_2d.x, head_2d.y), circle_radius, color, segments, visuals->skeleton_thickness);

			}
		}
		__forceinline void draw_box(Vector2 Headbox, Vector2 bottom, float CornerWidth, ImColor visible_color, float thickness)
		{
			
			draw_list->AddLine(ImVec2(Headbox.x - (CornerWidth / 2), Headbox.y), ImVec2(bottom.x + (CornerWidth / 2), Headbox.y), visible_color, thickness);

			draw_list->AddLine(ImVec2(Headbox.x - (CornerWidth / 2), Headbox.y), ImVec2(Headbox.x - (CornerWidth / 2), bottom.y), visible_color, thickness); // L

			draw_list->AddLine(ImVec2(Headbox.x - (CornerWidth / 2), bottom.y), ImVec2(bottom.x + (CornerWidth / 2), bottom.y), visible_color, thickness); // T

			draw_list->AddLine(ImVec2(bottom.x + (CornerWidth / 2), Headbox.y), ImVec2(bottom.x + (CornerWidth / 2), bottom.y), visible_color, thickness); // R
		}



		__forceinline void AddToRadar(Vector2 WorldLocation, Vector3 LocalLocation, ImColor Color, Vector2 RadarPos, Vector2 RadarSize)
		{
			Vector2 Projected = utility->RotatePoint(RadarPos, RadarSize, LocalLocation, WorldLocation);
			draw_list->AddCircleFilled(ImVec2(Projected.x, Projected.y), 3, Color, 32);
		}
		__forceinline void draw_distance(float distance, Vector2 root_screen, ImColor color)
		{
			std::string distance_string = "" + std::to_string((int)distance) + _(" m");

			ImVec2 text_size = ImGui::CalcTextSize(distance_string.c_str());
			
			utility->DrawString(20, root_screen.x - (text_size.x / 2), root_screen.y + 5, color, false, true, distance_string.c_str());

		}
		

		__forceinline auto draw_username_platform(Vector2 head, uintptr_t player, ImColor color) -> void
		{
			auto name = decrypt->username(player);

			auto platform_str = decrypt->platform(player);

			int offset;
			if (visuals->weapon)
				offset = 30;
			else
				offset = 15;

			ImVec2 textPosition(head.x, head.y - offset);

			std::string combinedText = name + " - [" + platform_str + "]";
			utility->draw_outlined_text(textPosition, color, combinedText.c_str());
		}

		__forceinline void draw_username(Vector2 head, uintptr_t playerstate, ImColor color)
		{
			std::string username_str = decrypt->username(playerstate);
			ImVec2 text_size = ImGui::CalcTextSize(username_str.c_str());
			utility->DrawString(20, head.x - (text_size.x / 2), head.y - 20, ImColor(255, 255, 255), false, true, username_str.c_str());
		}

		__forceinline auto draw_platform(Vector2 head, uintptr_t player, ImColor color) -> void
		{
			auto platform_str = decrypt->platform(player);
			int offset;
			if (visuals->weapon)
				offset = 30;
			else
				offset = 15;

			ImVec2 textPosition(head.x, head.y - offset);

			utility->draw_outlined_text(textPosition, color, platform_str.c_str());
		}

		__forceinline auto draw_weapon( Vector2 head, Vector2 bottom,  uintptr_t weapon, ImColor color ) -> void
		{
			auto name = decrypt->weapon(weapon);
			ImVec2 TextSize = ImGui::CalcTextSize(name.c_str());
			utility->draw_outlined_text(ImVec2((bottom.x) - (TextSize.x / 2), (head.y - 15)), color, name.c_str());
		}

		__forceinline void draw_fov()
		{
			draw_list->AddCircle(ImVec2(globals->width / 2, globals->height / 2), aimbot->fovsize, ImColor(0, 0, 0), 64, 2.5);
			draw_list->AddCircle(ImVec2(globals->width / 2, globals->height / 2), aimbot->fovsize, ImColor(1.f, 1.f, 1.f, 1.0f), 64, 1);
		}

		__forceinline void draw_cornered_box(int x, int y, int w, int h, int borderPx, ImU32 color)
		{
			int borderWidth = (borderPx < (w / 2) && borderPx < (h / 2)) ? borderPx : ((w / 2) < (h / 2) ? (w / 2) : (h / 2));

			
			utility->DrawFilledRect(x + borderWidth, y, w / 3, borderWidth, color);//, round_box); //top 
			utility->DrawFilledRect(x + w - w / 3, y, w / 3, borderWidth, color); //top 
			utility->DrawFilledRect(x, y, borderWidth, h / 3, color); //left 
			utility->DrawFilledRect(x, y + h - h / 3, borderWidth, h / 3, color); //left 
			utility->DrawFilledRect(x + borderWidth, y + h - borderWidth, w / 3, borderWidth, color); //bottom 
			utility->DrawFilledRect(x + w - w / 3, y + h - borderWidth, w / 3, borderWidth, color); //bottom 
			utility->DrawFilledRect(x + w - borderWidth, y, borderWidth, h / 3, color);//right 
			utility->DrawFilledRect(x + w - borderWidth, y + h - h / 3, borderWidth, h / 3, color);//right 
		}


		__forceinline void draw_skeleton2(uintptr_t mesh, ImColor color)
		{
			auto pelvis = uengine->w2s(uengine->bonewithrotation(mesh, bones->humanpelvis));
			auto left_shoulder = uengine->w2s(uengine->bonewithrotation(mesh, 9));
			auto left_elbow = uengine->w2s(uengine->bonewithrotation(mesh, 10));
			auto left_hand = uengine->w2s(uengine->bonewithrotation(mesh, bones->humanlhand));
			auto right_shoulder = uengine->w2s(uengine->bonewithrotation(mesh, 38));
			auto neck = uengine->w2s(uengine->bonewithrotation(mesh, 67));

			auto right_elbow = uengine->w2s(uengine->bonewithrotation(mesh, 39));
			auto right_hand = uengine->w2s(uengine->bonewithrotation(mesh, 40));
			auto left_knee = uengine->w2s(uengine->bonewithrotation(mesh, 72));
			auto left_foot = uengine->w2s(uengine->bonewithrotation(mesh, 86));
			auto right_knee = uengine->w2s(uengine->bonewithrotation(mesh, 79));
			auto right_foot = uengine->w2s(uengine->bonewithrotation(mesh, 82));

			if (visuals->skeleton_outlined)
			{
				draw_list->AddLine(ImVec2(neck.x, neck.y), ImVec2(pelvis.x, pelvis.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(neck.x, neck.y), ImVec2(neck.x, neck.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(neck.x, neck.y), ImVec2(left_elbow.x, left_elbow.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(left_elbow.x, left_elbow.y), ImVec2(left_hand.x, left_hand.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(neck.x, neck.y), ImVec2(right_elbow.x, right_elbow.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(right_elbow.x, right_elbow.y), ImVec2(right_hand.x, right_hand.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(left_knee.x, left_knee.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(left_knee.x, left_knee.y), ImVec2(left_foot.x, left_foot.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(right_knee.x, right_knee.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(right_knee.x, right_knee.y), ImVec2(right_foot.x, right_foot.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
			}

			draw_list->AddLine(ImVec2(neck.x, neck.y), ImVec2(pelvis.x, pelvis.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(neck.x, neck.y), ImVec2(neck.x, neck.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(neck.x, neck.y), ImVec2(left_elbow.x, left_elbow.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(left_elbow.x, left_elbow.y), ImVec2(left_hand.x, left_hand.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(neck.x, neck.y), ImVec2(right_elbow.x, right_elbow.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(right_elbow.x, right_elbow.y), ImVec2(right_hand.x, right_hand.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(left_knee.x, left_knee.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(left_knee.x, left_knee.y), ImVec2(left_foot.x, left_foot.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(right_knee.x, right_knee.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(right_knee.x, right_knee.y), ImVec2(right_foot.x, right_foot.y), color, visuals->skeleton_thickness);
		}

		__forceinline void draw_skeleton(uintptr_t mesh, ImColor color)
		{
			auto neck = uengine->w2s(uengine->bonewithrotation(mesh, bones->humanneck));
			auto left_shoulder = uengine->w2s(uengine->bonewithrotation(mesh, 9));
			auto left_elbow = uengine->w2s(uengine->bonewithrotation(mesh, bones->humanlforearm23));
			auto left_hand = uengine->w2s(uengine->bonewithrotation(mesh, bones->humanlhand));
			auto right_shoulder = uengine->w2s(uengine->bonewithrotation(mesh, 38));
			auto right_elbow = uengine->w2s(uengine->bonewithrotation(mesh, bones->humanrforearm23));
			auto right_hand = uengine->w2s(uengine->bonewithrotation(mesh, bones->humanrhand));
			auto pelvis = uengine->w2s(uengine->bonewithrotation(mesh, bones->humanpelvis));
			auto left_hip = uengine->w2s(uengine->bonewithrotation(mesh, bones->humanlthigh1));
			auto left_knee = uengine->w2s(uengine->bonewithrotation(mesh, bones->humanlthigh3));
			auto left_foot = uengine->w2s(uengine->bonewithrotation(mesh, bones->humanlfoot2));
			auto right_hip = uengine->w2s(uengine->bonewithrotation(mesh, bones->humanrthigh1));
			auto right_knee = uengine->w2s(uengine->bonewithrotation(mesh, bones->humanrthigh3));
			auto right_foot = uengine->w2s(uengine->bonewithrotation(mesh, bones->humanrfoot2));
			if (visuals->skeleton_outlined)
			{
				draw_list->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(right_shoulder.x, right_shoulder.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(left_elbow.x, left_elbow.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(left_elbow.x, left_elbow.y), ImVec2(left_hand.x, left_hand.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(right_shoulder.x, right_shoulder.y), ImVec2(right_elbow.x, right_elbow.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(right_elbow.x, right_elbow.y), ImVec2(right_hand.x, right_hand.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(neck.x, neck.y), ImVec2(pelvis.x, pelvis.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(left_hip.x, left_hip.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(left_hip.x, left_hip.y), ImVec2(left_knee.x, left_knee.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(left_knee.x, left_knee.y), ImVec2(left_foot.x, left_foot.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(right_hip.x, right_hip.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(right_hip.x, right_hip.y), ImVec2(right_knee.x, right_knee.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
				draw_list->AddLine(ImVec2(right_knee.x, right_knee.y), ImVec2(right_foot.x, right_foot.y), ImColor(0, 0, 0), visuals->skeleton_thickness + 2.5);
			}
			draw_list->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(right_shoulder.x, right_shoulder.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(left_shoulder.x, left_shoulder.y), ImVec2(left_elbow.x, left_elbow.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(left_elbow.x, left_elbow.y), ImVec2(left_hand.x, left_hand.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(right_shoulder.x, right_shoulder.y), ImVec2(right_elbow.x, right_elbow.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(right_elbow.x, right_elbow.y), ImVec2(right_hand.x, right_hand.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(neck.x, neck.y), ImVec2(pelvis.x, pelvis.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(left_hip.x, left_hip.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(left_hip.x, left_hip.y), ImVec2(left_knee.x, left_knee.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(left_knee.x, left_knee.y), ImVec2(left_foot.x, left_foot.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(pelvis.x, pelvis.y), ImVec2(right_hip.x, right_hip.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(right_hip.x, right_hip.y), ImVec2(right_knee.x, right_knee.y), color, visuals->skeleton_thickness);
			draw_list->AddLine(ImVec2(right_knee.x, right_knee.y), ImVec2(right_foot.x, right_foot.y), color, visuals->skeleton_thickness);
		}
	};
}

static v::draw* drawer;
static v::vis* visualiser;
#endif
