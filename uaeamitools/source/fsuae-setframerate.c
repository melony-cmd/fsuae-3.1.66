/***********************************************************
 * UAE - The U*nix Amiga Emulator                          *
 *                                                         *
 * template -- Emulator Control from Inside Emulation *
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

  int frmrate;

  if (!strcmp(argv[1],"/?") || !strcmp(argv[1],"")) {
    printf("fsuae-setframerate v0.1 (c) 2023 T.J.Roughton\n");
    printf("usage: setframerate 0-? \n\n");
  } else {
    frmrate = atoi( argv[1] );
    SetFrameRate(frmrate);
  }

  return 0;
}

