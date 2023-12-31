/*
* UAE - The U*nix Amiga Emulator
*
* UAE Library v0.3.2
*
* (c) 1996 Tauno Taipaleenmaki <tataipal@raita.oulu.fi>
* -- and 27 YEARS LATER!
* (c) 2023 T.J.Roughton <melonytr@gmail.com>
*
* Change UAE parameters and other stuff from inside the emulation.
*/

#define USE_SDL

#ifdef USE_SDL
	#include <SDL.h>
#endif

#include "sysconfig.h"
#include "sysdeps.h"

#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>

#include "options.h"
#include "uae.h"
#include "uae/memory.h"
#include "custom.h"
#include "newcpu.h"
#include "xwin.h"
#include "autoconf.h"
#include "traps.h"
#include "disk.h"
#include "debug.h"
#include "gensound.h"
#include "picasso96.h"
#include "filesys.h"
#include "savestate.h"

#define LIBUAEVERSION "0.3.2"

/*
 * Save State
 */
static uae_u32 emulib_SaveState(uae_u32 emulib_slot){
	printf("UAELib - emulib_SaveState(%d)\n",emulib_slot);
	savestate_quick (emulib_slot,1);
	return 0;
}

/*
 * Load State
 */
static uae_u32 emulib_LoadState(uae_u32 emulib_slot){
	printf("UAELib - emulib_LoadState(%d)\n",emulib_slot);
	savestate_quick (emulib_slot,0);
	return 0;
}

/*
 * Load State
 */
static uae_u32 emulib_DeleteState(uaecptr statename){

	return 0;
}

/*
 *  Get_X11WindowList()
 */
Window *uaelib_x11winlist;

Window *Get_WindowList (Display *disp, unsigned long *len);
char *Get_WindowName (Display *disp, Window win);

/* -- */
static int client_msg(Display *disp, Window win, char *msg, unsigned long data0,
																														unsigned long data1,
																														unsigned long data2,
																														unsigned long data3,
																														unsigned long data4) {
  XEvent event;
  long mask = SubstructureRedirectMask | SubstructureNotifyMask;

  event.xclient.type = ClientMessage;
  event.xclient.serial = 0;
  event.xclient.send_event = True;
  event.xclient.message_type = XInternAtom(disp, msg, False);
  event.xclient.window = win;
  event.xclient.format = 32;
  event.xclient.data.l[0] = data0;
  event.xclient.data.l[1] = data1;
  event.xclient.data.l[2] = data2;
  event.xclient.data.l[3] = data3;
  event.xclient.data.l[4] = data4;

  if (XSendEvent(disp, DefaultRootWindow(disp), False, mask, &event)) {
    return EXIT_SUCCESS;
  }
  else {
    fprintf(stderr, "Cannot send %s event.\n", msg);
    return EXIT_FAILURE;
  }
}

/* -- */
static int Activate_Window (Display *disp, Window win) {

	client_msg(disp, win, "_NET_ACTIVE_WINDOW", 0, 0, 0, 0, 0);
	XMapRaised(disp, win);

	return EXIT_SUCCESS;
}

/* -- */
Window *Get_WindowList (Display *disp, unsigned long *len) {
    Atom prop = XInternAtom(disp,"_NET_CLIENT_LIST",False), type;
    int form;
    unsigned long remain;
    unsigned char *list;

    errno = 0;
    if (XGetWindowProperty(disp,XDefaultRootWindow(disp),prop,0,1024,False,XA_WINDOW,
                &type,&form,len,&remain,&list) != Success) {
        perror("Get_WindowList() -- GetWinProp");
        return 0;
    }

    return (Window*)list;
}

/* -- */
char *Get_WindowName (Display *disp, Window win) {
    Atom prop = XInternAtom(disp,"WM_NAME",False), type;
    int form;
    unsigned long remain, len;
    unsigned char *list;

    errno = 0;
    if (XGetWindowProperty(disp,win,prop,0,1024,False,XA_STRING,
                &type,&form,&len,&remain,&list) != Success) {
        perror("winlist() -- GetWinProp");
        return NULL;
    }

    return (char*)list;
}

