#pragma once
#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "../../../includes.hpp"
#include "overlay/menu/render.hpp"
#include "../aimbot/movement/controller.hpp"
#include "../aimbot/function.hpp"
#include "sdk/engine/decrypt.hpp"

#include <string>

namespace cach
{
	class c
	{
	public:
		uintptr_t
			gworld,
			gameinstance,
			localplayers,
			localplayer,
			playercontroller,
			acknowledgedpawn,
			entityarray,
			persistentlevel,
			playerstate,
			gamestate,
			currentweapon,
			mesh,
			rootcomp,
			holdingweapon,
			WorldSettings,
			WorldGravityZ;
		int
			entitycount;

		Vector3
			velocity,
			relativelocation;

		char
			teamid;
	};
}

typedef struct Actors {
	uintptr_t Actor;
	uintptr_t Player;
	uintptr_t Velocity;
	DWORD_PTR IsVisible;
	uintptr_t SkeletalMesh;
	uintptr_t PlayerState;
	uintptr_t CurrentWeapon;
	char bIsKnocked;
	int TeamIndex;
}Actors;

std::vector<Actors> inline ActorList;

std::vector<Actors> ActorTempList;


static cach::c* cached = new cach::c();
static ImColor visible_color; 
bool debug = true;

Vector3 head_loc;
Vector3 velocity;
Vector3 loc;

float distancee;



namespace loop
{
	class entity
	{
	public:
		__forceinline auto aimbot_thread() -> void
		{
			float ClosestDistance = FLT_MAX;
			uintptr_t closestPawn = NULL;
			for (Actors Actor : ActorList)
			{
				Vector2 Headbox = uengine->w2s(Vector3(uengine->bonewithrotation(Actor.SkeletalMesh, 110).x, uengine->bonewithrotation(Actor.SkeletalMesh, 110).y, uengine->bonewithrotation(Actor.SkeletalMesh, 110).z + 15));
				
				auto dx = Headbox.x - (globals->width / 2);
				auto dy = Headbox.y - (globals->height / 2);
				auto dist = sqrtf(dx * dx + dy * dy);
				if (uengine->visibilitycheck(Actor.SkeletalMesh))
				{
					if (dist < aimbot->fovsize && dist < ClosestDistance) {

						ClosestDistance = dist;
						closestPawn = Actor.Actor;
					}
				}
				

			}
			if (aimbot->enable && closestPawn && (GetAsyncKeyState(aimbot->key) < 0))
			{

				aim->aimbot_function(closestPawn, cached->gworld, cached->playercontroller, closestPawn, cached->velocity, cached->relativelocation, decryptcached->weaponname);
			}
		}

