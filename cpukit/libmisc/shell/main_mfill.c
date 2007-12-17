/*
 *  MFILL Shell Command Implmentation
 *
 *  Author: Fernando RUIZ CASAS
 *  Work: fernando.ruiz@ctv.es
 *  Home: correo@fernando-ruiz.com
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include <rtems.h>
#include <rtems/shell.h>
#include "internal.h"

extern int rtems_shell_main_mdump(int, char *);

int rtems_shell_main_mfill(int argc,char * argv[]) {
  uintptr_t  addr;
  size_t     size;
  unsigned char value;

  if (argc<4) {
    fprintf(stdout,"too few arguments\n");
    return 0;
  }

  addr = rtems_shell_str2int(argv[1]);
  size = rtems_shell_str2int(argv[2]);
  value= rtems_shell_str2int(argv[3]) % 0x100;
  memset((unsigned char*)addr,size,value);
  rtems_current_shell_env->mdump_addr = addr;

  return rtems_shell_main_mdump(0,NULL);
}

rtems_shell_cmd_t rtems_shell_MFILL_Command = {
  "mfill",                                      /* name */
  "mfill addr size value",                      /* usage */
  "mem",                                        /* topic */
  rtems_shell_main_mfill,                       /* command */
  NULL,                                         /* alias */
  NULL                                          /* next */
};
