/*

 Intercept Messages from external source and sends them to the emulation
 T.J.Roughton

 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "message.h"

int istack = 0;

// Initalize Messages
int init_message(void) {
  printf("Initalize Message\n");
}

// DeInitalize Messages
int deinit_message(void) {
  printf("Deinitalize Message\n");
}

// Process Messages
void process_message_events(void) {
  if(istack==0) { printf("Listening for Messages\n"); }

  istack++;

  return;
}
