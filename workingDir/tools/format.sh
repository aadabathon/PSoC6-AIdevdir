#!/usr/bin/env bash
# Format every .c/.cpp/.h/.hpp file under app/src and app/include using
# clang-format with the repo's .clang-format style. Run before committing.

set -euo pipefail

REPO_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

if ! command -v clang-format >/dev/null 2>&1; then
    echo "error: clang-format not found in PATH"
    echo "  install: apt install clang-format   (Linux)"
    echo "           brew install clang-format  (Mac)"
    echo "           winget install LLVM        (Windows)"
    exit 1
fi

cd "$REPO_ROOT"

# Files we want to format. Excludes generated dirs, third_party, build output.
files=$(find app/src app/include -type f \
    \( -name '*.c' -o -name '*.cpp' -o -name '*.h' -o -name '*.hpp' \) \
    2>/dev/null)

# Include main.cpp if present
if [ -f app/main.cpp ]; then
    files="$files app/main.cpp"
fi

if [ -z "$files" ]; then
    echo "no source files found to format"
    exit 0
fi

echo "$files" | xargs clang-format -i --style=file
echo "formatted $(echo "$files" | wc -w) files"
