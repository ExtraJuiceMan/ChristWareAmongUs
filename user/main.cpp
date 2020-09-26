#include "il2cpp-appdata.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d11.h>
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <string>
#include <iostream>
#include <vector>
#include <set>
#include <queue>
#include <thread>
#include "GameUtility.hpp"
#include "StringUtility.hpp"
#include "IterationCounter.hpp"
#include "D3D11Hooking.hpp"
#include "CWState.hpp"
#include "CWConstants.hpp"
#include "magic_enum.hpp"
#include "detours.h"
#include "helpers.h"
#include "radar.hpp"

using namespace app;

extern const LPCWSTR LOG_FILE = L"il2cpp-log.txt";

IDXGISwapChain* SwapChain;
ID3D11Device* Device;
ID3D11DeviceContext* Ctx;
ID3D11RenderTargetView* RenderTargetView;

D3D_PRESENT_FUNCTION OriginalD3DFunction;
WNDPROC OriginalWndProcFunction;
void (*OriginalHudFunction)(MethodInfo*);

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

LRESULT CALLBACK WndProcHook(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    ImGuiIO& io = ImGui::GetIO();
    POINT mPos;
    GetCursorPos(&mPos);
    ScreenToClient(CWState::Window, &mPos);

    ImGui::GetIO().MousePos.x = mPos.x;
    ImGui::GetIO().MousePos.y = mPos.y;

    if (uMsg == WM_KEYUP && wParam == VK_DELETE)
        CWState::ShowMenu = !CWState::ShowMenu;

    if (CWState::ShowMenu)
        ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

    return CallWindowProcW(OriginalWndProcFunction, hWnd, uMsg, wParam, lParam);
}

