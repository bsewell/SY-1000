#!/bin/bash
# deploy.sh — macdeployqt + deterministic bottom-up signing.
set -euo pipefail

APP="/Users/bsewell/010-MUSIC-STUDIO/SY-1000/build/build/packager/SY-1000FloorBoard.app"
DEST="/Applications/SY-1000FloorBoard.app"

if [ ! -d "$APP" ]; then
  echo "Bundle not found: $APP"
  exit 1
fi

if [ -x "/opt/homebrew/opt/qt/bin/macdeployqt" ]; then
  MACDEPLOYQT="/opt/homebrew/opt/qt/bin/macdeployqt"
elif command -v macdeployqt >/dev/null 2>&1; then
  MACDEPLOYQT="$(command -v macdeployqt)"
else
  echo "macdeployqt not found"
  exit 1
fi

sign_bundle() {
  local bundle="$1"
  echo "=== Signing bundle ==="
  # Use one deep ad-hoc signing pass after all path rewrites are complete.
  # The prior bottom-up file signing flow was leaving framework bundles in an
  # invalid state on macOS 26, which then crashed at launch with Code 2.
  codesign --force --deep --sign - "$bundle"

  echo "=== Verifying signature ==="
  codesign --verify --deep --strict --verbose=2 "$bundle"
}

rewrite_bundle_paths() {
  local bundle="$1"
  local contents="$bundle/Contents"

  map_dep_path() {
    local dep="$1"
    local mapped=""
    if [[ "$dep" =~ ([A-Za-z0-9_]+\.framework/Versions/[A-Za-z0-9_]+/[A-Za-z0-9_]+)$ ]]; then
      mapped="@rpath/${BASH_REMATCH[1]}"
    elif [[ "$dep" =~ \.dylib$ ]]; then
      mapped="@executable_path/../Frameworks/$(basename "$dep")"
    fi
    printf '%s' "$mapped"
  }

  echo "=== Rewriting absolute Homebrew install names in $bundle ==="
  while IFS= read -r -d '' f; do
    if ! file "$f" | grep -q "Mach-O"; then
      continue
    fi

    local dylib_id=""
    dylib_id="$(otool -D "$f" 2>/dev/null | sed -n '2p' || true)"
    if [[ "$dylib_id" == /opt/homebrew/* || "$dylib_id" == /usr/local/* ]]; then
      local new_id=""
      new_id="$(map_dep_path "$dylib_id")"
      if [[ -n "$new_id" ]]; then
        install_name_tool -id "$new_id" "$f" || true
      fi
    fi

    while IFS= read -r dep; do
      dep="${dep%% *}"
      [[ -z "$dep" ]] && continue
      if [[ "$dep" == /opt/homebrew/* || "$dep" == /usr/local/* ]]; then
        local mapped=""
        mapped="$(map_dep_path "$dep")"
        if [[ -n "$mapped" ]]; then
          install_name_tool -change "$dep" "$mapped" "$f" || true
        fi
      fi
    done < <(otool -L "$f" 2>/dev/null | tail -n +2 | sed 's/^[[:space:]]*//')
  done < <(find "$contents" -type f -print0)

  echo "=== Remaining absolute Homebrew deps (should be none) ==="
  python3 - "$bundle" <<'PY'
import os, subprocess, sys
bundle = sys.argv[1]
base = os.path.join(bundle, "Contents")
bad = []
for root, _, files in os.walk(base):
    for name in files:
        path = os.path.join(root, name)
        try:
            ft = subprocess.check_output(["file", path], text=True, stderr=subprocess.DEVNULL)
        except Exception:
            continue
        if "Mach-O" not in ft:
            continue
        try:
            deps = subprocess.check_output(["otool", "-L", path], text=True, stderr=subprocess.DEVNULL).splitlines()[1:]
        except Exception:
            continue
        for dep in deps:
            dep = dep.strip().split(" (", 1)[0]
            if dep.startswith("/opt/homebrew/") or dep.startswith("/usr/local/"):
                bad.append((path, dep))
if not bad:
    print("OK: no absolute /opt/homebrew or /usr/local dependencies found")
else:
    for path, dep in bad:
        print(f"{path}\n  {dep}")
    sys.exit(1)
PY
}

echo "=== Running macdeployqt ==="
QMLDIR="/Users/bsewell/010-MUSIC-STUDIO/SY-1000/qml"
"$MACDEPLOYQT" "$APP" -always-overwrite -verbose=1 -qmldir="$QMLDIR"
rewrite_bundle_paths "$APP"

sign_bundle "$APP"

echo "=== Installing to /Applications ==="
rm -rf "$DEST"
cp -R "$APP" "$DEST"
rewrite_bundle_paths "$DEST"
sign_bundle "$DEST"

echo "=== Done ==="
