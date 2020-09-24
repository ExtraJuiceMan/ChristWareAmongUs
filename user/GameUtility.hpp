#pragma once
#include "il2cpp-appdata.h"
#include <imgui.h>
#include <vector>

using namespace app;

// The output of il2cppdumper is incorrect for this struct
// it's actually a union with int32_t rgba and an rgba struct
// but we won't actually use the union here
struct CorrectedColor32 {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

ImVec4 AmongUsColorToImVec4(Color color);

ImVec4 AmongUsColorToImVec4(CorrectedColor32 color);

uint8_t GetNextColor(uint8_t c);

std::vector<PlayerControl*> GetAllPlayers();

GameData_PlayerInfo* GetPlayerData(PlayerControl* player);

bool CheckColorAvailable(uint8_t color);

bool IsInGame();

CorrectedColor32 GetPlayerColor(uint8_t colorId);