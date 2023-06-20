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


long doCmd(UBYTE *command) {
  struct TagItem stags[4];
  stags[0].ti_Tag = SYS_Asynch;
  stags[0].ti_Data = TRUE;
  stags[1].ti_Tag = TAG_END;
  return System(command, stags);
}

int main(int argc,char *argv[]){

  char buf[257];
  char runbuf[1024];

  if (!strcmp(argv[1],"/?") || !strcmp(argv[1],"")) {
    printf("fsuae-server v0.1 (c) 2023 T.J.Roughton\n");
    printf("usage: start\n");
  } else {
    while (1) {
      AmigaRunProgram((UBYTE *)&buf);
      if(strcmp(buf,"null")!=0) {
        printf("{%d} msg:(%d)[%s]\n",strcmp(buf,"null"),strlen(buf),buf);
        //doCmd(buf);
        //System(buf,SYS_Asynch);
        sprintf(runbuf,"Run %s",buf);
        printf(runbuf);
        System(runbuf);
      }
      Delay(50);
      printf("tick!\n");
    }
  }
  return 0;
}

