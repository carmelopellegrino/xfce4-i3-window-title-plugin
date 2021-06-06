/*  $Id$
 *
 *  Copyright (C) 2021 Carmelo Pellegrino <carmelo.pellegrino@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "i3wm-window-title.h"
#include <libxfce4panel/libxfce4panel.h>
#include <libxfce4util/libxfce4util.h>

#include <stdio.h>
#include <string.h>

static void i3constructor(XfcePanelPlugin *plugin);

static
char const* focused_window_title(i3ipcConnection* conn)
{
  i3ipcCon* root = i3ipc_connection_get_tree(conn, NULL);
  i3ipcCon* focused = i3ipc_con_find_focused(root);

  return i3ipc_con_get_name(focused);
}

static
gsize min(gsize a, gsize b)
{
  return a > b ? b : a;
}

static
gboolean show_title(i3WindowTitlePlugin* i3wmtp)
{
  char const* current = focused_window_title(i3wmtp->conn);
  gsize const text_space = i3wmtp->text_space;
  gsize const current_len = current == NULL ? 0 : strlen(current);

  static char buffer[1024];
  gsize const max_len = min(min(text_space, current_len), 1024);

  snprintf(buffer, max_len , "%s", current);
  gtk_label_set_text(i3wmtp->title, buffer);

  return TRUE;
}

static
gboolean size_changed(XfcePanelPlugin *plugin, gint size, i3WindowTitlePlugin* i3wmtp)
{
  i3wmtp->text_space = size;
  return show_title(i3wmtp);
}

static
void on_title_event(i3ipcConnection* conn, i3ipcGenericEvent* e, gpointer user)
{
  i3WindowTitlePlugin* i3wmtp = (i3WindowTitlePlugin*) user;
  show_title(i3wmtp);
}

static
void init_connection(i3WindowTitlePlugin* i3wmtp)
{
  i3wmtp->conn = i3ipc_connection_new(NULL, NULL);

  i3ipcCommandReply* reply = i3ipc_connection_subscribe(i3wmtp->conn, I3IPC_EVENT_WINDOW, NULL);

  if (reply->success) {
    g_signal_connect_after(i3wmtp->conn, "window", G_CALLBACK(on_title_event), i3wmtp);
  } else {
    // booooh!
  }

  i3ipc_command_reply_free(reply);
}

static
void on_reconnect(GtkMenuItem* mitem, i3WindowTitlePlugin* i3wmtp)
{
  g_object_unref(i3wmtp->conn);
  init_connection(i3wmtp);
}

static
void i3constructor(XfcePanelPlugin* plugin) {
  i3WindowTitlePlugin* i3wmtp = g_slice_new0(i3WindowTitlePlugin);

  GtkWidget* label = gtk_label_new("");

  i3wmtp->title = GTK_LABEL(label);

  init_connection(i3wmtp);

  g_signal_connect(G_OBJECT(plugin), "size-changed", G_CALLBACK(size_changed), i3wmtp);

  GtkWidget* reconnect_button = gtk_menu_item_new_with_label("Reconnect");
  xfce_panel_plugin_menu_insert_item(plugin, GTK_MENU_ITEM(reconnect_button));
  gtk_widget_show(GTK_WIDGET(reconnect_button));
  g_signal_connect(G_OBJECT(reconnect_button), "activate", G_CALLBACK(on_reconnect), i3wmtp);
  gtk_container_add(GTK_CONTAINER(plugin), label);
  gtk_widget_show_all(label);
  xfce_panel_plugin_set_expand(XFCE_PANEL_PLUGIN(plugin), TRUE);
}

XFCE_PANEL_PLUGIN_REGISTER(i3constructor);
