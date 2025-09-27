#include "globals.h"
#include "plugin_includes.h"

#include "debug_logging.h"

#include "main.h"

Global GlobalVarsFuncs;

// Startup function, used to specify previously-missing variables that was not set yet
bool Globals_Startup()
{
    // Reset to the default values
    GlobalVarsFuncs.Reset();

    // Set all previously-unset variables

    // --------

    // -- Plugin Version ("X.X.X") --

    GlobalVarsFuncs.PluginVersion = std::to_string(GlobalVarsFuncs.PluginVersion_Major) + std::to_string(GlobalVarsFuncs.PluginVersion_Moderate) + std::to_string(GlobalVarsFuncs.PluginVersion_Minor);

    // -- Full Plugin Version ("vX.X.X") --

    GlobalVarsFuncs.PluginVersionFull = "v" + std::to_string(GlobalVarsFuncs.PluginVersion_Major) + std::to_string(GlobalVarsFuncs.PluginVersion_Moderate) + std::to_string(GlobalVarsFuncs.PluginVersion_Minor);

    // -- Plugin Author ("First M. Last") --
    GlobalVarsFuncs.PluginAuthor = GlobalVarsFuncs.PluginAuthor_First + " " + GlobalVarsFuncs.PluginAuthor_Middle + " " + GlobalVarsFuncs.PluginAuthor_Last;

    // --------

    if (ENABLE_CONSOLE_LOG)
    {
        WHBLogPrint("Global variables and functions generated");
    }

    return true;
}
