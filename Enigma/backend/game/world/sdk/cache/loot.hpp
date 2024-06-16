//#include "../../../../../includes.hpp"
//#include "../../entity.hpp"
//uintptr_t RootComponentt(uintptr_t actor)
//{
//	return ioctl.read <uintptr_t>(actor + offsets->rootcomp);
//}
//Vector3 GetLocationn(uintptr_t actor)
//{
//	return  ioctl.read <Vector3>(RootComponentt(actor) + offsets->relativelocation);
//}
//#define FNAMEPOOL_OFFSET 0x1198BD00
//#define debug_loot true
//class item {
//public:
//	uintptr_t
//		Actor;
//
//	std::string
//		Name,
//		isVehicle,
//		isChest,
//		isLamma,
//		isPickup,
//		isAmmoBox;
//	float
//		distance;
//
//	Vector2
//		screenloc;
//
//	Vector3
//		pos;
//};
//std::vector<item> item_pawns;
//
//typedef struct _LootEntity {
//	std::string GNames;
//	uintptr_t ACurrentItem;
//}LootEntity;
//
//static std::vector<LootEntity> LootentityList;
//
//
//class World
//{
//public:
//	static std::string GetNameFromIndex(int key)
//	{
//		// Calculate offsets and pointers based on the provided key
//		uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
//		uint16_t NameOffset = (uint16_t)key;
//		uint64_t NamePoolChunk = ioctl.read<uint64_t>(globals->imagebase + FNAMEPOOL_OFFSET + (8 * ChunkOffset) + 16) + (unsigned int)(2 * NameOffset);
//		uint16_t nameEntry = ioctl.read<uint16_t>(NamePoolChunk);
//		int nameLength = nameEntry >> 6;
//		char buff[1024] = {};
//
//		char* v2; // Pointer to the buffer
//		int v4; // Length of the name
//		__int16 result; // Temporary result for each character
//		int v6; // Loop counter
//		int v7; // Temporary variable
//		int v8; // Temporary variable
//		unsigned int v9; // Temporary variable
//
//		v2 = buff;
//		v4 = nameLength;
//		v6 = 0;
//		v7 = 24;
//
//		// If the name has characters, read them and perform a transformation
//		if (v4)
//		{
//			ioctl.read_process((NamePoolChunk + 2), (PVOID)buff, 2 * nameLength);
//			do
//			{
//				v8 = v6++;
//				v9 = (v8 | 0xB000) + v7;
//				result = v9 ^ ~*v2;
//				v7 = v9 >> 2;
//				*v2++ = result;
//			} while (v6 < v4);
//
//			buff[nameLength] = '\0';
//			return std::string(buff);
//		}
//		return std::string("");
//	}
//
//	// Function to get a string name from FName using the index or another index if the length condition is met
//	static std::string fname(int key)
//	{
//		uint32_t ChunkOffset = (uint32_t)((int)(key) >> 16);
//		uint16_t NameOffset = (uint16_t)key;
//
//		uint64_t NamePoolChunk = ioctl.read<uint64_t>(globals->imagebase + FNAMEPOOL_OFFSET + (8 * ChunkOffset) + 16) + (unsigned int)(2 * NameOffset);
//
//		// If the length condition is met, get another index and use it to get the name
//		if (ioctl.read<uint16_t>(NamePoolChunk) < 64)
//		{
//			auto a1 = ioctl.read<DWORD>(NamePoolChunk + 2);
//			return GetNameFromIndex(a1);
//		}
//		else
//		{
//			return GetNameFromIndex(key);
//		}
//	}
//}; static World* world = new World();
//
//
//Vector3 Location;
//Vector2 ChestPosition;
//
//
//class loott
//{
//public:
//	static auto CacheLevels(uintptr_t pawn, uintptr_t uworld) -> void {
//		// Function to continuously cache information about in-game items
//		while (true) {
//			//std::cout << "pawn -> " << pawn << std::endl;
//			//std::cout << "uworld -> " << uworld << std::endl;
//			if (cached->acknowledgedpawn)
//			{
//				if (!(loot->draw_chests || loot->pickups || loot->draw_ammo) || !cached->gworld) {
//					//std::cout << "blackies\n";
//					std::this_thread::sleep_for(std::chrono::milliseconds(1200));
//					continue;
//					// Check if necessary conditions are met, then iterate over in-game items and cache relevant information
//					// Sleep for a short duration before repeating the process
//				}
//				
//				std::vector<item> mrxd;
//				mrxd.reserve(100);
//				uintptr_t ItemLevels = ioctl.read<uintptr_t>(cached->gworld + 0x170);
//				DWORD itemSize = ioctl.read<DWORD>(cached->gworld + (0x170 + sizeof(PVOID)));
//
//				for (int i = 0; i < itemSize; ++i) {
//					uintptr_t ItemLevel = ioctl.read<uintptr_t>(ItemLevels + (i * sizeof(uintptr_t)));
//
//					for (int j = 0; j < ioctl.read<DWORD>(ItemLevel + (0xA0 + sizeof(PVOID))); ++j) {
//						uintptr_t ItemsPawns = ioctl.read<uintptr_t>(ItemLevel + 0xA0);
//						uintptr_t CurrentItemPawn = ioctl.read<uintptr_t>(ItemsPawns + (j * sizeof(uintptr_t)));
//
//						Vector3 ItemPosition = GetLocationn(CurrentItemPawn);
//						float ItemDist = camera.location.Distance(ItemPosition) / 100.f;
//
//						int ItemIndex = ioctl.read<int>(CurrentItemPawn + 0x18);
//						auto CurrentItemPawnName = world->fname(ItemIndex);
//
//						/*if (CurrentItemPawnName == "FortPickupAthena") 
//						{ */
//							Location = GetLocationn(CurrentItemPawn);
//							ChestPosition = uengine->w2s(Location);
//							//std::cout << CurrentItemPawnName << std::endl; 
//							
//							///*std::cout << "x -> " << ChestPosition.x << std::endl;
//
//							//std::cout << "y -> " << ChestPosition.y << std::endl;*/
//							//ImVec2 text_size = ImGui::CalcTextSize("black");
//							//utility->DrawString(18, ItemPosition.x - (text_size.x / 2), ItemPosition.y - 18, ImColor(255, 255, 255), true, true, "black");
//						//}
//						//std::cout << CurrentItemPawnName << "\n";
//						
//						bool shouldUpdate = false;
//						bool bIsPickup = false;
//
//						if (ioctl.is_valid(CurrentItemPawn) && (loot->pickups || loot->draw_ammo || loot->draw_chests)) {
//							bIsPickup = true;
//							shouldUpdate = true;
//						}
//
//						if (shouldUpdate) {
//							item item{};
//							item.Actor = CurrentItemPawn;
//							item.Name = CurrentItemPawnName;
//							item.isPickup = bIsPickup;
//							item.distance = ItemDist;
//							mrxd.push_back(item);
//						}
//						
//					}
//					
//				}
//				ImGui::GetBackgroundDrawList()->AddText(ImVec2(ChestPosition.x, ChestPosition.y), ImColor(255, 255, 255), "nerd");
//				item_pawns.clear();
//				item_pawns = mrxd;
//				std::this_thread::sleep_for(std::chrono::milliseconds(10));
//			}
//		}
//	}
//
//
//
//
//	void LevelDrawing() {
//		// Copy the list of cached items
//		//auto levelListCopy = item_pawns;
//		
//		// Iterate over each item in the copied list and draw relevant information on the screen
//		/*for (;;)
//		{
//			if (!item_pawns.empty())
//			{
//				std::cout << "real\n";
//			}
//			else
//			{
//				std::cout << "blackass\n";
//			}
//			
//		}*/
//		
//		//for (auto entity : item_pawns)
//		//{
//		//	std::cout << "in da loop\n";
//		//	//if (arrays->acknowledged_pawn)
//		//	//{
//		//	//	// Check various conditions and draw information based on item type and distance
//		//	//	if (loot->draw_chests && strstr(entity.Name.c_str(), _xor_("Tiered_Chest").c_str()) || strstr(entity.Name.c_str(), _xor_("AlwaysSpawn_NormalChest").c_str()) || strstr(entity.Name.c_str(), _xor_("AlwaysSpawn_RareChest").c_str())) {
//		//	//		if (entity.distance <= loot->chestrender)
//		//	//		{
//		//	//			fvector Location = GetLocation(entity.Actor);
//		//	//			fvector2d Screen = ue5->w2s(Location);
//		//	//			if (ue5->in_screen(Screen))
//		//	//			{
//		//	//				std::string wtf2 = ("Chest [") + std::to_string((int)entity.distance) + ("m]");
//		//	//				ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
//
//		//	//				g_draw->draw_string(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(245, 245, 0), true, true, wtf2.c_str());
//		//	//			}
//		//	//		}
//		//	//	}
//
//		//	//	if (loot->draw_ammo && strstr(entity.Name.c_str(), _xor_("Tiered_Ammo").c_str())) {
//		//	//		if (entity.distance <= loot->ammo_render)
//		//	//		{
//		//	//			fvector Location = GetLocation(entity.Actor);
//		//	//			fvector2d Screen = ue5->w2s(Location);
//
//		//	//			if (ue5->in_screen(Screen))
//		//	//			{
//		//	//				std::string wtf2 = ("Ammobox [") + std::to_string((int)entity.distance) + ("m]");
//		//	//				ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
//		//	//				g_draw->draw_string(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(192, 192, 192), true, true, wtf2.c_str());
//		//	//			}
//		//	//		}
//		//	//	}
//
//		//	//	if (loot->draw_vehicles && strstr(entity.Name.c_str(), _xor_("Valet_SportsCar_Vehicle_C").c_str())) {
//		//	//		if (entity.distance <= loot->vehiclesrender)
//		//	//		{
//		//	//			fvector Location = GetLocation(entity.Actor);
//		//	//			fvector2d Screen = ue5->w2s(Location);
//
//		//	//			if (ue5->in_screen(Screen))
//		//	//			{
//		//	//				std::string wtf2 = ("Sports Car [") + std::to_string((int)entity.distance) + ("m]");
//		//	//				ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
//		//	//				g_draw->draw_string(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(192, 192, 192), true, true, wtf2.c_str());
//		//	//			}
//		//	//		}
//		//	//	}
//
//		//	//	if (loot->draw_vehicles && strstr(entity.Name.c_str(), _xor_("Motorcycle_DirtBike_Vehicle_C").c_str())) {
//		//	//		if (entity.distance <= loot->vehiclesrender)
//		//	//		{
//		//	//			fvector Location = GetLocation(entity.Actor);
//		//	//			fvector2d Screen = ue5->w2s(Location);
//
//		//	//			if (ue5->in_screen(Screen))
//		//	//			{
//		//	//				std::string wtf2 = ("Motorcycle [") + std::to_string((int)entity.distance) + ("m]");
//		//	//				ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
//		//	//				g_draw->draw_string(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(192, 192, 192), true, true, wtf2.c_str());
//		//	//			}
//		//	//		}
//		//	//	}
//
//		//	//	if (loot->draw_cooler && strstr(entity.Name.c_str(), _xor_("Cooler_Container_C").c_str()))
//		//	//	{
//		//	//		if (entity.distance <= loot->cooler_render)
//		//	//		{
//		//	//			fvector Location = GetLocation(entity.Actor);
//		//	//			fvector2d Screen = ue5->w2s(Location);
//
//		//	//			if (ue5->in_screen(Screen))
//		//	//			{
//		//	//				std::string wtf2 = ("Cooler Container [") + std::to_string((int)entity.distance) + ("m]");
//		//	//				ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
//		//	//				g_draw->draw_string(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(186, 242, 239), true, true, wtf2.c_str());
//		//	//			}
//		//	//		}
//		//	//	}
//
//		//	//	if (loot->draw_vehicles && strstr(entity.Name.c_str(), _xor_("Valet_BasicSUV_Vehicle_C").c_str()))
//		//	//	{
//		//	//		if (entity.distance <= loot->vehiclesrender)
//		//	//		{
//		//	//			fvector Location = GetLocation(entity.Actor);
//		//	//			fvector2d Screen = ue5->w2s(Location);
//		//	//			if (ue5->in_screen(Screen))
//		//	//			{
//		//	//				std::string wtf2 = ("SUV [") + std::to_string((int)entity.distance) + ("m]");
//		//	//				ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
//		//	//				g_draw->draw_string(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 255, 255), true, true, wtf2.c_str());
//		//	//			}
//		//	//		}
//		//	//	}
//
//		//	//	if (loot->lamma && strstr(entity.Name.c_str(), _xor_("AthenaSupplyDrop_Llama_C").c_str()))
//		//	//	{
//		//	//		if (entity.distance <= loot->lammarender)
//		//	//		{
//		//	//			fvector Location = GetLocation(entity.Actor);
//		//	//			fvector2d Screen = ue5->w2s(Location);
//		//	//			if (ue5->in_screen(Screen))
//		//	//			{
//		//	//				std::string wtf2 = ("Lamma [") + std::to_string((int)entity.distance) + ("m]");
//		//	//				ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
//		//	//				g_draw->draw_string(13, Screen.x - (text_size.x / 2), Screen.y - 18, ImColor(255, 0, 255), true, true, wtf2.c_str());
//		//	//			}
//		//	//		}
//		//	//	}
//		//	
//		//		if (loot->pickups && strstr(entity.Name.c_str(), _("FortPickupAthena")))
//		//		{
//		//			
//		//			if (entity.distance <= loot->pickupsrender)
//		//			{
//		//				auto definition = ioctl.read <uint64_t>(entity.Actor + 0x350 + 0x18); // AFortPickup::PrimaryPickupItemEntry : FFortItemEntry::ItemDefinition -> 0x18
//		//				std::cout << "def -> " << definition << "\n";
//		//				if (ioctl.is_valid(definition))
//		//				{
//		//					BYTE tier = ioctl.read <BYTE>(definition + 0x11b); //UFortItemDefinition::Tier
//		//					ImColor Color, RGBAColor;
//		//					Vector3 Location = GetLocationn(entity.Actor);
//		//					Vector2 ChestPosition = uengine->w2s(Location);
//		//					uint64_t ftext_ptr = ioctl.read <uint64_t>(definition + 0x30);
//		//					std::cout << "IM REAL BLACK\n";
//		//					if (ioctl.is_valid(ftext_ptr))
//		//					{
//		//						std::cout << "IM BLACK\n";
//		//						uint64_t ftext_data = ioctl.read <uint64_t>(ftext_ptr + 0x28);
//		//						int ftext_length = ioctl.read <int>(ftext_ptr + 0x30);
//		//						if (ftext_length > 0 && ftext_length < 50)
//		//						{
//		//							wchar_t* ftext_buf = new wchar_t[ftext_length];
//		//							ioctl.read_process(ftext_data, ftext_buf, ftext_length * sizeof(wchar_t));
//		//							wchar_t* WeaponName = ftext_buf;
//		//							delete[] ftext_buf;
//
//		//							std::string Text = utility->wchar_to_char(WeaponName);
//		//							std::string wtf2 = Text + " [" + std::to_string((int)entity.distance) + ("m]");
//		//							if (tier == 2 && (loot->Common))
//		//							{
//		//								Color = ImColor(0, 255, 0);
//		//							}
//		//							else if ((tier == 3) && (loot->rare))
//		//							{
//		//								Color = ImColor(0, 150, 255);
//		//							}
//		//							else if ((tier == 4) && (loot->purple))
//		//							{
//		//								Color = ImColor(255, 0, 255);
//		//							}
//		//							else if ((tier == 5) && (loot->gold))
//		//							{
//		//								Color = ImColor(255, 255, 0);
//		//							}
//		//							else if ((tier == 6) && (loot->mythic))
//		//							{
//		//								Color = ImColor(245, 245, 0);
//		//							}
//
//		//							else if ((tier == 0) || (tier == 1) && (loot->UnCommon))
//		//							{
//		//								Color = ImColor(255, 255, 255);
//		//							}
//
//		//							ImVec2 text_size = ImGui::CalcTextSize(wtf2.c_str());
//		//							utility->DrawString(13, ChestPosition.x - (text_size.x / 2), ChestPosition.y - 18, Color, true, true, wtf2.c_str());
//		//						}
//		//					}
//		//				}
//		//			}
//		//		}
//			
//		//}
//	}
//
//}; static loott* loot_cache = new loott;
