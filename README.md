<p align="center">
  <img width="850" height="200" alt="PicoBanner" src="https://github.com/user-attachments/assets/6afbbe72-f474-4e1d-addf-7647200a1093" />
</p>
<p align="center">
  Get your very own Pico Dragon right on your desktop!!!
</p>

## Installation
 
1. Head to [Releases](../../releases)
2. Download `DesktopPico_Setup.exe` from the latest release
3. Run the installer and follow the prompts (you can choose to add a desktop shortcut and/or have Pico launch automatically on Windows startup)
4. Launch Desktop Pico from the Start Menu or your desktop shortcut
 
> **Note:** Windows SmartScreen may show a warning on first launch. Click **More info → Run anyway** to proceed.
 
## Features
 
- **71 random colour presets** — every time you drop a Pico Dragon, it picks a new palette for its body, tummy, spikes, and toes
- **Click interactions** — click a Pico Dragon for a random encouraging message; click too fast five times in a row and the Pico Dragon gets overwhelmed and needs a moment
- **Random check-ins** — every so often, the Pico Dragon pops up with a message on its own, unprompted
- **Drag to reposition** — click and drag the Pico Dragon anywhere on your screen
- **Right-click to remove** — in case you're sick of your friend :(
- **Multiple Picos** — launching the app more than once spawns additional, independent Picos, each with their own random colour palette

## Customization
 
### Colours & Dialogue
 
Colour presets and dialogue live in `data/colours.json` and `data/dialogue.json` respectively. You can edit these to add your own!

 
## Building from Source
 
Desktop Pico is built with **C++ and Qt 6**. To build it yourself:
 
1. Install [Qt 6](https://www.qt.io/download-qt-installer) (MinGW or MSVC kit) and [CMake](https://cmake.org/download/)
2. From the project root:
```
   cmake -S . -B build -G "Ninja" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="path/to/your/Qt/kit"
   cmake --build build
```
3. Deploy the required Qt runtime DLLs next to the built exe:
```
   windeployqt build\DesktopPico.exe
```
4. Run `build\DesktopPico.exe`
A GitHub Actions workflow (`.github/workflows/build.yml`) also builds an installer automatically on every tagged release, see that file if you want to package your own build the same way.

