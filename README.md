TinyMount
=========

TinyMount is a little GUI tool for mount/unmount devices, using UDisks.

TinyMount is inspired by internal KDE mounter, but has no dependencies on
KDE libs (but it Qt based, sorry) and can be used with lightweight 
window managers like awesome, *boxes, etc. with internal tray support, or
with separate tray applications (like stalonetray) if your manager doesn't
support tray.

Main features are:
------------------

 * Tray icon with menu to see currently connected devices.
 * Notifications about connected/disconnected devices.
 * Mounting/unmounting by one click.
 * Automatic mounting.

Usage
-----

TinyMount is quite simple to use. When it launched there is an icon in your
tray.  Right click on the icon opens menu with the devices list. Click on
already mounted device performs device unmounting. If the device is not mounted
click on it to mount.

If you need to define icon theme for the program use `--iconTheme` command line
parameter.

Other settings stored in ~/.config/tinymount/tinymount.conf file, but also
can be defined from settings dialog.

Installation:
-------------

### Gentoo Linux.

If you are Gentoo user please use ebuild file available in downloads section
on TinyMount github page. The ebuild can be used both for stable versions, and
as live ebuild (if you set version to 9999).  Currently TinyMount is tested only
on i686 and x86_64 hosts, but I suppose there should not be any problems on
other architectures.

If you need help in using custom ebuilds see overlays guide:
http://www.gentoo.org/proj/en/overlays/userguide.xml

USE flags:

 * libnotify - turns libnotify support on/off.

### Debian/Ubuntu

Use deb files attached to the TinyMount page. If you need to build latest version,
or use TinyMount on other architectures, you can use code from branch `debian`,
which contains debian folder. Use `dpkg-buildpackage` to build deb package.

### Other distributives / Building from source

TinyMount uses qmake, so the building procedure is quite simple:

 1. `qmake`
 2. `make`
 3. `make install`

By default TinyMount is installed to /usr/local/bin. If you need to change a location
pass PREFIX parameter to qmake call (e.g. `qmake PREFIX=/bin` to install in /usr/bin).

If you want to enable libnotify support run qmake with CONFIG+=with_libnotify
parameter.
