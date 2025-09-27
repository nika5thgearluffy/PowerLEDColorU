# PowerLEDColorU
This is a Wii U plugin for Aroma. This can let you change the Power LED Color while the system is on.

This can be useful for those wanting to customize the colors, or change the system's Power LED color back to blue for `isfshax` users.

## Usage
- Download the latest version from "Releases", or the very latest commit build under "Actions"
- Copy the `PowerLEDColorU.wps` file to your SD Card
    - The file should be in `sd:/wiiu/environments/aroma/plugins/`
- Eject the SD Card, put it in your Wii U, and turn it on if it was off

When powering the console on, the LED will be set to blue by default.

> To change this plugin's settings, go on the Aroma plugin menu,
> and head down to the `PowerLEDColorU` option and select it

## Plugin Options
Many options on the plugin can be set to your heart's content.

### Main Settings
- Enable Plugin: Turn this off to disable the plugin.

### LED Settings
- Enable LED Light: If this is disabled, the Power LED light will not be on.
    - You can prank your friends with this!
- Enable LED Blinking: If this is on, the Power LED will blink like it's notifying something.
    - More options, such as LED blink rate, is possibly coming later
- LED Color: The choice of color to use for the system's power LED.
    - This is still being figured out at this time, so stick around!

## Building
To build this plugin, you'll need to do the following:

1. Install devKitPro (It contains the core Wii U libraries needed to build this project)
2. Run MSYS2.
3. Install a few extra packages in MSYS2, which will be needed to build the plugin (`pacman -S [package-name]`):

- wups (For the Aroma plugin SDK)
- wums (For the notifications of this plugin)
- whb (For Wii U debug logging)
- libmocha (For gaining higher system privileges on the Wii U, so the LED state can change)

4. After those are installed, just run `make` to perform the build.

If `make` is not installed, execute `pacman -S make` to install it.