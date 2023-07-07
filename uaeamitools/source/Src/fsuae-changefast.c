/***********************************************************
 * UAE - The U*nix Amiga Emulator                          *
 *                                                         *
 * fsuae-changefast                                        *
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
    printf("usage: CFASTMEM_1MB | CFASTMEM_2MB | CFASTMEM_4MB | CFASTMEM_8MB | NODELAY\n\n");
    printf("** will reboot emulation! \n");
  } else {
    if(!strcmp(argv[1],"CFASTMEM_1MB")) {n = CFASTMEM_1MB;}
    if(!strcmp(argv[1],"CFASTMEM_2MB")) {n = CFASTMEM_2MB;}
    if(!strcmp(argv[1],"CFASTMEM_4MB")) {n = CFASTMEM_4MB;}
    if(!strcmp(argv[1],"CFASTMEM_8MB")) {n = CFASTMEM_8MB;}

    if(n!=NULL) {
      printf("Setting %s\n",argv[1]);
      if(strcmp(argv[2],"NODELAY")) {
        Delay(100);
      }
      ChgFMemSize(n);
    }
  }

  return 0;
}

