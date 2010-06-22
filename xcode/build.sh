#!/bin/bash

xcodebuild -project FieldKit.xcodeproj -target "All Libraries" -configuration Debug
xcodebuild -project FieldKit.xcodeproj -target "All Libraries" -configuration Release
