#include "Cheats.h"
#include <tlhelp32.h>
#include <iostream>
#include "SigScan.h"
#include "Patcher.h"


bool isLocalPlayer = false;
int localPlayers = 0;

bool bRunOnce = true;


const char* CallCmdSetBodyPreferenceSig = "\x55\x48\x8b\xec\x48\x83\xec\x30\x48\x89\x4d\xF8\x48\x89\x55\xF0\x48\x8d\x64\x24\x00\x90\x49\xbb\x00\x00\x00\x00\x00\x00\x00\x00\x41\xff\xd3\x48\x8d\x65\x00\x5d\xc3\x00\x00\x00\x00\x00\x00\x00\x01\x04\x00\x05\x00\x00\x00\x00\x00\xe8\x72";
const char* CallCmdSetBodyPrefMask = "xxxxxxxxxxxxxxxxxxxx?xxx????????xxxxxx?xxxxxxxxxxx?x????xxx";
HMODULE GetUnityPlayerModule()
{
	HMODULE hUnityPlayer = NULL;

	HANDLE ths = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
	if (ths != INVALID_HANDLE_VALUE)
	{
		MODULEENTRY32 me;
		me.dwSize = sizeof(me);

		if (Module32First(ths, &me))
		{
			do
			{

				if (strcmp(me.szModule, "UnityPlayer.dll") == 0) {

					hUnityPlayer = me.hModule;

				}

			} while (Module32Next(ths, &me));

		}
		CloseHandle(ths);
	}

	return hUnityPlayer;
}

GameObjectManager* pGOM = 0;


bool IsLocalPlayer(CharacterBody* characterBody)
{
			
	NetworkIdentity* networkIdentity = characterBody->GetNetworkIdentity();

	if (networkIdentity) {

		char* isLocalPlayer = networkIdentity->GetHasAuthority();

		if (*isLocalPlayer == 1) {
			localPlayers++;
			return true;
		}
	}

	return false;


}


void NoCooldown(CharacterBody* characterBody)
{
	

	SkillLocator* skills = characterBody->GetSkillLocator();
	if (skills) {
		Skill* primary = skills->GetPrimary();
		Skill* secondary = skills->GetSecondary();
		Skill* utility = skills->GetUtility();
		Skill* special = skills->GetSpecial();

		if (primary) { float* primaryTimer = primary->GetTimer(); *primaryTimer = 99.0f; }
		if (secondary) { float* secondaryTimer = secondary->GetTimer(); *secondaryTimer = 99.0f; };
		if (utility) { float* utilityTimer = utility->GetTimer(); *utilityTimer = 99.0f; };
		if (special) { float* specialTimer = special->GetTimer(); *specialTimer = 99.0f; };
	
	}

	
}

void SuperReacher(Components* components)
{
	Params* params = components->GetInteractorParams();

	if (params) {
		Interactor* interactor = params->GetInteractor();

		if (interactor) {
			float* maxInteractDistance = interactor->GetMaxInteractionDistance();
			*maxInteractDistance = 99999.0f;
		}
	}

}

void ChangeFOV(Components* components) 
{
	Params* params = components->GetCameraTargetParamsParams();

	if (params) {
		CameraTargetParams* cameraTargetParams = params->GetCameraTargetParams();

		if (cameraTargetParams) {
			float* fovOverride = cameraTargetParams->GetFOVOverride();
			*fovOverride = UserInterface::general_fov;
		}
	}
}

void TeamDamage(CharacterBody* characterBody)
{


	TeamComponent* teamComponent = characterBody->GetTeamComponent();
	if (teamComponent) {
		char* teamIndex = teamComponent->GetTeamIndex();
		*teamIndex = (char)UserInterface::general_teamnumber;
	}

}

