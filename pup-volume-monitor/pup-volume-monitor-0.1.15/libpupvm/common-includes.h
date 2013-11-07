//common-includes.h
//Common library header file

#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/un.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <fcntl.h>
#include <mntent.h>
#include <sys/mount.h>

#include "pupvm-plugin.h"

#undef G_LOG_DOMAIN
#define G_LOG_DOMAIN "pup-volume-monitor"

