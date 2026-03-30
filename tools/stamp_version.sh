#!/bin/sh
# Stamps the version from the .pro VERSION variable into preferences.xml.dist
# and MyInfo.plist. Called as a pre-build step by qmake.
# Usage: stamp_version.sh <version> <dist_file>
# Cross-platform: macOS and Linux.
VERSION="$1"
DIST="$2"
if [ -z "$VERSION" ] || [ -z "$DIST" ]; then
    echo "Usage: stamp_version.sh <version> <dist_file>" >&2
    exit 1
fi
OS="$(uname -s)"
# Strip dots: 2026.03.20 → 20260320
CLEAN=$(echo "$VERSION" | tr -d '.')
# Only match the Application version line, not the XML declaration
# (sed -i syntax differs between BSD/macOS and GNU/Linux)
if [ "$OS" = "Darwin" ]; then
    sed -i '' "s|<Application version=\"[^\"]*\"/>|<Application version=\"${CLEAN}\"/>|" "$DIST"
else
    sed -i "s|<Application version=\"[^\"]*\"/>|<Application version=\"${CLEAN}\"/>|" "$DIST"
fi

# Also stamp MyInfo.plist — macOS app bundle only, skip on Linux
PLIST="$(dirname "$DIST")/MyInfo.plist"
if [ "$OS" = "Darwin" ] && [ -f "$PLIST" ]; then
    # SHORT = e.g. "2026.03.21", FULL = e.g. "2026.03.21.33"
    SHORT=$(echo "$VERSION" | cut -d. -f1-3)
    sed -i '' "/<key>CFBundleShortVersionString<\/key>/{ n; s|<string>[^<]*</string>|<string>${SHORT}</string>|; }" "$PLIST"
    sed -i '' "/<key>CFBundleVersion<\/key>/{ n; s|<string>[^<]*</string>|<string>${VERSION}</string>|; }" "$PLIST"
fi