/*
	NAME
				emulib_UpdateX11WindowList - get x11 list of open windows.
	SYNOPSIS
	  		uae_u32 listsize = emulib_UpdateX11WindowList(void)
	FUNCTION
				Attempts to aquire the x11 open window list.
	INPUTS
	TAGS
	RESULTS
				listsize - uae_u32 how many indexs of windows in the list.
	EXAMPLE
	NOTES
	SEE ALSO
	BUGS
 */
static uae_u32 emulib_UpdateX11WindowList( void ){
	int i;
	unsigned long len;
	Display *disp = XOpenDisplay(NULL);
	char *name;

	if (!disp) { return -1; }							// well that's highly unlikely!

	XFree(uaelib_x11winlist);

	uaelib_x11winlist = (Window*) Get_WindowList(disp,&len);

  for (i=0;i<(int)len;i++) {
		name = Get_WindowName(disp,uaelib_x11winlist[i]);
    printf("-->%s<--\n",name);
    free(name);
	}
/*
	if(strlen(message)!=0) {
		for (i = 0; i < 256; i++) {
			put_byte (program + i, message[i]);
		}
	}
  XFree(list);
*/

	XCloseDisplay(disp);
	return len;
}

/*
	NAME
				emulib_GetX11WindowName - get window name
	SYNOPSIS
	  		uae_u32 = emulib_GetX11WindowName(index,name)
	FUNCTION
				Gets the window name defined by index
	INPUTS
				index - index number of window name
				name - place to store the text of the name
	TAGS
	RESULTS
				null
	EXAMPLE
	NOTES
	SEE ALSO
	BUGS
*/
static uae_u32 emulib_GetX11WindowName(uae_u32 index, uaecptr name){
	int i;
	Display *disp = XOpenDisplay(NULL);
	char *gw_name = Get_WindowName(disp,uaelib_x11winlist[index]);

	if(strlen(gw_name)!=0) {
		for (i = 0; i < 256; i++) {
			put_byte (name + i, gw_name[i]);
		}
	}

	XCloseDisplay(disp);
	return 0;
}

/*
 *
 */
static uae_u32 emulib_RaiseWindowByIndex(uae_u32 index) {
	Display *disp = XOpenDisplay(NULL);

	Activate_Window (disp,uaelib_x11winlist[index]); // <<-- get name

	XCloseDisplay(disp);

	return 0;
}

/*
 *
 */
static uae_u32 emulib_RaiseWindowByName(uaecptr name,uae_u32 index = -1) {
	Display *disp = XOpenDisplay(NULL);

	Activate_Window (disp,uaelib_x11winlist[index]); // <<-- get name

	XCloseDisplay(disp);

	return 0;
}

/*
* Runs command on host
* - Probably the easist of the 2 functions as it just telling the emulator to call an external program and run it.
*/
static uae_u32 emulib_HostRunProgram (uaecptr program) {
	printf("emulib_HostRunProgram()\n");
	int i = 0;
	char real_program[256];
	char buffer[1024];

	TCHAR *s;
	while ((real_program[i] = get_byte (program + i)) != 0 && i++ != 254);

	if (i == 255)
		return 0; /* ENAMETOOLONG */

	sprintf(buffer,"%s&",real_program);
	printf("Run:%s\n",buffer);
	system(buffer);
	return 0;
}

/*
* Disable/Enable Screen Saver (SDL)
*/
static uae_u32 emulib_HostScreenSaver(uae_u32 state) {

	if(state==0) {
		SDL_DisableScreenSaver();
	}

	if(state==1) {
		SDL_EnableScreenSaver();
	}

	if(SDL_IsScreenSaverEnabled()==SDL_TRUE){
			printf("ScreenSave - Enabled\n");
	} else {
			printf("ScreenSave - Disabled\n");
	}

	return 0;
}


/*
 * emulib_AmigaRunProgram()
 */
static uae_u32 emulib_AmigaRunProgram (uaecptr program) {
	//printf("emulib_AmigaRunProgram()\n");

	int i;
	int fd;
	char *myfifo = (char *)"/tmp/fsuae-message";
	char message[512] = "null";

  mkfifo(myfifo,0666);
  fd = open(myfifo,O_RDONLY|O_NONBLOCK);
	read(fd,message,512);

	//printf("msg:(%d) %s\n",strlen(message),message);

	close(fd);

	if(strlen(message)!=0) {
		for (i = 0; i < 256; i++) {
			put_byte (program + i, message[i]);
		}
	}

	return 0;
}

