#pragma once
#include "il2cpp-appdata.h"
#include <imgui.h>
#include <vector>

using namespace app;

ImVec4 AmongUsColorToImVec4(Color color);

ImVec4 AmongUsColorToImVec4(Color32 color);

uint8_t GetNextColor(uint8_t c);

std::vector<PlayerControl*> GetAllPlayers();

GameData_PlayerInfo* GetPlayerData(PlayerControl* player);

bool CheckColorAvailable(uint8_t color);