#pragma once
#include <Windows.h>
#include <iostream>
#include <fstream>
#include "offsets.h"
#include "ue4.h"
#include "ue4_enums.h"
#include "ue4_functions.h"
#include "util.h"

namespace core
{
	GObjects* GlobalObjects;
	static void** GWorld;

	void* Controller;
	void* Pawn;

	static DWORD WINAPI Inputs(LPVOID lpParam)
	{
		/*
		auto Jump_Object = GlobalObjects->FindObjectByFullName("Function /Script/Engine.Character.Jump");

		while (1)
		{
			if (GetKeyState(VK_SPACE) & 0x8000)
			{
				UE4::ProcessEvent(Pawn, Jump_Object, nullptr, 0);
			}

			Sleep(1000 / 60);
		}
		*/

		return 0;
	}
	

	static void LoadAthena()
	{
		Controller = UE4::GetFirstPlayerController(*GWorld);

		// -------------
		// Cheat Manager
		void* cheatManager = UE4::StaticConstructObject_Internal
		(
			GlobalObjects->FindObjectByFullName("Class /Script/Engine.CheatManager")
			, Controller
			, nullptr
			, 0
			, 0
			, nullptr
			, false
			, nullptr
			, false
		);
		// -------------
		// Cheat Manager

		// -------------
		// Summon
		auto Summon_offset = GlobalObjects->FindObjectByFullName("Function /Script/Engine.CheatManager.Summon");

		if (Summon_offset)
		{
			struct
			{
				FString ClassName;
			} SummonParams;

			SummonParams.ClassName = L"PlayerPawn_Athena_C";

			UE4::ProcessEvent(cheatManager, Summon_offset, &SummonParams, 0);

			std::cout << "Summoned!" << std::endl;
		}
		// -------------
		// Summon


		Pawn = GlobalObjects->FindObject("PlayerPawn_Athena_C_");

		// Possess
		// -------------
		auto Possess_offset = GlobalObjects->FindObjectByFullName("Function /Script/Engine.Controller.Possess");

		if (Possess_offset)
		{
			if (Pawn)
			{
				struct
				{
					void* InPawn;
				} PossessParams;

				PossessParams.InPawn = Pawn;

				UE4::ProcessEvent(Controller, Possess_offset, &PossessParams, 0);

				std::cout << "Possessed!" << std::endl;
			}
		}
		// -------------
		// Possess


		// ServerReadyToStartMatch
		// -------------
		auto ServerReadyToStartMatch_offset = GlobalObjects->FindObjectByFullName("Function /Script/FortniteGame.FortPlayerController.ServerReadyToStartMatch");

		if (ServerReadyToStartMatch_offset)
		{
			UE4::ProcessEvent(Controller, ServerReadyToStartMatch_offset, nullptr, 0);

			std::cout << "Controller is ready to start match!" << std::endl;
		}
		// -------------
		// ServerReadyToStartMatch


		// -------------
		// StartMatch
		auto StartMatch_offset = GlobalObjects->FindObjectByFullName("Function /Script/Engine.GameMode.StartMatch");

		if (StartMatch_offset)
		{
			auto mode = GlobalObjects->FindObject("Athena_GameMode_C_");

			if (mode)
			{
				UE4::ProcessEvent(mode, StartMatch_offset, nullptr, 0);

				std::cout << "Started match!" << std::endl;
			}
		}
		// -------------
		// StartMatch

		std::cout << "Done!" << std::endl;

		// -------------
		// Extras
		auto K2_TeleportTo_offset = GlobalObjects->FindObjectByFullName("Function /Script/Engine.Actor.K2_TeleportTo");

		if (K2_TeleportTo_offset)
		{
			struct
			{
				FVector DestLocation;
				FRotator DestRotation;
			} TeleportParams;

			TeleportParams.DestLocation = FVector{ 4024.503662, -631.651001, 3533.149902 }; // Salty Springs
			TeleportParams.DestRotation = FRotator{ 1,1,1 };

			UE4::ProcessEvent(Pawn, K2_TeleportTo_offset, &TeleportParams, 0);

			std::cout << "Teleported to Salty Springs!" << std::endl;
		}


		auto DestroyAll_offset = GlobalObjects->FindObjectByFullName("Function /Script/Engine.CheatManager.DestroyAll");

		if (DestroyAll_offset)
		{
			struct
			{
				void* aClass;
			} DestroyParams;

			DestroyParams.aClass = GlobalObjects->FindObject("FortHLODSMActor");

			UE4::ProcessEvent(cheatManager, DestroyAll_offset, &DestroyParams, 0);

			std::cout << "Destroyed all instances of FortHLODSMActor!" << std::endl;
		}
		// -------------
		// Extras

		// Character
		auto PlayerState = ReadPointer(Controller, 0x248);
		if (PlayerState)
		{
			auto head = GlobalObjects->FindObjectByFullName("CustomCharacterPart /Game/Characters/CharacterParts/Male/Medium/Heads/CP_Athena_Head_M_AshtonMilo.CP_Athena_Head_M_AshtonMilo");
			auto body = GlobalObjects->FindObjectByFullName("CustomCharacterPart /Game/Athena/Heroes/Meshes/Bodies/CP_Athena_Body_M_AshtonMilo.CP_Athena_Body_M_AshtonMilo");

			AFortPlayerState* MyFortPlayerState = reinterpret_cast<AFortPlayerState*>(PlayerState);

			MyFortPlayerState->Parts[1] = head;
			MyFortPlayerState->Parts[2] = body;

			auto CharacterParts_offset = GlobalObjects->FindObjectByFullName("Function /Script/FortniteGame.FortPlayerState.OnRep_CharacterParts");

			if (CharacterParts_offset)
			{
				UE4::ProcessEvent(PlayerState, CharacterParts_offset, nullptr, 0);
			}
		}

		auto obj = GlobalObjects->FindObjectByFullName("BP_CattusDoggus_Scripting_C /Game/Athena/Maps/Athena_POI_Foundations.Athena_POI_Foundations.PersistentLevel.BP_CattusDoggus_Scripting_2");
		auto func1 = GlobalObjects->FindObjectByFullName("Function /Game/Athena/Prototype/Blueprints/Cattus/BP_CattusDoggus_Scripting.BP_CattusDoggus_Scripting_C.LoadCattusLevel");
		auto func2 = GlobalObjects->FindObjectByFullName("Function /Game/Athena/Prototype/Blueprints/Cattus/BP_CattusDoggus_Scripting.BP_CattusDoggus_Scripting_C.startevent");
		struct
		{
			bool Condition;
		} FirstParams;
		FirstParams.Condition = true;

		UE4::ProcessEvent(obj, func1, &FirstParams, 0);
		UE4::ProcessEvent(obj, func2, nullptr, 0);

		// GlobalObjects->Test();

		std::cout << "Done!" << std::endl;
	};

	static void Setup()
	{
		GlobalObjects = reinterpret_cast<GObjects*>(util::GET_BASE_ADDRESS() + GOBJECTS_OFFSET);
		GWorld = reinterpret_cast<void**>(util::GET_BASE_ADDRESS() + GWORLD_OFFSET);

		LoadAthena();
	};
}