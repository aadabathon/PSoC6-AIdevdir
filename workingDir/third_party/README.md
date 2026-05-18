# third_party/

Vendored libraries NOT pulled by ModusToolbox `make getlibs`.

## Recommended additions

```bash
cd third_party

# Bosch reference drivers (the way to talk to BMI270 / BMM350):
git submodule add https://github.com/BoschSensortec/BMI270-Sensor-API.git
git submodule add https://github.com/BoschSensortec/BMM350-SensorAPI.git

# Embedded Template Library — no-heap STL replacements:
git submodule add https://github.com/ETLCPP/etl.git
```

Then in `app/Makefile.local.mk`, uncomment the relevant lines under the "Third-party" comment.

## Why submodules instead of `git clone` + commit

Pinning to a specific commit of an external repo is exactly what submodules do. You get reproducible builds without ballooning your repo. Clone the project with `--recurse-submodules` (or run `git submodule update --init --recursive` after cloning) and you have everything.
