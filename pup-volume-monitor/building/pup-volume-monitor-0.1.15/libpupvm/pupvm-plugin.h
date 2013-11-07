//		     PUP-VOLUME-MONITOR : Volume management

//pupvm-plugin.h
//Plugin API header file

//Warning: Include only this header, don't directly include others in this
//directory.

#ifndef PUP_VM_H
#define PUP_VM_H

#include <gio/gio.h>

#include <libudev.h>

#include <sys/socket.h>

G_BEGIN_DECLS

#define PUP_VM_H_INSIDE

#define PUPSOCK_ERR_DOMAIN g_quark_from_string("pupsock")
#include "utils.h"
#include "core.h"
#include "socket.h"
#include "transfer.h"
#include "protocol.h"
#include "conv.h"
#include "pupvm-utils.h"
#include "pupvm-settings.h"
#include "pupvm-classes.h"
#include "pupvm-monitor.h"

#undef PUP_VM_H_INSIDE

G_END_DECLS

#endif //PUP_VM_H