/*
* Returns UaeLib Version
*/
static uae_u32 emulib_GetUAELibVersion (uaecptr nversion) {
	int i;
	char buf_version[16];

	sprintf(buf_version,"%s",LIBUAEVERSION);
	//printf("%s\n",buf_version);
	for (i = 0; i < strlen(buf_version); i++) {
		put_byte (nversion + i, buf_version[i]);
	}
	return version;
}

/*
* Returns UAE Version
* 	FIXED: Now actually does something more than the nothing it did for 27 years!
*/
static uae_u32 emulib_GetVersion (uaecptr nversion) {
	int i;
	char buf_version[16];

	sprintf(buf_version,"%d.%d.%d",UAEMAJOR,UAEMINOR,UAESUBREV);
	//printf("%s\n",buf_version);
	for (i = 0; i < strlen(buf_version); i++) {
		put_byte (nversion + i, buf_version[i]);
	}
	return version;
}

/*
* Resets your amiga
*/
static uae_u32 emulib_HardReset (void) {
	uae_reset(1, 1);
	return 0;
}

static uae_u32 emulib_Reset (void) {
	uae_reset(0, 0);
	return 0;
}

/*
* Enables SOUND
*/
static uae_u32 emulib_EnableSound (uae_u32 val) {
	if (!sound_available || currprefs.produce_sound == 2)
		return 0;

	currprefs.produce_sound = val;
	return 1;
}

/*
* Enables FAKE JOYSTICK
*/
static uae_u32 emulib_EnableJoystick (uae_u32 val) {
	currprefs.jports[0].id = val & 255;
	currprefs.jports[1].id = (val >> 8) & 255;
	return 1;
}

/*
* Sets the framerate
*/
static uae_u32 emulib_SetFrameRate (uae_u32 val) {
	if (val == 0)
		return 0;
	else if (val > 20)
		return 0;
	else {
		currprefs.gfx_framerate = val;
		return 1;
	}
}

/*
 * Changes keyboard language settings
 */
static uae_u32 emulib_ChangeLanguage (uae_u32 which) {
	if (which > 6)
		return 0;
	else {
		switch (which) {
		case 0:
			currprefs.keyboard_lang = KBD_LANG_US;
			break;
		case 1:
			currprefs.keyboard_lang = KBD_LANG_DK;
			break;
		case 2:
			currprefs.keyboard_lang = KBD_LANG_DE;
			break;
		case 3:
			currprefs.keyboard_lang = KBD_LANG_SE;
			break;
		case 4:
			currprefs.keyboard_lang = KBD_LANG_FR;
			break;
		case 5:
			currprefs.keyboard_lang = KBD_LANG_IT;
			break;
		case 6:
			currprefs.keyboard_lang = KBD_LANG_ES;
			break;
		default:
			break;
		}
		return 1;
	}
}

/* The following ones don't work as we never realloc the arrays...
 * -> then why get me all excited by leaving the code behind hmmm?/*
 */
/*
* Changes chip memory size
*  (reboots)
*/
static uae_u32 REGPARAM2 emulib_ChgCMemSize (uae_u32 memsize) {
	if (memsize != 0x80000 && memsize != 0x100000 &&
		memsize != 0x200000) {
			memsize = 0x200000;
			write_log (_T("Unsupported chipmem size!\n"));
	}
	m68k_dreg (regs, 0) = 0;

	changed_prefs.chipmem_size = memsize;
	uae_reset(1, 1);
	return 1;
}

/*
* Changes slow memory size
*  (reboots)
*/
static uae_u32 REGPARAM2 emulib_ChgSMemSize (uae_u32 memsize) {
	if (memsize != 0x80000 && memsize != 0x100000 &&
		memsize != 0x180000 && memsize != 0x1C0000) {
			memsize = 0;
			write_log (_T("Unsupported bogomem size!\n"));
	}

	m68k_dreg (regs, 0) = 0;
	changed_prefs.bogomem_size = memsize;
	uae_reset (1, 1);
	return 1;
}

