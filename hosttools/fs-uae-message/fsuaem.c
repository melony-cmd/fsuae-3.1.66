/***********************************************************
 * UAE - The U*nix Amiga Emulator                          *
 *                                                         *
 * FSUAE-Message - Emulator Control from outside Emulation *
 *  (c) 2023 T.J.Roughton <melonytr@gmail.com>
 *                                                         *
 * Version 0.1                                             *
 *                                                         *
 ***********************************************************/
#include<stdio.h>
#include<errno.h>
#include<stdlib.h>
#include<string.h>

extern int errno;

void main (int argc,char *argv[]) {
  printf("FS-UAE Message v0.1\n");

  // check arguments
  if(argc==1) {
    printf("no arguments given\n");
    return;
  }

  // find fs-uae pid
  char pidline[1024];
  char *pid;
  int i =0;
  int errnum;
  int pidno[64];

  FILE *fp = popen("pidof fs-uae","r");
  if(fp == NULL) {
      errnum = errno;
      fprintf(stderr,"Value of errno: %d\n",errno);
      fprintf(stderr,"Error : %s\n",strerror(errnum));
      return;
  } else {
    fgets(pidline,1024,fp);

    pid = strtok (pidline," ");
    pidno[i] = atoi(pid);

    if(pidno[i]==0) {
      printf("Unable to locate fs-uae process\n");
      pclose(fp);
      return;
    } else {
      printf("Located fs-uae process %d\n",pidno[i]);
    }
    pclose(fp);
  }
  // do passed arguments.

}
