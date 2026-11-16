# XED Install Guide
```
mkdir xed-workspace && cd xed-workspace
git clone https://github.com/intelxed/xed.git xed
git clone https://github.com/intelxed/mbuild.git mbuild
```

```
cd xed
python3 mfile.py --share --install-dir=../xed-install install
```

```
sudo cp -r ../xed-install/include/* /usr/local/include/
sudo cp -r ../xed-install/lib/* /usr/local/lib/
sudo ldconfig
```