#pragma once
#ifndef ESP_H
#define ESP_H


#include <cstdint>
#include <ImGui/imgui.h>
#include "../utilities/globals.h"
#include "../utilities/Math.h"


struct Offsets {
	uintptr_t health = 0xEC;
	uintptr_t armor = 0x00F0;
	uintptr_t mtpAmmo = 0x0140;
	uintptr_t mtpClip = 0x0140;
	uintptr_t NadesNumber = 0x0144;
	uintptr_t MkModIsActive = 0x0100;
	uintptr_t MkModsAmmo = 0x0148;
	uintptr_t i_team = 0x30C;
	uintptr_t name = 0x204;
	uintptr_t viewAngles = 0x34;
	uintptr_t pos = 0x04;

};

struct Address
{
	uintptr_t localPlayer = 0x0018AC00;
	uintptr_t ViewMatrix = 0x0057DFD0;
	uintptr_t entList = 0x0018AC04;
	uintptr_t numOfPlayers = 0x58AC0C;

};

struct Ent
{
	short team;
};

class ESP
{
public:
	Vec3 GetEnnemyPos();
	//bool IsEnnemy();
	//bool IsTeammate();
	void DrawLines(bool showTeam, float* color);
	int GetPlayersInGame();
	void DrawBox(bool showTeam, float* EnnemiColor, float* TeamColor);
	void DrawHealthBar();


	//bool isEntTeam(Ent* ent);
	

};


#endif

