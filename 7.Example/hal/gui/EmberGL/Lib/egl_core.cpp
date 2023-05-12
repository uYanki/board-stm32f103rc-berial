//============================================================================
// EmberGL
//
// Copyright (c) 2022, Jarkko Lempiainen
// All rights reserved.
//============================================================================

#include "egl_core.h"
EGL_USING_NAMESPACE
//----------------------------------------------------------------------------

//============================================================================
// init_serial
//============================================================================
void EGL_NAMESPACE_PREFIX init_serial()
{
#if EGL_BUILDOP_LOGS == 1 && defined(ARDUINO)
    // setup serial logging
    static const uint32_t s_bailout_time_ms = 500;
    uint32_t              start_time        = millis();
    Serial.begin(9600);
    while (!Serial && millis() - start_time < s_bailout_time_ms)
        ;
#endif
}
//----------------------------------------------------------------------------

//============================================================================
// bcd16_version_str
//============================================================================
version_str EGL_NAMESPACE_PREFIX bcd16_version_str(uint16_t version_)
{
    version_str s;
    EGL_SPRINTF(s.str, sizeof(s.str), "%i.", version_ >> 12);
    version_ <<= 4;
    size_t l = strlen(s.str);
    do {
        EGL_SPRINTF(s.str + l, sizeof(s.str) - l, "%x", version_ >> 12);
        ++l;
        version_ <<= 4;
    } while (version_);
    s.str[l] = 0;
    return s;
}
//----------------------------------------------------------------------------
