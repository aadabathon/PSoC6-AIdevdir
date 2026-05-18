/**
 * @file    version.hpp
 * @brief   Firmware version and build identification.
 *
 * VERSION_GIT_HASH is replaced at build time by tools/version_gen.py.
 * If you see "unknown" in the field, the version stamping step didn't run.
 */
#pragma once

namespace app {

constexpr int kVersionMajor = 0;
constexpr int kVersionMinor = 1;
constexpr int kVersionPatch = 0;

constexpr const char* kVersionString  = "0.1.0";
constexpr const char* kVersionGitHash = "unknown";  // overridden by version_gen.py
constexpr const char* kBuildDate      = __DATE__ " " __TIME__;

}  // namespace app