/*
* Changes fast memory size
*  (reboots)
*/
static uae_u32 REGPARAM2 emulib_ChgFMemSize (uae_u32 memsize) {
	if (memsize != 0x100000 && memsize != 0x200000 &&
		memsize != 0x400000 && memsize != 0x800000) {
			memsize = 0;
			write_log (_T("Unsupported fastmem size!\n"));
	}
	m68k_dreg (regs, 0) = 0;
	changed_prefs.fastmem_size = memsize;
	uae_reset (1, 1);
	return 0;
}

/*
* Inserts a disk
*/
static uae_u32 emulib_InsertDisk (uaecptr name, uae_u32 drive) {
	int i = 0;
	char real_name[256];
	TCHAR *s;

	if (drive > 3)
		return 0;

	while ((real_name[i] = get_byte (name + i)) != 0 && i++ != 254);

	if (i == 255)
		return 0; /* ENAMETOOLONG */

	s = au (real_name);
	_tcscpy (changed_prefs.floppyslots[drive].df, s);
	xfree (s);

	return 1;
}

/*
* Exits the emulator
*/
static uae_u32 emulib_ExitEmu (void) {
	uae_quit ();
	return 1;
}

/*
* Gets UAE Configuration
*/
static uae_u32 emulib_GetUaeConfig (uaecptr place) {
	int i, j;

	put_long (place, version);
	put_long (place + 4, chipmem_bank.allocated);
	put_long (place + 8, bogomem_bank.allocated);
	put_long (place + 12, fastmem_bank.allocated);
	put_long (place + 16, currprefs.gfx_framerate);
	put_long (place + 20, currprefs.produce_sound);
	put_long (place + 24, currprefs.jports[0].id | (currprefs.jports[1].id << 8));
	put_long (place + 28, currprefs.keyboard_lang);
	if (disk_empty (0))
		put_byte (place + 32, 0);
	else
		put_byte (place + 32, 1);
	if (disk_empty (1))
		put_byte (place + 33, 0);
	else
		put_byte (place + 33, 1);
	if (disk_empty(2))
		put_byte (place + 34, 0);
	else
		put_byte (place + 34, 1);
	if (disk_empty(3))
		put_byte (place + 35, 0);
	else
		put_byte (place + 35, 1);

	for (j = 0; j < 4; j++) {
		char *s = ua (currprefs.floppyslots[j].df);
		for (i = 0; i < 256; i++)
			put_byte (place + 36 + i + j * 256, s[i]);
		xfree (s);
	}
	return 1;
}

/*
* Sets UAE Configuration
*
* NOT IMPLEMENTED YET
*/
static uae_u32 emulib_SetUaeConfig (uaecptr place) {
	return 1;
}

/*
* Gets the name of the disk in the given drive
*/
static uae_u32 emulib_GetDisk (uae_u32 drive, uaecptr name) {
	int i;
	if (drive > 3)
		return 0;

	for (i = 0; i < 256; i++) {
		put_byte (name + i, currprefs.floppyslots[drive].df[i]);
	}
	return 1;
}

/*
* Enter debugging state
*/
static uae_u32 emulib_Debug (void) {

	#ifdef DEBUGGER
		activate_debugger ();
		return 1;
	#else
		return 0;
	#endif
}

/*
 * What an infernal mess!
 */
#define CREATE_NATIVE_FUNC_PTR uae_u32 (* native_func) (uae_u32, uae_u32, uae_u32, uae_u32, uae_u32, uae_u32, uae_u32, \
	uae_u32, uae_u32, uae_u32, uae_u32, uae_u32, uae_u32)

#define SET_NATIVE_FUNC(x) native_func = (uae_u32 (*)(uae_u32, uae_u32, uae_u32, uae_u32, uae_u32, uae_u32, uae_u32, uae_u32, uae_u32, uae_u32, uae_u32, uae_u32, uae_u32))(x)

#define CALL_NATIVE_FUNC( d1,d2,d3,d4,d5,d6,d7,a1,a2,a3,a4,a5,a6 ) if(native_func) native_func( d1,d2,d3,d4,d5,d6,d7,a1,a2,a3,a4,a5,a6 )
/* A0 - Contains a ptr to the native .obj data.  This ptr is Amiga-based. */
/*      We simply find the first function in this .obj data, and execute it. */

/*
 *
 */
