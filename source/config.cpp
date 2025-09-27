#include "config.h"

#include "plugin_includes.h"

#include "main.h"
#include "debug_logging.h"
#include "globals.h"

bool Config::Settings_EnablePlugin = GlobalVarsFuncs.PluginConfigDefaults_EnablePlugin;
bool Config::Settings_EnableLED = GlobalVarsFuncs.PluginConfigDefaults_EnableLED;

uint32_t Config::Settings_ColorValue = GlobalVarsFuncs.PluginConfigDefaults_ColorValue;
bool Config::Settings_EnableBlinking = GlobalVarsFuncs.PluginConfigDefaults_EnableBlinking;

bool Config::Settings_EnableDebugOverlay = GlobalVarsFuncs.PluginConfigDefaults_EnableDebugOverlay;

// Retrieves the Power LED state
static int getPowerLEDState()
{
    uint8_t powerLEDState;
    BSPError error = bspRead("SMC", 0, "NotificationLED", 1, &powerLEDState);
    if(error != BSP_ERROR_OK)
    {
        if (ENABLE_CONSOLE_LOG)
        {
            WHBLogPrint(std::string("Failed to retrieve the Power LED color. Error: " + std::to_string(error)).c_str());
        }
        return -1;
    }

    if (ENABLE_CONSOLE_LOG)
    {
        WHBLogPrint(std::string("Power LED color: " + std::to_string(powerLEDState)).c_str());
    }
    return (int)powerLEDState;
}

// This sets the Power LED state, by writing a value over to the BSP in the console.
static int setPowerLEDState(uint8_t stateValue)
{
    // Initalize BSP
    BSPError initalizedBSP = bspInitializeShimInterface();

    if(initalizedBSP != BSP_ERROR_OK)

    // This one function should do everything for us on this program
    BSPError error = bspWrite("SMC", 1, "NotificationLED", 1, &stateValue);

    // Check for errors
    if(error != BSP_ERROR_OK)
    {
        if (ENABLE_CONSOLE_LOG)
        {
            WHBLogPrint(std::string("Failed to set the Power LED color. Error: " + std::to_string(error)).c_str());
        }

        return 1;
    }

    if (ENABLE_CONSOLE_LOG)
    {
        WHBLogPrint(std::string("Power LED color is now set to: " + std::to_string(stateValue)).c_str());
    }

    return 0;
}

// Function to change the plugin status. Used when setting an option in the plugin menu
static void PluginConfigFunctions_EnablePlugin(ConfigItemBoolean* item, bool new_value)
{
    // Log the change
    DEBUG_FUNCTION_LINE("PowerLEDColorU: Setting 'Enable Plugin' config value to '%d'...", new_value);

    // Change the variable
    Config::Settings_EnablePlugin = new_value;

    // Revert LED & debug overlay to the default settings
    if (!Config::Settings_EnablePlugin)
    {
        setPowerLEDState(GlobalVarsFuncs.PluginConfigDefaults_ColorValue);
        Config::Settings_EnableDebugOverlay = false;
    }
    else
    {
        setPowerLEDState(0x20);
    }

    // Store the new setting, unless an error occurs
    if (WUPSStorageAPI::Store<bool>(item->identifier, new_value) != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the \"Settings_EnablePlugin\" config value (Value that was specified - '%d')", Config::Settings_EnablePlugin);
    }
}

// Function to change the LED status
static void PluginConfigFunctions_EnableLED(ConfigItemBoolean* item, bool new_value)
{
    GlobalVarsFuncs.changeConfigValue(GlobalVarsFuncs.PluginConfigSettings_EnableLED, new_value, "led_enabled");

    // Change the variable
    if (Config::Settings_EnablePlugin)
    {
        Config::Settings_EnableLED = new_value;

        // Set the power LED state depending on if the LED should be on or not
        if (!Config::Settings_EnableLED)
        {
            setPowerLEDState(0x00);
        }
        else
        {
            setPowerLEDState(Config::Settings_ColorValue);
        }
    }

    // Store the new setting, unless an error occurs
    if (WUPSStorageAPI::Store<bool>(item->identifier, new_value) != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the \"Settings_EnableLED\" config value (Value that was specified - '%d')", Config::Settings_EnableLED);
    }
}


