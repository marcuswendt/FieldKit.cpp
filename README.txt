
                                                                           
      _____  __  _____  __     ____                                   
     / ___/ / / /____/ / /    /    \   FieldKit
    / ___/ /_/ /____/ / /__  /  /  /   (c) 2011, FIELD. All rights reserved.              
   /_/        /____/ /____/ /_____/    http://www.field.io           

   
Created by Marcus Wendt on 24/05/2010.


=== Overview ===

Building blocks for generative art and computational design projects.
Ranging from audio-visual interactive installations to generative 
graphic design applications.

For example uses see: http://www.field.io

The library itself builds heavily on top of the Cinder "open source library for 
professional-quality creative coding in C++"



=== FieldKit ===

Provides core math and animation utilities;

* Physics; An extendible verlet particle system that comes with 
  a growing library of animation behaviours.
  
* Math: Common geometry and math utilities that are not already implemented in Cinder.

- Requirements -

* MS Windows 7 + MS Visual Studio 2008 + Service Pack 1
* Apple Mac OSX 10.6.4+ + Apple XCode 3.2
* Cinder - latest version from github
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
~/Documents/Development/Libraries/v8
 

  
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



=== ScriptKit ===

- Requirements -

* v8 JavaScript Engine

- Installation Mac OSX -

To make sure Xcode can find the header and library files set either:
a) Edit the V8_PATH user-defined setting in the ScriptKit target (if you built v8 previously)
b) Or build v8 next to your fieldkit.cpp folder e.g.
   Open Terminal.app
   $ /path/to/fieldkit.cpp/..
   $ svn checkout http://v8.googlecode.com/svn/trunk/ v8
   $ cd v8/
   $ scons mode=release library=static arch=ia32
   $ scons mode=debug library=static arch=ia32




= Thanks =

* Andrew Bell & Cinder Team - http://libcinder.org
* Dean McNamee - V8 tricks and TypedArrays implementation https://github.com/deanm + http://www.deanmcnamee.com
* Karsten Schmidt - great resource & inspiration http://toxiclibs.org
* many more :)

