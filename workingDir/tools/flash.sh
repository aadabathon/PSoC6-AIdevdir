#!/usr/bin/env bash
# flash.sh — build (if needed) and flash the firmware to the board.
# Run from repo root.

set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$REPO_ROOT/app"

# Stamp version before build so the running firmware reports the right hash.
python "$REPO_ROOT/tools/version_gen.py" || true

make build -j8
make program
