#ifndef LEDCOLORU_CONFIG_H
#define LEDCOLORU_CONFIG_H

#include "plugin_includes.h"

class Config
{
public:
    static void Init();

    // ----------------

    static bool Settings_EnablePlugin;
    static bool Settings_EnableLED;

    static uint32_t Settings_ColorValue;
    static bool Settings_EnableBlinking;

    static bool Settings_EnableDebugOverlay;

    // ----------------
};

void ConfigMenuClosedCallback();

#endif //LEDCOLORU_CONFIG_H
