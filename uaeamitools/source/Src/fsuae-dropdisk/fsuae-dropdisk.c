/*----------------------------------------------------------------------------*/
/* Drop Disk v0.1                                                             */
/*  By T.J.Roughton (c) 2023                                                  */
/*                                                                            */
/* Notes:                                                                     */
/* - Problem                                                                  */
/* Clearly Device/Assignment are not going to translate well to host machine  */
/* - Solution                                                                 */
/* Need some tomfoolery configuration file that replaces Dev/Assign with the  */
/* host path, or it will never find anything to insert as a disk              */
/*                                                                            */
/* TODO:                                                                      */
/*  CHIP/FAST need default value for each platform.                           */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* System Includes                                                            */
/*----------------------------------------------------------------------------*/
#include <exec/memory.h>
#include <intuition/gadgetclass.h>
#include <libraries/gadtools.h>
#include <proto/exec.h> 
#include <proto/dos.h>
#include <proto/intuition.h>
#include <proto/gadtools.h>
#include <proto/utility.h>
#include <proto/asl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <dos.h>

/*----------------------------------------------------------------------------*/
/* FSUAE                                                                      */
/*----------------------------------------------------------------------------*/
#include "include/uae-control.h"
#include "include/uae_pragmas.h"

/*----------------------------------------------------------------------------*/
/* 3rd Party Includes                                                         */
/*----------------------------------------------------------------------------*/
#include "config.h"

/*----------------------------------------------------------------------------*/
/* Gadget IDs                                                                 */
/*----------------------------------------------------------------------------*/
#define GID_LIST        1001
#define GID_STRING      1002
#define GID_ADD         1003
#define GID_DELETE      1004
#define GID_CLEAR       1005
#define GID_PLATFORM    1006
#define GID_CHIPRAM     1007
#define GID_FASTRAM     1008
#define GID_BOOT        1009

#define GID_INSERTDF0   2000
#define GID_INSERTDF1   2001
#define GID_INSERTDF2   2002
#define GID_INSERTDF3   2003
#define GID_EJECTDF0    3000
#define GID_EJECTDF1    3001
#define GID_EJECTDF2    3002
#define GID_EJECTDF3    3003

/*----------------------------------------------------------------------------*/
/* Macros                                                                     */
/*----------------------------------------------------------------------------*/
#ifndef __amigaos4__
  #define NewList(list) ((list)->lh_Head = (struct Node *)&(list)->lh_Tail,(list)->lh_TailPred = (struct Node *)&(list)->lh_Head,(list)->lh_Tail = NULL)
  #define GetHead(list) ((list) && (list)->lh_Head && (list)->lh_Head->ln_Succ ? (list)->lh_Head : NULL)
  #define GetTail(list) ((list) && (list)->lh_TailPred && (list)->lh_TailPred->ln_Pred ? (list)->lh_TailPred : NULL)
  #define GetSucc(node) ((node) && (node)->ln_Succ->ln_Succ ? (node)->ln_Succ : NULL)
  #define GetPred(node) ((node) && (node)->ln_Pred->ln_Pred ? (node)->ln_Pred : NULL)
#endif

/*----------------------------------------------------------------------------*/
/* Globals
/*----------------------------------------------------------------------------*/
struct WBArg *frargs;

static const char *cyPlatforms[] = {"A1000",
	                                  "A2000",
	                                  "A2000A",
	                                  "A2500HD",
	                                  "A500",
	                                  "A3000",
	                                  "A500+",
	                                  "A600",
	                                  "A4000",
	                                  "A1200",
	                                  "Custom",
	                                  NULL};

// DEFAULT MEMORY SETTINGS SET WHEN CHANGING PLATFORMS
static const int cyPDefaultCMem[] = {0,              // A1000
	                                   1,              // A2000
	                                   1,              // A2000A
	                                   1,              // A2500HD
	                                   0,              // A500
	                                   1,              // A3000
	                                   1,              // A500+
	                                   1,              // A600
	                                   2,              // A4000
	                                   2,              // A1200
	                                   0,              // Custom
	                                   NULL};                