static uae_u32 REGPARAM2 emulib_ExecuteNativeCode (void) {
#if 0
	uaecptr object_AAM = m68k_areg (regs, 0);
	uae_u32 d1 = m68k_dreg (regs, 1);
	uae_u32 d2 = m68k_dreg (regs, 2);
	uae_u32 d3 = m68k_dreg (regs, 3);
	uae_u32 d4 = m68k_dreg (regs, 4);
	uae_u32 d5 = m68k_dreg (regs, 5);
	uae_u32 d6 = m68k_dreg (regs, 6);
	uae_u32 d7 = m68k_dreg (regs, 7);
	uae_u32 a1 = m68k_areg (regs, 1);
	uae_u32 a2 = m68k_areg (regs, 2);
	uae_u32 a3 = m68k_areg (regs, 3);
	uae_u32 a4 = m68k_areg (regs, 4);
	uae_u32 a5 = m68k_areg (regs, 5);
	uae_u32 a6 = m68k_areg (regs, 6);

	uae_u8* object_UAM = NULL;
	CREATE_NATIVE_FUNC_PTR;

	if (get_mem_bank(object_AAM).check (object_AAM, 1))
		object_UAM = get_mem_bank (object_AAM).xlateaddr (object_AAM);

	if (object_UAM) {
		SET_NATIVE_FUNC (FindFunctionInObject (object_UAM));
		CALL_NATIVE_FUNC (d1, d2, d3, d4, d5, d6, d7, a1, a2, a3, a4, a5, a6);
	}
	return 1;
#endif
	return 0;
}

/*
 * Genius this does... nothing! :)
 */
static uae_u32 emulib_Minimize (void) {
	return 0; // OSDEP_minimize_uae();
}

/*
 * What on earth is this meant to do?
 */
static int native_dos_op (uae_u32 mode, uae_u32 p1, uae_u32 p2, uae_u32 p3) {
	TCHAR tmp[MAX_DPATH];
	char *s;
	int v, i;

	if (mode)
		return -1;
	/* receive native path from lock
	* p1 = dos.library:Lock, p2 = buffer, p3 = max buffer size
	*/
	v = get_native_path (p1, tmp);
	if (v)
		return v;
	s = ua (tmp);
	for (i = 0; i <= strlen (s) && i < p3 - 1; i++) {
		put_byte (p2 + i, s[i]);
		put_byte (p2 + i + 1, 0);
	}
	xfree (s);
	return 0;
}

/*
 *  All calltrap()'s seem to land here
 */
static uae_u32 uaelib_demux_common(uae_u32 ARG0, uae_u32 ARG1, uae_u32 ARG2, uae_u32 ARG3, uae_u32 ARG4, uae_u32 ARG5) {

	write_log(_T("uaelib_demux_common() TRAP CALL: %d\n"), ARG0);

	switch (ARG0) {
		case 0: return emulib_GetVersion(ARG1);
		case 1: return emulib_GetUaeConfig(ARG1);
		case 2: return emulib_SetUaeConfig(ARG1);
		case 3: return emulib_HardReset();
		case 4: return emulib_Reset();
		case 5: return emulib_InsertDisk(ARG1, ARG2);
		case 6: return emulib_EnableSound(ARG1);
		case 7: return emulib_EnableJoystick(ARG1);
		case 8: return emulib_SetFrameRate(ARG1);
		case 9: return emulib_ChgCMemSize(ARG1);
		case 10: return emulib_ChgSMemSize(ARG1);
		case 11: return emulib_ChgFMemSize(ARG1);
		case 12: return emulib_ChangeLanguage(ARG1);
			/* The next call brings bad luck */
		case 13: return emulib_ExitEmu();
		case 14: return emulib_GetDisk(ARG1, ARG2);
		case 15: return emulib_Debug();

		case 68: return emulib_Minimize();
		case 69: return emulib_ExecuteNativeCode();

		case 70: return 0; /* RESERVED. Something uses this.. */

		case 80:
		if (!currprefs.maprom)
			return 0xffffffff;
		/* Disable possible ROM protection */
		unprotect_maprom();
		return currprefs.maprom;
		case 81: return cfgfile_uaelib(ARG1, ARG2, ARG3, ARG4);
		case 82: return cfgfile_uaelib_modify(ARG1, ARG2, ARG3, ARG4, ARG5);
		case 83: currprefs.mmkeyboard = ARG1 ? 1 : 0; return currprefs.mmkeyboard;
	#ifdef DEBUGGER
		case 84: return mmu_init(ARG1, ARG2, ARG3);
	#endif
		case 85: return native_dos_op(ARG1, ARG2, ARG3, ARG4);
		case 86:
		if (valid_address(ARG1, 1)) {
			TCHAR *s = au((char*)get_real_address(ARG1));
			write_log(_T("DBG: %s\n"), s);
			xfree(s);
			return 1;
		}
		return 0;
		case 87:
		{
			uae_u32 d0, d1;
			d0 = emulib_target_getcpurate(ARG1, &d1);
			m68k_dreg(regs, 1) = d1;
			return d0;
		}

		/* keep away from whatever is going above ;) */
		case 128: return emulib_GetUAELibVersion(ARG1);
		case 129: return emulib_HostScreenSaver(ARG1);
		case 130: return emulib_HostRunProgram(ARG1);
		case 131: return emulib_AmigaRunProgram(ARG1);
		case 132: return emulib_UpdateX11WindowList();
		case 133: return emulib_GetX11WindowName(ARG1,ARG2);
		case 134: return emulib_RaiseWindowByName(ARG1);
		case 135: return emulib_RaiseWindowByIndex(ARG1);
		case 136: return emulib_SaveState(ARG1);
		case 137: return emulib_LoadState(ARG1);
	}
	return 0;
}

