/***********************************************************
 * UAE - The U*nix Amiga Emulator                          *
 *                                                         *
 * fsuae-insertdisk                                        *
 *               -- Emulator Control from Inside Emulation *
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
    printf("fsuae-insertdisk v0.1 (c) 2023 T.J.Roughton\n");
    printf("usage: <adf/dms> 0->3\n\n");
    printf("NB: Use 'HOST' path(s) to files NOT internal paths within emulation!\n");
  } else {
    strcpy(bufFilePath,argv[1]);
    drvnum = atoi( argv[2] );
    printf("Attempting to Insert ... %s into %d\n",bufFilePath,drvnum);

    if((drvnum >= 0) && (drvnum <= 3)) {
      printf("Inserting into %d\n",drvnum);
      InsertDisk( (UBYTE *)&bufFilePath, drvnum );
    } else {
      printf("Drive number out of range 0-3\n");
    }
  }

  return 0;
}

