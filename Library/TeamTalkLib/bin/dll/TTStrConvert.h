#pragma once
// Helpers: convert between narrow ACE_TString (char) and TTCHAR (wchar_t on WIN32)
#include <myace/MyACE.h>
#include <string>

#if defined(WIN32)
static inline void A2TT(TTCHAR* dst, const char* src, size_t len)
{
    if (!src || len == 0) return;
    mbstowcs(dst, src, len);
    dst[len-1] = L'\0';
}
static inline void A2TT(TTCHAR* dst, const TTCHAR* src, size_t len)
{
    ACE_OS::strsncpy(dst, src, len);
}
static inline ACE_TString TT2A(const TTCHAR* src)
{
    if (!src) return ACE_TString();
    size_t wlen = wcslen(src);
    std::string s(wlen * 4 + 1, '\0');
    wcstombs(&s[0], src, s.size());
    return ACE_TString(s.c_str());
}
#else
// On Linux TTCHAR == char, so only one overload needed
static inline void A2TT(TTCHAR* dst, const char* src, size_t len)
{
    ACE_OS::strsncpy(dst, src, len);
}
static inline ACE_TString TT2A(const TTCHAR* src)
{
    return ACE_TString(src);
}
#endif
