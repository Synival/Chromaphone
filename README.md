# Chromaphone
A useful (and pretty!) audio visualizer for musicians

# Videos

Desktop version:

[![Video: Chromaphone on Desktop](https://i9.ytimg.com/vi_webp/eVlAu86uFdM/mq2.webp?sqp=CNiljqAG-oaymwEmCMACELQB8quKqQMa8AEB-AH-CYAC0AWKAgwIABABGFAgZShKMA8=&rs=AOn4CLDmsBlytnTxngEMChBWleH-kRvVRQ)](https://www.youtube.com/watch?v=0XlLdKYJeAA)

Android version (old as of 2023-03-04):

[![Video: Chromaphone on Android](https://i9.ytimg.com/vi_webp/0XlLdKYJeAA/mq2.webp?sqp=COzIjqAG-oaymwEmCMACELQB8quKqQMa8AEB-AH-BIAC4AKKAgwIABABGFEgWihlMA8=&rs=AOn4CLDn9n4x0zHGyKH1tou0Mvavd5O7_Q)](https://www.youtube.com/watch?v=eVlAu86uFdM)

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

# License?
GPL 3.0
