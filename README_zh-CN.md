# "Fuck off EA App"

[![Github All Releases](https://img.shields.io/github/downloads/p0358/Fuck_off_EA_App/total.svg)](https://github.com/p0358/Fuck_off_EA_App/releases)

[English](README.md) | 中文

这是一个简单修改了Origin的小项目，能让你继续在Origin当钉子户，而不是被迫升级到EA APP。

如果你启动Origin却被提示需要更新到EA APP，那么恭喜你来对地方了。

截止至2023-06-25，Origin至少还有以下功能可用:
* ✔️ 登录
* ✔️ 主界面
* ✔️ 启动游戏
* ✔️ 下载/更新游戏
* ✔️ 好友系统
* ✔️ 从Steam启动游戏

## 下载安装

### 使用安装器自动安装

**[下载](https://github.com/p0358/Fuck_off_EA_App/releases/download/v2/Fuck_off_EA_App_installer.exe)** 这个安装器然后双击运行即可

![installer screenshot](https://github.com/p0358/Fuck_off_EA_App/assets/5182588/1ab48e7b-e160-4248-a59a-31eb25a5b764)

### 手动安装

1. 在 [Releases](https://github.com/p0358/Fuck_off_EA_App/releases) 中下载最新版本的`version.dll`
2. 把它放到你的Origin安装的根目录下，替换即可, 安装的默认路径通常在`C:\Program Files (x86)\Origin`
3. 启动Origin

Note: 你也可以自己动手编译这个dll，步骤会贴在下边。

## 为什么会有这个项目

我们并不是想拉踩EA, 而是EA APP现在做的实在是太烂了。 问题多如牛毛，使用体验极其糟糕。 这个项目的存在的意义是为了让人们尽可能长时间地使用Origin，在EA APP得到显著改进之前，Origin相对来说是一个更好的选择。

如果这个项目有帮到你，记得点亮Star ;)

## FAQ

### 我已经把Origin删掉了，怎么才能重新下载Origin？

用这个地址下载就行: https://download.dm.origin.com/origin/live/OriginSetup.exe

### 我需要安装某个特定版本的Origin吗？

不需要，它应该与 `10.5.122.52971` ， `10.5.119.52718` 和更早的版本保持兼容 (包括我之前提到过的降级&修改local.xml的方法，如果你用到过的话)。

### 我还需要修改 `local.xml`吗?

不需要，使用这个项目的方法不再需要修改这个文件。

### 这个项目的原理是什么？

通过替换Origin原有的version.dll到我们修改过的版本，在Origin启动的时候，它会执行阻止升级通知出现的功能hooks和补丁，你可以自己看看源代码来了解更多细节。

###  `"Pretend EA App is installed"` 选项的实现原理是什么？

几乎所有登陆了Steam平台的EA游戏的文件中都会包含一个EA APP的安装脚本，这个脚本会指示Steam检查系统注册表中`HKEY_LOCAL_MACHINE\SOFTWARE\Electronic Arts\EA Desktop\InstallSuccessful` 一项的值是否为 `true`，以此来判断EA APP是否已经安装，如果已经安装，则跳过这个自带的安装脚本。如果你已经安装了EA APP，勾选此项也不会造成额外的问题。

### 这个方法能够永久有效吗？如果EA再破坏某些接口怎么办？

考虑到几次先例，我不能保证这个方法会永远有效，建议你关注这个项目以获得最新的更新(点个Star⭐️)

### 如果我在Steam上启动EA的游戏，EA APP会再次替代Origin启动吗？

理论上不会，如果你遇到了这个问题，多半是你在安装了Origin后又安装/更新了EA APP，所以游戏与`link2ea://` 协议相关联，而不是Origin，重新安装Origin并替换dll后应该可以解决这个问题。如果你得到了没有应用程序能启动游戏的报错，并且弹出了文件的启动方式选择窗口，则意味着你可能卸载了EA应用程序，之后没有再次运行Origin安装程序，重新安装Origin并替换dll后应该也可以解决这个问题。

### 我尝试在Steam上启动游戏，会出现错误“The game ******** does not appear to be installed. Please install it or try relaunching the game.”

打开`C:\ProgramData\Origin\LocalContent` 文件夹， 确认你要启动的游戏在这里有一个子目录存在。如果没有,在Steam中 [验证游戏文件完整性](https://help.steampowered.com/faqs/view/0C48-FCBD-DA71-93EB). 这些文件会在你点击`启动`按钮后自动生成。如果还是没有，则需要你在游戏的根目录下找到`installScript.vdf`，使用记事本或者别的什么编辑器打开它，搜索`"LocFiles"`一栏来确定需要复制的文件以及手动复制的位置。

### 它能在Linux系统下工作在Wine/Proton兼容层上吗？

查看这个issue来了解现在的情况和指引：[#3](https://github.com/p0358/Fuck_off_EA_App/issues/3)

简单来说, 你需要覆盖 `version` DLL 的设置为 `native, then built-in`, 可以通过`winecfg`或者你所使用的配置或其他特定工具实现, 你也可以在启动选项中使用环境变量 `WINEDLLOVERRIDES="version=n,b"` 来实现。但我并不能保证这两种方法能在所有Linux发行版或所有Wine/Proton版本中可用。

![native, then builtin](https://github.com/p0358/Fuck_off_EA_App/assets/5182588/7094bd2d-c8f9-4ec8-b841-ef7ff7c3afd6)

### 以防万一，怎么才能禁止Origin自动更新？

我的建议是打开 `C:\Program Files (x86)\Origin\EACore.ini` 文件，然后复制粘贴以下文本。
```
[Bootstrap]
EnableUpdating=false
```
这种方式不会拖慢Origin启动的速度, 与之前修改local.xml的方式不同。不过这也会在Origin中启用`R&D mode`。

### 这个项目是安全的吗？

显然如此，这个项目是完全开源的。在Release中发布的dll和exe文件也是通过Github Actions自动构建推送的，如果你愿意折腾，也可以自己来构建编译这个dll和安装程序。

### 我要怎么做才能自己编译这个dll?

1. 使用git clone这个仓库，或者直接下载源代码，看你个人喜好。
2. 运行 `generate.bat`
3. 在`build`文件夹中找到.sln文件，使用Visual Studio 2022来打开它。
4. 编译
5. 编译生成的`version.dll`在`.\build\bin\Win32-Release\`下，把它复制到`C:\Program Files (x86)\Origin`替换原有的version.dll即可。

## 使用到的第三方库

* [minhook](https://github.com/TFORevive/minhook) (fork)
* [silver-bun](https://github.com/IcePixelx/silver-bun)
