/*
    PowerLEDColorU
    By "The Sun God: Nika"
*/

// Yes, I made a header for the main.cpp file
#include "main.h"

// All-in-one .h file that has everything to include in each .cpp/.h file
#include "plugin_includes.h"

// Specify local code files here
#include "debug_logging.h"

#include "globals.h"
#include "config.h"

/*
    Set the most-important plugin settings.

    - Name
    - Description
    - Version
    - Author
    - License

    These are needed, otherwise this plugin will fail
*/

// Because variables can't be used on macros, we'll need to specify everything separately :T

WUPS_PLUGIN_NAME("PowerLEDColorU");
WUPS_PLUGIN_DESCRIPTION("Allows you to change the Wii U's LED Color, located on the Power Button, while the console is turned on");
WUPS_PLUGIN_VERSION("v1.0.0");
WUPS_PLUGIN_AUTHOR("The Sun God: Nika");
WUPS_PLUGIN_LICENSE("MIT");

// Set to use a config file
WUPS_USE_STORAGE("PowerLEDColorU");

// Used for debug logs
WUPS_USE_WUT_DEVOPTAB();

// --------

// Set this to true if you want to see debug information on the screen. Useful for reporting bugs and issues
bool ENABLE_CONSOLE_LOG = true;

// --------

// Runs when the plugin first loads.
INITIALIZE_PLUGIN()
{
    // Refresh all the global variables and functions
    bool globalsLoaded = Globals_Startup();

    // Initalize the logging system
    WHBLogCafeInit();
    WHBLogUdpInit();

    if (ENABLE_CONSOLE_LOG)
    {
        WHBLogPrint("Cafe logging and UDP logging initalized");
    }

    // Start Mocha. If Mocha fails, stop the plugin
    const auto MochaInitalized = Mocha_InitLibrary();
    if (MochaInitalized != MOCHA_RESULT_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("Mocha init failed with code %d!", MochaInitalized);
        if (ENABLE_CONSOLE_LOG)
        {
            WHBLogPrint(std::string("Mocha failed to initalize. Error: " + std::to_string(MochaInitalized)).c_str());
        }
        return;
    }
    else
    {
        if (ENABLE_CONSOLE_LOG)
        {
            WHBLogPrint("Mocha successfully initalized");
        }
    }

    // Start the Notification Module. If the Notification Module fails, keep going, but mark that it failed
    if (NotificationModule_InitLibrary() != NOTIFICATION_MODULE_RESULT_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("NotificationModule_InitLibrary failed");
        if (ENABLE_CONSOLE_LOG)
        {
            WHBLogPrint("Notification Module failed to initalize");
        }
        else
        {
            if (ENABLE_CONSOLE_LOG)
            {
                WHBLogPrint("Notification Module successfully initalized");
            }
        }
    }

    // Initalize the config values and menu options
    Config::Init();
    
    if (ENABLE_CONSOLE_LOG)
    {
        WHBLogPrint("Config initalized");
    }

    // Notify that the plugin has officially started and is active, if on the Wii U menu
    if ((GlobalVarsFuncs.isOnWiiUMenu() && (!GlobalVarsFuncs.PluginConfig_PluginFirstStarted)))
    {
        GlobalVarsFuncs.displayNotification(GlobalVarsFuncs.PluginName + " is active and has started!");
        GlobalVarsFuncs.PluginConfig_PluginFirstStarted = true;
        if (ENABLE_CONSOLE_LOG)
        {
            WHBLogPrint("Plugin detected as first started");
        }
    }

    if (ENABLE_CONSOLE_LOG)
    {
        WHBLogPrint("Start complete");
    }

    GlobalVarsFuncs.PluginConfig_PluginStarted = true;
}

// Runs when the plugin is about to close.
DEINITIALIZE_PLUGIN()
{
    if (ENABLE_CONSOLE_LOG)
    {
        WHBLogPrint("Deinitalizing Mocha...");
    }
    // Close Mocha
    Mocha_DeInitLibrary();

    if (ENABLE_CONSOLE_LOG)
    {
        WHBLogPrint("Deinitalizing the Notification Module...");
    }
    // Close the Notification Module
    NotificationModule_DeInitLibrary();

    if (ENABLE_CONSOLE_LOG)
    {
        WHBLogPrint("Deinitalizing logging and fully closing...");
    }

    // Plugin is now closing, so make this false
    GlobalVarsFuncs.PluginConfig_PluginStarted = false;

    // Close logging any further
    WHBLogCafeDeinit();
    WHBLogUdpDeinit();

}

// Runs when any Wii U title starts.
ON_APPLICATION_START()
{
    
}

// Runs when any Wii U title closes.
ON_APPLICATION_ENDS()
{

}

// Runs on every frame while the plugin is running.
int main()
{
    // Debug Console stuff for testing purposes.
    if (ENABLE_CONSOLE_LOG)
    {
        WHBLogConsoleInit();
        WHBLogConsoleSetColor(0xFF000000);
        if (ENABLE_CONSOLE_LOG)
        {
            WHBLogPrint("Logging initalized");
        }
    }

    // Run anything at anytime while the plugin is officially active
    while(1)
    {
        // Runs stuff only if the plugin is officially started and beyond
        if(GlobalVarsFuncs.PluginConfigSettings_EnablePlugin && GlobalVarsFuncs.PluginConfig_PluginStarted)
        {
            // --------

            // Debug Overlay: Drawing
            if (Config::Settings_EnableDebugOverlay)
            {
                // WIP
            }

            // --------

            // Debug Console: Drawing
            if (ENABLE_CONSOLE_LOG)
            {
                WHBLogConsoleDraw();
            }
        }
    }
    
    WHBLogConsoleFree();
}
