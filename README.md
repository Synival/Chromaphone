# Chromaphone
A useful (and pretty!) audio visualizer for musicians

# Videos and screenshots

Video - Desktop version:

[![Video: Chromaphone on Desktop](http://simonbielman.com/portfolio/chromaphone/Chromaphone_Screenshot_Desktop.png)](https://www.youtube.com/watch?v=eVlAu86uFdM)

Video - Android version (old as of 2023-03-04):

[![Video: Chromaphone on Android](http://simonbielman.com/portfolio/chromaphone/Chromaphone_Screenshot_Android.png)](https://www.youtube.com/watch?v=0XlLdKYJeAA)

# What is Chromaphone?

Chromaphone is a visualizer that displays individual pitches heard from a microphone or other audio input source onto a piano keyboard. It has two goals: to be a useful tool for musicians to analyse music, and to enhance your musical experience by adding pretty visuals. Unlike other visualization software, Chromaphone aims for *pitch accuracy* above all else. You should be able to transcribe music using Chromaphone.

# How can I use it?

Follow the build instructions, run it, and select your audio input source using the dropdown menu. The dropdown menu will update automatically when the default device is changed or the device list is updated. If the input signal isn't strong enough, you can give it a boost using the "Sensitivity" slider in the lower-left corner.

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

**A.** Each *pitch class* (C, C#, D, etc.) is assigned a hue, rotating around the color wheel along the circle of 5ths, starting with C as **red**. In other words: C is red, G is orange, D is yellow, and so on. If a note is out of tune, it is desaturated; quarter-step sharp/flat notes are completely white.

The background color is determined by mixing all notes heard together, weighted by their intensity. Every individual tone color is also skewed slightly toward the background color to give a better impression of the entire chord. For example: C Major traids tend to have a shimmering gold color, comprised of oranges and yellows, while a Gb Major triad is a deep blueish-purple. Pretty!

Fun fact: Due to the way the background/overall color is mixed, chords or sets of pitches that have no clear tonal center appear white or desaturated. This applies to tritones, augmented triads, fully-diminished 7 chords, percussion, and (not always) speech.

**Q. Why does it show so many extra notes?**

**A.** What you're seeing are the *harmonics* produced by the instruments Chromaphone is picking up. Depending on the timbre of the instrument, the fundamental pitch (i.e, the "played" note) may be more or less difficult to discern. A simple sine wave will show a single pitch in Chromaphone, while a cello may show many more pitches  than you would expect due to its rich timbre.

**Q. You know what would be cool? If it could [insert feature request here]**

**A.** Trust me, I know - there a *a million things* that would be cool. If you'd like to see more features in Chromaphone, you're more than welcome to contribute!

# License?
GPL 3.0
