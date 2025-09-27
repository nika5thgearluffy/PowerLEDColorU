#ifndef LEDCOLORU_GLOBALS_H
#define LEDCOLORU_GLOBALS_H

#include "plugin_includes.h"

#include "debug_logging.h"

struct Global
{
    // Plugin Globals: Name
    std::string PluginName;
    
    // Plugin Globals: Description
    std::string PluginDescription;

    // Plugin Globals: Config Name
    std::string PluginConfigName;

    // Plugin Globals: Version Number
    int PluginVersion_Major;
    int PluginVersion_Moderate;
    int PluginVersion_Minor;

    std::string PluginVersion;
    std::string PluginVersionFull;

    // Plugin Globals: Author
    std::string PluginAuthor_First;
    std::string PluginAuthor_Middle;
    std::string PluginAuthor_Last;

    std::string PluginAuthor;

    // --------

    // Plugin Globals: Strings
    std::vector<std::string> PluginConfigStrings_MenuSections;
    std::vector<std::string> PluginConfigStrings_MenuOptions;
    std::string PluginConfigStrings_MenuSeparator;

    // --------

    // Plugin Globals: System Option Values
    bool PluginConfig_PluginStarted;
    bool PluginConfig_PluginFirstStarted;

    // Plugin Globals: Default Option Values
    bool PluginConfigDefaults_EnablePlugin;
    bool PluginConfigDefaults_EnableLED;

    uint32_t PluginConfigDefaults_ColorValue;
    bool PluginConfigDefaults_EnableBlinking;

    bool PluginConfigDefaults_EnableDebugOverlay;

    // --------

    // Plugin Globals: Option Handles
    WUPSConfigItemHandle PluginConfigHandles_EnablePlugin;
    WUPSConfigItemHandle PluginConfigHandles_EnableLED;

    WUPSConfigItemHandle PluginConfigHandles_ColorValue;
    WUPSConfigItemHandle PluginConfigHandles_IsBlinking;

    WUPSConfigItemHandle PluginConfigHandles_EnableDebugOverlay;

    // --------

    Global()
    {
        Reset();
    }

    void Reset()
    {
        // Plugin Globals: Name
        PluginName = "PowerLEDColorU";
        
        // Plugin Globals: Description
        PluginDescription = "Allows you to change the Wii U's LED Color, located on the Power Button, while the console is turned on";

        // Plugin Globals: Config Name
        PluginConfigName = "PowerLEDColorU";

        // Plugin Globals: Version Number
        PluginVersion_Major = 1;
        PluginVersion_Moderate = 0;
        PluginVersion_Minor = 0;

        // Plugin Globals: Author
        PluginAuthor_First = "Spencerly";
        PluginAuthor_Middle = "D.";
        PluginAuthor_Last = "Everly";

        // Plugin Globals: Strings
        std::vector<std::string> PluginConfigStrings_MenuSections = {"LED Settings", "Debug Settings"};
        std::vector<std::string> PluginConfigStrings_MenuOptions = {"Enable LED Light", "LED Color", "Enable LED Blinking", "Enable Plugin", "Enable Debug Overlay"};
        std::string PluginConfigStrings_MenuSeparator = "----------------";

        // --------

        // Plugin Globals: System Option Values
        PluginConfig_PluginStarted = false;
        PluginConfig_PluginFirstStarted = false;


        // Plugin Globals: Default Option Values (Never changed)
        PluginConfigDefaults_EnablePlugin = true;
        PluginConfigDefaults_EnableLED = true;

        PluginConfigDefaults_ColorValue = 0x20;

        PluginConfigDefaults_EnableBlinking = false;

        PluginConfigDefaults_EnableDebugOverlay = false;

        // --------
    }

    // True if on the Wii U menu, else this is false
    bool isOnWiiUMenu()
    {
        uint64_t current_title_id = OSGetTitleID();
        uint64_t wiiu_menu_tid = _SYSGetSystemApplicationTitleId(SYSTEM_APP_ID_WII_U_MENU);

        return (current_title_id == wiiu_menu_tid);
    }

    // Retrieves the Power LED state
    int getPowerLEDState()
    {
        uint8_t powerLEDState;
        BSPError error = bspRead("SMC", 0, "NotificationLED", 1, *powerLEDState);
        if(error != BSP_ERROR_OK)
        {
            return -1;
        }

        return (int)powerLEDState;
    }

    // This sets the Power LED state, by writing a value over to the BSP in the console.
    int setPowerLEDState(uint8_t stateValue)
    {
        // This one function should do everything for us on this program
        BSPError error = bspWrite("SMC", 1, "NotificationLED", 1, *stateValue);

        if(error != BSP_ERROR_OK)
        {
            return 1;
        }

        return 0;
    }