void ModifyMovement(CharacterBody* characterBody)
{

	CharacterMotor* characterMotor = characterBody->GetCharacterMotor();

	if (characterMotor) {

		float* walkSpeed = characterMotor->GetWalkSpeedPenaltyCoefficient();
		float* mass = characterMotor->GetMass();
		float* airControl = characterMotor->GetAirControl();
		char* gravity = characterMotor->GetUseGravity();
		char* fly = characterMotor->GetIsFlying();
		char* jump = characterMotor->GetJumpCount();

		*walkSpeed = UserInterface::movement_walkspeed;
		*mass = UserInterface::movement_mass;
		*airControl = UserInterface::movement_aircontrol;

		*gravity = (UserInterface::movement_gravity) ? 1 : 0;
		*fly = (UserInterface::movement_fly) ? 1 : 0;
		*jump = (UserInterface::movement_jumpcount) ? 0 : 1;
	
	}
}

void PatchMPCharacterSelect()
{
	const char* modifySig = "\x2D\x02\x26\x2A\x02\x7B\x23\x26\x00\x04";
	const char* modifySigMask = "xxxxxxxxxx";
	const char newBytes[] = "\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x7B\x31\x10\x00\x04\x6F\x98\x11\x00\x06\x2A";

	uintptr_t modifyLocation = FindPattern((char*)modifySig, (char*)modifySigMask, 0x0);
	std::cout << "Modify Location: " << modifyLocation << std::endl;
	if (modifyLocation) {
		Patch((void*)modifyLocation, (void*)newBytes, sizeof(newBytes));
		std::cout << "Location modified " << sizeof(newBytes) << " bytes" << std::endl;
	}


}



// TODO: Fix to be less hacky
void PatchCallCmdSetBodyPreference()
{

	uintptr_t location = FindPattern((char*)CallCmdSetBodyPreferenceSig, (char*)CallCmdSetBodyPrefMask, 0x2);
	std::cout << "Location: " << std::hex << location << std::endl;

}

