SampleBrowser
=============

Sound sample browser and previewer
----------------------------------

**Warning**: *this is early alpha version*. 

### DEPENDENCIES
You need **Qt5 development packages** installed and properly configured.

Additional library dependencies are:

| Library    | Version   | Package            | Purpose               |
|------------|-----------|--------------------|-----------------------|
| sndfile    | >= 1.0.27 | libsndfile1-dev    | Audio file I/O        |
| RtAudio    | >= 4.1.2  | librtaudio-dev     | Realtime audio driver |
| samplerate | >= 0.1.8  | libsamplerate0-dev | Audio resampler       |

Install command for Debian:

`sudo apt-get install libsndfile1-dev librtaudio-dev libsamplerate0-dev`

Optional libraries:

| Library    | Version   | Package            | Purpose               |
|------------|-----------|--------------------|-----------------------|
| mpg123     | >= 1.22.4 | libmpg123-dev      | MPEG audio decoder    |
| soxr-lsr   | >= 0.1.2  | libsoxr-dev        | Alternative resampler |

Library *mpg123* is required for MP3 file support. *soxr-lsr* is alternative resampling library.

Install command for Debian:

`sudo apt-get install libmpg123-dev libsoxr-dev`

### INSTALL
Submodule initialization is required before build.

```
git clone https://github.com/aasfalcon/samplebrowser
cd samplebrowser
git submodule init
git submodule update
qmake
make
sudo make install

```

### RUN

`build/wexplorer`

## CHHANGELOG

- 0.2.1 - Prototype functionality reached
- 0.2.0 - Started Qt/C++ rewrite
- 0.1.1 - Added waveform image display
- 0.1.0 - Initial [Python-based prototype](https://github.com/aasfalcon/psb) with modified QT file dialog

## LICENSE

See [LICENSE.md](LICENSE.md) for full license text.

__Copyright (C) 2016-2017  Andy S.__

*This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.*

*This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.*

*You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.*