INCLUDES = \
	-I$(top_srcdir)

FLAGS = \
	-DG_LOG_DOMAIN=\"xfce4-i3-window-title-plugin\" \
	-DPACKAGE_LOCALE_DIR=\"$(localedir)\" \
	$(PLATFORM_CPPFLAGS)

#
# i3 window title plugin
#
plugin_LTLIBRARIES = \
	libi3windowtitle.la

plugindir = \
	$(libdir)/xfce4/panel/plugins

libi3windowtitle_la_SOURCES = i3wm-window-title.h i3wm-window-title.c

libi3windowtitle_la_CFLAGS = \
	$(LIBXFCE4UTIL_CFLAGS) \
	$(LIBXFCE4UI_CFLAGS) \
	$(LIBXFCE4PANEL_CFLAGS) \
	$(PLATFORM_CFLAGS)

libi3windowtitle_la_LDFLAGS = \
       -avoid-version \
       -module \
       -no-undefined \
       -export-symbols-regex '^xfce_panel_module_(preinit|init|construct)' \
       $(LIBX11_CFLAGS)
       $(LIBXRANDR_CFLAGS)
       $(PLATFORM_LDFLAGS)

libi3windowtitle_la_LIBADD = \
	$(LIBXFCE4UTIL_LIBS) \
	$(LIBXFCE4UI_LIBS) \
	$(LIBXFCE4PANEL_LIBS) \
	$(LIBI3IPCGLIB_LIBS)

#
# Desktop file
#
desktopdir =								\
	$(datadir)/xfce4/panel/plugins

desktop_DATA =								\
	i3-window-title.desktop

@INTLTOOL_DESKTOP_RULE@

EXTRA_DIST =								\
	i3-window-title.desktop.in

CLEANFILES =								\
	$(desktop_DATA)

# vi:set ts=8 sw=8 noet ai nocindent syntax=automake:
