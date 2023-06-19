/*

 Intercept Messages from external source and sends them to the emulation
 T.J.Roughton

 */
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "message.h"

int fd;
char message[512];
char old_message[512];

// Initalize Messages
int init_message(void) {
  printf("Initalize Message\n");
  char *myfifo = "/tmp/fsuae-message";
  mkfifo(myfifo,0666);
  fd = open(myfifo,O_RDONLY|O_NONBLOCK);
}

// DeInitalize Messages
int deinit_message(void) {
  printf("Deinitalize Message\n");
  if(fd) { close(fd); }
}

// Process Messages
void process_message_events(void) {
  //printf("Listening for Messages\n");
  read(fd,message,512);
  if(strlen(message)!=0) {
    if (strcmp(old_message,message)!=0) {
      message[strlen(message)-1] = 0;
      printf("msg:(%d)[%s]\n",strlen(message),message);
      strcpy(old_message,message);
    }
  }
  return;
}
