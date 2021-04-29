@ECHO OFF
REM call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvarsall.bat" x64
REM call "%VS140COMNTOOLS%\vsvars32.bat"
SET MyQtDir=D:\Qt\5.15.1\msvc2019_64
SET MyBuildDir=D:\Projects\NeocronLogParser\Source\build-neocron-log-parser-Desktop_Qt_5_15_1_MSVC2019_64bit-Release
SET MyInstallDir=%~dp0\install
SET MyRedistDir=C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Redist\MSVC\14.28.29325\x64\Microsoft.VC142.CRT

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