static const int cyPDefaultFMem[] = {0,              // A1000
	                                   0,              // A2000
	                                   0,              // A2000A
	                                   0,              // A2500HD
	                                   0,              // A500
	                                   1,              // A3000
	                                   0,              // A500+
	                                   0,              // A600
	                                   0,              // A4000
	                                   0,              // A1200
	                                   0,              // Custom
	                                   NULL};                


static const char *cyChipRam[] = {"512",
	                                "1024",
	                                "2048",
	                                "4096",
                                  NULL};

static const char *cyFastRam[] = {"0",
	                                "1024",
	                                "2048",
	                                "4096",
	                                "8192",
                                  NULL};


char configfile [] = {"fsuae-dropdisk.cfg"};
char amiga_tmpfolderpath[256];
char host_tmpfolderpath[256];
char asldefaultdir[256];
int defaultPlatform;

/*----------------------------------------------------------------------------*/
/* Copy part of a string to another
/*----------------------------------------------------------------------------*/
void copySubstring(const char* source, char* destination, size_t startIndex, size_t length){
    size_t i;

    // Copy characters from the source string to the destination
    for (i = 0; i < length && source[startIndex + i] != '\0'; i++)    {
        destination[i] = source[startIndex + i];
    }

    // Null-terminate the destination buffer
    destination[i] = '\0';
}

/*----------------------------------------------------------------------------*/
/* Get a Char in a string position
/*----------------------------------------------------------------------------*/
int getCharPosition(const char *string, char searchChar)
{
    int position = -1; // Initialize position to -1 (not found)
    int i;

    for (i = 0; i != strlen(string); i++) {
        if (string[i] == searchChar) {
            position = i; // Store the position and break the loop
            break;
        }
    }
    
    return position;
}

/*----------------------------------------------------------------------------*/
/* Get Node
/*----------------------------------------------------------------------------*/
struct Node *Get_Node (struct List *list,ULONG num) {
	struct Node *node;
	ULONG i = 0;

	for (node = GetHead(list); node; node = GetSucc(node)) {
		if (i == num)
			return (node);
		i ++;
	}

	return (NULL);
}

/*----------------------------------------------------------------------------*/
/* Number Node                                                                */
/*----------------------------------------------------------------------------*/
ULONG Node_Number (struct List *list,struct Node *node_to_find)	{
	struct Node *node;
  ULONG i = 0;

	for (node = GetHead(list); node; node = GetSucc(node)) {
	  if (node == node_to_find) {
		  return (i);
    }
		i ++;
	}
	return (-1);
}

/*----------------------------------------------------------------------------*/
/* Add Node                                                                   */
/*----------------------------------------------------------------------------*/
struct Node *Add_NodeName(struct List *list,char *name){
  struct Node *node;

  if (node = AllocVec (sizeof(struct Node) + strlen(name) + 1,MEMF_CLEAR)) {
    node->ln_Name = (STRPTR)(node + 1);
    strcpy (node->ln_Name,name);
    AddTail (list,node);
	} else {
    return (NULL);    
  }
  return (node);
}

/*----------------------------------------------------------------------------*/
/* Delete Node                                                                */
/*----------------------------------------------------------------------------*/
struct Node *Delete_Node(struct Node *node){
  struct Node *nextnode;

  nextnode = GetSucc (node);
	Remove (node);
	FreeVec (node);

  return nextnode;
}


/*----------------------------------------------------------------------------*/
/* Free List                                                                  */
/*----------------------------------------------------------------------------*/
void free_list (struct List *list) {
	struct Node *node;
	while (node = RemHead (list)) {
		FreeVec (node);
  }
}

/*----------------------------------------------------------------------------*/
/* Get fsuae configuration file list                                          */
/* Bugs: Well this was how we was going to do it, however CYCLE_KIND doesn't  */
/* leand itself well to this situation, it should be a LISTVIEW_KIND to the   */
/* right of the disk list view if we REALLY want to get that indepth with     */
/* .fsuae configurations thus I'm leaving this code here as is in case I need */
/* it in the future                                                           */
/*----------------------------------------------------------------------------*/
void getfsuae_configlist(void) {
  /*
  struct FileInfoBlock fib;
  BPTR lock;
  int count = 0;

  char *directoryPath = "Sys:"; //"Configurations";

  printf("Read Configuration Direcory\n");

  lock = Lock(directoryPath, ACCESS_READ);
  if(!lock) {
    printf("Failed to lock directory %s\n",directoryPath);
    return 1;
  }

  if(Examine(lock,&fib)){
    do {
      printf("%s\n",fib.fib_FileName);

      count++;
    } while (ExNext(lock,&fib));
  }
  UnLock(lock);
  */
}

