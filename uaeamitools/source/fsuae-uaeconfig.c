/***********************************************************
 * UAE - The U*nix Amiga Emulator                          *
 *                                                         *
 * fsuae-uaeconfig                                         *
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

#define MAX_NUM_TRAPS	512

struct UAE_CONFIG	config;

char	*langs[]={"US\0","DE\0","SE\0","FR\0","IT\0",};

int main(int argc,char *argv[]){

  int i = GetUaeConfig( &config );

  printf("FSUAE Uaeconfig v%s\n",);


  printf("Version : %d\n",config.version);

  printf("ChipMemory : %d\n",config.chipmemsize);
  printf("SlowMemory : %d\n",config.slowmemsize);
  printf("FastMemory : %d\n",config.fastmemsize);

  printf("FrameRate : %d\n",config.framerate);
  
  printf("Output Sound : %d\n",config.do_output_sound);
  printf("Fake JoyStick : %d\n",config.do_fake_joystick);
  printf("keyboard : %s\n",langs[config.keyboard]);

	printf("DF0 : %s\n", config.disk_in_df0 ? config.df0_name : "EMPTY");
	printf("DF1 : %s\n", config.disk_in_df1 ? config.df1_name : "EMPTY");
	printf("DF2 : %s\n", config.disk_in_df2 ? config.df2_name : "EMPTY");
	printf("DF3 : %s\n", config.disk_in_df3 ? config.df3_name : "EMPTY");

  return 0;
}

