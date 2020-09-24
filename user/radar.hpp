#pragma once
#define _USE_MATH_DEFINES
#include <imgui.h>
#include <imgui_impl_win32.h>
#include <imgui_impl_dx11.h>
#include <math.h>
#include <algorithm>
#include <cmath>
#include <iostream>
#include "il2cpp-appdata.h"
#include "GameUtility.hpp"
#include "CWConstants.hpp"

namespace Radar
{
	void RenderRadar(bool* state, float zoom);
}