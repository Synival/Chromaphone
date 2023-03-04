# Chromaphone
A useful (and pretty!) audio visualizer for musicians

# Videos and screenshots

Video - Desktop version:

[![Video: Chromaphone on Desktop](https://i9.ytimg.com/vi_webp/0XlLdKYJeAA/mq2.webp?sqp=COzIjqAG-oaymwEmCMACELQB8quKqQMa8AEB-AH-BIAC4AKKAgwIABABGFEgWihlMA8=&rs=AOn4CLDn9n4x0zHGyKH1tou0Mvavd5O7_Q)](https://www.youtube.com/watch?v=0XlLdKYJeAA)

Video - Android version (old as of 2023-03-04):

[![Video: Chromaphone on Android](https://i9.ytimg.com/vi_webp/eVlAu86uFdM/mq2.webp?sqp=CNiljqAG-oaymwEmCMACELQB8quKqQMa8AEB-AH-CYAC0AWKAgwIABABGFAgZShKMA8=&rs=AOn4CLDmsBlytnTxngEMChBWleH-kRvVRQ)](https://www.youtube.com/watch?v=eVlAu86uFdM)

# What is Chromaphone?

Chromaphone is a visualizer that displays notes read from a microphone or other audio input on a piano keyboard. It has two goals: to be a useful tool for musicians to analyse music, and to enhance your musical experience by adding pretty visuals. Unlike other visualization software, Chromaphone aims for *accuracy* above all else. You should be able to transcribe music using Chromaphone.

# How can I use it?

Follow the build instructions, run it, and select your audio input device using the dropdown menu. It will update automatically when the default device is changed or the device list is updated. If the input signal isn't strong enough, you can give it a boost using the "Sensitivity" slider on the left side.

# Build instructions
Chromaphone has only been built in the Windows environment using Qt Creator (version 9.0.1). To build, please install the following:

1. Qt Creator ([9.0.1 download link](https://download.qt.io/official_releases/qtcreator/9.0/9.0.1/))
2. A build environment (e.g, [MSYS2 mingw-w64-gcc](https://packages.msys2.org/base/mingw-w64-gcc))
3. Qt Framework 6 ([version 6.4.0 download link](https://doc.qt.io/qt-6.4/get-and-install-qt.html))

Fetch the following project(s):
1. [LFT](https://github.com/Synival/lft). Clone to parent folder. (In other words, if Chromaphone is in `Devel/Chromaphone`, place LFT in `Devel/lft`.)

Open "Chromaphone.pro" is Qt Creator:
1. Choose a desired architecture (Android or Desktop)
2. Build!

# FAQ

**Q. How do the colors work?**

**A.** Each pitch class (C, C#, D, etc.) is assigned a color going around the color wheel along the circle of 5ths, starting with red. So, C is red, G is orange, D is yellow, and so on. If a note is out of tune, it is desaturated; quarter-step sharp or flat notes are completely white.

An overall color is determined by mixing all notes together, weighted by their intensity. This is shown as a subtle background color. It also shifts every other color slightly towards this color to give an impression that the entire chord has a color. A C Major traid, for example, tends to have a shimmering gold color, made of oranges and yellows, while a Gb Major triad is a deep blueish-purple. Neat!

**Q. Why does it show so many extra notes?**

**A.** What you're seeing are the *harmonics* produced by the instruments Chromaphone is picking up. Depending on the timbre of the instrument, the fundamental tone (i.e, the "real" tone) of the note may be more or less difficult to see. A simple sine wave should show a single note in Chromaphone, while a cello may show many more tones than you'd expect due to its rich timbre.

**Q. You know what would be cool? If you-**

**A.** Stop. There a *a million things* that would be cool. If you'd like to see more features in Chromaphone, you'll more than welcome to contribute!

# License?
GPL 3.0
