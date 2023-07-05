/***********************************************************
 * UAE - The U*nix Amiga Emulator                          *
 *                                                         *
 * WBStarup_Execute                                        *
 *             -- Executes a script from with in WBStartup *
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
  System("Execute S:User-WBStartup");
  return 0;
}