// Function to change the LED color.
static void PluginConfigFunctions_ChangeColor(ConfigItemIntegerRange* item, int new_value)
{
    // Change the variable
    Config::Settings_ColorValue = new_value;

    // Only change the color when the plugin is on, and the LED is enabled
    if((GlobalVarsFuncs.PluginConfigSettings_EnablePlugin) && (GlobalVarsFuncs.PluginConfigSettings_EnableLED))
    {
        // Switch the color to the new one
        setPowerLEDState(Config::Settings_ColorValue);
    }

    // Store the new setting, unless an error occurs
    if (WUPSStorageAPI::Store<uint32_t>(item->identifier, new_value) != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the 'Settings_ColorValue' config value (Value that was specified - '%d')", Config::Settings_ColorValue);
    }
}

// Function to change the LED blinking status.
static void PluginConfigFunctions_IsBlinking(ConfigItemBoolean* item, bool new_value)
{
    GlobalVarsFuncs.changeConfigValue(GlobalVarsFuncs.PluginConfigSettings_IsBlinking, new_value, "is_blinking");

    // Change the variable
    if (Config::Settings_EnablePlugin)
    {
        Config::Settings_EnableBlinking = new_value;

        // Switch whatever color is specified to the matching blinking version of the same color
        if (!Config::Settings_EnableBlinking)
        {
            // WIP
        }
        else
        {
            // WIP
        }
    }

    // Store the new setting, unless an error occurs
    if (WUPSStorageAPI::Store<bool>(item->identifier, new_value) != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the \"Settings_EnableBlinking\" config value (Value that was specified - '%d')", Config::Settings_EnableBlinking);
    }
}


// Function to change the debug overlay status
/*static void PluginConfigFunctions_ToggleDebugOverlay(ConfigItemBoolean* item, bool new_value)
{
    // Log the change
    DEBUG_FUNCTION_LINE("PowerLEDColorU: Setting 'Enable Debug Overlay' config value to '%d'...", new_value);

    // Change the variable
    if (Config::Settings_EnablePlugin)
    {
        Config::Settings_EnableDebugOverlay = new_value;
    }

    // Store the new setting, unless an error occurs
    if (WUPSStorageAPI::Store<bool>(item->identifier, new_value) != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the 'Settings_EnableDebugOverlay' config value (Value that was specified - '%d')", Config::Settings_EnableDebugOverlay);
    }
}*/


// The function that sets up the plugin's menus.
WUPSConfigAPICallbackStatus ConfigMenuOpenedCallback(WUPSConfigCategoryHandle rootHandle)
{
    if (ENABLE_CONSOLE_LOG)
    {
        WHBLogPrint("Config menu opened!");
    }

    // Getting the root category itself
    WUPSConfigCategory root = WUPSConfigCategory(rootHandle);

    // Add the options over to the root of the plugin's menu, and add sections as empty text separators

    // -- Enable Plugin --
    root.add(WUPSConfigItemBoolean::CreateEx("EnablePlugin", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[3], true, GlobalVarsFuncs.PluginConfigSettings_EnablePlugin, &PluginConfigFunctions_EnablePlugin, "On", "Off"));

    // Create a new category that will hold all menu items
    auto section1 = WUPSConfigCategory::Create("Menu");

    if (ENABLE_CONSOLE_LOG)
    {
        WHBLogPrint("Config menu category created.");
    }

    // Add the options over to the root of the plugin's menu, and sections as empty text separators

    // --------

    // [-- LED SETTINGS --]
    section1.add(WUPSConfigItemStub::Create(GlobalVarsFuncs.PluginConfigStrings_MenuSections[0]));
    section1.add(WUPSConfigItemStub::Create(GlobalVarsFuncs.PluginConfigStrings_MenuSeparator));

    // -- LED Color --
    section1.add(WUPSConfigItemIntegerRange::Create("Settings_ColorValue", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[1], Config::Settings_ColorValue, GlobalVarsFuncs.PluginConfigDefaults_ColorValue, 0x01, 0xFF, &PluginConfigFunctions_ChangeColor));

    // -- Enable LED Light --
    section1.add(WUPSConfigItemBoolean::Create("Settings_EnableLED", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[0], true, GlobalVarsFuncs.PluginConfigDefaults_EnableLED, &PluginConfigFunctions_ToggleLED));

    // -- Enable LED Blinking --
    section1.add(WUPSConfigItemBoolean::Create("Settings_EnableBlinking", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[2], false, GlobalVarsFuncs.PluginConfigDefaults_EnableBlinking, &PluginConfigFunctions_ToggleBlinking));

    // --------

    section1.add(WUPSConfigItemStub::Create(GlobalVarsFuncs.PluginConfigStrings_MenuSeparator));

    // [-- DEBUG SETTINGS --]
    /*
    section1.add(WUPSConfigItemStub::Create(GlobalVarsFuncs.PluginConfigStrings_MenuSections[1]));
    section1.add(WUPSConfigItemStub::Create(GlobalVarsFuncs.PluginConfigStrings_MenuSeparator));

    // -- Enable Debug Overlay --
    section1.add(WUPSConfigItemBoolean::Create("Settings_EnableDebugOverlay", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[4], false, GlobalVarsFuncs.PluginConfigDefaults_EnableDebugOverlay, &PluginConfigFunctions_ToggleDebugOverlay));*/

    // -- Add the category over to the root --
    root.add(std::move(section1));

    // -- Finally, add the "Enable Plugin" option last --
    root.add(WUPSConfigItemBoolean::Create("Settings_EnablePlugin", GlobalVarsFuncs.PluginConfigStrings_MenuOptions[3], true, Config::Settings_EnablePlugin, &PluginConfigFunctions_TogglePlugin));

    if (ENABLE_CONSOLE_LOG)
    {
        WHBLogPrint("Menu options created");
    }
}

