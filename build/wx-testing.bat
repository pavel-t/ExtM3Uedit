cd ..\dep\wxWidgets\build\msw
mingw32-make -s -f makefile.gcc CFG="-testing" BUILD=release DEBUG_FLAG=2 CXXFLAGS="-std=c++17"
rmdir gcc_mswu-testing /s /q
pause