    // Shows a notification on the Wii U's screens, on the top-left corner
    void displayNotification(std::string_view notification)
    {
        // 
        auto err1 = NotificationModule_SetDefaultValue(NOTIFICATION_MODULE_NOTIFICATION_TYPE_INFO, NOTIFICATION_MODULE_DEFAULT_OPTION_KEEP_UNTIL_SHOWN, true);
        auto err2 = NotificationModule_SetDefaultValue(NOTIFICATION_MODULE_NOTIFICATION_TYPE_INFO, NOTIFICATION_MODULE_DEFAULT_OPTION_DURATION_BEFORE_FADE_OUT, 15.0f);

        if (err1 != NOTIFICATION_MODULE_RESULT_SUCCESS || err2 != NOTIFICATION_MODULE_RESULT_SUCCESS)
        {
            return;
        }

        NotificationModule_AddInfoNotification(notification.data());
    }

    // Basically a string. "true" is "On", "false" is "Off"
    std::string booleanToSwitchIndicator(bool value)
    {
        if(value)
        {
            return "On";
        }
        else
        {
            return "Off";
        }
    }

    // Basically a string. "true" is "Yes", "false" is "No"
    std::string booleanToAnswerIndicator(bool value)
    {
        if(value)
        {
            return "Yes";
        }
        else
        {
            return "No";
        }
    }

    void changeConfigValue(bool configItem, bool newValue, std::string rawVariableString)
    {
        // Log the change
        DEBUG_FUNCTION_LINE("PowerLEDColorU: Setting config value into config file... (Value: '%d')", newValue);

        // Change the variable
        configItem = newValue;

        // Store the new setting, unless an error occurs
        if (WUPSStorageAPI::Store<bool>(rawVariableString.c_str(), newValue) != WUPS_STORAGE_ERROR_SUCCESS)
        {
            DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save a config value to the config file (Value that was specified - '%d')", newValue);
        }
    }

    void changeConfigValue(uint32_t configItem, uint32_t newValue, std::string rawVariableString)
    {
        // Log the change
        DEBUG_FUNCTION_LINE("PowerLEDColorU: Setting config value into config file... (Value: '%d')", newValue);

        // Change the variable
        configItem = newValue;

        // Store the new setting, unless an error occurs
        if (WUPSStorageAPI::Store<uint32_t>(rawVariableString.c_str(), newValue) != WUPS_STORAGE_ERROR_SUCCESS)
        {
            DEBUG_FUNCTION_LINE("PowerLEDColorU: Failed to save a config value to the config file (Value that was specified - '%d')", newValue);
        }
    }

    // Runs when initating a config variable
    void readConfig_firstRead_Bool(bool var, bool defaultVar, std::string configName, std::string defaultSettingName, std::string rawVariableString)
    {
        // Create "storageRes"
        WUPSStorageError storageRes;

        // Get the value from the config file
        storageRes = WUPSStorageAPI::Get<bool>(rawVariableString, var);

        // If there is nothing, create the config value with the default settings
        if (storageRes == WUPS_STORAGE_ERROR_NOT_FOUND)
        {
            // Store the default value to the main variable
            var = defaultVar;

            // Store the default value to the plugin config, else error out
            if (WUPSStorageAPI::Store<bool>(rawVariableString, defaultVar) != WUPS_STORAGE_ERROR_SUCCESS)
            {
                DEBUG_FUNCTION_LINE("PowerLEDColorU: Failed to save a config value to the config file (Value that was specified - '%d')", var);
            }
        }

        // If nothing still works, we couldn't read where the config should be at
        else if (storageRes != WUPS_STORAGE_ERROR_SUCCESS)
        {
            DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save to the config file (Status: '%s', Error: '%d')", WUPSStorageAPI_GetStatusStr(storageRes), storageRes);
        }
    }

    void readConfig_firstRead_Int32(uint32_t var, uint32_t defaultVar, std::string configName, std::string defaultSettingName, std::string rawVariableString)
    {
        // Create "storageRes"
        WUPSStorageError storageRes;

        // Get the value from the config file
        storageRes = WUPSStorageAPI::Get<uint32_t>(rawVariableString, var);

        // If there is nothing, create the config value with the default settings
        if (storageRes == WUPS_STORAGE_ERROR_NOT_FOUND)
        {
            // Store the default value to the main variable
            var = defaultVar;

            // Store the default value to the plugin config, else error out
            if (WUPSStorageAPI::Store<uint32_t>(rawVariableString, defaultVar) != WUPS_STORAGE_ERROR_SUCCESS)
            {
                DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save a config value to the config file (Value that was specified - '%d')", var);
            }
        }

        // If nothing still works, we couldn't read where the config should be at
        else if (storageRes != WUPS_STORAGE_ERROR_SUCCESS)
        {
            DEBUG_FUNCTION_LINE("PowerLEDColorU: ERROR - Failed to save to the config file (Status: '%s', Error: '%d')", WUPSStorageAPI_GetStatusStr(storageRes), storageRes);
        }
    }
};

extern Global GlobalVarsFuncs;

extern bool Globals_Startup();

#endif //LEDCOLORU_CONFIG_H
