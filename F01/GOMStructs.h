#pragma once

// f8 28 42

struct CharacterMotor
{
	float* GetWalkSpeedPenaltyCoefficient() { return (float*)(this + 0x60); }
	float* GetMuteWalkMotion() { return (float*)(this + 0x64); }
	float* GetMass() { return (float*)(this + 0x68); }
	float* GetAirControl() { return (float*)(this + 0x6C); }
	char* GetDisableAirControlUntilCollision() { return (char*)(this + 0x70); }
	char* GetGenerateParametersOnAwake() { return (char*)(this + 0x71); }
	char* GetUseGravity() { return (char*)(this + 0x72); }
	char* GetIsFlying() { return (char*)(this + 0x73); }
	char* GetAlive() { return (char*)(this + 0x74); }
	float* GetRestStopwatch() { return (float*)(this + 0x78); }
	float* GetPreviousPosition() { return (float*)(this + 0x7C); }
	char* GetHasEffectiveAuthority() { return (char*)(this + 0x88); }
	char* GetJumpCount() { return (char*)(this + 0x8C); }
	char* GetNetIsGrounded() { return (char*)(this + 0x90); }
	char* GetNetGroundedNormal() { return (char*)(this + 0x94); }
	float* GetVelocity() { return (float*)(this + 0xA0); }
	float* GetLastVelocity() { return (float*)(this + 0xAC); }
	float* GetRootMotion() { return (float*)(this + 0xB8); }
	float* GetMoveDirection() { return (float*)(this + 0xC4); }
	float* GetLastGroundedTime() { return (float*)(this + 0xD0); }
	int* GetSafeCollisionEnableCount() { return (int*)(this + 0xD4); }
};



struct Skill
{
	float* GetTimer() { return (float*)(this + 0x7C); }
};

struct SkillLocator
{
	Skill* GetPrimary() { return *(Skill**)(this + 0x30); }
	Skill* GetSecondary() { return *(Skill**)(this + 0x38); }
	Skill* GetUtility() { return *(Skill**)(this + 0x40); }
	Skill* GetSpecial() { return *(Skill**)(this + 0x48); }
};
struct Interactor
{
	float* GetMaxInteractionDistance() { return (float*)(this + 0x30); }
};
struct CameraTargetParams
{
	float* GetFOVOverride() { return (float*)(this + 0x40); }
};
struct TeamComponent
{
	char* GetTeamIndex() { return (char*)(this + 0x41); }
};

// Look into networkidentity

struct NetworkIdentity
{
	char* GetLocalPlayerAuthority() { return (char*)(this + 0x5D); }
	char* GetHasAuthority() { return (char*)(this + 0x60); }
};

struct CharacterBody
{
	// TODO: Clean up
	TeamComponent* GetTeamComponent() {
		void* teamComponentField = *(void**)(this + 0xA0);
		void* paramField = *(void**)((char*)teamComponentField + 0x10);
		return *(TeamComponent**)((char*)paramField + 0x28);
	}
	SkillLocator* GetSkillLocator() { return *(SkillLocator**)(this + 0xC0); }
	CharacterMotor* GetCharacterMotor() { return *(CharacterMotor**)(this + 0x90); }
	char* GetIsPlayerControlled() { return (char*)(this + 0x201); }
	NetworkIdentity* GetNetworkIdentity() { return *(NetworkIdentity**)(this + 0x88); }
};



struct NetworkUser
{

	int* GetBodyIndexPreference() { return (int*)(this + 0xB8); }

};

struct CharacterSelectController
{

	NetworkUser* GetNetworkUser() {
		void* localUser = *(void**)(this + 0x28);
		return *(NetworkUser**)((char*)localUser + 0x28);
	}


};
struct Params
{

	void* GetParams() { return *(void**)(this + 0x28); }

	Interactor* GetInteractor() { return *(Interactor**)(this + 0x28); }
	CameraTargetParams* GetCameraTargetParams() { return *(CameraTargetParams**)(this + 0x28); }
	CharacterBody* GetCharacterBody() { return *(CharacterBody**)(this + 0x28); }
	CharacterSelectController* GetCharacterSelectController() { return *(CharacterSelectController**)(this + 0x28); }
};


struct Components
{
	Params* GetInteractorParams() { return *(Params**)(this + 0x118); }
	Params* GetCameraTargetParamsParams() { return *(Params**)(this + 0x58); }
	Params* GetTeamComponentParams() { return *(Params**)(this + 0xF8); }
	Params* GetCharacterMotorParams() { return *(Params**)(this + 0x28); }
	Params* GetCharacterBodyParams() { return *(Params**)(this + 0x48); }
	Params* GetEngiCharacterBodyParams() { return *(Params**)(this + 0x58); }
	Params* GetCharacterSelectControllerParams() { return *(Params**)(this + 0x68); }
};
struct GameObject
{
	Components* GetComponents() { return *(Components**)(this + 0x30); }
	char* ReadName() { return *(char**)(this + 0x60); }
};
struct BaseObject
{
	BaseObject* GetNextObjectLink() { return *(BaseObject**)(this + 0x8); }
	GameObject* GetGameObject() { return *(GameObject**)(this + 0x10); }
};
struct GameObjectManager
{
	BaseObject* GetLastTaggedObject() { return *(BaseObject**)(this + 0x0); }
	BaseObject* GetTaggedObject() { return *(BaseObject**)(this + 0x8); }
	BaseObject* GetLastActiveObject() { return *(BaseObject**)(this + 0x10); }
	BaseObject* GetActiveObject() { return *(BaseObject**)(this + 0x18); }
};

