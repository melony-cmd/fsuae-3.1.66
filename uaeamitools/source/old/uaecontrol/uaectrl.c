/***********************************************************
 * UAE - The U*nix Amiga Emulator                          *
 *                                                         *
 * UAE-Ctrl -- Emulator Control from Inside Emulation      *
 *  (c) 1996 Tauno Taipaleenmaki <tataipal@raita.oulu.fi>  *
 *  (c) 2023 T.J.Roughton <melonytr@gmail.com>
 *                                                         *
 * Version 0.2                                             *
 *                                                         *
 * Command line version, Should work with any KS version   *
 *                                                         *
 ***********************************************************/

/* sc uaectrl.c opt link to uaectrl noicons */

/*
 0.1 -- Orginal Version
 0.2 -- Code cleaning because no one can work in a mess!
				Calling Traps (tests)
 */

#include <clib/exec_protos.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "uae-control.h"
#include "uae_pragmas.h"

#define MAX_DRV_NAME	20
#define MAX_NUM_TRAPS	512

struct UAE_CONFIG			config;

void print_drive_status(void);
void quit_program(int error, char *text);

/************************************
 * Main program                     *
 ************************************/
int main() {
	int	quit = 0,i, correct,number;
	char	buf[257];
	char bufAmigaRun[512];

  char	*langs[]={"US\0","DE\0","SE\0","FR\0","IT\0",};

	/* Read UAE configuration */
	i = GetUaeConfig( &config );

	while( quit == 0 ) {
		printf("  UAE-Control v0.3\n\n");
		printf("  1) Reset\n");
		printf("  2) Debug\n");
		printf("  3) Exit Emulator\n");
		printf("  4) Change framerate     (Currently : %ld)\n", config.framerate);
		printf("  5) Toggle sound         (Currently : %s)\n", config.do_output_sound ? "ON" : "OFF");
		printf("  6) Toggle fake joystick (Currently : %s)\n", config.do_fake_joystick ? "ON" : "OFF");
		printf("  7) Change language      (Currently : %s)\n", langs[config.keyboard]);
		printf("  8) Eject a disk\n");
		printf("  9) Insert a disk\n\n");
		printf("128) GetHostClipboard\n");
		printf("129) GetAmigaClipboard\n");
		printf("130) Run program on Host\n");
		printf("131) Run program on Amiga (testing purposes)\n");
		printf("99) Exit UAE-Control\n\n");
		correct = 0;

		while( correct == 0 ) {
			printf(" Command : ");
			gets( buf );
			i = atoi( buf );
			if ((i > 0) && (i < MAX_NUM_TRAPS)) {
				correct = 1;
			}
		}

		switch( i ) {
			case 1:
				HardReset();
				break;

			case 2:
				DebugFunc();
				break;

			case 3:
				ExitEmu();
			break;

			case 4:
				printf(" Enter new framerate (1-20) :");
				gets( buf );
				number = atoi( buf );

				if (SetFrameRate (number)) {
					GetUaeConfig(&config);
				} else {
					printf(" Illegal value, not changed.\n");
				}
				break;

			case 5:
				if (config.do_output_sound) {
					DisableSound();
				} else {
					EnableSound();
					GetUaeConfig( &config );
				}
				break;

			case 6:
				if (config.do_fake_joystick) {
					DisableJoystick();
				} else {
					EnableJoystick();
					GetUaeConfig( &config );
				}
				break;

			case 7:
				printf(" 1 = US, 2 = DE, 3 = SE, 4 = FR, 5 = IT\n");
				printf(" What will it be : ");
				gets( buf );
				number = atoi( buf );

				if ((number >= 1) && (number <= 5)) {
					ChangeLanguage( number-1 );
					GetUaeConfig( &config );
					} else {
						printf(" Illegal value, not changed.\n");
				}
				break;

			case 8:
				print_drive_status();
				printf(" Eject which drive (1-4): ");
				gets( buf );
				number = atoi( buf );

				if ((number >= 1) && (number <=4 )) {
					EjectDisk( number-1 );
					GetUaeConfig( &config );
				} else {
					printf(" Illegal drive, not changed.\n");
				}
				break;

			case 9:
				print_drive_status();
				printf(" Enter disk to drive (1-4): ");
				gets( buf );
				number = atoi( buf );

				if ((number >= 1) && (number <= 4)) {
					printf("Name of diskfile :");
					gets( buf );
					InsertDisk( (UBYTE *)&buf, number - 1 );
					GetUaeConfig( &config );
				} else {
					printf(" Illegal drive, not changed.\n");
				}
				break;

			case 99:
				quit = 1;
				break;

			case 128:
				GetHostClipboard();
				break;

			case 129:
				GetAmigaClipboard();
				break;

			case 130:
	  		printf("(HOST) Name of Program:");
  			gets( buf );
				HostRunProgram((UBYTE *)&buf);
				break;

			case 131:
				AmigaRunProgram();
				break;



		}
	}
	quit_program(0, "");
	return(0);
}

/******************************************
 * Prints drive status										*
 ******************************************/
void print_drive_status(void) {
	printf(" DF0 : %s\n", config.disk_in_df0 ? config.df0_name : "EMPTY");
	printf(" DF1 : %s\n", config.disk_in_df1 ? config.df1_name : "EMPTY");
	printf(" DF2 : %s\n", config.disk_in_df2 ? config.df2_name : "EMPTY");
	printf(" DF3 : %s\n", config.disk_in_df3 ? config.df3_name : "EMPTY");
}



/******************************************
 * Quits the program											*
 ******************************************/
void quit_program(int error, char *text) {
	if (error > 0) {
		printf(" UAE-Control v0.3\n");
		printf("  (c)1996 Tauno Taipaleenmaki\n");
		printf("  (c)2023 T.J.Roughton\n\n");
		printf(" ERROR: %s\n", text);
	}
}
