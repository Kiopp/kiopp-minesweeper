# kiopp-minesweeper
It's just a minesweeper clone I made for fun.

# Compilation
Compilation is made easy and platform independant using cmake. Raylib is included in the compilation and therefore is not required to be installed on your system. Simply follow these steps:

### Linux
I use the following commands to build the project:
```
mkdir build
cd build
cmake ..
```
Then to compile I simply run:
```
make && ./kiopp_mine
```
### Windows
(Not tested yet) To build the project on windows I recommend using cmake GUI with Visual Studio.
1. Browse to your project's source directory from the cmake GUI.
2. Specify a directory for the build files (usually a separate "build" directory).   
3. Click "Configure" and choose your generator (e.g. "Visual Studio 2019").
4. Click "Generate" to create the project files.
5. Open the generated .sln solution in Visual Studio and Build the project from there.
