#include "radar.hpp"

float iconScale = 4.5f;

using namespace app;

namespace Radar {

	ImColor ColorCodes[12] = {
		ImColor(155, 0, 0, 255),
		ImColor(0, 0, 185, 255),
		ImColor(0, 185, 0, 255),
		ImColor(255, 20, 147, 255),
		ImColor(255, 140, 0, 255),
		ImColor(255, 255, 0, 255),
		ImColor(80, 80, 80, 255),
		ImColor(255, 255, 255, 255),
		ImColor(128, 0, 128, 255),
		ImColor(139, 69, 19, 255),
		ImColor(0, 255, 255, 255),
		ImColor(0, 255, 0, 255)
	};

	ImColor GetRadarPlayerColor(PlayerControl* player) {
		auto data = GetPlayerData(player);
		return ColorCodes[data->fields.ColorId];
	}

	ImColor GetRadarPlayerColorStatus(PlayerControl* player) {
		auto data = GetPlayerData(player);
		if (data->fields.IsImpostor)
			return ImColor(255, 0, 0, 255);
		else if (data->fields.IsDead)
			return ImColor(255, 255, 255, 255);
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

		ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(255 / 255.0F, 215 / 255.0F, 0.0F, 1.0F));
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(255 / 255.0F, 215 / 255.0F, 0.0F, 1.0F));
		ImGui::PushStyleColor(ImGuiCol_TitleBgCollapsed, ImVec4(255 / 255.0F, 215 / 255.0F, 0.0F, 1.0F));
		if (ImGui::Begin("Radar", state, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoResize))
		{
			ImDrawList* draw_list = ImGui::GetWindowDrawList();

			ImVec2 winpos = ImGui::GetWindowPos();
			ImVec2 winsize = ImGui::GetWindowSize();

			draw_list->AddLine(ImVec2(winpos.x + winsize.x * 0.5f, winpos.y), ImVec2(winpos.x + winsize.x * 0.5f, winpos.y + winsize.y), ImColor(70, 70, 70, 255), 1.f);
			draw_list->AddLine(ImVec2(winpos.x, winpos.y + winsize.y * 0.5f), ImVec2(winpos.x + winsize.x, winpos.y + winsize.y * 0.5f), ImColor(70, 70, 70, 255), 1.f);

			PlayerControl* localPlayer = (*PlayerControl__TypeInfo)->static_fields->LocalPlayer;

			if (!localPlayer) {
				ImGui::End();
				return;
			}

			draw_list->AddCircleFilled(ImVec2(winpos.x + winsize.x * 0.5f, winpos.y + winsize.y * 0.5f), iconScale, GetRadarPlayerColor(localPlayer));
			draw_list->AddCircle(ImVec2(winpos.x + winsize.x * 0.5f, winpos.y + winsize.y * 0.5f), iconScale + 0.5f, GetRadarPlayerColorStatus(localPlayer), 0, 2.0f);

			Vector2 localPos = PlayerControl_GetTruePosition(localPlayer, NULL);
			for (auto player : GetAllPlayers()) {
				Vector2 playerPos = PlayerControl_GetTruePosition(player, NULL);

				if (localPos.x == playerPos.x && localPos.y == playerPos.y)
					continue;

				float rad_x = std::clamp(winpos.x + ((playerPos.x - localPos.x) * radarZoom + winsize.x * 0.5f), winpos.x + iconScale * 0.5f, winpos.x + winsize.x - iconScale * 0.5f);
				float rad_y = std::clamp(winpos.y + ((localPos.y - playerPos.y) * radarZoom + winsize.y * 0.5f), winpos.y + iconScale * 0.5f, winpos.y + winsize.y - iconScale * 0.5f);

				draw_list->AddCircleFilled(ImVec2(rad_x, rad_y), iconScale, GetRadarPlayerColor(player));
				draw_list->AddCircle(ImVec2(rad_x, rad_y), iconScale + 0.5f, GetRadarPlayerColorStatus(player), 0, 2.0f);
			}

			ImGui::End();
		}
	}
}