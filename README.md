## Install openframeworks

In the pi home : `cd ~`

[https://openframeworks.cc/setup/raspberrypi/](https://openframeworks.cc/setup/raspberrypi/)

## Install ofxOMXPlayer

```
cd ~/of_v0.10.0_linuxarmv6l_release/addons/
git clone https://github.com/jvcleave/ofxOMXPlayer.git
```

## Install wiringPi

```
cd ~
git clone git://git.drogon.net/wiringPi
cd wiringPi
./build
```

## `/etc/fstab` for USB stick

USB stick must be FAT32 and add this at the end of the file.
Should work with any USB stick, if not use `sudo blkid` to get the UUID and use it instead of `/dev/sda1` (e.g. `UUID=E3AB-886E`).

```
/dev/sda1 /media/movies       vfat    defaults          0       2
```

## Clone the project

```
cd ~
git clone https://github.com/collectif-pow/pow-player.git
```

## Compile and run it

Add the env variables to the `/etc/environment` file (once)

```
MAKEFLAGS=-j4
```

Compile and run (for developement)

```
make
make run
```

## `init.d` script

Copy the following:

```
sudo cp misc/pow-player /etc/init.d/
sudo chmod +x /etc/init.d/pow-player
sudo update-rc.d pow-player defaults
```

This will make the app run at startup

## Configuration

At the root of the USB stick, place an `xml` file like the following:

```
<?xml version="1.0"?>
<file>test.mp4</file>
<online>true</online>
```

The available options are:

*   file: the file name on the folder `movies` on the USB stick
*   online: `true` or `false`, if `true` the player will respond to network messages, if `false`, it will act as a video looper.

## Convert a video

Not sure it's the best way, but it makes the video very light.

```
avconv -i input.mp4 -vcodec h264 -profile:v main -preset medium -tune animation -crf 18 -b-pyramid none -acodec ac3 -ab 1536k -scodec copy output.mp4
```
