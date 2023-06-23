#define MyAppName "the ""Fuck off EA App"" patch by p0358"
;#define MyAppVersion "1" -- specified on the command line
#define MyAppPublisher "p0358"
#define MyAppURL "https://github.com/p0358/Fuck_off_EA_App"

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{1A5F933F-77F5-496D-9738-D3DCC8917A9D}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
;AppVerName={#MyAppName} {#MyAppVersion}
AppPublisher={#MyAppPublisher}
AppPublisherURL={#MyAppURL}
AppSupportURL={#MyAppURL}
AppUpdatesURL={#MyAppURL}
DefaultDirName={code:GetInstallationPath}
DefaultGroupName=FuckOffEAApp
AllowNoIcons=yes
DisableProgramGroupPage=yes
;InfoBeforeFile=pre_install_info.txt
DisableWelcomePage=yes
;InfoAfterFile=post_install_info.txt
OutputBaseFilename=Fuck_off_EA_App_installer
OutputDir=.
Compression=lzma
SolidCompression=yes
PrivilegesRequired=admin
DisableReadyPage=yes
ShowLanguageDialog=no
CreateUninstallRegKey=no
DirExistsWarning=no
EnableDirDoesntExistWarning=yes
CloseApplications=force
CloseApplicationsFilter=*.exe,*.dll,*.chm,*.bik,*.asi,*.log,*.json
Uninstallable=no

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "build\bin\Win32-Release\version.dll"; DestDir: "{app}"; Flags: ignoreversion

[Registry]
Root: HKLM32; Subkey: "SOFTWARE\Electronic Arts\EA Desktop"; ValueType: string; ValueName: "InstallSuccessful"; ValueData: "true"; Flags: createvalueifdoesntexist; Check: IsEAAppCheckBoxChecked

[Code]
function InitializeSetup: Boolean;
begin
  Result := True;
end;



var
  EAAppCheckBox: TNewCheckBox;

procedure InitializeWizard;
begin
    EAAppCheckBox := TNewCheckBox.Create(WizardForm);
    EAAppCheckBox.Parent := WizardForm.SelectDirPage;
    EAAppCheckBox.Top := 120;
    EAAppCheckBox.Left := 0;
    EAAppCheckBox.Width := 500;
    EAAppCheckBox.Caption := 'Pretend EA App is installed (so that Steam does not start its installer)';
end;

function IsEAAppCheckBoxChecked: Boolean;
begin
  Result := EAAppCheckBox.Checked;
end;



procedure RegisterExtraCloseApplicationsResources;
begin
  RegisterExtraCloseApplicationsResource(False, ExpandConstant('{app}\Origin.exe')); 
end;



procedure CurPageChanged(CurPageID: Integer);
begin
  if CurPageID = wpSelectDir then
    WizardForm.NextButton.Caption := SetupMessage(msgButtonInstall)
  //else
  //  WizardForm.NextButton.Caption := SetupMessage(msgButtonNext);
end;



var
  InstallationPath: string;

function GetInstallationPath(Param: string): string;
begin
  { Detected path is cached, as this gets called multiple times }
  if InstallationPath = '' then
  begin
    if RegQueryStringValue(
         HKLM32, 'SOFTWARE\Origin',
         'OriginPath', InstallationPath) then
    begin
      StringChangeEx(InstallationPath, '\Origin.exe', '', True)
      Log('Detected Origin installation: ' + InstallationPath);
    end
      else
    begin
      InstallationPath := 'C:\Program Files (x86)\Origin';
      Log('No installation detected, using the default path: ' + InstallationPath);
    end;
  end;
  Result := InstallationPath;
end;
