#include "il2cpp-appdata.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <set>
#include <queue>
#include <optional>
#include "IterationCounter.hpp"

using namespace app;

namespace CWState
{
    HWND Window;
    bool ImGuiInitialized = false;
    bool ShowMenu = false;

    bool NoClip = false;
    bool MarkImposters = false;
    bool ShowRadar = false;
    float RadarZoom = 8.0F;

    bool SpamChat = false;
    bool AllPlayersSpam = false;
    char SpamMessage[256] = "";

    bool PlayerList = false;

    bool ColorShift = false;
    int CurrentColor = 0;

    bool ModifySpeed = false;
    float SpeedModifier = 10.0F;

    bool ModifyKillCooldown = false;
    float KillCooldownModifier = 0.001F;

    bool ModifyKillDistance = false;
    int KillDistanceModifier = 2;

    bool ModifyLight = false;
    float LightModifier = 10.0F;

    bool PinDoors = false;

    std::set<SystemTypes__Enum> PinnedDoors;
    std::queue<PlayerControl*> MurderQueue;
    std::optional<uint8_t> ColorTarget = std::nullopt;
    std::optional<PlayerControl*> MurderTarget = std::nullopt;
    std::optional<PlayerControl*> VoteTarget = std::nullopt;
    std::optional<PlayerControl*> KickTarget = std::nullopt;

    bool ShiftAllColors = false;
    bool ShiftAllClothes = false;

    IterationCounter ChatCounter = IterationCounter(12);
    IterationCounter AllClothesCounter = IterationCounter(18);
    IterationCounter AllColorsCounter = IterationCounter(2);
    IterationCounter ColorCounter = IterationCounter(12);
}