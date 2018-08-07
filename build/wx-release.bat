cd ..\dep\wxWidgets\build\msw
mingw32-make -s -f makefile.gcc CFG="-release" BUILD=release DEBUG_FLAG=1 CPPFLAGS="-flto" CXXFLAGS="-std=c++17"
rmdir gcc_mswu-release /s /q
pause
