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

  listsize = UpdateX11WindowList();

  for (i = 0; i < listsize; i++) {
    GetX11WindowName(i,&buf_winname);
    printf("%s\n",buf_winname);
  }

  return 0;
}

