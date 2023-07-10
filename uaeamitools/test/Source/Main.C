/* UpdateNewIcon 1.0 by Dominic Clifton */

/*

  Just a little GUI to facilitate copying icon images from one icon to another

  With NewIcon support.

  Took three evenings work to complete.  And quite nice it is too!

  contact me at dom@dsltd.force9.net
  or via the web: http://come.to/dominicc

  If you use this source code or any part of it then please contact me
  and credit it me in your program! Cheerz!


  ideas.

> P.S. I saw another iconupdater which could load source icons with a
> file-requester. In other words, instead of dragging it to the source window, you
> could simply load it. I find this pretty useful, and is probably relatively easy
> to implement.



*/

#include <exec/types.h>
#include <exec/memory.h>
#include <dos/dosextens.h>
#include <dos/dos.h>
#include <dos/dostags.h>
#include <intuition/screens.h>
#include <intuition/intuition.h>
#include <intuition/gadgetclass.h>
#include <libraries/gadtools.h>
#include <diskfont/diskfont.h>
#include <utility/utility.h>
#include <graphics/gfxbase.h>
#include <workbench/workbench.h>
#include <workbench/startup.h>   /* This has WBStartup and WBArg structs    */
#include <graphics/scale.h>
#include <clib/icon_protos.h>
#include <clib/locale_protos.h>
#include <clib/exec_protos.h>
#include <clib/wb_protos.h>
#include <clib/intuition_protos.h>
#include <clib/gadtools_protos.h>
#include <clib/graphics_protos.h>
#include <clib/utility_protos.h>
#include <string.h>
#include <clib/diskfont_protos.h>

#include "UpdateNewIcon.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <libraries/newicon.h>
#include <proto/newicon.h>


#include <proto/dos.h>

long __stack=16384;

// Some global variables

char ConsoleFile[]="CON:100/250/640/200/UpdateNewIcon Console/AUTO/CLOSE";
char tmpfilename[]="T:UpdateNewIcon.tmp";
char filename[4096+1]; // big enough I should think, holds the file names of
                       // files dropped into the appwindow.

struct Screen *scr;

struct NewIconBase  *NewIconBase;
struct Library *IconBase;

struct NewDiskObject *SourceIcon=NULL,*DestIcon=NULL;
struct Image *SourceImage[2]={NULL,NULL}; // for normal and selected images

// so we can keep track of what's being displayed.
// curently no icons are drawn in the Dest box.

#define IMAGE_NONE 0
#define IMAGE_NINORMAL 1
#define IMAGE_NISELECTED 2
#define IMAGE_NORMAL 3
#define IMAGE_SELECTED 4

#define POS_SOURCE 0
#define POS_DEST 1

#define MAX_IMAGE_WIDTH 96
#define MAX_IMAGE_HEIGHT 96

#define LEFTIMAGE_X  9
#define RIGHTIMAGE_X 111

int LastDisplayedImage[2]={IMAGE_NONE,IMAGE_NONE};

// used when copying images (make local instead of global ?)
struct ChunkyImage *TmpImage[2];
struct Gadget TmpGadget;

// Defaults
BOOL CopyNewIconImage = TRUE;
BOOL CopyNormalIconImage = TRUE;
BOOL VerboseOutput = FALSE;


LONG RunDOSCMD(char *command,BOOL ASYNC)
{
  LONG retval;

  static ULONG sysargs[] =
  {
    SYS_Input,NULL,
    SYS_Output,NULL,
    SYS_Asynch,TRUE,
    SYS_UserShell,TRUE,
    NP_Priority,0L,
    NP_StackSize,16384,
    TAG_DONE
  };
  BOOL openedfile=FALSE;

  sysargs[5]=ASYNC;

  if (sysargs[1]=(ULONG)Open(ConsoleFile,MODE_OLDFILE)) openedfile=TRUE;

  retval=SystemTagList(command,(struct TagItem *)sysargs);

  if (!ASYNC && openedfile) Close(sysargs[1]); // close window if not async

  return(retval);
}