/*----------------------------------------------------------------------------*/
/* Get File(s) from ASL
/* BUGS: Device: / Assign: needs to be striped from path before comparing it   
   with .cfg files definitions.
*/
/*----------------------------------------------------------------------------*/
void GetFile_List(struct Window *win,struct FileRequester *filereq,struct List *flist,struct List *dflist)	{
  int x;
	char fn_name[256];
	char pf_name[256];
  char prefbuffer[256];
  char devassign[256];
  char remdevassign[256];

  if (AslRequestTags (filereq,
                      ASLFR_Window,win,
                      ASLFR_DoSaveMode,FALSE,
                      ASL_Dir,asldefaultdir,
                      ASL_FuncFlags, FILF_MULTISELECT | FILF_PATGAD,
                      TAG_END)) {

    printf("%s\n",filereq->fr_Drawer);
    printf("%s\n",filereq->fr_File);

    if(filereq->rf_NumArgs){
      printf("Selected %d\n",filereq->rf_NumArgs);

      frargs = filereq->rf_ArgList;
      for( x=0; x<filereq->rf_NumArgs; x++){
        printf("%ld : path=%s file=%s \n",x,filereq->rf_Dir,frargs[x].wa_Name);

        if(strlen(filereq->rf_Dir)!=0) {
          /* Does rf_Dir contain a : */
          //printf("== %d\n",getCharPosition(filereq->rf_Dir,':'));
          copySubstring(filereq->rf_Dir,devassign,0,getCharPosition(filereq->rf_Dir,':')+1);

          /* Strip device/assign from path */
          copySubstring(filereq->rf_Dir,remdevassign,getCharPosition(filereq->rf_Dir,':')+1,strlen(filereq->rf_Dir));

          /* Must be a mounted device */
          printf("Search Config Sections for %s\n",devassign);          
          ReadConfig(configfile,devassign,"Path",prefbuffer,256,"(unknown)");
          printf("Path: %s\n",prefbuffer);

          // FULL & Complete path to file
          sprintf(fn_name,"%s",frargs[x].wa_Name);
          sprintf(pf_name,"%s/%s/%s",prefbuffer,remdevassign,frargs[x].wa_Name);

          Add_NodeName(flist,fn_name);              // ListView_Kind -- list (ONLY!)
          Add_NodeName(dflist,pf_name);             // Path file List
        } else {
          /* Is not a mounted device, but local to where the we was launched from 
             we should do nothing with it, as we depend on a device: / assign:
             to repath the disk.
          */
        }
      }      
    }

  } else {
    printf("Failed to open asl window!\n");
  }
}

/*----------------------------------------------------------------------------*/
/* Insert Disk                                                                */
/*  -- Inserts to the current emulation a disk                                */
/*----------------------------------------------------------------------------*/
void EjectFloppy(int gadid){
  printf("Eject df%d:\n",(gadid-3000));
  EjectDisk( gadid-3000 );
}

/*----------------------------------------------------------------------------*/
/* Insert Disk                                                                */
/*  -- Inserts to the current emulation a disk                                */
/*----------------------------------------------------------------------------*/
void InsertFloppy(int selection,int gadid,struct List *list){
  struct Node *node = Get_Node(list,selection);

  printf("Insert %s into df%d:\n",node->ln_Name,(gadid-2000));
  InsertDisk((UBYTE *)node->ln_Name, gadid-2000 );
}

