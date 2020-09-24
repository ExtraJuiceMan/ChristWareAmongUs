#include "il2cpp-appdata.h"
#include <imgui.h>
#include <vector>
#include "GameUtility.hpp"

using namespace app;

ImVec4 AmongUsColorToImVec4(Color color)
{
    return ImVec4(color.r, color.g, color.b, color.a);
}

ImVec4 AmongUsColorToImVec4(CorrectedColor32 color)
{
    return ImVec4(color.r / 255.0F, color.g / 255.0F, color.b / 255.0F, color.a / 255.0F);
}

uint8_t GetNextColor(uint8_t c)
{
    c++;

    if (c >= (*Palette__TypeInfo)->static_fields->PlayerColors->max_length)
    {
        c = 0;
    }

    return c;
}

std::vector<PlayerControl*> GetAllPlayers()
{
    std::vector<PlayerControl*> vec = std::vector<PlayerControl*>();

    auto playerList = (*PlayerControl__TypeInfo)->static_fields->AllPlayerControls;

    for (int i = 0; i < List_1_PlayerControl__get_Count(playerList, NULL); i++)
        vec.push_back(List_1_PlayerControl__get_Item(playerList, i, NULL));

    return vec;
}

GameData_PlayerInfo* GetPlayerData(PlayerControl* player)
{
    return PlayerControl_get_Data(player, NULL);
}

bool CheckColorAvailable(uint8_t color)
{
    auto players = GetAllPlayers();
    for (auto player : players)
    {
        if (GetPlayerData(player)->fields.ColorId == color)
            return false;
    }

    return true;
}

bool IsInGame()
{
    return (*AmongUsClient__TypeInfo)->static_fields->Instance->fields._.GameState == InnerNetClient_GameStates__Enum_Joined
        || (*AmongUsClient__TypeInfo)->static_fields->Instance->fields._.GameState == InnerNetClient_GameStates__Enum_Started;
}

CorrectedColor32 GetPlayerColor(uint8_t colorId)
{
    CorrectedColor32* colorArray = (CorrectedColor32*)(*Palette__TypeInfo)->static_fields->PlayerColors->vector;
    return colorArray[colorId];
}