;
; Precondition
; - Run "deploy.bat" before running this script.
;

#define MyAppVersion "1.1.0"
#define MyAppName "Steel's Neocron Tool"
#define MyAppNameFileSafe "SteelsNeocronTool"

[Setup]
AppId={{89B4B71E-B90D-4726-BFD8-C6A027A27726}
AppName={#MyAppName}
AppVersion={#MyAppVersion}
OutputBaseFilename={#MyAppNameFileSafe}-{#MyAppVersion}-Setup
OutputManifestFile={#MyAppNameFileSafe}-{#MyAppVersion}-Setup-Manifest.txt
DefaultDirName={autopf64}\{#MyAppNameFileSafe}
SourceDir=..\install\bin
OutputDir=..\

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: "*"; DestDir: "{app}"; Flags: recursesubdirs ignoreversion

[Icons]
Name: "{userdesktop}\{#MyAppNameFileSafe}"; Filename: "{app}\SteelsNeocronTool.exe"
