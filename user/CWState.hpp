#pragma once

#include "il2cpp-appdata.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <set>
#include <queue>
#include <optional>

using namespace app;

namespace CWState
{
    extern HWND Window;
    extern bool ImGuiInitialized;
    extern bool ShowMenu;

    extern bool NoClip;
    extern bool MarkImposters;
    extern bool ShowRadar;
    extern float RadarZoom;

    extern bool SpamChat;
    extern bool AllPlayersSpam;
    extern char SpamMessage[256];

    extern bool PlayerList;

    extern bool ColorShift;
    extern int CurrentColor;

    extern bool ModifySpeed;
    extern float SpeedModifier;

    extern bool ModifyKillCooldown;
    extern float KillCooldownModifier;

    extern bool ModifyKillDistance;
    extern int KillDistanceModifier;

    extern bool ModifyLight;
    extern float LightModifier;

    extern bool PinDoors;

    extern std::set<SystemTypes__Enum> PinnedDoors;
    extern std::queue<PlayerControl*> MurderQueue;
    extern std::optional<uint8_t> ColorTarget;
    extern std::optional<PlayerControl*> MurderTarget ;
    extern std::optional<PlayerControl*> VoteTarget;
    extern std::optional<PlayerControl*> KickTarget;

    extern bool ShiftAllColors;
    extern bool ShiftAllClothes;

    extern IterationCounter ChatCounter;
    extern IterationCounter AllClothesCounter;
    extern IterationCounter AllColorsCounter;
    extern IterationCounter ColorCounter;
}