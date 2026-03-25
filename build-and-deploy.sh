#!/bin/bash
# build-and-deploy.sh — compile + deploy to /Applications in one step
set -euo pipefail

PROJECT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$PROJECT_DIR"

echo "=== Building ==="
make -j$(sysctl -n hw.logicalcpu)

echo "=== Deploying ==="
./deploy.sh

echo "=== Build + Deploy complete ==="
