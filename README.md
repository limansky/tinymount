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

Since version 0.2.4 tinymount is available in gentoo portage tree. So, can install
it using `emerge tinymount`.

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

Troubleshooting
---------------

Tinymount itself doesn't perform any low level operations, it just uses UDisks DBus API
for disk management.  So if UDisk works fine, everything should work.  You can try to 
mount/unmount disks using udisks command line tool, e.g:

`udisks --mount /dev/sdb1`
`udisks --unmount /dev/sdb1`

The common problem is that the ConsoleKit does not work.  You have to ensure that the
`console-kit-daemon` is running and the ConsoleKit session is started.  Use `ck-list-sessions`
to list current sessions.  If you using display managers check, if it supports
ConsoleKit and compiled with ConsoleKit support.  If your display manager does not have
ConsoleKit support or you are using startx script you should start ConsoleKit session manually.
For example launch your window mananger from .xinitrc this way:

`exec ck-launch-session openbox-session`
