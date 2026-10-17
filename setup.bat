rem Resolve every path below against the repository root, wherever this is run from.
cd /d "%~dp0"

rem Fetch only the engine. Its own nested submodules are not needed and use the retired git:// protocol.
git submodule update --init cocos2d || exit /b 1

rem cocos2d-x keeps its third-party libraries outside its repository. Its download-deps.py
rem needs distutils, which Python 3.12 removed, so fetch the version named in
rem cocos2d/external/config.json directly.
if not exist cocos2d\external\version.json (
    curl -fL -o "%TEMP%\cocos2d-x-deps.zip" https://github.com/cocos2d/cocos2d-x-3rd-party-libs-bin/archive/metal-support-22.zip || exit /b 1
    %SystemRoot%\System32\tar.exe -xf "%TEMP%\cocos2d-x-deps.zip" -C cocos2d\external --strip-components=1 || exit /b 1
    del "%TEMP%\cocos2d-x-deps.zip"
)

mkdir win32-build
cd win32-build
cmake .. -G "Visual Studio 17 2022" -T v143 -A Win32