/*
 *
 */
uae_u32 uaeboard_demux(uae_u32 *board) {
	uae_u32 arg0, arg1, arg2, arg3, arg4, arg5;

	arg0 = do_get_mem_word((uae_u16*)&board[0]);
	arg1 = do_get_mem_long(&board[2]);
	arg2 = do_get_mem_long(&board[3]);
	arg3 = do_get_mem_long(&board[4]);
	arg4 = do_get_mem_long(&board[5]);
	arg5 = do_get_mem_long(&board[6]);
	return uaelib_demux_common(arg0, arg1, arg2, arg3, arg4, arg5);
}

/*
 *
 */
static uae_u32 REGPARAM2 uaelib_demux2 (TrapContext *context) {
	#define ARG0 (get_long (m68k_areg (regs, 7) + 4))
	#define ARG1 (get_long (m68k_areg (regs, 7) + 8))
	#define ARG2 (get_long (m68k_areg (regs, 7) + 12))
	#define ARG3 (get_long (m68k_areg (regs, 7) + 16))
	#define ARG4 (get_long (m68k_areg (regs, 7) + 20))
	#define ARG5 (get_long (m68k_areg (regs, 7) + 24))

	#ifdef PICASSO96
		if (ARG0 >= 16 && ARG0 <= 39) {
			return picasso_demux(ARG0, context);
		}
	#endif
	return uaelib_demux_common(ARG0, ARG1, ARG2, ARG3, ARG4, ARG5);
}

/*
 *
 */
extern int uaelib_debug;

static uae_u32 REGPARAM2 uaelib_demux (TrapContext *context) {
	uae_u32 v;
	struct regstruct *r = &regs;

	if (uaelib_debug)
		write_log (_T("%d: %08x %08x %08x %08x %08x %08x %08x %08x, %08x %08x %08x %08x %08x %08x %08x %08x\n"),
		ARG0,
		r->regs[0],r->regs[1],r->regs[2],r->regs[3],r->regs[4],r->regs[5],r->regs[6],r->regs[7],
		r->regs[8],r->regs[9],r->regs[10],r->regs[11],r->regs[12],r->regs[13],r->regs[14],r->regs[15]);
	v = uaelib_demux2 (context);

	if (uaelib_debug)
		write_log (_T("=%08x\n"), v);

	return v;
}

/*
 * Installs the UAE LIBRARY
 */
void emulib_install (void) {
	uaecptr a;
	if (!uae_boot_rom_type) { return; }
	a = here ();
	currprefs.mmkeyboard = 0;
	org (rtarea_base + 0xFF60);
	#if 0
		dw (0x4eb9);
		dw ((rtarea_base >> 16) | get_word (rtarea_base + 36));
		dw (get_word (rtarea_base + 38) + 12);
	#endif
	calltrap (deftrapres (uaelib_demux, 0, _T("uaelib_demux")));
	dw (RTS);
	org (a);
}