/*----------------------------------------------------------------------------*/
/* Boot!                                                                      */
/*  -- now the fun can start  :)                                              */
/*----------------------------------------------------------------------------*/
int Boot(int platformnum,int chipnum,int fastnum,struct List *list){

  char cfgfile[256];
  char tmpfile[256];
  char hostboot[1024];
  int fd_read;
  int fd_bytesread;
  int fd_write;
  char buffer[256];
  struct Node *node;
  int ni = NULL;

  sprintf(cfgfile,"Configurations/%s.fs-uae",cyPlatforms[platformnum]);
  printf("cfgfile = %s\n",cfgfile);

  sprintf(tmpfile,"%s/temp_config.fs-uae",amiga_tmpfolderpath);
  printf("tmpfile = %s\n",tmpfile);

  fd_write = Open(tmpfile,MODE_NEWFILE);
  if(fd_write) {
    fd_read = Open(cfgfile,MODE_OLDFILE);
    if(fd_read) {
      while(fd_bytesread = Read(fd_read,buffer,256)) {
        printf("%s",buffer);
        Write(fd_write,buffer,fd_bytesread);
      }
      Write(fd_write,"\n",1);


      //floppy_image_#
			//if (node = Get_Node (list,0)) {
			//GT_SetGadgetAttrs (strgad,win,NULL,GTST_String,node->ln_Name,GA_Disabled,FALSE,TAG_END);
			//}

      // Write the memory settings.
      
      sprintf(buffer,"chip_memory = %s\n",cyChipRam[chipnum]);
      Write(fd_write,buffer,strlen(buffer));

      sprintf(buffer,"fast_memory = %s\n",cyFastRam[fastnum]);
      Write(fd_write,buffer,strlen(buffer));

      sprintf(buffer,"slow_memory = 0\n");
      Write(fd_write,buffer,strlen(buffer));

      // now we get all the disks that (might be added in the listview and add them to the outbound configuration file!)

      while(node = Get_Node(list,ni)) {
        if(ni==0) {
          sprintf(buffer,"floppy_drive_0 = %s\n",node->ln_Name);
          Write(fd_write,buffer,strlen(buffer));
        }
        sprintf(buffer,"floppy_image_%d = %s\n",ni,node->ln_Name);
        printf("------ >> %s \n",buffer);        
        Write(fd_write,buffer,strlen(buffer));
        ni++;
      }



      Close(fd_read);
      Close(fd_write);

      printf("\nfs-uae %s/temp_config.fs-uae\n",host_tmpfolderpath);

      sprintf(hostboot,"fs-uae %s/temp_config.fs-uae",host_tmpfolderpath);

      HostRunProgram(hostboot);
    } else {
      printf("Cannot find %s\n",cfgfile);
    }
  } else {
    printf("Cannot write %s\n",tmpfile);
  }

  return 0;
}

