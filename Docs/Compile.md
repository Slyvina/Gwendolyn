# Compiling Gwendolyn

This document will get into the deep of compiling Gwendolyn for Windows and Linux

# What you need:
- [Jalondi](http://github.com/JCR6/Jalondi) a tool for creating JCR6 files. The executable must be in the PATH.
- Git
- SDL2
## For Windows 
- PowerShell 
- Visual Studio
## Linux
- Python
- GCC
- SCons


# Before you begin, make sure you have a directory dedicated to Slyvina projects.
Then work this way. (Works in both Shell as PowerShell).
~~~sh
cd Slyvina
mkdir Apps
mkdir Libs
git clone https://github.com/Slyvina/Gwendolyn Apps/Gwendolyn
git clone https://github.com/Slyvina/JCR6 Libs/JCR6
git clone https://github.com/Slyvina/June19 Libs/June19
git clone https://github.com/Slyvina/TQSL Libs/TQSL
git clone https://github.com/Slyvina/Units Libs/Units
~~~

Now JCR6 will require to have a folder 3rdParty/zlib in which zlib.h and zconf.h live
In Linux you need in that folder an src folder were the source code of zlib is. In Windows, precompiled libraries zlib.lib and zlib.dll must live in 3rdParty/zlib.

# SDL2 Windows
Now in Windows, you will need a folder where SDL2 lives, but in precompiled form. When typing "dir" (in PowerShell) you should see this
~~~
    Directory: Slyvina\Libs\TQSL\SDL2

Mode                 LastWriteTime         Length Name
----                 -------------         ------ ----
d----          12/17/2022 11:49 AM                include
-a---          10/31/2018  4:00 PM         441344 libFLAC-8.dll
-a---           6/30/2019  6:53 AM         244224 libjpeg-9.dll
-a---          10/31/2018  4:00 PM         252928 libmodplug-1.dll
-a---          10/31/2018  4:00 PM         337408 libmpg123-0.dll
-a---          10/31/2018  4:00 PM          52224 libogg-0.dll
-a---          10/31/2018  4:00 PM         124928 libopus-0.dll
-a---          10/31/2018  4:00 PM          46592 libopusfile-0.dll
-a---           6/30/2019  6:53 AM         210944 libpng16-16.dll
-a---           6/30/2019  6:53 AM         432640 libtiff-5.dll
-a---          10/31/2018  4:00 PM         251904 libvorbis-0.dll
-a---          10/31/2018  4:00 PM          69632 libvorbisfile-3.dll
-a---           6/30/2019  6:53 AM         447488 libwebp-7.dll
-a---          10/31/2018  4:00 PM           1656 LICENSE.FLAC.txt
-a---          10/31/2018  4:00 PM            193 LICENSE.modplug.txt
-a---          10/31/2018  4:00 PM          41624 LICENSE.mpg123.txt
-a---          10/31/2018  4:00 PM           1632 LICENSE.ogg-vorbis.txt
-a---          10/31/2018  4:00 PM           2107 LICENSE.opus.txt
-a---          10/31/2018  4:00 PM           1649 LICENSE.opusfile.txt
-a---          12/17/2022 11:43 AM           6691 LICENSE.SDL2_Image.zip
-a---          12/17/2022 11:53 AM            523 ReadMe.md
-a---           6/30/2019  6:53 AM         125440 SDL2_image.dll
-a---           6/30/2019  6:53 AM          10120 SDL2_image.lib
-a---          10/31/2018  4:00 PM         123904 SDL2_mixer.dll
-a---          10/31/2018  4:00 PM          17182 SDL2_mixer.lib
-a---           3/11/2020  2:39 AM        1471488 SDL2.dll
-a---            3/9/2020  3:04 AM         150888 SDL2.lib
-a---            3/9/2020  3:04 AM          41196 SDL2main.lib
-a---            3/9/2020  3:04 AM         935746 SDL2test.lib
-a---           6/30/2019  6:53 AM         108544 zlib1.dll
~~~
The folder "include" must contain all the header files.

# SDL2 in Linux
Just obtain the source from the SDL2 site and compile and install the developer versions according to the instructions.
Also make sure you have the run-time libraries of SDL2 and SDL2_Mixer installed, or Gwendolyn won't run after compilation.



# Compiling in Windows
Just load the Gwendolyn.sln in Visual Studio and press ctrl-b to build or F5 to build-and-run inside VS.

# Compiling in Linux
Make sure your terminal is in the directory where Gwendolyn's source code lives and type "scons"
### WARNING!
It might be possible that Exe/Linux/Gwendolyn.jcr was not created. This is a Jalondi issue that I need to get fixed. Just run "scons" again, and it should appear now.


# Installing in Windows
Easiest way to go is to just use Inno Setup to compile the .iss Script in the Inno directory and to get things installed. Of course you should only do this AFTER compiling.

# Installing in Linux
~~~sh
sudo sh Gwen_LinInstall.sh
~~~
Please mind the "sh". Gwen was set up on an ExFAT device on which I cannot add exectable attributes. That's an ExFAT thing, so no way to fix.
This script will merge the exectable and the internal resources into one file and install it in /usr/bin/ 
It will then also rename it "gwendolyn" (with a lower 'g') in order to be more in line with regular Linux commands. Please note that this will only allow you to quickly access Gwendolyn from the terminal. It will not add Gwendolyn to a Start Menu or whatever, simply because in order to do that I need to take all distros in mind. 



# Important notes
Many assets Gwendolyn needs are missing from the repository.
- For the sound effects that is due to copyright reasons. You are free to find your own files, link the Jalondi script to them and see if you can get them to work in Gwendolyn after compiling.
- The mascot pictures you'll get in official releases are mine and not for distribution under a free license. When not present Gwendolyn simply ignores them.
- The fonts were a bit of a nasty thing, as I have them in a central directory in order not having to copy them into every source dir, only risking conflicts when I upgrade them. I hope I can address that more properly in the future.

