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

  char disk[1024];
  int drvnum;

  if (!strcmp(argv[1],"/?") || !strcmp(argv[1],"")) {
    printf("fsuae-getdisk v0.1 (c) 2023 T.J.Roughton\n");
    printf("usage: getdisk 0->3\n\n");
  } else {
    drvnum = atoi( argv[1] );
    if((drvnum >= 0) && (drvnum <= 3)) {
      GetDisk(drvnum,(UBYTE *)&disk);
      printf("DF%d: = %s\n",drvnum,disk);
    } else {
      printf("Drive number out of range 0-3\n");      
    }
  }

  ExitEmu();

  return 0;
}

