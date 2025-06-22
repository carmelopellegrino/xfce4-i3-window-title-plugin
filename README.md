# An i3wm-aware xfce4-panel plugin

This `xfce4-panel` plugin shows the title of the currently focused window in an
`i3wm` session.

## Installation

To build from sources, you'll need the following dependencies installed:

- i3ipc-glib
- xfce4-panel
- xfce4-dev-tools
- autoconf
- automake
- gettext
- a C compiler (e.g. gcc)

Clone the repo, then:

```shell
cd xfce4-i3-window-title-plugin
./autogen.sh --prefix=/usr
make
sudo make install
```

When installing on `ArchLinux`, you may want to use the `PKGBUILD` available on
AUR: `xfce4-i3-window-title-plugin-git`.

## Usage

To start using the plugin, start the xfce4-panel and add it to the panel.
That's it. No further configuration is foreseen. Text is redered using the
global configuration of the panel.

To start the panel at startup you may want to add the following line to your
`~/.config/i3/config` file:

```config
exec --no-startup-id xfce4-panel -d
```
