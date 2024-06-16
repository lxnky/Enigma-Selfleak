#pragma once
#ifndef OFFSETS_HPP
#define OFFSETS_HPP

struct o
{
	uintptr_t gworld = 0x126cf528;

	uintptr_t gameinstance = 0x1d8; //struct UGameInstance* OwningGameInstance;

	uintptr_t localplayers = 0x38; //struct TArray<struct ULocalPlayer*> LocalPlayers;

	uintptr_t playerstate = 0x2b0; //struct APlayerState* PlayerState; 

	uintptr_t persistentlevel = 0x30; //struct ULevel* PersistentLevel;

	uintptr_t acknowledgedpawn = 0x338; //struct APawn* AcknowledgedPawn;

	uintptr_t playercontroller = 0x30; //struct APlayerController* PlayerController;

	uintptr_t rootcomp = 0x198; //struct USceneComponent* RootComponent;

	uintptr_t gamestate = 0x160; //struct AGameStateBase* GameState;

	uintptr_t entitycount = 0xA8; //struct TArray<struct AActor*> OutActors ??

	uintptr_t pawnpriv = 0x308; //struct APawn* PawnPrivate;

	uintptr_t entityarray = 0x2A8; //struct TArray<struct APlayerState*> PlayerArray;

	uintptr_t relativelocation = 0x120; //struct Vector3 RelativeLocation;

	uintptr_t velocity = 0x168; //struct FVector ComponentVelocity;

	uintptr_t mesh = 0x318; //struct USkeletalMeshComponent* Mesh;

	uintptr_t vehicle = 0x2960; //struct AActor* CurrentVehicle;

	uintptr_t teamid = 0x1211; //char TeamIndex; (AFortPlayerStateAthena : AFortPlayerStateZone)

	uintptr_t weapondata = 0x4f0; //struct UFortWeaponItemDefinition* WeaponData;

	uintptr_t displayname = 0x98; //??

	uintptr_t length = 0x40; //??

	uintptr_t currentweapon = 0xa20; //struct AFortWeapon* CurrentWeapon;

	uintptr_t comptowrld = 0x1c0; // NOT IN SDK

	uintptr_t bonearray = 0x5B0;// NOT IN SDK

	uintptr_t bonecache = 0x5F8; // NOT IN SDK
	uintptr_t bonec = 0x60;// NOT IN SDK

	uintptr_t targetedpawn = 0x18A0;

	uintptr_t namepoolchunk = 0x10EFDA00; // NOT IN SDK

	uintptr_t lastsubmit = 0x2E8; // NOT IN SDK
	uintptr_t lastrender = 0x2F0; // NOT IN SDK

	uintptr_t dying = 0x758;

	uintptr_t dbno = 0x93a;

	uintptr_t settingsrecord = 0x390;

	uintptr_t hud = 0x4c0;


	struct b
	{
		int humanbase = 0;
		int	humanpelvis = 2;
		int	humanlthigh1 = 71;
		int	humanlthigh2 = 77;
		int	humanlthigh3 = 72;
		int	humanlcalf = 74;
		int	humanlfoot = 86;
		int humanlfoot2 = 86;
		int	humanltoe = 76;
		int	humanrthigh1 = 78;
		int	humanrthigh2 = 84;
		int	humanrthigh3 = 79;
		int humanrfoot2 = 82;
		int	humanrcalf = 81;
		int	humanrfoot = 87;
		int	humanrtoe = 83;
		int	humanspine1 = 7;
		int	humanspine2 = 5;
		int	humanspine3 = 2;
		int	humanlcollarbone = 9;
		int	humanlupperarm = 35;
		int	humanlforearm1 = 36;
		int humanchestright = 37;
		int humanchestleft = 8;
		int	humanlforearm23 = 10;
		int	humanlforearm2 = 34;
		int	humanlforearm3 = 33;
		int	humanlpalm = 32;
		int humanlhand = 11;
		int	humanrcollarbone = 98;
		int	humanrupperarm = 64;
		int	humanrforearm1 = 65;
		int	humanrforearm23 = 39;
		int humanrhand = 40;
		int	humanrforearm2 = 63;
		int	humanrforearm3 = 62;
		int	humanrpalm = 58;
		int	humanneck = 67;
		int	humanhead = 68;
		int	humanchest = 66;
	};
};

std::unique_ptr<o> offsets = std::make_unique<o>();
std::unique_ptr<o::b> bones = std::make_unique<o::b>();

#endif
