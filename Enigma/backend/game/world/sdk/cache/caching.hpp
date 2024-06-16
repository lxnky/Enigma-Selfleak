#include "../../../../../includes.hpp"


namespace ca
{
	class caching
	{
	public:
		
		__forceinline auto viewpoint() -> void
		{
			std::thread( [&]() 
			{ 
				for (;;) 
				{ 
					uengine->viewpoint(cached->gworld, cached->playercontroller); 
					std::this_thread::sleep_for(std::chrono::milliseconds(10));
				} 
			} ).detach();
		}
		__forceinline auto actors( ) -> void
		{
			std::thread( [&](  )
			{
				for (;;)
				{
					entity->cache();
					std::this_thread::sleep_for(std::chrono::milliseconds(200));
				}
			} ).detach(  );
		}

		__forceinline auto aimcahce() -> void
		{
			std::thread([&]()
				{
					for (;;)
					{
						entity->aimbot_thread();
						std::this_thread::sleep_for(std::chrono::milliseconds(10));
					}
				}).detach();
		}
		__forceinline auto menu() -> void
		{
			std::thread([&]()
				{
					//for (;;)
					//{
						render->draw();
						std::this_thread::sleep_for(std::chrono::milliseconds(10));
					//}
				}).detach();
		}
		//__forceinline auto loot() -> void
		//{
		//	std::thread([&]()
		//		{
		//			loot_cache->CacheLevels(cached->acknowledgedpawn, cached->gworld);
		//			//loot_cache->LevelsLoop(cached->gworld, cached->relativelocation);
		//		}).detach();
		//}

		__forceinline auto start() -> void
		{
			actors();
			viewpoint();
			//loot();
			aimcahce();
		}
	};
}
static ca::caching* caching = new ca::caching;