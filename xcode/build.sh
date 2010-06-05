#!/bin/bash

xcodebuild -project FieldKit.xcodeproj -target FieldKit -configuration Debug
xcodebuild -project FieldKit.xcodeproj -target FieldKit -configuration Release