BOOL inrectangle(int x,int y, int w, int h,int tx, int ty)
{
  if (tx >=x  && tx <= x+w && ty >=y && ty <= y+h) return(TRUE); else return(FALSE);
}



void ClearImage(int Pos)
{
  int x,y;

  x=IconDisplay->BorderLeft+(Pos == POS_SOURCE ? LEFTIMAGE_X : RIGHTIMAGE_X);
  y=IconDisplay->BorderTop+19;
  EraseRect(IconDisplay->RPort,x,y,x+MAX_IMAGE_WIDTH-1,y+MAX_IMAGE_HEIGHT-1); // offsets from 0 not 1
}

void DisplayImage(int Pos,struct Image *image)
{
  if (image)
  {
    if (image->Width < MAX_IMAGE_WIDTH && image->Height < MAX_IMAGE_HEIGHT)
    {
      ClearImage(Pos);
      DrawImage( IconDisplay->RPort, image,
      IconDisplay->BorderLeft+(Pos == POS_SOURCE ? LEFTIMAGE_X : RIGHTIMAGE_X) +(MAX_IMAGE_WIDTH / 2-(image->Width / 2)),
      IconDisplay->BorderTop+19+(MAX_IMAGE_WIDTH / 2-(image->Height / 2)));
    }
    else
    {
      ClearImage(Pos);

      SetAPen(IconDisplay->RPort,1); // black
      Move(IconDisplay->RPort,IconDisplay->BorderLeft+(Pos == POS_SOURCE ? LEFTIMAGE_X : RIGHTIMAGE_X)+26,IconDisplay->BorderTop+19+(MAX_IMAGE_HEIGHT/2));

      Text(IconDisplay->RPort,"!",1);
    }
  }
  else
  {
    ClearImage(Pos);

    SetAPen(IconDisplay->RPort,1); // black
    Move(IconDisplay->RPort,IconDisplay->BorderLeft+(Pos == POS_SOURCE ? LEFTIMAGE_X : RIGHTIMAGE_X)+5,IconDisplay->BorderTop+19+(MAX_IMAGE_HEIGHT/2));

    Text(IconDisplay->RPort,"NoImage",7);
  }
}

void UpdateImage(int Pos)
{
  if (Pos == POS_SOURCE)
  {
    switch(LastDisplayedImage[Pos])
    {
      case IMAGE_NINORMAL:
      {
        DisplayImage(Pos,SourceImage[0]);
        break;
      }
      case IMAGE_NORMAL:
      {
        DisplayImage(Pos,(struct Image *)SourceIcon->ndo_StdObject->do_Gadget.GadgetRender);
        break;
      }
      case IMAGE_NISELECTED:
      {
        DisplayImage(Pos,SourceImage[1]);
        break;
      }
      case IMAGE_SELECTED:
      {
        DisplayImage(Pos,(struct Image *)SourceIcon->ndo_StdObject->do_Gadget.SelectRender);
        break;
      }
    }
  }
}

void DisplayHelp( void )
{
  char filename[4096];

  BPTR FL;

  // we can't just run multiview progdir:help.guide because the progdir: is expanded
  // by multiview, and not our program! (doh!)

  strcpy(filename,"Multiview ");

  if (FL=Lock("progdir:",ACCESS_READ))
  {
    if (NameFromLock(FL,filename+strlen(filename),4000)) // don't use all the buffer. need some space
                                                         // for multiview and the filename
    {
      strcat(filename,"/UpdateNewIcon.Guide");

      RunDOSCMD(filename,TRUE);
    }
    UnLock(FL);
  }
}

