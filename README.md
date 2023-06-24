# "Fuck off EA App"

[![Github All Releases](https://img.shields.io/github/downloads/p0358/Fuck_off_EA_App/total.svg)](https://github.com/p0358/Fuck_off_EA_App/releases)

A small project to patch the EA Origin app and keep it running, avoiding the nags to upgrade to the new EA App.

If you start up Origin app and see a prompt telling you to install EA App, you've come to the right place.

What works (as of 2023-06-24):
* ✔️ login
* ✔️ main page
* ✔️ game startup
* ✔️ game downloads
* ✔️ friends section
* ✔️ Steam games

## Download

### Automatically with the installer

**[Download](https://github.com/p0358/Fuck_off_EA_App/releases/download/v1/Fuck_off_EA_App_installer.exe)** the installer and run it!

![installer screenshot](https://github.com/p0358/Fuck_off_EA_App/assets/5182588/1ab48e7b-e160-4248-a59a-31eb25a5b764)

### Manually

1. Go into [Releases section](https://github.com/p0358/Fuck_off_EA_App/releases) and grab the latest `version.dll` file
2. Drop it into your Origin installation folder, by default `C:\Program Files (x86)\Origin`
3. Restart Origin

Note: you can also compile the DLL manually, instructions are at the bottom of this file.

## Why

We're not ones wanting to step on EA's toes, however their new app just does not work properly for many users. The issues they experience are countless, ranging from small annoyances to outright being unable to use the EA App or launch the games at all. This project exists to let people keep using Origin for as long as it's possible, at least until EA App is significantly improved, as currently Origin is the superior solution...

Remember to leave a star and follow if it helped you ;)

## FAQ

### How do I re-install Origin if I already deleted it?

Use this link: https://download.dm.origin.com/origin/live/OriginSetup.exe

### Do I need a specific version of Origin?

No, it should work fine with both the latest `10.5.122.52971` and with the older `10.5.119.52718` or earlier (if you used the downgrade method before and disabled updates).

### What about `local.xml`?

You don't need to either add or remove anything from that file anymore when using this project, conveniently.

### How does it work?

Our custom DLL (named `version.dll`) is loaded by Origin during its startup, after you insert it under its installation directory. It conducts function hooks and patches that prevent the upgrade notice from appearing. You can study the code for more details.

### How does the `"Pretend EA App is installed"` option work?

EA games on Steam contain installer script, which instructs the Steam to check whether EA App is already installed by checking the system registry for key `HKEY_LOCAL_MACHINE\SOFTWARE\Electronic Arts\EA Desktop\InstallSuccessful` with string value of `true`, skipping the launch of the installer if it's present. So ticking the checkbox simply creates that key and fulfills that condition. There's no harm done if you have actually installed the EA App already and accidentally checked it either.

### What if EA breaks something again? Will this keep working forever?

Given they already broke the older workarounds before a few times, probably not forever. In any case, follow this repository for updates (*star the project ;)*) -- if the workaround stops working, I will probably try to update it to work again in the future -- contributions are also welcome.

### If I try to launch Steam games, EA App comes up instead of Origin?

You probably installed/updated EA App after installing Origin, and as such EA App was associated to the `link2ea://` protocl instead of Origin. To fix this, run through the Origin installer again. If you get an error that no application is assigned to the protocol, that means you probably uninstalled EA App without running the Origin installer afterwards again.

### Does it work on Linux under WINE/Proton?

Check out this issue for current status and directions: [#3](https://github.com/p0358/Fuck_off_EA_App/issues/3)

In short, you will need to override the setting for `version` DLL to run as `native, then built-in`, which can be accomplished through `winecfg` or the config or the particular tool you use, or by using the environment variable `WINEDLLOVERRIDES="version=n,b"` in launch options. Note that currently not everything seems to fully work...

### Is it safe?

Yeah, the thing is open-sourced, and the actual DLL and installer in Releases section is built by GitHub Actions CI. You can always compile it yourself too if so you wish...

### How do I compile it myself?

1. Clone the repository with git
2. Run `generate.bat`
3. Open the .sln file from `build` folder in Visual Studio 2022
4. Compile the solution
5. Copy `version.dll` from `.\build\bin\Win32-Release\` into `C:\Program Files (x86)\Origin`

## Third-party libraries used

* [minhook](https://github.com/TFORevive/minhook) (fork)
* [silver-bun](https://github.com/IcePixelx/silver-bun)
