#!/bin/bash

xcodebuild -project FieldKit.xcodeproj -alltargets -configuration Debug -parallelizeTargets
xcodebuild -project FieldKit.xcodeproj -alltargets -configuration Release -parallelizeTargets
