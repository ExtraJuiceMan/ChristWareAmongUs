#include "radar.hpp"
using namespace app;

namespace Radar {
	ImColor GetRadarPlayerColor(PlayerControl* player) {
		auto data = GetPlayerData(player);
		return AmongUsColorToImVec4(GetPlayerColor(data->fields.ColorId));
	}

	ImColor GetRadarPlayerColorStatus(PlayerControl* player) {
		auto data = GetPlayerData(player);
		if (data->fields.IsImpostor)
			return AmongUsColorToImVec4((*Palette__TypeInfo)->static_fields->ImpostorRed);
		else if (data->fields.IsDead)
			return AmongUsColorToImVec4((*Palette__TypeInfo)->static_fields->DisabledGrey);
		else
			return ImColor(0, 0, 0, 0);
	}

	void SquareConstraint(ImGuiSizeCallbackData* data)
	{
		data->DesiredSize = ImVec2(data->DesiredSize.x, data->DesiredSize.y);
	}

	void RenderRadar(bool* state, float radarZoom) {
		ImGui::SetNextWindowSize(ImVec2(256, 256));
		ImGui::SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, FLT_MAX), SquareConstraint);

		ImGui::PushStyleColor(ImGuiCol_TitleBg, CWConstants::CW_GOLD);
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, CWConstants::CW_GOLD);
		ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, CWConstants::CW_GOLD);

		ImGui::Begin("Radar", state, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize);

		ImDrawList* drawList = ImGui::GetWindowDrawList();

		ImVec2 winpos = ImGui::GetWindowPos();
		ImVec2 winsize = ImGui::GetWindowSize();

		drawList->AddLine(ImVec2(winpos.x + winsize.x * 0.5F, winpos.y),
			ImVec2(winpos.x + winsize.x * 0.5F, winpos.y + winsize.y), ImColor(70, 70, 70, 255), 1.0F);
		drawList->AddLine(ImVec2(winpos.x, winpos.y + winsize.y * 0.5F),
			ImVec2(winpos.x + winsize.x, winpos.y + winsize.y * 0.5F), ImColor(70, 70, 70, 255), 1.0F);

		if (IsInGame())
		{
			PlayerControl* localPlayer = (*PlayerControl__TypeInfo)->static_fields->LocalPlayer;

			if (!localPlayer) {
				ImGui::End();
				return;
			}

			drawList->AddCircleFilled(ImVec2(winpos.x + winsize.x * 0.5F, winpos.y + winsize.y * 0.5F),
				CWConstants::ICON_SCALE, GetRadarPlayerColor(localPlayer));
			drawList->AddCircle(ImVec2(winpos.x + winsize.x * 0.5F, winpos.y + winsize.y * 0.5F),
				CWConstants::ICON_SCALE + 0.5f, GetRadarPlayerColorStatus(localPlayer), 0, 2.0F);

			Vector2 localPos = PlayerControl_GetTruePosition(localPlayer, NULL);

			for (auto player : GetAllPlayers()) {
				if (localPlayer == player)
					continue;

				Vector2 playerPos = PlayerControl_GetTruePosition(player, NULL);

				float radX = std::clamp(winpos.x + ((playerPos.x - localPos.x) * radarZoom + winsize.x * 0.5F),
					winpos.x + CWConstants::ICON_SCALE * 0.5F, winpos.x + winsize.x - CWConstants::ICON_SCALE * 0.5F);
				float radY = std::clamp(winpos.y + ((localPos.y - playerPos.y) * radarZoom + winsize.y * 0.5F),
					winpos.y + CWConstants::ICON_SCALE * 0.5F, winpos.y + winsize.y - CWConstants::ICON_SCALE * 0.5F);

				drawList->AddCircleFilled(ImVec2(radX, radY), CWConstants::ICON_SCALE, GetRadarPlayerColor(player));
				drawList->AddCircle(ImVec2(radX, radY), CWConstants::ICON_SCALE + 0.5F, GetRadarPlayerColorStatus(player), 0, 2.0F);
			}
		}
		ImGui::End();
	}
}