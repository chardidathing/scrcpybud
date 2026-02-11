# scrcpybud

Tool for use with scrcpy - easily launch scrcpy for a specific device from your system tray.

## Requirements (i can only be bothered writing them for arch :P)

### Arch Linux (or distros)

```
pacman -S base-devel qt6-base scrcpy android-tools
```

## Build

```
qmake6
make -j$(nproc)
```

## Usage

```
./scrcpybud              # run in foreground
./scrcpybud --detached   # fork to background
```

Right-click the tray icon to select a connected ADB device and launch scrcpy.