void ProcessWindowIconDisplay( LONG Class, UWORD Code, APTR IAddress, struct IntuiMessage *imsg )
{
  struct Gadget *gad;
  switch ( Class )
  {
    case IDCMP_GADGETUP :
      /* Gadget message, gadget = gad. */
      gad = (struct Gadget *)IAddress;
      switch ( gad->GadgetID )
      {
        case IconDisplay_NICopy :
          /* CheckBox changed, Text of gadget : Copy NI */
          CopyNewIconImage = (gad->Flags & GFLG_SELECTED);
          break;
        case IconDisplay_CopyNormal :
          /* CheckBox changed, Text of gadget : Copy Normal */
          CopyNormalIconImage = (gad->Flags & GFLG_SELECTED);
          break;
        case IconDisplay_Verbose :
          /* CheckBox changed, Text of gadget : Copy Normal */
          VerboseOutput = (gad->Flags & GFLG_SELECTED);
          break;
        case IconDisplay_Help :
          /* CheckBox changed, Text of gadget : Copy Normal */
          DisplayHelp();
          break;
      }
      break;
    case IDCMP_CLOSEWINDOW :
      /* CloseWindow Now */
      break;
    case IDCMP_MOUSEBUTTONS :
      if (inrectangle(IconDisplay->BorderLeft+7,IconDisplay->BorderTop+18,MAX_IMAGE_WIDTH+4,MAX_IMAGE_WIDTH+2,imsg->MouseX,imsg->MouseY))
      {
        if (Code == SELECTDOWN) // mouse button down
        {
          if (SourceIcon)
          {
            LastDisplayedImage[0]++;
            if (LastDisplayedImage[0]>4) LastDisplayedImage[0]=1;
            UpdateImage(POS_SOURCE);
          }
        }
      }
      break;
    case IDCMP_REFRESHWINDOW :
      GT_BeginRefresh( IconDisplay);
      /* Refresh window. */
      RendWindowIconDisplay( IconDisplay, IconDisplayVisualInfo );
      GT_EndRefresh( IconDisplay, TRUE);
      GT_RefreshWindow( IconDisplay, NULL);
      RefreshGList( IconDisplayGList, IconDisplay, NULL, ~0);

      UpdateImage(POS_SOURCE);
      break;
  }
}

struct NewDiskObject *SelectNewDiskObject(char *filename)
{
  int icontype;
  struct FileInfoBlock FIB;
  BPTR FL;
  struct DiskObject *tmpdskobj=NULL;


  // determine correct type

  icontype = WBPROJECT;

  if (FL = Lock(filename,ACCESS_READ))
  {
    if (Examine(FL,&FIB))
    {
      if (FIB.fib_Protection && FIBF_EXECUTE) icontype = WBTOOL;
      if (FIB.fib_DirEntryType > 0) icontype = WBDRAWER;
    }
    UnLock(FL);
  }

  if (filename[strlen(filename)-1] == ':')
  {
    icontype = WBDISK;
  }

  // use the (newicons) patched GetDefDiskObject(), then save that icon, then load it back using the
  // newicons functions

  if (tmpdskobj = GetDefDiskObject(icontype))
  {
    PutDiskObject(tmpfilename,tmpdskobj);
    FreeDiskObject(tmpdskobj);

    return(GetNewDiskObject(tmpfilename));
  }
  else return(NULL);
}

#define AWID 1

