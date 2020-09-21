#include "string.h"
#include <cstring>

String::String()
{
    length = 0;
    buffer = nullptr;
}

String::String(const char* str)
{
    length = strlen(str);
    buffer = new char[length + 1];
    strncpy(buffer, str, length);
    buffer[length] = '\0';
}

String::String(const String& other)
{
    length = other.length;
    buffer = new char[length + 1];
    strncpy(buffer, other.buffer, length);
    buffer[length] = '\0';
}

String::~String()
{
    if(buffer != nullptr)
        delete[] buffer;
}

const char* String::C_Str() const
{
    return buffer;
}

bool String::operator==(const String& rhs) const
{
    return length == rhs.length && !strcmp(buffer, rhs.buffer);
}