/***********************************************************
 * UAE - The U*nix Amiga Emulator                          *
 *                                                         *
 * fsuae-changechip                                        *
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

  unsigned long n = NULL;

  if (!strcmp(argv[1],"/?") || !strcmp(argv[1],"")) {
    printf("fsuae-changefast v0.1 (c) 2023 T.J.Roughton\n");
    printf("usage: CCHIPMEM_512K | CCHIPMEM_1MB | CCHIPMEM_1_5MB | CCHIPMEM_1_8MB | NODELAY\n\n");
    printf("** will reboot emulation! \n");
  } else {
    if(!strcmp(argv[1],"CCHIPMEM_512K")) {n = CCHIPMEM_512K;}
    if(!strcmp(argv[1],"CCHIPMEM_1MB")) {n = CCHIPMEM_1MB;}
    if(!strcmp(argv[1],"CCHIPMEM_1_5MB")) {n = CCHIPMEM_1_5MB;}
    if(!strcmp(argv[1],"CCHIPMEM_1_8MB")) {n = CCHIPMEM_1_8MB;}

    if(n!=NULL) {
      printf("Setting %s\n",argv[1]);
      if(strcmp(argv[2],"NODELAY")) {
        Delay(100);
      }
      ChgCMemSize(n);
    }
  }


  return 0;
}

