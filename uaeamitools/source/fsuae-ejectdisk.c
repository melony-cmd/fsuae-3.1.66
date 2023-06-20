/***********************************************************
 * UAE - The U*nix Amiga Emulator                          *
 *                                                         *
 * fsuae-ejectdisk -- EmuControl from Inside Emulation     *
 *  (c) 2023 T.J.Roughton <melonytr@gmail.com>             *
 *                                                         *
 * Version 0.x                                             *
 *                                                         *
 ***********************************************************/

#include <clib/exec_protos.h>
#include <clib/dos_protos.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <dos.h>

#include "include/uae-control.h"
#include "include/uae_pragmas.h"

int main(int argc,char *argv[]){

  char bufFilePath[1024];
  int drvnum;
  int fd;

  if (!strcmp(argv[1],"/?") || !strcmp(argv[1],"")) {
    printf("fsuae-ejectdisk v0.1 (c) 2023 T.J.Roughton\n");
    printf("usage: drive 0->3\n\n");
  } else {
    drvnum = atoi( argv[1] );
    if((drvnum >= 0) && (drvnum <= 3)) {
      EjectDisk( drvnum );
    } else {
      printf("Drive number out of range 0-3\n");      
    }
  }

  return 0;
}