int main(char **argv, int argc)
{
  int loop,done=0;
  ULONG class,RetSigs,WinSig,AppSig;
  UWORD code;
  struct Gadget *pgsel;
  struct IntuiMessage *imsg;

  struct MsgPort *awmp = NULL;
  struct AppMessage *awmsg=NULL;

  if (OpenLibs()==0)
  {
    if (scr = LockPubScreen(NULL))
    {

      if (NewIconBase = (struct NewIconBase *) OpenLibrary("newicon.library",39L))
      {
        if (IconBase = OpenLibrary("icon.library",39L))
        {
          if (awmp = CreateMsgPort())
          {
            if (OpenWindowIconDisplay(awmp,AWID)==0)
            {
              // Setup our signals

              WinSig = 1L << IconDisplay->UserPort->mp_SigBit;
              AppSig = 1L  << awmp->mp_SigBit;

              while(done==0)
              {
                // Wait for either a Window event (close, mouseclick, gadgetpress)
                // or an AppIcon to be dropped.

                RetSigs = Wait(WinSig | AppSig);

                // Check to see what woke us up!

                // User clicked a gadget (or something)

                if (RetSigs & WinSig)
                {
                  imsg=GT_GetIMsg(IconDisplay->UserPort);
                  while (imsg != NULL )
                  {
                    class=imsg->Class;
                    code=imsg->Code;
                    pgsel=(struct Gadget *)imsg->IAddress; /* Only reference if it is a gadget message */
                    GT_ReplyIMsg(imsg);
                    ProcessWindowIconDisplay(class, code, pgsel,imsg);

                    if (class==IDCMP_CLOSEWINDOW) done=1;

                    // get the next message if there is one..
                    imsg=GT_GetIMsg(IconDisplay->UserPort);
                  }
                }

                // User dropped an Icon!

                if (RetSigs & AppSig)
                {
                  if (awmsg = (struct AppMessage *)GetMsg(awmp)) // get the message from the port
                  {
                    if (awmsg->am_Type == AMTYPE_APPWINDOW) // check the message type
                    {
                      if(awmsg->am_NumArgs>0L) // could it be less than 0 ?
                      {
                        for(loop=0;loop<awmsg->am_NumArgs;loop++) // process all icons dropped as they arrive in one message
                        {
                          if (awmsg->am_ArgList[loop].wa_Lock) // did user drop an actual file or another AppIcon ?
                          {
                            NameFromLock(awmsg->am_ArgList[loop].wa_Lock,filename,4096);
                            AddPart(filename,awmsg->am_ArgList[loop].wa_Name,4096);

                            // No we've got the filename of the icon that was dropped we can do stuff with it.
                            // So first lets determine if the user dropped the icon in the source or destination
                            // boxes of out window.

                            if (inrectangle(IconDisplay->BorderLeft+LEFTIMAGE_X,IconDisplay->BorderTop+18,MAX_IMAGE_WIDTH+4,MAX_IMAGE_HEIGHT+2,awmsg->am_MouseX,awmsg->am_MouseY))
                            {
                              // Icon dropped in Source box.

                              // change the name to get xxx:disk.info if a device icon was dropped

                              if (filename[strlen(filename)-1] == ':')
                              {
                                strcat(filename,"disk");
                              }

                              if (filename[strlen(filename)-1] == '/')
                              {
                                filename[strlen(filename)-1]=0;
                              }

                              if (VerboseOutput) printf("Getting Icon for %s\n",filename);

                              if (SourceIcon) FreeNewDiskObject(SourceIcon); // free the icon if we have one
                              if (SourceImage[0]) FreeRemappedImage(SourceImage[0],scr);
                              if (SourceImage[1]) FreeRemappedImage(SourceImage[1],scr);

                              SourceImage[0]=NULL; // to make sure we don't free them
                              SourceImage[1]=NULL; // again if a new allocation fails.

                              // we dont unset SourceImage as if the next call fails it'll be done anyway

                              if (SourceIcon = GetNewDiskObject( filename ))
                              {
                                if (VerboseOutput) puts("Got correct icon!");
                              }
                              else
                              {

                                if (VerboseOutput) puts("Attempting to get default icon");

                                if (SourceIcon = SelectNewDiskObject(filename))
                                {
                                  if (VerboseOutput) puts("Got default icon!");
                                }
                              }

                              if (SourceIcon != NULL)
                              {
                                if (VerboseOutput) puts("Icon OK");

                                // display the newicon image..

                                if ((SourceIcon->ndo_NormalImage) && (SourceImage[0] = RemapChunkyImage(SourceIcon->ndo_NormalImage,scr)))
                                {
                                  // we don't care if this one fails really..  but we must always check
                                  // to see if the pointers are null whenever they are used.

                                  SourceImage[1] = RemapChunkyImage(SourceIcon->ndo_SelectedImage,scr);

                                  DisplayImage(POS_SOURCE,SourceImage[0]);
                                  LastDisplayedImage[POS_SOURCE]=IMAGE_NINORMAL;
                                }
                                else
                                {

                                  //not got one, display the normal one instead!

                                  DisplayImage(POS_SOURCE,(struct Image *)SourceIcon->ndo_StdObject->do_Gadget.GadgetRender);
                                  LastDisplayedImage[POS_SOURCE]=IMAGE_NORMAL;
                                  if (VerboseOutput) puts("Dropped icon has no NewIcon image!");
                                }
                              }
                              else
                              {
                                ClearImage(POS_SOURCE);
                                if (VerboseOutput) puts("Icon FAILED");
                              }
                            }
                            else
                            if (inrectangle(IconDisplay->BorderLeft+RIGHTIMAGE_X,IconDisplay->BorderTop+18,MAX_IMAGE_WIDTH+4,MAX_IMAGE_HEIGHT+2,awmsg->am_MouseX,awmsg->am_MouseY))
                            {
                              // one or the other option must be selected and we must have a source icon to copy!

                              if (SourceIcon  && (CopyNewIconImage || CopyNormalIconImage))
                              {

                                if (VerboseOutput) printf("Getting Icon for %s\n",filename);

                                // change the name to get xxx:disk.info if a device icon was dropped

                                if (filename[strlen(filename)-1] == ':')
                                {
                                  strcat(filename,"disk");
                                }

                                if (filename[strlen(filename)-1] == '/')
                                {
                                  filename[strlen(filename)-1]=0;
                                }

                                if (DestIcon = GetNewDiskObject( filename ))
                                {
                                  if (VerboseOutput) puts("Got Icon!");
                                }
                                else
                                {

                                  DestIcon = SelectNewDiskObject(filename);

                                }

                                if (DestIcon)
                                {
                                  // Copy Image Over..

                                  // Save old image pointers so we can free them later

                                  TmpImage[0]=DestIcon->ndo_NormalImage;
                                  TmpImage[1]=DestIcon->ndo_SelectedImage;

                                  // I think this next line is the correct way to copy *just* the images from
                                  // one disk object to another..

                                  CopyMem(&DestIcon->ndo_StdObject->do_Gadget,&TmpGadget,sizeof (struct Gadget));

                                  // Set image pointers to the same as the source icon's image

                                  if (CopyNewIconImage)
                                  {
                                    DestIcon->ndo_NormalImage = SourceIcon->ndo_NormalImage;
                                    DestIcon->ndo_SelectedImage = SourceIcon->ndo_SelectedImage;
                                  }

                                  if (CopyNormalIconImage)
                                  {
                                    CopyMem(&SourceIcon->ndo_StdObject->do_Gadget,&DestIcon->ndo_StdObject->do_Gadget,sizeof (struct Gadget));
                                  }

                                  if (PutNewDiskObject(filename,DestIcon))
                                  {
                                    if (VerboseOutput) printf("Wrote NewIcon for %s\n",filename);
                                    // message ? nah...
                                  }
                                  else
                                  {
                                    if (VerboseOutput) printf("Unable to write NewIcon for %s\n",filename);
                                    DisplayBeep(scr);
                                  }


                                  // Restore old image pointers so we can free it.
                                  CopyMem(&TmpGadget,&DestIcon->ndo_StdObject->do_Gadget,sizeof (struct Gadget));
                                  DestIcon->ndo_NormalImage=TmpImage[0];
                                  DestIcon->ndo_SelectedImage=TmpImage[1];

                                  FreeNewDiskObject(DestIcon);
                                }
                              }
                            }
                            else
                            {
                              if (VerboseOutput) printf("Appicon dropped incorrectly! X:%ld Y:%ld\n",awmsg->am_MouseX,awmsg->am_MouseY);
                            }
                          }
                          else
                          {
                            DisplayBeep(scr);
                          }
                        }
                      }

                    }
                    ReplyMsg((struct Message *)awmsg);
                  }
                }
              }

              CloseWindowIconDisplay();
            }
            else if (VerboseOutput) printf("Cannot open window.\n");

            DeleteMsgPort(awmp);
          }

          // Free source and destination icons if we have them loaded.

          if (SourceIcon) FreeNewDiskObject(SourceIcon);

          if (SourceImage[0]) FreeRemappedImage(SourceImage[0],scr);
          if (SourceImage[1]) FreeRemappedImage(SourceImage[1],scr);

          CloseLibrary(IconBase);
        }
        CloseLibrary((struct Library*)NewIconBase);
      }
      UnlockPubScreen(NULL,scr);
    }
    CloseLibs();
  }
}


