#!/bin/bash
# build.sh — always increments VERSION before compiling.
# Never call qmake/make directly. Always use this script.
# Cross-platform: macOS and Linux. Windows: use tools/build.bat instead.

set -e

PRO_FILE="SY-1000FloorBoard.pro"
TODAY=$(date +%Y.%m.%d)
OS="$(uname -s)"

# Read current version
CURRENT=$(grep '^VERSION = ' "$PRO_FILE" | awk '{print $3}')
BUILD_NUM=$(echo "$CURRENT" | awk -F. '{print $4}')
NEXT_BUILD=$((BUILD_NUM + 1))
NEW_VERSION="${TODAY}.${NEXT_BUILD}"

# Update .pro file (sed -i syntax differs between BSD/macOS and GNU/Linux)
if [ "$OS" = "Darwin" ]; then
    sed -i '' "s/^VERSION = .*/VERSION = ${NEW_VERSION}/" "$PRO_FILE"
else
    sed -i "s/^VERSION = .*/VERSION = ${NEW_VERSION}/" "$PRO_FILE"
fi

# Force version into preferences.xml.dist and bust the rcc cache so the
# new version string is always baked into the binary, even on incremental builds.
./tools/stamp_version.sh "$NEW_VERSION" preferences.xml.dist
touch preferences.xml.dist
rm -f build/release/qrc_SY-1000FloorBoard.o

echo "──────────────────────────────────────"
echo "Version: $CURRENT → $NEW_VERSION"
echo "──────────────────────────────────────"

qmake SY-1000FloorBoard.pro

# CPU count differs between macOS (sysctl) and Linux (nproc)
if [ "$OS" = "Darwin" ]; then
    JOBS=$(sysctl -n hw.logicalcpu)
else
    JOBS=$(nproc)
fi
make -j"$JOBS"

echo "──────────────────────────────────────"
echo "BUILD SUCCEEDED: $NEW_VERSION"
echo "──────────────────────────────────────"

# Deploy to /Applications — macOS only
if [ "$OS" = "Darwin" ]; then
    rm -rf /Applications/SY-1000FloorBoard.app
    cp -R build/packager/SY-1000FloorBoard.app /Applications/SY-1000FloorBoard.app
    echo "DEPLOYED: /Applications/SY-1000FloorBoard.app"
    echo "──────────────────────────────────────"
fi
