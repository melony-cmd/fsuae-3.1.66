/***********************************************************
 * UAE - The U*nix Amiga Emulator                          *
 *                                                         *
 * fsuae-getx11windowlist                                  *
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
  int listsize;
  int index = 0;
  int i;
  char *buf_winname[256];
  char *p;

  if (!strcmp(argv[1],"/?") || !strcmp(argv[1],"")) {
    printf("fsuae-getx11windowlist v0.1 (c) 2023 T.J.Roughton\n");
    printf("usage: LIST - Lists open windows on X11\n");
    printf("       WindowNum  - Pushes given window number to front\n");
    printf("       WindowName - Pushes given window name to front\n");
    printf("Example: fsuae-getx11windowlist WindowNum 7\n");
    printf("Example: fsuae-getx11windowlist WindowName ThisWindow\n\n");
  } else {
      if(!strcmp(argv[1],"LIST")) {
        listsize = UpdateX11WindowList();
        for (i = 0; i < listsize; i++) {
          GetX11WindowName(i,&buf_winname);
          printf("%d : %s\n",i,buf_winname);
        }
      }
      if(!strcmp(argv[1],"WindowNum")) {
        index = strtol(argv[2], &p, 10);
        RaiseWindowByIndex(index);
      }
      if(!strcmp(argv[1],"WindowName")) {
        /* W.I.P */
      }
  }


  return 0;
}

