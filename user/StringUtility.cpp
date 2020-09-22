#include "il2cpp-appdata.h"

#include <string>
#include <locale>
#include <codecvt>

using namespace app;

std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10FFFF,
    std::codecvt_mode::little_endian>, char16_t> UTF16_TO_UTF8;

std::string GetUTF8StringFromNETString(String* netString)
{
    if (netString == NULL)
        return NULL;

    uint16_t* buffer = new uint16_t[netString->fields.m_stringLength + 1];
    memcpy(buffer, &netString->fields.m_firstChar, netString->fields.m_stringLength * sizeof(uint16_t));
    buffer[netString->fields.m_stringLength] = L'\0';
    std::string newString = UTF16_TO_UTF8.to_bytes((const char16_t*)buffer);

    delete[] buffer;

    return newString;
}

String* CreateNETStringFromANSI(const char* string)
{
    return Marshal_PtrToStringAnsi((void*)string, NULL);
}

String* CreateNETStringFromUTF8(const char* string)
{
    return Marshal_PtrToStringUni((void*)string, NULL);
}