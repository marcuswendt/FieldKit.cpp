
                                                                           
      _____  __  _____  __     ____                                   
     / ___/ / / /____/ / /    /    \   FieldKit
    / ___/ /_/ /____/ / /__  /  /  /   (c) 2010, FIELD. All rights reserved.              
   /_/        /____/ /____/ /_____/    http://www.field.io           

   
Created by Marcus Wendt on 24/05/2010.


=== Overview ===

Building blocks for generative art and computational design projects.
Ranging from audio-visual interactive installations to generative 
graphic design applications.

For example uses see: http://www.field.io

The library itself builds on top of the Cinder "open source library for 
professional-quality creative coding in C++"



=== FieldKit ===

Provides core math, animation and persistance utilities;

* Physics; An extendible verlet particle system that comes with 
  a growing library of animation behaviours.
  
* Math: Common geometry and math utilities that are not already implemented in Cinder.

* Tweak: A simple solution to define an application data model. Used to create 
  an abstraction layer between graphical interface and application allowing to 
  load/save an applications configuration/ state.


- Requirements -

* MS Windows 7 + MS Visual Studio 2008 + Service Pack 1
* Apple Mac OSX 10.6.4+ + Apple XCode 3.2
* Cinder V0.81+
  http://libcinder.org
  http://github.com/cinder/Cinder

  
- Optional -

* Mercurial to get latest source from bitbucket.org
  http://mercurial.selenic.com/
  hg clone https://field@bitbucket.org/field/fieldkit.cpp


- Installation -

To make sure all headers and libraries can be found by Xcode/Visual Studio place the FieldKit.cpp folder and its dependent libraries next to each other in the same folder e.g. on my OSX machine

~/Documents/Development/Libraries/fieldkit.cpp
~/Documents/Development/Libraries/Cinder (latest version from github)
~/Documents/Development/Libraries/FBXSDK (link to /Applications/Autodesk/FBXSDK2011_3)
~/Documents/Development/Libraries/OpenCV


=== Vision Kit ===

Aims to provide a robust and extendible solution to common computer-vision problems;
Blob detection, blob tracking, contouring and finger tip detection.


- Requirements -

* FieldKit
* OpenCV 2.1
  Windows
  http://sourceforge.net/projects/opencvlibrary/files/opencv-win/2.1/OpenCV-2.1.0-win32-vs2008.exe/download
  
  Mac OSX
  http://sourceforge.net/projects/opencvlibrary/files/opencv-unix/2.1/OpenCV-2.1.0.tar.bz2/download

  

  
=== FBX Kit ===

Simplifies using geometry and animation data from 3D content creation tools 
e.g. Maya, Cinema 4D, 3D Studio Max in realtime applications.


- Requirements -

* FieldKit
* Autodesk FBX SDK 2011.3 (Windows/ Mac OSX/ Linux)
  http://usa.autodesk.com/adsk/servlet/pc/index?siteID=123112&id=6837478


- Installation Mac OSX -

On Mac OSX the SDK installs to /Applications/Autodesk/FBXSDK$version 
To make sure Xcode can find the header and library files set either:
a) Edit the FBXSDK_PATH user-defined setting in the FBXKit target 
b) Or create a symbolic link to the current SDK next to your fieldkit.cpp folder e.g.
   Open Terminal.app
   $ cd /path/to/fieldkit.cpp
   $ ln -s /Applications/Autodesk/FBXSDK20113_1 ../FBXSDK





