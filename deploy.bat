@ECHO OFF
REM call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
REM call "%VS140COMNTOOLS%\vsvars32.bat"
SET MyQtDir=D:\Development\C_Workspace\thirdparty\qt\6.8.0\msvc2022_64
SET MyBuildDir=D:\Development\C_Workspace\steels-neocron-tool\build\Desktop_Qt_6_8_0_MSVC2022_64bit-RelWithDebInfo
SET MyInstallDir=%~dp0\install
SET MyRedistDir=C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Redist\MSVC\14.42.34433\x64\Microsoft.VC143.CRT

ECHO ---------- BEGIN [Clean Up]
ECHO Clean Up?
ECHO ECHO MyInstallDir=%MyInstallDir%
PAUSE
RMDIR /s /q "%MyInstallDir%"

ECHO ---------- BEGIN [CMake Install]
ECHO Run 'cmake --install'?
ECHO MyBuildDir=%MyBuildDir%
PAUSE
cmake --install %MyBuildDir% --prefix "%MyInstallDir%"

ECHO ---------- BEGIN [Deploy Qt Dependencies]
ECHO Run 'windeployqt'?
ECHO MyQtDir=%MyQtDir%
ECHO MyInstallDir=%MyInstallDir%
PAUSE
"%MyQtDir%\bin\windeployqt.exe" --release --force --qmlimport "%MyQtDir%\qml" --qmldir "%~dp0\apps\damage-log-viewer\src" "%MyInstallDir%\bin"

ECHO ---------- BEGIN [Deploy System Dependencies]
ECHO Run 'copy'?
ECHO MyRedistDir=%MyRedistDir%
PAUSE
COPY "%MyRedistDir%\*.dll" "%MyInstallDir%\bin"

ECHO ---------- BEGIN [Create Setup/Installer]
ECHO Run 'isc'?
PAUSE
"C:\Program Files (x86)\Inno Setup 6\ISCC.exe" "%~dp0\innosetup\client-installation.iss"

ECHO ---------- END
PAUSE