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
void on_window_event(i3ipcConnection *conn, i3ipcWindowEvent *e, gpointer user)
{
  i3WindowTitlePlugin* i3wmtp = (i3WindowTitlePlugin*) user;

  if (strcmp(e->change, "close") == 0) {
    gtk_label_set_text(i3wmtp->title, "");
    return;
  }

  gtk_label_set_text(i3wmtp->title, i3ipc_con_get_name(e->container));
}

static
void on_workspace_event(i3ipcConnection *conn, i3ipcWorkspaceEvent *e, gpointer user)
{
  i3WindowTitlePlugin* i3wmtp = (i3WindowTitlePlugin*) user;

  if (strcmp(e->change, "init") == 0) {
    gtk_label_set_text(i3wmtp->title, "");
  }
}

static
void init_connection(i3WindowTitlePlugin* i3wmtp)
{
  i3ipcCommandReply* reply = NULL;
  i3wmtp->conn = i3ipc_connection_new(NULL, NULL);

  reply = i3ipc_connection_subscribe(i3wmtp->conn, I3IPC_EVENT_WINDOW, NULL);

  if (reply->success) {
    i3ipcCon* root = i3ipc_connection_get_tree(i3wmtp->conn, NULL);
    i3ipcCon* focused = i3ipc_con_find_focused(root);
    gtk_label_set_text(i3wmtp->title, i3ipc_con_get_name(focused));

    g_signal_connect_after(i3wmtp->conn, "window", G_CALLBACK(on_window_event), i3wmtp);
  } else {
    gtk_label_set_text(i3wmtp->title, "Error subscribing to i3wm");
  }

  i3ipc_command_reply_free(reply);

  reply = i3ipc_connection_subscribe(i3wmtp->conn, I3IPC_EVENT_WORKSPACE, NULL);

  if (reply->success) {
    g_signal_connect_after(i3wmtp->conn, "workspace", G_CALLBACK(on_workspace_event), i3wmtp);
  } else {
    gtk_label_set_text(i3wmtp->title, "Error subscribing to i3wm");
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
void i3free(XfcePanelPlugin *plugin, i3WindowTitlePlugin *i3wmtp)
{
  gtk_widget_destroy(GTK_WIDGET(i3wmtp->title));
  g_object_unref(i3wmtp->conn);
  g_slice_free(i3WindowTitlePlugin, i3wmtp);
}

static
void i3constructor(XfcePanelPlugin* plugin) {
  i3WindowTitlePlugin* i3wmtp = g_slice_new0(i3WindowTitlePlugin);

  g_signal_connect(G_OBJECT(plugin), "free-data", G_CALLBACK(i3free), i3wmtp);

  GtkWidget* label = gtk_label_new("");

  i3wmtp->title = GTK_LABEL(label);
  gtk_label_set_xalign(i3wmtp->title, 0.);
  gtk_label_set_ellipsize(i3wmtp->title, PANGO_ELLIPSIZE_END);

  init_connection(i3wmtp);

  GtkWidget* reconnect_button = gtk_menu_item_new_with_label("Reconnect");
  xfce_panel_plugin_menu_insert_item(plugin, GTK_MENU_ITEM(reconnect_button));
  gtk_widget_show(GTK_WIDGET(reconnect_button));
  g_signal_connect(G_OBJECT(reconnect_button), "activate", G_CALLBACK(on_reconnect), i3wmtp);
  gtk_container_add(GTK_CONTAINER(plugin), label);
  gtk_widget_show_all(GTK_WIDGET(plugin));
  xfce_panel_plugin_set_expand(plugin, TRUE);
}

XFCE_PANEL_PLUGIN_REGISTER(i3constructor);
