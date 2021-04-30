;
; Precondition
; - Run "deploy.bat" before running this script.
;

#define MyAppVersion "1.1.0"
#define MyAppName "Steels Neocron Tool"
#define MyAppNameFileSafe "SteelsNeocronTool"

[Setup]
AppId={{89B4B71E-B90D-4726-BFD8-C6A027A27726}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
AppPublisher=Manuel Freiholz
AppPublisherURL=https://mfreiholz.de
AppContact=Manuel Freiholz <mfreiholz at gmail.com>
AppCopyright=Copyright 2021 Manuel Freiholz
OutputBaseFilename={#MyAppNameFileSafe}-{#MyAppVersion}-Setup
OutputManifestFile={#MyAppNameFileSafe}-{#MyAppVersion}-Setup-Manifest.txt
DefaultDirName={autopf64}\{#MyAppNameFileSafe}
DefaultGroupName={#MyAppName}
SourceDir=..\install\bin
OutputDir=..\
LicenseFile=..\..\COPYING
DisableProgramGroupPage=yes
PrivilegesRequired=lowest
PrivilegesRequiredOverridesAllowed=commandline dialog

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "*"; DestDir: "{app}"; Flags: recursesubdirs ignoreversion

[Icons]
Name: "{userdesktop}\{#MyAppNameFileSafe}"; Filename: "{app}\SteelsNeocronTool.exe"; Tasks: desktopicon

[Tasks]
Name: "desktopicon"; Description: "{cm:CreateDesktopIcon}"; GroupDescription: "{cm:AdditionalIcons}"

[Run]
Filename: "{app}\SteelsNeocronTool.exe"; Description: {cm:LaunchProgram,{#MyAppName}}; Flags: nowait postinstall skipifsilent
