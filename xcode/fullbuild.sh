#!/bin/bash

PROJ=FieldKit.xcodeproj

# Mac
xcodebuild -project $PROJ -target FieldKit-OSX -configuration Debug -parallelizeTargets
xcodebuild -project $PROJ -target FieldKit-OSX -configuration Release -parallelizeTargets

# iOS
xcodebuild -project $PROJ -target FieldKit-iOS -configuration Debug -sdk iphoneos
xcodebuild -project $PROJ -target FieldKit-iOS-Sim -configuration Debug -sdk iphonesimulator
xcodebuild -project $PROJ -target FieldKit-iOS -configuration Release -sdk iphoneos
xcodebuild -project $PROJ -target FieldKit-iOS-Sim -configuration Release -sdk iphonesimulator
