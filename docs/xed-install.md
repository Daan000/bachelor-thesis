# XED Install Guide

The `xed` CLI is installed at `~/.local/bin/xed` (already on `PATH`).
The headers and libraries used by the thesis live at `~/.local/xed-install`.

## Rebuilding the libraries

```
mkdir -p ~/xed-workspace && cd ~/xed-workspace
git clone https://github.com/intelxed/xed.git xed
git clone https://github.com/intelxed/mbuild.git mbuild
```

```
cd xed
python3 mfile.py --share --install-dir=$HOME/.local/xed-install install
```

The thesis `Makefile` picks this up via `XED_DIR = $(HOME)/.local/xed-install`;
no `/usr/local` copy or `ldconfig` is needed (the binary gets an rpath).
