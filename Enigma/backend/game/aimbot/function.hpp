#pragma once
#ifndef MOUSE
#define MOUSE

#include <iostream>
#include "../../../includes.hpp"
#include "movement/mouse.hpp"


bool has_clicked;
std::chrono::steady_clock::time_point tb_begin;
std::chrono::steady_clock::time_point tb_end;
int tb_time_since;
float custom_delay = 10;

__forceinline auto RandomFloat(float a, float b) -> float
{
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}
float custom_fabsf(float x) {

	__m128 x_vec = _mm_set_ss(x);
	x_vec = _mm_and_ps(x_vec, _mm_castsi128_ps(_mm_set1_epi32(0x7FFFFFFF)));
	return _mm_cvtss_f32(x_vec);
}
Vector3 PredictPlayerPosition(Vector3 CurrentLocation, float Distance, Vector3 Velocity)
{
	Vector3 CalculatedPosition = CurrentLocation;
	float ProjectileSpeed = 60000.f;//60000;
	float ProjectileGravity = 3.0f;//3.0f;
	if (!ProjectileSpeed) return CalculatedPosition;


	float TimeToTarget = Distance / custom_fabsf(ProjectileSpeed);
	float CalculatedBulletDrop = (((-2800) * 0.5f * (TimeToTarget))) * 10;


	CalculatedPosition.x += Velocity.x * (TimeToTarget) * 120;
	CalculatedPosition.y += Velocity.y * (TimeToTarget) * 120;
	CalculatedPosition.z += Velocity.z * (TimeToTarget) * 120;
	CalculatedPosition.z += custom_fabsf((-49000 / 50) * ProjectileGravity) / 2.0f * (TimeToTarget * 25);


	return CalculatedPosition;
}
namespace a
{
	class aim
	{
	public:
		void mouse_aim(double x, double y, int smooth, Vector3 bone, Vector3 velocity, Vector3 loc, std::string weapon)
		{
			float ScreenCenterX = (ImGui::GetIO().DisplaySize.x / 2);
			float ScreenCenterY = (ImGui::GetIO().DisplaySize.y / 2);


			double x_pred;
			double y_pred;
			int AimSpeed = smooth;
			float TargetX = 0;
			float TargetY = 0;



			if (aimbot->prediction && weapon == _("Reaper Sniper Rifle"))
			{
				auto dist = loc.Distance(bone) / 100;
				auto target = PredictPlayerPosition(bone, dist, velocity);


				x_pred = uengine->w2s(target).x;
				y_pred = uengine->w2s(target).y;
			}
			else
			{
				x_pred = x;
				y_pred = y;
			}
			


				if (x_pred != 0)
				{
					if (x_pred > ScreenCenterX)
					{
						TargetX = -(ScreenCenterX - x_pred);
						TargetX /= AimSpeed;
						if (TargetX + ScreenCenterX > ScreenCenterX * 2) TargetX = 0;
					}

					if (x_pred < ScreenCenterX)
					{
						TargetX = x_pred - ScreenCenterX;
						TargetX /= AimSpeed;
						if (TargetX + ScreenCenterX < 0) TargetX = 0;
					}
				}

				if (y_pred != 0)
				{
					if (y_pred > ScreenCenterY)
					{
						TargetY = -(ScreenCenterY - y_pred);
						TargetY /= AimSpeed;
						if (TargetY + ScreenCenterY > ScreenCenterY * 2) TargetY = 0;
					}

					if (y_pred < ScreenCenterY)
					{
						TargetY = y_pred - ScreenCenterY;
						TargetY /= AimSpeed;
						if (TargetY + ScreenCenterY < 0) TargetY = 0;
					}
				}


			


			float targetx_min = TargetX - 1;
			float targetx_max = TargetX + 1;

			float targety_min = TargetY - 1;
			float targety_max = TargetY + 1;

			float offset_x = RandomFloat(targetx_min, targetx_max);
			float offset_y = RandomFloat(targety_min, targety_max);

			SonyDriverHelper::api::MouseMove(static_cast<int>((float)offset_x), static_cast<int>((float)offset_y / 1));
			return;
		}

		__forceinline auto aimbot_function(uintptr_t entity, uintptr_t gworld, uintptr_t playercontroller, uintptr_t pawnprivate, Vector3 velocity, Vector3 loc, std::string weapon) -> void
		{
			Vector2 bone{};


			auto test = ioctl.read<uintptr_t>(entity + offsets->rootcomp);
			auto Velocity = ioctl.read<Vector3>(test + offsets->velocity);


			uintptr_t currentactormesh = ioctl.read<uintptr_t>(entity + offsets->mesh);

			auto head = uengine->bonewithrotation(currentactormesh, bones->humanneck);

			if (aimbot->bone == 0) {
				bone = uengine->w2s(uengine->bonewithrotation(currentactormesh, bones->humanneck));
			}
			else if (aimbot->bone == 1) {
				bone = uengine->w2s(uengine->bonewithrotation(currentactormesh, bones->humanneck));
			}
			else if (aimbot->bone == 2) 
			{
				/*Vector2 chest_left = uengine->w2s(uengine->bonewithrotation(currentactormesh, bones->humanchestright));
				Vector2 chest_right = uengine->w2s(uengine->bonewithrotation(currentactormesh, bones->humanchestleft));
				bone = { chest_left.x + (chest_right.x - chest_left.x) / 2, chest_left.y };*/
				bone = uengine->w2s(uengine->bonewithrotation(currentactormesh, bones->humanspine2));
			}

			if (bone.x != 0 || bone.y != 0)
			{
				bone = uengine->w2s(uengine->bonewithrotation(currentactormesh, bones->humanneck));
				if ((aimbot->triggerbot) && (weapon.find(_("Shotgun")) != std::string::npos) && (GetAsyncKeyState(aimbot->triggerkey) < 0)) {

					if ((ioctl.read<uintptr_t>(playercontroller + offsets->targetedpawn))) //TargetedFortPawn
					{
						if (has_clicked) {
							tb_begin = std::chrono::steady_clock::now();
							has_clicked = 0;
						}
						tb_end = std::chrono::steady_clock::now();
						tb_time_since = std::chrono::duration_cast<std::chrono::milliseconds>(tb_end - tb_begin).count();
						if (tb_time_since >= aimbot->triggerspeed * 10) {
							mouse_event(MOUSEEVENTF_LEFTDOWN, DWORD(NULL), DWORD(NULL), DWORD(0x0002), ULONG_PTR(NULL));
							mouse_event(MOUSEEVENTF_LEFTUP, DWORD(NULL), DWORD(NULL), DWORD(0x0004), ULONG_PTR(NULL));
							has_clicked = 1;
						}
					}
				}
				mouse_aim(bone.x, bone.y, aimbot->smoothing, head, Velocity, loc, weapon);

				


				if (aimbot->playerfreeze)
				{
					if ((GetAsyncKeyState(aimbot->key) < 0)) 
					{
						if (ioctl.write<float>(pawnprivate + 0x68, 0.0f))
						{
						//	std::cout << "black\n";
						}//CustomTimeDilation;
					}
					else
					{
						ioctl.write<float>(pawnprivate + 0x68, 1.0f); //CustomTimeDilation;
					}	
				}
			}
		}

	};
}






static a::aim* aim = new a::aim();
#endif 