		__forceinline auto cache() -> void
		{
			ActorTempList.clear();

			cached->gworld = ioctl.read<uintptr_t>(globals->imagebase + offsets->gworld);

			
			
			cached->gamestate = ioctl.read<uintptr_t>(cached->gworld + offsets->gamestate);

			cached->gameinstance = ioctl.read<uintptr_t>(cached->gworld + offsets->gameinstance);


			cached->localplayers = ioctl.read<uintptr_t>(cached->gameinstance + offsets->localplayers);

			cached->localplayer = ioctl.read<uintptr_t>(cached->localplayers);

			cached->playercontroller = ioctl.read<uintptr_t>(cached->localplayer + offsets->playercontroller);

			cached->acknowledgedpawn = ioctl.read<uintptr_t>(cached->playercontroller + offsets->acknowledgedpawn);

			cached->playerstate = ioctl.read<uintptr_t>(cached->acknowledgedpawn + offsets->playerstate);

			cached->rootcomp = ioctl.read<uintptr_t>(cached->acknowledgedpawn + offsets->rootcomp);

			cached->relativelocation = ioctl.read<Vector3>(cached->rootcomp + offsets->relativelocation);

			cached->teamid = ioctl.read<int>(cached->playerstate + offsets->teamid);

			cached->entityarray = ioctl.read<uintptr_t>(cached->gamestate + offsets->entityarray);

			cached->entitycount = ioctl.read<int>(cached->gamestate + (offsets->entityarray + sizeof(uintptr_t)));

			cached->velocity = ioctl.read<Vector3>(cached->rootcomp + offsets->velocity);

			cached->holdingweapon = ioctl.read<uintptr_t>(cached->acknowledgedpawn + offsets->currentweapon);
		//	std::cout << "holding weapon -> " << cached->holdingweapon << "\n";
			//if (debug)
			//{
			//	std::cout << "cached->gworld -> " << cached->gworld << std::endl;

			//	std::cout << "cached->gamestate -> " << cached->gamestate << std::endl;

			//	std::cout << "cached->gameinstance -> " << cached->gameinstance << std::endl;

			//	std::cout << "cached->persistentlevel -> " << cached->persistentlevel << std::endl;

			//	std::cout << "cached->localplayers -> " << cached->localplayers << std::endl;

			//	std::cout << "cached->localplayer -> " << cached->localplayer << std::endl;

			//	std::cout << "cached->playercontroller -> " << cached->playercontroller << std::endl;

			//	std::cout << "cached->acknowledgedpawn -> " << cached->acknowledgedpawn << std::endl;

			//	std::cout << "cached->playerstate -> " << cached->playerstate << std::endl;

			//	std::cout << "cached->rootcomp -> " << cached->rootcomp << std::endl;

			//	//std::cout << "cached->relativelocation -> " << cached->relativelocation;

			//	std::cout << "cached->entityarray -> " << cached->entityarray << std::endl;

			//	std::cout << "cached->entitycount -> " << cached->entitycount << std::endl;

			//	debug = false;
			//}
			


			for (int i = 0; i < cached->entitycount; ++i)
			{

				auto player = ioctl.read<uintptr_t>(cached->entityarray + (i * sizeof(uintptr_t)));

				auto current_actor = ioctl.read<uintptr_t>(player + offsets->pawnpriv);


				auto rootcomp = ioctl.read<uintptr_t>(current_actor + offsets -> rootcomp);
				auto location  = ioctl.read <Vector3> (rootcomp + offsets->relativelocation);

				if ( (!uengine -> screencheck(uengine -> w2s ( location ) )) && (!ioctl.read<uintptr_t>(current_actor + offsets->vehicle)) ) continue;

				Actors FortniteActor{ };

				FortniteActor.Player = player;

				FortniteActor.Actor = current_actor;

				if (!current_actor) continue;

				if (current_actor == cached->acknowledgedpawn) continue;
				
				FortniteActor.TeamIndex = ioctl.read<int>(player + offsets->teamid);

				FortniteActor.CurrentWeapon = ioctl.read<uintptr_t>(player + offsets->currentweapon);

				if (cached->acknowledgedpawn) {
					if (FortniteActor.TeamIndex == cached->teamid) continue;
				}


				FortniteActor.SkeletalMesh = ioctl.read<uintptr_t>(current_actor + offsets->mesh);

				FortniteActor.PlayerState = ioctl.read<uintptr_t>(current_actor + offsets->playerstate);



				/*std::cout << "FortniteActor.Player -> " << FortniteActor.Player << std::endl;

				std::cout << "FortniteActor.Actor -> " << FortniteActor.Actor << std::endl;

				std::cout << "FortniteActor.TeamIndex -> " << FortniteActor.TeamIndex << std::endl;

				std::cout << "FortniteActor.CurrentWeapon -> " << FortniteActor.CurrentWeapon << std::endl;

				std::cout << "FortniteActor.SkeletalMesh -> " << FortniteActor.SkeletalMesh << std::endl;

				std::cout << "FortniteActor.PlayerState -> " << FortniteActor.PlayerState << std::endl;*/
				//std::cout << "FortniteActor.SkeletalMesh -> " << FortniteActor.SkeletalMesh << std::endl;
				//printf("mesh -> %llx\n", FortniteActor.SkeletalMesh);
				ActorTempList.push_back(FortniteActor);
			}
			ActorList.clear();
			ActorList = ActorTempList;
		}
		
		
		__forceinline auto loop() -> void
		{
			visualiser->watermark();

			if (misc->fps) {
				visualiser->fps();
			}

			if (aimbot->fov)
			{
				visualiser->draw_fov();
			}

			if (globals->developermode) 
			{
				ShowWindow(GetConsoleWindow(), SW_SHOW);
				misc->bones = true;


			}
			
			for (Actors Actor : ActorList)
			{
				decryptcached->weaponname = decrypt->weapon(cached->holdingweapon);
				//std::cout << "weapon -> " << decryptcached->weaponname << std::endl;
				Vector2 Headbox = uengine->w2s(Vector3(uengine->bonewithrotation(Actor.SkeletalMesh, 110).x, uengine->bonewithrotation(Actor.SkeletalMesh, 110).y, uengine->bonewithrotation(Actor.SkeletalMesh, 110).z + 15));
				Vector2 bottom = uengine->w2s(uengine->bonewithrotation(Actor.SkeletalMesh, 0));
				
				//printf("Headbox.x -> %d\n Headbox.y -> %d\n", Headbox.x, Headbox.y);


				float CornerHeight = abs(Headbox.y - bottom.y);
				float CornerWidth = CornerHeight * 0.450;

				distancee = camera.location.Distance(uengine->bonewithrotation(Actor.SkeletalMesh, 68)) / 100;


				/*if ()
				{
					uintptr_t AActorRootComponent = ioctl.read<uintptr_t>(Actor.Actor + offsets->rootcomp);
					ioctl.write<Vector3>(AActorRootComponent + offsets->relativelocation, Vector3(cached->relativelocation.x, cached->relativelocation.y, cached->relativelocation.z + 200));
				}*/

				if (uengine->visibilitycheck(Actor.SkeletalMesh))
				{
					visible_color = ImGui::GetColorU32({ colours->visible_color_float[0], colours->visible_color_float[1], colours->visible_color_float[2],  1.0f });
					//visuals->enemyIDvis += 1;
				}
				else
				{
					visible_color = ImGui::GetColorU32({ colours->invisible_color_float[0], colours->invisible_color_float[1], colours->invisible_color_float[2],  1.0f });
				}

				if (misc->bones)
				{
					Vector2 bone_screen;
					for (int bone = 0; bone < 110; bone++)
					{
						bone_screen = uengine->w2s(uengine->bonewithrotation(Actor.SkeletalMesh, bone));
						utility->DrawString(20 ,bone_screen.x, bone_screen.y, ImColor(255, 0, 0, 255), false, true, "%d", bone);
					}
				}

				if (visuals->enable)
				{
					if (visuals->head)
					{
						
						visualiser->draw_head(head_loc, camera.fov, camera.location, cached->acknowledgedpawn, visible_color);
					}
					if (visuals->skeleton)
					{
						visualiser->draw_skeleton(Actor.SkeletalMesh, visible_color);
					}
					if (visuals->box) {
						
						if (visuals->regular)
						{
							
							if (visuals->outlinedbox)
							{
								visualiser->TwoD(Headbox.x - (CornerWidth / 2), Headbox.y, CornerWidth, CornerHeight, ImColor(0, 0, 0, 255), visuals->box_thickness + 2);
							}
							if (visuals->filled)
							{
								utility->DrawFilledRect(Headbox.x, Headbox.y, CornerWidth, CornerHeight, ImColor(0, 0, 0, 125));
							}
							visualiser->TwoD(Headbox.x - (CornerWidth / 2), Headbox.y, CornerWidth, CornerHeight, visible_color, visuals->box_thickness);
						}
						else if (visuals->cornered)
						{

							if (visuals->outlinedbox)
							{
								visualiser->draw_cornered_box(Headbox.x - CornerWidth / 2, Headbox.y, CornerWidth, CornerHeight, visuals->box_thickness + 2, ImColor(0, 0, 0, 255));
							}
							visualiser->draw_cornered_box(Headbox.x - CornerWidth / 2, Headbox.y, CornerWidth, CornerHeight, visuals->box_thickness, visible_color);
						}
						else if (visuals->rounded)
						{
							visualiser->rounded(Headbox, CornerWidth, CornerHeight, visible_color, visuals->box_thickness);
						}

					}

					if (visuals->distance)
					{
						visualiser->draw_distance(distancee, bottom, visible_color);
					}

					if (visuals->chams)
					{
						auto materials = ioctl.read<TArray<uintptr_t>>(Actor.Player + 0x5c40);
						for (int i = 0; i < materials.Size(); i++)
						{
							auto MaterialInstance = materials[i];

							auto Material = ioctl.read<uintptr_t>(MaterialInstance + 0x18);

							ioctl.write_bits<uintptr_t>(Material + 0x1b8, uintptr_t(), 6); //Wireframe is the 6th index
						}

					}

					if ( visuals -> username && visuals -> platform )
					{
						visualiser -> draw_username_platform ( Headbox, Actor.Player, visible_color );
					}
					
					else if (visuals -> username)
					{
						visualiser -> draw_username( Headbox, Actor.Player, visible_color );
					}


					else if ( visuals -> platform )
					{
						visualiser -> draw_platform( Headbox, Actor.Player, visible_color );
					}

					if ( visuals -> weapon )
					{
						visualiser -> draw_weapon( Headbox, bottom, Actor.CurrentWeapon, visible_color );
					}

				}
			}
		}
	};
}

static loop::entity* entity = new loop::entity();

#endif