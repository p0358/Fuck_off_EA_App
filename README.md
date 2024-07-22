# "Fuck off EA App"

[![Github All Releases](https://img.shields.io/github/downloads/p0358/Fuck_off_EA_App/total.svg)](https://github.com/p0358/Fuck_off_EA_App/releases)

English | [中文](README_zh-CN.md)

A small project to patch the EA Origin app and keep it running, avoiding the nags to upgrade to the new EA App.

If you start up Origin app and see a prompt telling you to install EA App, you've come to the right place.

![Introductory image](https://github.com/user-attachments/assets/35f0b8c7-6e5b-42d9-9b56-34db7d9ce1b4)

What works (as of 2024-07-22):
* ✔️ login
* ✔️ main page
* ✔️ game startup
* ✔️ game downloads
* ✔️ friends section
* ✔️ Steam games
* ✔️ searching for people (fixed recently)

## Download

### Pre-requisite

You need to have [Origin already installed](https://download.dm.origin.com/origin/live/OriginSetup.exe) before continuing with installing the patch.

### Automatically with the installer

**[Download](https://github.com/p0358/Fuck_off_EA_App/releases/download/v3/Fuck_off_EA_App_installer.exe)** the installer and run it!

![installer screenshot](https://github.com/p0358/Fuck_off_EA_App/assets/5182588/1ab48e7b-e160-4248-a59a-31eb25a5b764)

### Or manually

Alternatively, if you don't want to use the installer above:
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

### Can I have Origin installed alongside EA App at the same time?

Yes, it should be fine. Origin in its newer versions is prepared to handle that gracefully. Both programs should also see games installed by each other.

### How does the `"Pretend EA App is installed"` option work?

EA games on Steam contain installer script, which instructs the Steam to check whether EA App is already installed by checking the system registry for key `HKEY_LOCAL_MACHINE\SOFTWARE\WOW6432Node\Electronic Arts\EA Desktop\InstallSuccessful` with the string value of `true`, skipping the launch of the installer if it's present. So ticking the checkbox simply creates that key and fulfills that condition. There's no harm done if you have actually installed the EA App already and accidentally checked it either.

### What if EA breaks something again? Will this keep working forever?

Given they already broke the older workarounds before a few times, probably not forever. In any case, follow this repository for updates (*star the project ;)*) -- if the workaround stops working, I will probably try to update it to work again in the future -- contributions are also welcome.

### If I try to launch Steam games, EA App's installer comes up instead?

Just close the installer, and Origin should then still start up. If it does not, look at the question below. You can avoid the EA App installer running up by selecting the option "Pretend EA App is installed" in the patch's installer.

### If I try to launch Steam games, EA App comes up instead of Origin?

You probably installed/updated EA App after installing Origin, and as such EA App was associated to the `link2ea://` protocol instead of Origin. To fix this, run through the Origin installer again. If you get an error that no application is assigned to the protocol, that means you probably uninstalled EA App without running the Origin installer afterwards again.

### If I try to launch Steam games, the error "The game X does not appear to be installed. Please install it or try relaunching the game." comes up?

Open up `C:\ProgramData\Origin\LocalContent` folder and ensure your game has a subdirectory in there. If not, [repair the game files on Steam](https://help.steampowered.com/faqs/view/0C48-FCBD-DA71-93EB). Remember that you need to press *Play* afterwards for the aforementioned files to appear. If they're still not there, browse local game files, open up `installScript.vdf` in Notepad, and search for `"LocFiles"` to figure out which files to copy and where manually...

### Does it work on Linux under WINE/Proton?

Check out this issue for current status and directions: [#3](https://github.com/p0358/Fuck_off_EA_App/issues/3)

In short, you will need to override the setting for `version` DLL to run as `native, then built-in`, which can be accomplished through `winecfg` or the config of the particular tool you use (like Bottles), or by using the environment variable `WINEDLLOVERRIDES="version=n,b"`. Note that currently not everything seems to fully work...

On Steam, enter this in launch options field: `WINEDLLOVERRIDES="version=n,b" %command%`

![native, then builtin](https://github.com/p0358/Fuck_off_EA_App/assets/5182588/7094bd2d-c8f9-4ec8-b841-ef7ff7c3afd6)

### How to disable Origin updates just in case?

My currently recommended method is to open the file at `C:\Program Files (x86)\Origin\EACore.ini` and insert the following inside it:
```
[Bootstrap]
EnableUpdating=false
```
This method will not slow down startup time, unlike the older local.xml method that I've shared. Note that as a side effect it'll enable "R&D mode" in the app.

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

## Donate

If you want to: https://www.paypal.me/p0358donate ¯\\_(ツ)_/¯

[![Star History Chart](https://api.star-history.com/svg?repos=p0358/Fuck_off_EA_App&type=Date)](https://star-history.com/#p0358/Fuck_off_EA_App&Date)
