/***********************************************************
 * UAE - The U*nix Amiga Emulator                          *
 *                                                         *
 * fsuae-savestate                                         *
 *               -- Emulator Control from Inside Emulation *
 *  (c) 2023 T.J.Roughton <melonytr@gmail.com>             *
 *                                                         *
 * Version 0.x                                             *
 *                                                         *
 ***********************************************************/

#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <dos/dos.h>
#include <dos/dostags.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "include/uae-control.h"
#include "include/uae_pragmas.h"

int main(int argc,char *argv[]){

  unsigned long slot;

  if (!strcmp(argv[1],"/?") || !strcmp(argv[1],"")) {
    printf("fsuae-savestate v0.1 (c) 2023 T.J.Roughton\n");
    printf("usage: <slot number> | NOOVERWRITE\n\n");
  } else {
    if(!strcmp(argv[2],"NOOVERWRITE")) { // no over write is intended to prevent a slot from being overwritten.

    } else {
      slot = atoi( argv[1] );
      SaveState(slot);
    }
  }

  return 0;
}

