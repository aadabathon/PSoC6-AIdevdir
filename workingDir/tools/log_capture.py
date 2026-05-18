#!/usr/bin/env python3
"""
log_capture.py — Read from a serial port, print to stdout, save to a file
with timestamps. Useful for debugging long-running sessions.

Usage:
    python tools/log_capture.py COM5
    python tools/log_capture.py /dev/ttyACM0 --baud 115200 --out logs/run.txt

Requires: pip install pyserial
"""

import argparse
import datetime
import sys
from pathlib import Path

try:
    import serial
except ImportError:
    print("error: pyserial not installed. run: pip install pyserial", file=sys.stderr)
    sys.exit(1)


def main() -> None:
    p = argparse.ArgumentParser()
    p.add_argument("port", help="serial port (COM5, /dev/ttyACM0, etc.)")
    p.add_argument("--baud", type=int, default=115200)
    p.add_argument("--out",  type=Path, default=None,
                   help="optional output file; defaults to logs/<timestamp>.txt")
    args = p.parse_args()

    if args.out is None:
        ts = datetime.datetime.now().strftime("%Y%m%d-%H%M%S")
        args.out = Path("logs") / f"{ts}.txt"
    args.out.parent.mkdir(parents=True, exist_ok=True)

    print(f"opening {args.port} @ {args.baud}, logging to {args.out}")
    with serial.Serial(args.port, args.baud, timeout=0.1) as ser, \
         args.out.open("w", buffering=1) as f:
        try:
            while True:
                line = ser.readline().decode(errors="replace").rstrip("\r\n")
                if not line:
                    continue
                stamp = datetime.datetime.now().strftime("%H:%M:%S.%f")[:-3]
                out = f"[{stamp}] {line}"
                print(out)
                f.write(out + "\n")
        except KeyboardInterrupt:
            print("\nstopped.")


if __name__ == "__main__":
    main()
