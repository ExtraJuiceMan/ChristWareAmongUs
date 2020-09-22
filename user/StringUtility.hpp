#pragma once
#include "il2cpp-appdata.h"

using namespace app;

std::string GetUTF8StringFromNETString(String* netString);

String* CreateNETStringFromANSI(const char* string);

String* CreateNETStringFromUTF8(const char* string);