void CheatLoop()
{
	if (UserInterface::character_select_enabled) {

		if (bRunOnce) {
			PatchMPCharacterSelect();
			bRunOnce = false;
		}

	}
	if (UserInterface::character_change) {

		pGOM = *(GameObjectManager**)((uintptr_t)GetUnityPlayerModule() + 0x01563A28);
		BaseObject* currentBase = pGOM->GetActiveObject();
		GameObject* currentObject = currentBase->GetGameObject();
		BaseObject* lastBase = pGOM->GetLastActiveObject();
		GameObject* lastObject = lastBase->GetGameObject();

		int count = 0;

		while (currentObject) {

			/*if ((std::string(currentObject->ReadName()).find("Character") != std::string::npos) || 
				(std::string(currentObject->ReadName()).find("UI") != std::string::npos) ||
				(std::string(currentObject->ReadName()).find("Survivor") != std::string::npos) ||
				(std::string(currentObject->ReadName()).find("Icon") != std::string::npos) ||
				(std::string(currentObject->ReadName()).find("(Clone)") != std::string::npos) ||
				(std::string(currentObject->ReadName()).find("BodyCatalog") != std::string::npos))
				std::cout << currentObject->ReadName() << " " << std::hex << currentBase << std::endl;*/
			bRunOnce = false;
			//std::cout << currentObject->ReadName() << std::endl;
			if ((std::string(currentObject->ReadName()).find("CharacterSelectUI") != std::string::npos)) {

				std::cout << currentObject->ReadName() << " " << std::hex << currentBase << std::endl;

				Components* components = currentObject->GetComponents();
				if (components) {

					Params* params = components->GetCharacterSelectControllerParams();

					if (params) {

						CharacterSelectController* characterSelectController = params->GetCharacterSelectController();

						if (characterSelectController) {

							NetworkUser* networkUser = characterSelectController->GetNetworkUser();

							if (networkUser) {

								int* bodyIndexPreference = networkUser->GetBodyIndexPreference();

								//std::cout << "bodyIndexPreference: " << std::dec << *bodyIndexPreference << std::endl;

								*bodyIndexPreference = UserInterface::character_select;

								




							}

						}

					}

				}



			}

			count++;

			if (currentObject == lastObject)
				break;

			if (currentBase) {
				currentBase = currentBase->GetNextObjectLink();
				currentObject = currentBase->GetGameObject();
			}

		}
		std::cout << "Items: " << std::dec << count << std::endl;

	}
	if (UserInterface::enabled) {

		

		pGOM = *(GameObjectManager**)((uintptr_t)GetUnityPlayerModule() + 0x01563A28);

		BaseObject* currentBase = pGOM->GetTaggedObject();
		GameObject* currentObject = currentBase->GetGameObject();

		//std::cout << "currentBase" << std::hex << currentBase << std::endl;
		//std::cout << "currentObject" << std::hex << currentObject << std::endl;

		BaseObject* lastBase = pGOM->GetLastTaggedObject();
		GameObject* lastObject = lastBase->GetGameObject();

		//std::cout << "lastBase" << std::hex << lastBase << std::endl;
		//std::cout << "lastObject" << std::hex << lastObject << std::endl;
		// Test
		//BaseObject* currentBase = pGOM->GetActiveObject();
		//GameObject* currentObject = currentBase->GetGameObject();
		//BaseObject* lastBase = pGOM->GetLastActiveObject();
		//GameObject* lastObject = lastBase->GetGameObject();

		//char log[256];
		//sprintf(log, " currentBase: [%p]\n currentObject: [%p]\n lastBase: [%p]\n lastObject: [%p]\n", currentBase, currentObject, lastBase, lastObject);
		//printf(log);

		int count = 0;
		//std::cout << "Cheat loop" << std::endl;
		while (currentObject) {

			

			if ((std::string(currentObject->ReadName()).find("(Clone)") != std::string::npos)){// ||
				//(std::string(currentObject->ReadName()).find("CommandoBody") != std::string::npos) ||
				//(std::string(currentObject->ReadName()).find("ToolbotBody") != std::string::npos) ||
				//(std::string(currentObject->ReadName()).find("EngiBody") != std::string::npos) ||
				//(std::string(currentObject->ReadName()).find("MageBody") != std::string::npos) ||
				//(std::string(currentObject->ReadName()).find("MercBody") != std::string::npos) ||
				//(std::string(currentObject->ReadName()).find("TreebotBody") != std::string::npos)) { // Find other characters too*/


				count += 1;
				//std::cout << "Character found" << std::endl;


				Components* components = currentObject->GetComponents();
				if (components) {


					//std::cout << "Components found" << std::endl;
					//Params* params = components->GetParams();
					CharacterBody* characterBody = 0;
					Params* params = 0;
					if ((std::string(currentObject->ReadName()).find("EngiBody") != std::string::npos) ||
						(std::string(currentObject->ReadName()).find("AssassinBody") != std::string::npos)) {
						params = components->GetEngiCharacterBodyParams();
						//std::cout << "Engineer found" << std::endl;
					}
					else {
						params = components->GetCharacterBodyParams();
						//std::cout << "Other found" << std::endl;
					}
					if (params) {
						CharacterBody* characterBody = params->GetCharacterBody();

						if (characterBody) {

							if (IsLocalPlayer(characterBody)) {


								//std::cout << "IsLocalPlayer" << std::endl;


								
								if (UserInterface::general_cooldown) {

									NoCooldown(characterBody);
								}

								if (UserInterface::general_teamdamage) {

									TeamDamage(characterBody);

								}
								if (UserInterface::movement_enable) {

									ModifyMovement(characterBody);

								}
								



								if (UserInterface::general_reach) {

									SuperReacher(components);
								}
								if (UserInterface::general_enableFOV) {

									ChangeFOV(components);

								}



							}
						}
					}

				}
				//}
			}

			//std::cout << currentObject->ReadName() << std::endl;
			//std::cout << "Local player count: " << localPlayers << std::endl;

			if (currentObject == lastObject)
				break;

			if (currentBase) {
				currentBase = currentBase->GetNextObjectLink();
				currentObject = currentBase->GetGameObject();
			}
		}

		localPlayers = 0;

	}
}




