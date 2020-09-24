#pragma once

struct String
{
    unsigned long length;
    char* buffer;

    String();
    String(const char* str);
    String(const String& other);
    ~String();

    const char* C_Str() const;

    bool operator==(const String& rhs) const;
};