/*----------------------------------------------------------------------------*/
/* Main Program
/*----------------------------------------------------------------------------*/
int main (int argc,char *argv[])	{
	struct Screen *scr;
	struct Window *win;
	struct NewGadget ng;
	ULONG winsig;
	ULONG sigs;
	BOOL cont;
	struct Gadget *glist;
	struct Gadget *gad;
	struct Gadget *lvgad;
	struct Gadget *cygad_platform;
	struct Gadget *cygad_chipram;
	struct Gadget *cygad_fastram;
	struct FileRequester *filereq;
	
  struct List lvlist;
  struct List fplist;

	struct Node *node;
	ULONG fontw,fonth;
	ULONG winw,winh;
	struct IntuiMessage *imsg;
	ULONG num = NULL;
  ULONG chipcynum = NULL;
  ULONG fastcynum = NULL;

	NewList (&lvlist);        // Filename list;
                            // This list is for exclusive use of LISTVIEW_KIND (only!) do not refer to it to do anything functional.

  NewList (&fplist);        // Path & Filename list;
                            // Yes I know, I should only need ONE list for this
                            // but since documenation is lacking in how to extend 'struct Node' or use my own 'struct MyNode'
                            // and logical attempts to do so only produced a catatonic vomit of seizure inducing compile errors,
                            // we will do it thus....
  
  ReadConfig(configfile,"SETTINGS","AmigaTempFolder",amiga_tmpfolderpath,256,"(unknown)");
  ReadConfig(configfile,"SETTINGS","HostTempFolder",host_tmpfolderpath,256,"(unknown)");
  ReadConfig(configfile,"SETTINGS","ASLDefaultDir",asldefaultdir,256,"#?");

  defaultPlatform = ReadConfigNumber(configfile,"SETTINGS","DefaultPlatform",3);

  printf("(Amiga) TempFolder Path = %s\n",amiga_tmpfolderpath);
  printf("(Host) TempFolder Path = %s\n",host_tmpfolderpath);
  printf("Default Platform = %d\n",defaultPlatform);
  printf("Argv[0] = %s\n",argv[0]);

	if (scr = LockPubScreen (NULL))	{
		glist = NULL;
		gad = CreateContext (&glist);

		ng.ng_VisualInfo = GetVisualInfo (scr,TAG_END);
		ng.ng_TextAttr   = scr->Font;
		ng.ng_Flags      = 0;

		fontw = scr->RastPort.TxWidth;
		fonth = scr->RastPort.TxHeight;

		winw = scr->WBorTop + fonth + 5;  /* top of listview */
		winh = 20 * fonth + 4;            /* height of listview */

		ng.ng_LeftEdge   = scr->WBorLeft + 4;
		ng.ng_TopEdge    = winw + winh;
		ng.ng_Height     = fonth + 6;
		ng.ng_Width      = 316; //3*16 * fontw + 3*8 + 2*4;
		ng.ng_GadgetText = NULL;
		ng.ng_GadgetID   = GID_STRING;
		gad = CreateGadget (STRING_KIND,gad,&ng,GA_Disabled,TRUE,TAG_END);
		//strgad = gad;

		ng.ng_TopEdge    = winw;
		ng.ng_Height    += winh;      /* including string height */
		ng.ng_GadgetID   = GID_LIST;
		gad = CreateGadget (LISTVIEW_KIND,gad,&ng,GTLV_Labels,&lvlist,GTLV_ShowSelected,gad,TAG_END);
		lvgad = gad;

    // ROW 1
    ng.ng_TopEdge   += ng.ng_Height + 4;
		ng.ng_Width      = 2 * fontw + 13;
		ng.ng_Height     = fonth + 6;
		ng.ng_GadgetText = "+";
		ng.ng_GadgetID   = GID_ADD;
		gad = CreateGadget (BUTTON_KIND,gad,&ng,TAG_END);

    ng.ng_LeftEdge  += ng.ng_Width + 4;
		ng.ng_GadgetText = "-";
		ng.ng_GadgetID   = GID_DELETE;
		gad = CreateGadget (BUTTON_KIND,gad,&ng,TAG_END);

    ng.ng_LeftEdge  += ng.ng_Width + 4;
		ng.ng_GadgetText = "C";
		ng.ng_GadgetID   = GID_CLEAR;
		gad = CreateGadget (BUTTON_KIND,gad,&ng,TAG_END);

    ng.ng_LeftEdge  += ng.ng_Width + 4;
		ng.ng_Width      = 8 * fontw + 13;
		ng.ng_GadgetID   = GID_PLATFORM;
    ng.ng_GadgetText = "";
		gad = CreateGadget (CYCLE_KIND,gad,&ng,GTCY_Labels,&cyPlatforms,TAG_END); // cyPlatforms
    cygad_platform = gad;

    ng.ng_LeftEdge  += ng.ng_Width + 4;
		ng.ng_Width      = 7 * fontw + 10;
		ng.ng_GadgetID   = GID_CHIPRAM;
    ng.ng_GadgetText = "";
		gad = CreateGadget (CYCLE_KIND,gad,&ng,GTCY_Labels,&cyChipRam,TAG_END); // cyPlatforms
    cygad_chipram = gad;

    ng.ng_LeftEdge  += ng.ng_Width + 4;
		ng.ng_Width      = 7 * fontw + 10;
		ng.ng_GadgetID   = GID_FASTRAM;
    ng.ng_GadgetText = "";
		gad = CreateGadget (CYCLE_KIND,gad,&ng,GTCY_Labels,&cyFastRam,TAG_END); // cyPlatforms
    cygad_fastram = gad;

    ng.ng_TopEdge   += ng.ng_Height + 4;
    ng.ng_LeftEdge   = scr->WBorLeft + 4;
		ng.ng_Width      = 39 * fontw + 4;
		ng.ng_Height     = fonth + 6;
		ng.ng_GadgetText = "Boot";
		ng.ng_GadgetID   = GID_BOOT;
		gad = CreateGadget (BUTTON_KIND,gad,&ng,TAG_END);

    // ROW 3
    ng.ng_TopEdge   += ng.ng_Height + 4;
    ng.ng_LeftEdge   = scr->WBorLeft + 4;
		ng.ng_Width      = 7 * fontw + 20;
		ng.ng_Height     = fonth + 6;
		ng.ng_GadgetText = "DF0:";
		ng.ng_GadgetID   = GID_INSERTDF0;
		gad = CreateGadget (BUTTON_KIND,gad,&ng,TAG_END);

    ng.ng_LeftEdge  += ng.ng_Width + 4;
		ng.ng_GadgetText = "DF1:";
		ng.ng_GadgetID   = GID_INSERTDF1;
		gad = CreateGadget (BUTTON_KIND,gad,&ng,TAG_END);

    ng.ng_LeftEdge  += ng.ng_Width + 4;
		ng.ng_GadgetText = "DF2:";
		ng.ng_GadgetID   = GID_INSERTDF2;
		gad = CreateGadget (BUTTON_KIND,gad,&ng,TAG_END);

    ng.ng_LeftEdge  += ng.ng_Width + 4;
		ng.ng_GadgetText = "DF3:";
		ng.ng_GadgetID   = GID_INSERTDF3;
		gad = CreateGadget (BUTTON_KIND,gad,&ng,TAG_END);

    // ROW 4
    ng.ng_TopEdge   += ng.ng_Height + 4;
    ng.ng_LeftEdge   = scr->WBorLeft + 4;
		ng.ng_Width      = 7 * fontw + 20;
		ng.ng_Height     = fonth + 6;
		ng.ng_GadgetText = "Eject";
		ng.ng_GadgetID   = GID_EJECTDF0;
		gad = CreateGadget (BUTTON_KIND,gad,&ng,TAG_END);

    ng.ng_LeftEdge  += ng.ng_Width + 4;
		ng.ng_GadgetText = "Eject";
		ng.ng_GadgetID   = GID_EJECTDF1;
		gad = CreateGadget (BUTTON_KIND,gad,&ng,TAG_END);

    ng.ng_LeftEdge  += ng.ng_Width + 4;
		ng.ng_GadgetText = "Eject";
		ng.ng_GadgetID   = GID_EJECTDF2;
		gad = CreateGadget (BUTTON_KIND,gad,&ng,TAG_END);

    ng.ng_LeftEdge  += ng.ng_Width + 4;
		ng.ng_GadgetText = "Eject";
		ng.ng_GadgetID   = GID_EJECTDF3;
		gad = CreateGadget (BUTTON_KIND,gad,&ng,TAG_END);


	  filereq = AllocAslRequestTags (ASL_FileRequest,
		  ASLFR_InitialLeftEdge,(scr->Width - winw) / 2,
		  ASLFR_InitialTopEdge,(scr->Height - winh) / 2,
		  ASLFR_InitialWidth,500,
		  ASLFR_InitialHeight,500,
		  TAG_END);

		if (gad) {
			winw = 332; //ng.ng_LeftEdge + ng.ng_Width + 4 + scr->WBorRight;
			winh = 270; //ng.ng_TopEdge + ng.ng_Height + 4 + scr->WBorBottom;
			if (win = OpenWindowTags (NULL,
					WA_Title,"FSUAE DropDisk v0.1",
					WA_Width,winw,
					WA_Height,winh,
					WA_Left,(scr->Width - winw) / 3,
					WA_Top,(scr->Height - winh) / 3,
					WA_Flags,WFLG_CLOSEGADGET | WFLG_DRAGBAR | WFLG_DEPTHGADGET | WFLG_ACTIVATE,
					WA_IDCMP,IDCMP_CLOSEWINDOW | IDCMP_VANILLAKEY | IDCMP_REFRESHWINDOW | BUTTONIDCMP | STRINGIDCMP | LISTVIEWIDCMP,
					WA_PubScreen,scr,
					WA_Gadgets,glist,
					TAG_END)) {

            GT_SetGadgetAttrs (cygad_platform,win,NULL,GTCY_Active,defaultPlatform,TAG_END);

            GT_RefreshWindow (win,NULL);
				    winsig = 1L << win->UserPort->mp_SigBit;
				    cont = TRUE;

				    do {
					    sigs = Wait (winsig | SIGBREAKF_CTRL_C);
					    if (sigs & SIGBREAKF_CTRL_C) { cont = FALSE; }
					    if (sigs & winsig) {
						    while (imsg = GT_GetIMsg (win->UserPort))	{
							    switch (imsg->Class) {
							      case IDCMP_GADGETUP:


								      gad = (struct Gadget *) imsg->IAddress;
								      switch (gad->GadgetID) {
								        case GID_LIST:
									        GT_GetGadgetAttrs (lvgad,win,NULL,GTLV_Selected,&num,TAG_END);
									      break;

                        case GID_ADD:
                          printf("GID_ADD\n");
                          GetFile_List(win,filereq,&lvlist,&fplist);
                          GT_SetGadgetAttrs (lvgad,win,NULL,GTLV_Labels,&lvlist,GTLV_Selected,-1,TAG_END);
                        break;

                        case GID_DELETE:
                          printf("GID_DELETE\n");
                          node = Get_Node (&lvlist,num);
         									if (node)	{
                            Delete_Node(Get_Node(&fplist,num));

        										GT_SetGadgetAttrs (lvgad,win,NULL,GTLV_Labels,-1,TAG_END);
										        node = Delete_Node(node);
										        
                            if (node)	{
											        GT_SetGadgetAttrs (lvgad,win,NULL,GTLV_Labels,&lvlist,TAG_END);
        										} else {
				        							GT_SetGadgetAttrs (lvgad,win,NULL,GTLV_Labels,&lvlist,GTLV_Selected,-1,TAG_END);
										        }
									        }
                        break;

                        case GID_CLEAR:
                          GT_SetGadgetAttrs (lvgad,win,NULL,GTLV_Labels,-1,TAG_END);
                          free_list (&lvlist);
                          free_list (&fplist);
                          GT_SetGadgetAttrs (lvgad,win,NULL,GTLV_Labels,&lvlist,GTLV_Selected,-1,TAG_END);
                        break;

                        case GID_PLATFORM:
                          GT_GetGadgetAttrs (cygad_platform,win,NULL,GTCY_Active,&num,TAG_END);
                          printf("%s %d\n",cyPlatforms[num],num);
                          
                          GT_SetGadgetAttrs (cygad_chipram,win,NULL,GTCY_Active,cyPDefaultCMem[num],TAG_END);
                          GT_SetGadgetAttrs (cygad_fastram,win,NULL,GTCY_Active,cyPDefaultFMem[num],TAG_END);

                        break;

                        case GID_BOOT:
                          printf("GID_BOOT\n");
                          GT_GetGadgetAttrs (cygad_platform,win,NULL,GTCY_Active,&num,TAG_END);
                          GT_GetGadgetAttrs (cygad_chipram,win,NULL,GTCY_Active,&chipcynum,TAG_END);
                          GT_GetGadgetAttrs (cygad_fastram,win,NULL,GTCY_Active,&fastcynum,TAG_END);
                          Boot(num,chipcynum,fastcynum,&fplist);
                        break;

                        case GID_INSERTDF0:
                        case GID_INSERTDF1:
                        case GID_INSERTDF2:
                        case GID_INSERTDF3:
                          GT_GetGadgetAttrs (lvgad,win,NULL,GTLV_Selected,&num,TAG_END);
                          if(num!=-1) {
                            InsertFloppy(num,gad->GadgetID,&fplist);
                          }
                        break;

                        case GID_EJECTDF0:
                        case GID_EJECTDF1:
                        case GID_EJECTDF2:
                        case GID_EJECTDF3:
                          EjectFloppy(gad->GadgetID);
                        break;
								      }
								      break;
							      case IDCMP_VANILLAKEY:
								      if (imsg->Code == 0x1b) { cont = FALSE; }
								      break;
							      case IDCMP_REFRESHWINDOW:
								      GT_BeginRefresh (win);
								      GT_EndRefresh (win,TRUE);
								      break;
							      case IDCMP_CLOSEWINDOW:
								      cont = FALSE;
								      break;
							    }
							    GT_ReplyIMsg (imsg);                   
						    }
					    }
				    } 
            while (cont);            
				    CloseWindow (win);
            
			    }	else {
				    Printf ("cannot open window\n");
		      }	

      } else {
			  Printf ("cannot create objects\n");

		    FreeGadgets (glist);
		    FreeAslRequest (filereq);

		    UnlockPubScreen (NULL,scr);
      }
	}
	
  free_list (&lvlist);  
  free_list (&fplist);  

	return (0);
}
