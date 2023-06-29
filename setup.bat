mkdir win32-build
copy "lib\sqlite3\sqlite-dll-win32-x86-3350500\sqlite3.dll" "win32-build\sqlite3.dll"
cd win32-build
cmake .. -G "Visual Studio 17 2022" -T v143 -A Win32