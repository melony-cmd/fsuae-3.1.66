/***********************************************************
 * UAE - The U*nix Amiga Emulator                          *
 *                                                         *
 * fsuae-hostrun                                           *
 *               -- Emulator Control from Inside Emulation *
 *  (c) 2023 T.J.Roughton <melonytr@gmail.com>             *
 *                                                         *
 * Version 0.x                                             *
 *                                                         *
 ***********************************************************/

#include <clib/exec_protos.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "include/uae-control.h"
#include "include/uae_pragmas.h"

int main(int argc,char *argv[]){
  if (!strcmp(argv[1],"/?") || !strcmp(argv[1],"")) {
    printf("fsuae-hostrun v0.1 (c) 2023 T.J.Roughton\n");
    printf("usage: <host command>\n");
  } else {
    HostRunProgram(argv[1]);
  }
  return 0;
}

