#!/usr/bin/env python3
"""
version_gen.py — stamps the current git short hash into app/include/version.hpp.

Usage (as a pre-build step):
    python tools/version_gen.py

Adds the equivalent of:
    constexpr const char* kVersionGitHash = "<short-hash>[-dirty]";
"""

import re
import subprocess
import sys
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parent.parent
VERSION_HEADER = REPO_ROOT / "app" / "include" / "version.hpp"


def git_short_hash() -> str:
    try:
        h = subprocess.check_output(
            ["git", "rev-parse", "--short", "HEAD"],
            cwd=REPO_ROOT,
            stderr=subprocess.DEVNULL,
        ).decode().strip()
    except (subprocess.CalledProcessError, FileNotFoundError):
        return "unknown"

    # Append "-dirty" if there are uncommitted changes.
    try:
        dirty = subprocess.check_output(
            ["git", "status", "--porcelain"],
            cwd=REPO_ROOT,
            stderr=subprocess.DEVNULL,
        ).decode().strip()
        if dirty:
            h += "-dirty"
    except subprocess.CalledProcessError:
        pass

    return h


def stamp(header: Path, new_hash: str) -> None:
    if not header.exists():
        print(f"error: {header} not found", file=sys.stderr)
        sys.exit(1)

    text = header.read_text()
    new_text, n = re.subn(
        r'(constexpr const char\* kVersionGitHash\s*=\s*)"[^"]*"',
        rf'\1"{new_hash}"',
        text,
    )
    if n == 0:
        print(f"warning: kVersionGitHash line not found in {header}", file=sys.stderr)
        return
    if new_text != text:
        header.write_text(new_text)
        print(f"stamped {header.relative_to(REPO_ROOT)} with git hash: {new_hash}")
    else:
        print(f"version.hpp already up to date ({new_hash})")


if __name__ == "__main__":
    stamp(VERSION_HEADER, git_short_hash())
