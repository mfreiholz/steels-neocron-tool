#define MyAppName "Steel's Neocron Tool"
#define AppBuildDir "../build-neocron-log-parser-Desktop_Qt_5_15_1_MSVC2019_64bit-Release"

[Setup]
AppId={{89B4B71E-B90D-4726-BFD8-C6A027A27726}

[Languages]
Name: "english"; MessagesFile: "compiler:Default.isl"

[Files]
Source: ""; DestDir: "{app}"; Flags: recursesubdirs ignoreversion

[Run]
Filename: "vcredist_msvc2019_x64.exe"; Parameters: "/install /quiet"; Flags: skipifdoesntexist; StatusMsg: "Installing VC Redistributable 2019 (64-bit)... This may take a while";
