from pathlib import Path

header = Path("registers.txt").read_text()

XED_REGISTERS = [line.split("uint64_t ")[1].split(';')[0] for line in header.split('\n')]