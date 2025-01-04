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

#ifndef _I3_WT_PLUGIN_H__
#define _I3_WT_PLUGIN_H__

#include <libxfce4panel/xfce-panel-plugin.h>
#include <i3ipc-glib/i3ipc-glib.h>

G_BEGIN_DECLS

typedef struct
{
  /* panel widgets */
  GtkLabel        *title;

  i3ipcConnection *conn;
} i3WindowTitlePlugin;

G_END_DECLS

#endif /* !_I3_WT_PLUGIN_H__ */