// This function runs when the plugin menu is closing.
void ConfigMenuClosedCallback()
{
    // Save all changes
    if (WUPSStorageAPI::SaveStorage() != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("Failed to save storage...");
    }
}

// The main function that sets up all the config stuff.
void Config::Init()
{
    // Initalize the Config API
    WUPSConfigAPIOptionsV1 configOptions = {.name = GlobalVarsFuncs.PluginName.c_str()};

    if (WUPSConfigAPI_Init(configOptions, ConfigMenuOpenedCallback, ConfigMenuClosedCallback) != WUPSCONFIG_API_RESULT_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("Failed to initalize the Config API for 'PowerLEDColorU'...");
    }

    // --------

    // Try to get values from storage, and if anything is missing, add the values to the storage config

    // -- Enable Plugin --
    if ((storageRes = WUPSStorageAPI::GetOrStoreDefault("Settings_EnablePlugin", Config::Settings_EnablePlugin, GlobalVarsFuncs.PluginConfigDefaults_EnablePlugin)) != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the 'Settings_EnablePlugin' config value (Value that was specified - '%d')", Config::Settings_EnablePlugin);
    }

    // -- Enable LED --
    if ((storageRes = WUPSStorageAPI::GetOrStoreDefault("Settings_EnableLED", Config::Settings_EnableLED, GlobalVarsFuncs.PluginConfigDefaults_EnableLED)) != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the 'Settings_EnableLED' config value (Value that was specified - '%d')", Config::Settings_EnableLED);
    }

    // -- Color Value --

    if ((storageRes = WUPSStorageAPI::GetOrStoreDefault("Settings_ColorValue", Config::Settings_ColorValue, GlobalVarsFuncs.PluginConfigDefaults_ColorValue)) != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the 'Settings_ColorValue' config value (Value that was specified - '%d')", Config::Settings_ColorValue);
    }

    // -- Enable Blinking --
    if ((storageRes = WUPSStorageAPI::GetOrStoreDefault("Settings_EnableBlinking", Config::Settings_EnableBlinking, GlobalVarsFuncs.PluginConfigDefaults_EnableBlinking)) != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the 'Settings_EnableBlinking' config value (Value that was specified - '%d')", Config::Settings_EnableBlinking);
    }

    // -- Enable Debug Overlay --
    /*if ((storageRes = WUPSStorageAPI::GetOrStoreDefault("Settings_EnableDebugOverlay", Config::Settings_EnableDebugOverlay, GlobalVarsFuncs.PluginConfigDefaults_EnableDebugOverlay)) != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the 'Settings_EnableDebugOverlay' config value (Value that was specified - '%d')", Config::Settings_EnableDebugOverlay);
    }*/

    // --------

    // Save all the values over to the config file
    if (WUPSStorageAPI::SaveStorage() != WUPS_STORAGE_ERROR_SUCCESS)
    {
        DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save the config file. Something must have gone horribly wrong.");
    }
}
