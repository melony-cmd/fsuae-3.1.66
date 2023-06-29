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

  int n;

  if (!strcmp(argv[1],"/?") || !strcmp(argv[1],"")) {
    printf("fsuae-changelanguage v0.1 (c) 2023 T.J.Roughton\n");
    printf("usage: 1 = US, 2 = DE, 3 = SE, 4 = FR, 5 = IT \n\n");
  } else {
    n = atoi( argv[1] );
    ChangeLanguage(n);
  }

  return 0;
}

