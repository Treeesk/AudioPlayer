# AudioPlayerQt

**AudioPlayerQt** is a macOS audio player with a Qt-based GUI, using a **custom audio engine via CoreAudio**.  
It supports playback of audio files (MP3, WAV, etc.) using FFmpeg for decoding and TagLib for reading metadata (track titles, artists, covers, duration).  

The application uses a **three-thread architecture**:
1. Qt UI thread (automatically created)
2. Audio engine thread (`AudioWrapper`) for playback
3. CoreAudio playback thread (created by the API during audio output)

The GUI is scalable and automatically adapts to window size, resizing album covers, text, and UI elements.

---

## Features

- **Audio formats:** MP3, WAV, and other PCM-based files (decoded via FFmpeg)
- **Playlist and navigation:** Load a folder of tracks; all tracks are displayed in a scrollable panel on the left with cover, title, and artist. The currently playing track is highlighted.
- **Main screen:** Shows enlarged album cover, title, artist, total track duration (via TagLib), and current playback time. Automatically switches to the next track when finished.
- **Playback controls:** Play/pause, next/previous track, seek slider, and volume slider with current/maximum level indicators.
- **Multithreading:** UI runs in its own Qt thread, audio engine in a separate thread (`AudioWrapper`), CoreAudio creates an internal playback thread. Ensures smooth playback without UI freezes.
- **Adaptive GUI:** Elements scale with window size, maintaining readability and layout.

---

## Screenshot

![Main Screen](/docs/screenshots/Снимок%20экрана%202026-01-21%20в%2017.52.37.png)  
*Main screen with current track, album cover, playback controls, seek and volume sliders, scrollable playlist panel with track covers, titles, artists, and highlighted current track.*  
All audio tracks and album covers shown in screenshots are used for demonstration purposes only.


---

## Install and Build

### Supported Platform
- **macOS only**  
  (The project uses CoreAudio / AudioToolbox)

---

### Prebuilt Release (recommended)

If you only want to run the application, download the latest prebuilt version from the **Releases** page.

1. Download `AudioPlayerQt-macOS.zip`
2. Unzip the archive
3. Run `AudioPlayerQt.app`

> On first launch, macOS may warn about an unknown developer.  
> Go to **System Settings → Privacy & Security** and allow the application to run.

---
### Install
```bash 
brew install qt ffmpeg taglib cmake 
```
---

### Build from Source
```bash
git clone https://github.com/Treeesk/AudioPlayer.git  
cd AudioPlayer

mkdir build
cd build

cmake ..
cmake --build .
```
---
### Prerequisites

- **Qt 6 (Widgets)**  
- **C++17 or later**  
- **FFmpeg** (`libavcodec`, `libavformat`, `libavutil`, `libswresample`)  
- **TagLib** (for metadata)  
- **AudioToolbox (CoreAudio)**  
- **CMake 3.16+**  
- **Xcode / Command Line Tools**    

---
## Usage
**Standalone Player**  
Run AudioPlayerQt. Load a folder with audio tracks. Select tracks in the playlist panel or control playback using the main screen controls.

### Dependencies
- **Qt 6 (Widgets / Core modules)**
- **C++17**
- **FFmpeg (Homebrew: brew install ffmpeg)**
- **TagLib (Homebrew: brew install taglib)**
- **AudioToolbox / CoreAudio (macOS system library)**
- **CMake**
- **Xcode / Clang**  
No additional libraries required; audio playback is handled directly via CoreAudio.
---

## License

[MIT © Yaroslav Kalinin.](/LICENSE)