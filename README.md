# Cave Crawler
A survival top-down shooter written in C++ using custom built Square Engine

## Getting Started
Visual Studio 2019 is recommended. This build currently only works with Windows OS.

You can clone the repository to a local destination using git:

`git clone https://github.com/Benner727/CaveCrawler`

Inside the scripts folder, run GenerateProject.bat to get the solution and project files.

### Setup:
This project uses SDL2, SDL2 Image, SDL2 TTF, and SDL2 Mixer. <br />
They have been linked in Visual Studio using vcpkg.

PowerShell (Administrator) Commands:
```
cd c:\
git clone https://github.com/Microsoft/vcpkg.git
.\bootstrap-vcpkg.bat
.\vcpkg integrate install
.\vcpkg install sdl2 sdl2-image sdl2-mixer sdl2-net sdl2-ttf
.\vcpkg install --triplet x64-windows sdl2 sdl2-image sdl2-mixer sdl2-net sdl2-ttf
```