HRESULT __stdcall D3D_FUNCTION_HOOK(IDXGISwapChain* pThis, UINT SyncInterval, UINT Flags)
{
    if (!CWState::ImGuiInitialized)
    {
        SwapChain = pThis;
        pThis->GetDevice(__uuidof(ID3D11Device), (void**)&Device);
        Device->GetImmediateContext(&Ctx);

        DXGI_SWAP_CHAIN_DESC desc;

        pThis->GetDesc(&desc);

        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        ImGui::StyleColorsDark();

        CWState::Window = desc.OutputWindow;

        ImGui_ImplWin32_Init(CWState::Window);
        ImGui_ImplDX11_Init(Device, Ctx);

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        OriginalWndProcFunction = (WNDPROC)SetWindowLongW(CWState::Window, GWLP_WNDPROC, (LONG)WndProcHook);

        ImGui::GetIO().ImeWindowHandle = CWState::Window;

        ID3D11Texture2D* pBackBuffer;
        pThis->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
        Device->CreateRenderTargetView(pBackBuffer, NULL, &RenderTargetView);
        pBackBuffer->Release();

        ImGui::GetStyle().WindowRounding = 0.0F;
        ImGui::GetStyle().ChildRounding = 0.0F;
        ImGui::GetStyle().FrameRounding = 0.0F;
        ImGui::GetStyle().GrabRounding = 0.0F;
        ImGui::GetStyle().PopupRounding = 0.0F;
        ImGui::GetStyle().ScrollbarRounding = 0.0F;

        CWState::ImGuiInitialized = true;
    }

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();

    if (CWState::ShowRadar) {
        Radar::RenderRadar(&CWState::ShowRadar, CWState::RadarZoom);
    }

    if (CWState::ShowMenu)
    {
        ImGui::PushStyleColor(ImGuiCol_TitleBg, CWConstants::CW_GOLD);
        ImGui::PushStyleColor(ImGuiCol_TitleBgActive, CWConstants::CW_GOLD);
        ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, CWConstants::CW_GOLD);
        ImGui::Begin("ChristWare: AmongUs", &CWState::ShowMenu);
        ImGui::PopStyleColor(3);

        ImGui::SetWindowSize(ImVec2(400, 600));

        ImGui::Text(CWConstants::CHRISTWARE_ASCII);
        ImGui::Text("\n\tI have the strength to face all conditions by\n\t\t\tthe power that Christ gives me.\n\n\t\t\t\t\tAmong Us Edition");

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Text("Author: ExtraConcentratedJuice");
        ImGui::Spacing();
        ImGui::Text("This cheat is free software.");
        ImGui::Text("If you paid for this software, demand a refund.");
        ImGui::Text("Source code released under the MIT License.");
        ImGui::Text("github.com/ExtraConcentratedJuice/ChristWareAmongUs");
        ImGui::Spacing();
        ImGui::Spacing();

        if (ImGui::CollapsingHeader("Game"))
        {    
        if (ImGui::Button("Call Meeting"))
            PlayerControl_CmdReportDeadBody((*PlayerControl__TypeInfo)->static_fields->LocalPlayer, NULL, NULL);

        if (ImGui::Button("End Meeting"))
            MeetingHud_ForceSkipAll((*MeetingHud__TypeInfo)->static_fields->Instance, NULL);
            
            ImGui::Checkbox("NoClip", &CWState::NoClip);
            ImGui::Checkbox("Mark Imposters", &CWState::MarkImposters);
            ImGui::Checkbox("Show Ghosts", &CWState::ShowGhosts);
            ImGui::Checkbox("Radar", &CWState::ShowRadar);
            ImGui::Text("Radar Zoom");
            ImGui::SliderFloat("##RadarZoom", &CWState::RadarZoom, 4.0F, 16.0F, "%.f", 1.0F);
        }

        if (ImGui::CollapsingHeader("Ban Points"))
        {
            if ((*StatsManager__TypeInfo)->static_fields->Instance == NULL)
            {
                ImGui::Text("Stats manager is not yet initialized.\nTry connecting to the public matchmaker.");
            }
            else
            {
                if (ImGui::Button("Reset"))
                {
                    (*StatsManager__TypeInfo)->static_fields->Instance->fields.banPoints = 0.0F;
                    StatsManager_SaveStats((*StatsManager__TypeInfo)->static_fields->Instance, NULL);
                }

                ImGui::Text(("Ban Points: " + std::to_string((*StatsManager__TypeInfo)->static_fields->Instance->fields.banPoints)).c_str());
            }
        }

        if (ImGui::CollapsingHeader("Chat"))
        {
            ImGui::Checkbox("All Players Spam", &CWState::AllPlayersSpam);
            ImGui::Checkbox("Spam Chat", &CWState::SpamChat);
            ImGui::Text("Message");
            ImGui::InputText("##SpamChatMessage", CWState::SpamMessage, IM_ARRAYSIZE(CWState::SpamMessage));
            ImGui::Text("Interval");
            CWState::ChatCounter.GenerateInput("##SpamChatInverval");
        }

        if (ImGui::CollapsingHeader("Players") && IsInGame())
        {
            if (GetAllPlayers().size() > 0
                && CWState::MurderQueue.empty())
            {
                if (ImGui::Button("Murder Crewmates"))
                {
                    for (auto player : GetAllPlayers())
                    {
                        if (!GetPlayerData(player)->fields.IsImpostor)
                            CWState::MurderQueue.push(player);
                    }
                }

                ImGui::SameLine();

                if (ImGui::Button("Murder Imposters"))
                {
                    for (auto player : GetAllPlayers())
                    {
                        if (GetPlayerData(player)->fields.IsImpostor)
                            CWState::MurderQueue.push(player);
                    }
                }
                
                ImGui::Spacing();
            }

            for (auto player : GetAllPlayers())
            {
                auto data = GetPlayerData(player);
                auto name = GetUTF8StringFromNETString(data->fields.PlayerName);

                ImVec4 nameColor;

                if (data->fields.IsImpostor)
                    nameColor = AmongUsColorToImVec4((*Palette__TypeInfo)->static_fields->ImpostorRed);
                else
                    nameColor = AmongUsColorToImVec4((*Palette__TypeInfo)->static_fields->CrewmateBlue);

                if (data->fields.IsDead)
                    nameColor = AmongUsColorToImVec4((*Palette__TypeInfo)->static_fields->DisabledGrey);

                if (ImGui::Button((std::string("Kick") + std::string("##") + name).c_str()))
                {
                    CWState::KickTarget = player;
                }

                ImGui::SameLine();

                if (CWState::VoteTarget.has_value() && player == CWState::VoteTarget.value())
                {
                    if (ImGui::Button((std::string("Next Vote") + std::string("##") + name).c_str()))
                    {
                        CWState::VoteTarget = std::nullopt;
                    }
                }
                else
                {
                    if (ImGui::Button((std::string("Vote Off") + std::string("##") + name).c_str()))
                    {
                        CWState::VoteTarget = player;
                    }
                }

                ImGui::SameLine();

                if (ImGui::Button((std::string("Teleport") + std::string("##") + name).c_str()))
                {
                    Transform* localTransform = Component_get_transform((Component*)(*PlayerControl__TypeInfo)->static_fields->LocalPlayer, NULL);
                    Transform* playerTransform = Component_get_transform((Component*)player, NULL);
                    Transform_set_position(localTransform, Transform_get_position(playerTransform, NULL), NULL);
                }

                ImGui::SameLine();
                    
                if (ImGui::Button((std::string("Murder") + std::string("##") + name).c_str()))
                    CWState::MurderTarget = player;

                ImGui::SameLine();
                   
                ImGui::TextColored(nameColor, name.c_str());
            }
        }

        if (ImGui::CollapsingHeader("Doors"))
        {
            if (CWState::PinDoors)
            {
                if (ImGui::Button("Select All"))
                {
                    for (auto entry : magic_enum::enum_values<SystemTypes__Enum>())
                    {
                        CWState::PinnedDoors.insert(entry);
                    }
                }
                
                ImGui::SameLine();

                if (ImGui::Button("Unselect All"))
                {
                    CWState::PinnedDoors.clear();
                }

                ImGui::Spacing();
            }

            ImGui::Checkbox("Keep Doors Closed", &CWState::PinDoors);

            if (CWState::PinDoors)
                ImGui::Indent(16.0F);

            for (auto entry : magic_enum::enum_entries<SystemTypes__Enum>())
            {
                if (CWState::PinDoors)
                {
                    bool checked = CWState::PinnedDoors.find(entry.first) != CWState::PinnedDoors.end();

                    ImGui::Checkbox((std::string(entry.second.data() + sizeof("SystemTypes__Enum")) + std::string("##Checkbox")).c_str(), &checked);

                    if (!checked)
                        CWState::PinnedDoors.erase(entry.first);
                    else
                        CWState::PinnedDoors.insert(entry.first);
                }
                else
                {
                    if (ImGui::Button((std::string(entry.second.data() + sizeof("SystemTypes__Enum")) + std::string("##Button")).c_str()))
                    {
                        ShipStatus_RpcCloseDoorsOfType((*ShipStatus__TypeInfo)->static_fields->Instance, entry.first, NULL);
                    }
                }
            }

            if (CWState::PinDoors)
                ImGui::Unindent(16.0F);
        }

        if (ImGui::CollapsingHeader("Repair"))
        {
            // yes i hardcoded the systems
            for (auto entry : CWConstants::SABOTAGE_SYSTEMS)
            {
                if (ImGui::Button((std::string(magic_enum::enum_name<SystemTypes__Enum>((SystemTypes__Enum)entry).data() + sizeof("SystemTypes__Enum")) + std::string("##RepairButton")).c_str()))
                {
                    ShipStatus_RepairSystem((*ShipStatus__TypeInfo)->static_fields->Instance, (SystemTypes__Enum)entry,
                        (*PlayerControl__TypeInfo)->static_fields->LocalPlayer, 0x80, NULL);
                }
            }
        }

        if (ImGui::CollapsingHeader("Sabotage"))
        {
            for (auto entry : CWConstants::SABOTAGE_SYSTEMS)
            {
                if (ImGui::Button((std::string(magic_enum::enum_name<SystemTypes__Enum>((SystemTypes__Enum)entry).data() + sizeof("SystemTypes__Enum")) + std::string("##SabotageButton")).c_str()))
                {
                    // yeah dude "repair system" my ass
                    ShipStatus_RpcRepairSystem((*ShipStatus__TypeInfo)->static_fields->Instance, SystemTypes__Enum_Sabotage, entry, NULL);
                }
            }
        }

        if (ImGui::CollapsingHeader("Color"))
        {
            ImGui::Checkbox("Rainbow Shift", &CWState::ColorShift);

            ImGui::Spacing();

            for (int i = 0; i < (*Palette__TypeInfo)->static_fields->PlayerColors->max_length; i++)
            {
                ImGui::PushStyleColor(ImGuiCol_Button, AmongUsColorToImVec4(GetPlayerColor(i)));
                if (ImGui::Button((std::string("Color ") + std::to_string(i)).c_str()))
                    CWState::ColorTarget = i;
                ImGui::PopStyleColor();

                if ((*PlayerControl__TypeInfo)->static_fields->LocalPlayer != NULL
                    && GetPlayerData((*PlayerControl__TypeInfo)->static_fields->LocalPlayer)->fields.ColorId == i)
                {
                    ImGui::SameLine();
                    ImGui::Text("Selected");
                }
            }
        }

        if ((*PlayerControl__TypeInfo)->static_fields->LocalPlayer != NULL
            && GetPlayerData((*PlayerControl__TypeInfo)->static_fields->LocalPlayer) != NULL
            && GetPlayerData((*PlayerControl__TypeInfo)->static_fields->LocalPlayer)->fields.Tasks != NULL
            && ImGui::CollapsingHeader("Tasks"))
        {
            List_1_GameData_TaskInfo_* tasks = GetPlayerData((*PlayerControl__TypeInfo)->static_fields->LocalPlayer)->fields.Tasks;

            if (List_1_GameData_TaskInfo__get_Count(tasks, NULL) > 0 
                && ImGui::Button("Complete All Tasks"))
            {
                for (int i = 0; i < List_1_GameData_TaskInfo__get_Count(tasks, NULL); i++)
                {
                    GameData_TaskInfo* task = List_1_GameData_TaskInfo__get_Item(tasks, i, NULL);

                    if (!task->fields.Complete)
                        PlayerControl_RpcCompleteTask((*PlayerControl__TypeInfo)->static_fields->LocalPlayer, task->fields.Id, NULL);
                }
            }

            ImGui::Spacing();
            
            for (int i = 0; i < List_1_GameData_TaskInfo__get_Count(tasks, NULL); i++)
            {
                GameData_TaskInfo* task = List_1_GameData_TaskInfo__get_Item(tasks, i, NULL);

                if (ImGui::Button(("Complete##Button" + std::to_string(task->fields.Id)).c_str()) && !task->fields.Complete)
                {
                    PlayerControl_RpcCompleteTask((*PlayerControl__TypeInfo)->static_fields->LocalPlayer, task->fields.Id, NULL);
                }

                ImGui::SameLine();
                
                ImGui::TextColored(task->fields.Complete
                    ? ImVec4(0.0F, 1.0F, 0.0F, 1.0F)
                    : AmongUsColorToImVec4((*Palette__TypeInfo)->static_fields->DisabledGrey) 
                    , (std::string("Task #") + std::to_string(task->fields.Id)).c_str());
            }
        }

        if (ImGui::CollapsingHeader("Modifiers"))
        {
            ImGui::Text("Light Mod Modifier");
            ImGui::Checkbox("##LightModModifierCheckbox", &CWState::ModifyLight);
            ImGui::SameLine();
            ImGui::SliderFloat("##LightModModifier", &CWState::LightModifier, 0, 10.0F);

            ImGui::Text("Speed Modifier");
            ImGui::Checkbox("##SpeedModifierCheckbox", &CWState::ModifySpeed);
            ImGui::SameLine();
            ImGui::SliderFloat("##SpeedModifier", &CWState::SpeedModifier, 0, 10);

            ImGui::Text("Kill Cooldown Modifier");
            ImGui::Checkbox("##KillCooldownModifierCheckbox", &CWState::ModifyKillCooldown);
            ImGui::SameLine();
            ImGui::SliderFloat("##KillCooldown", &CWState::KillCooldownModifier, 0.001F, 60);

            ImGui::Text("Kill Distance Modifier");
            ImGui::Checkbox("##KillDistanceModifierCheckbox", &CWState::ModifyKillDistance);
            
            String__Array* killDistancesNames = (*GameOptionsData__TypeInfo)->static_fields->KillDistanceStrings;

            // yes it is hardcoded, but the array bounds are null
            // maybe it's some kind of optimization on readonly fields?
            for (int i = 0; i < CWConstants::KILL_DISTANCES_LENGTH; i++)
            {
                ImGui::SameLine();
                ImGui::RadioButton(GetUTF8StringFromNETString(killDistancesNames->vector[i]).c_str(), &CWState::KillDistanceModifier, i);
            }
        }

        if (ImGui::CollapsingHeader("Funnies"))
        {
            ImGui::Checkbox("Rainbow Colors 4 All", &CWState::ShiftAllColors);
            ImGui::Text("Interval");
            ImGui::SameLine();
            CWState::AllColorsCounter.GenerateInput("##RandomColorsAllInterval");

            ImGui::Spacing();

            ImGui::Checkbox("Random Clothes 4 All", &CWState::ShiftAllClothes);
            ImGui::Text("Interval");
            ImGui::SameLine();
            CWState::AllClothesCounter.GenerateInput("##RandomClothesAllInterval");
        }

        ImGui::End();
    }
    
    ImGui::EndFrame();
    ImGui::Render();

    Ctx->OMSetRenderTargets(1, &RenderTargetView, NULL);

    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    return OriginalD3DFunction(pThis, SyncInterval, Flags);
}

void HudHook(MethodInfo* m)
{
    OriginalHudFunction(m);

    if (CWState::VoteTarget.has_value()
        && (*MeetingHud__TypeInfo)->static_fields->Instance != NULL)
    {       
        // Class appears to be 4 bytes off?
        // here we are checking vote state which appears to be in VoteEndingSound
        MeetingHud_VoteStates__Enum state =
            (MeetingHud_VoteStates__Enum)((intptr_t)(*MeetingHud__TypeInfo)->static_fields->Instance->fields.VoteEndingSound);

        if (state == MeetingHud_VoteStates__Enum_NotVoted
            || state == MeetingHud_VoteStates__Enum_Voted)
        {
            for (auto player : GetAllPlayers())
            {
                MeetingHud_CmdCastVote((*MeetingHud__TypeInfo)->static_fields->Instance,
                    GetPlayerData(player)->fields.PlayerId,
                    GetPlayerData(CWState::VoteTarget.value())->fields.PlayerId,
                    NULL);
            }

            CWState::VoteTarget = std::nullopt;
        }
    }

    if (CWState::AllClothesCounter.ProcessAction() && CWState::ShiftAllClothes)
    {
        for (auto player : GetAllPlayers())
        {
            // would write way to find max bound for these two
            // but im too lazy 
            PlayerControl_RpcSetHat(player, randi(0, 10), NULL);
            PlayerControl_RpcSetSkin(player, randi(0, 10), NULL);
        }
    }

    if (CWState::ChatCounter.ProcessAction() && CWState::SpamChat)
    {
        if (CWState::AllPlayersSpam)
        {
            for (auto player : GetAllPlayers())
                PlayerControl_RpcSendChat(player, CreateNETStringFromANSI(CWState::SpamMessage), NULL);
        }
        else
        {
            PlayerControl_RpcSendChat((*PlayerControl__TypeInfo)->static_fields->LocalPlayer, CreateNETStringFromANSI(CWState::SpamMessage), NULL);
        }
    };

    if (CWState::ColorCounter.ProcessAction() && CWState::ColorShift)
    {
        CWState::CurrentColor = GetNextColor(CWState::CurrentColor);

        while (!CheckColorAvailable(CWState::CurrentColor))
            CWState::CurrentColor = GetNextColor(CWState::CurrentColor);

        PlayerControl_CmdCheckColor((*PlayerControl__TypeInfo)->static_fields->LocalPlayer, CWState::CurrentColor, NULL);
    }

    if (CWState::AllColorsCounter.ProcessAction() && CWState::ShiftAllColors)
    {
        for (auto player : GetAllPlayers())
        {
            auto data = GetPlayerData(player);
            auto playerColorId = data->fields.ColorId;
            playerColorId = GetNextColor(playerColorId);

            while (!CheckColorAvailable(playerColorId))
                playerColorId = GetNextColor(playerColorId);

            PlayerControl_CmdCheckColor(player, playerColorId, NULL);
        }
    }

    if (CWState::KickTarget.has_value())
    {
        VoteBanSystem* vbSystem = (*VoteBanSystem__TypeInfo)->static_fields->Instance;
        InnerNetClient* net = (InnerNetClient*)(*AmongUsClient__TypeInfo)->static_fields->Instance;
        int32_t victimId = InnerNetClient_GetClientIdFromCharacter(net, (InnerNetObject*)CWState::KickTarget.value(), NULL);
        int32_t oldClientId = net->fields.ClientId;

        for (auto player : GetAllPlayers())
        {
            net->fields.ClientId = InnerNetClient_GetClientIdFromCharacter(net, (InnerNetObject*)player, NULL);
            VoteBanSystem_CmdAddVote((*VoteBanSystem__TypeInfo)->static_fields->Instance, victimId, NULL);
        }

        net->fields.ClientId = oldClientId;

        CWState::KickTarget = std::nullopt;
    }

    if (CWState::PinDoors)
    {
        for (auto entry : CWState::PinnedDoors)
        {
            ShipStatus_RpcCloseDoorsOfType((*ShipStatus__TypeInfo)->static_fields->Instance, entry, NULL);
        }
    }

    if (CWState::MurderTarget.has_value())
    {
        if (GetPlayerData((*PlayerControl__TypeInfo)->static_fields->LocalPlayer)->fields.IsImpostor)
        {
            PlayerControl_RpcMurderPlayer((*PlayerControl__TypeInfo)->static_fields->LocalPlayer, CWState::MurderTarget.value(), NULL);
        }
        else
        {
            for (auto player : GetAllPlayers())
            {
                if (GetPlayerData(player)->fields.IsImpostor && !GetPlayerData(player)->fields.IsDead)
                {
                    PlayerControl_RpcMurderPlayer(player, CWState::MurderTarget.value(), NULL);
                    break;
                }
            }
        }
        
        CWState::MurderTarget = std::nullopt;
    }

    if (CWState::ColorTarget.has_value())
    {
        PlayerControl_CmdCheckColor((*PlayerControl__TypeInfo)->static_fields->LocalPlayer, CWState::ColorTarget.value(), NULL);
        CWState::ColorTarget = std::nullopt;
    }

    while (!CWState::MurderQueue.empty())
    {
        auto front = CWState::MurderQueue.front();
        if (GetPlayerData((*PlayerControl__TypeInfo)->static_fields->LocalPlayer)->fields.IsImpostor)
        {
            PlayerControl_RpcMurderPlayer((*PlayerControl__TypeInfo)->static_fields->LocalPlayer, front, NULL);
        }
        else
        {
            for (auto player : GetAllPlayers())
            {
                if (GetPlayerData(player)->fields.IsImpostor && !GetPlayerData(player)->fields.IsDead)
                {
                    PlayerControl_RpcMurderPlayer(player, front, NULL);
                    break;
                }
            }
        }
        CWState::MurderQueue.pop();
    }
    
    if (CWState::ModifySpeed)
        (*PlayerControl__TypeInfo)->static_fields->GameOptions->fields.PlayerSpeedMod = CWState::SpeedModifier;

    if (CWState::ModifyKillCooldown)
        (*PlayerControl__TypeInfo)->static_fields->GameOptions->fields.KillCooldown = CWState::KillCooldownModifier;

    if (CWState::ModifyKillDistance)
        (*PlayerControl__TypeInfo)->static_fields->GameOptions->fields.KillDistance = CWState::KillDistanceModifier;

    if (CWState::ModifyLight)
    {
        (*PlayerControl__TypeInfo)->static_fields->GameOptions->fields.ImpostorLightMod = CWState::LightModifier;
        (*PlayerControl__TypeInfo)->static_fields->GameOptions->fields.CrewLightMod = CWState::LightModifier;
    }

    if (CWState::NoClip)
    {
        auto comp = Component_get_gameObject((Component*)(*PlayerControl__TypeInfo)->static_fields->LocalPlayer, NULL);
        GameObject_set_layer(comp, LayerMask_NameToLayer(CreateNETStringFromANSI("Ghost"), NULL), NULL);
    }

    if (CWState::ShowGhosts) {

        for (auto player : GetAllPlayers())
        {
            auto data = GetPlayerData(player);
            if (!PlayerControl_get_Visible(player, NULL) && data->fields.IsDead)
                PlayerControl_set_Visible(player, true, NULL);
        }
    }

    if (CWState::MarkImposters)
        for (auto player : GetAllPlayers())
        {
            auto data = GetPlayerData(player);

            // Interestingly enough, RemainingEmergencies is actually a pointer to the PlayerControl field nameText...
            TextRenderer* nameText = (TextRenderer*)(player->fields.RemainingEmergencies);

            nameText->fields.Color = data->fields.IsImpostor
                ? (*Palette__TypeInfo)->static_fields->ImpostorRed
                : (*Palette__TypeInfo)->static_fields->CrewmateBlue;
        }
    
    //auto hudManager = (HudManager*)DestroyableSingleton_1_InnerNet_InnerNetServer__get_Instance(*(MethodInfo**)(GetBaseAddress() + 0x00E2577C));
}

bool HookFunction(PVOID* original, PVOID detour, const char* funcName)
{
    LONG err = DetourAttach(original, detour);
    if (err != 0)
    {
        std::cout << "Failed to hook a function: " << funcName << "\n";
        return false;
    }
    else
    {
        std::cout << "Successfully hooked a function: " << funcName << "\n";
        return true;
    }
}

void Run()
{
    NewConsole();

    std::cout << "Initializing...\n";

    OriginalHudFunction = KeyboardJoystick_HandleHud;
    OriginalD3DFunction = GetD3D11PresentFunction();

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());

    if (!HookFunction(&(PVOID&)OriginalHudFunction, HudHook, "KeyboardJoystick_HandleHud"))
        return;

    if (OriginalD3DFunction == NULL || !HookFunction(&(PVOID&)OriginalD3DFunction, D3D_FUNCTION_HOOK, "D3D11Present"))
        return;

    DetourTransactionCommit();

    std::cout << "Initialization Complete\n";

    system("cls");

    std::cout << CWConstants::CHRISTWARE_ASCII << "\n\n\t\tAmong Us Edition";
}