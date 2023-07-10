/*********************************************/
/*                                           */
/*       Designer (C) Ian OConnor 1994       */
/*                                           */
/*      Designer Produced C include file     */
/*                                           */
/*********************************************/

#include <exec/types.h>
#include <exec/memory.h>
#include <dos/dosextens.h>
#include <intuition/screens.h>
#include <intuition/intuition.h>
#include <intuition/gadgetclass.h>
#include <libraries/gadtools.h>
#include <diskfont/diskfont.h>
#include <utility/utility.h>
#include <graphics/gfxbase.h>
#include <workbench/workbench.h>
#include <graphics/scale.h>
#include <clib/exec_protos.h>
#include <clib/wb_protos.h>
#include <clib/intuition_protos.h>
#include <clib/gadtools_protos.h>
#include <clib/graphics_protos.h>
#include <clib/utility_protos.h>
#include <string.h>
#include <clib/diskfont_protos.h>

#include "UpdateNewIcon.h"


ULONG BevelTags[] =
	{
	(GTBB_Recessed), TRUE,
	(GT_VisualInfo), 0,
	(TAG_DONE)
	};

struct IntuiText IconDisplayTexts[] =
	{
	2, 3, JAM1, 214, 8, &topaz801, (UBYTE *)"Options", &IconDisplayTexts[1],
	2, 3, JAM1, 11, 8, &topaz801, (UBYTE *)"Source", &IconDisplayTexts[2],
	2, 3, JAM1, 115, 8, &topaz801, (UBYTE *)"Dest", NULL
	};

struct Window *IconDisplay = NULL;
APTR IconDisplayVisualInfo;
APTR IconDisplayDrawInfo;
struct AppWindow *IconDisplayAppWin = NULL;
struct Gadget *IconDisplayGList;
struct Gadget *IconDisplayGadgets[4];
UBYTE IconDisplayFirstRun = 0;

ULONG IconDisplayGadgetTags[] =
	{
	(GTCB_Checked), TRUE,
	(TAG_END),
	(GTCB_Checked), TRUE,
	(TAG_END),
	};

UWORD IconDisplayGadgetTypes[] =
	{
	CHECKBOX_KIND,
	CHECKBOX_KIND,
	BUTTON_KIND,
	CHECKBOX_KIND,
	};

struct NewGadget IconDisplayNewGadgets[] =
	{
	211, 18, 26, 11, (UBYTE *)"Copy NewIcon", &topaz800, IconDisplay_NICopy, 2, NULL,  (APTR)&IconDisplayGadgetTags[0],
	211, 32, 26, 11, (UBYTE *)"Copy Old Icon", &topaz800, IconDisplay_CopyNormal, 2, NULL,  (APTR)&IconDisplayGadgetTags[3],
	211, 101, 142, 15, (UBYTE *)"Help", &topaz800, IconDisplay_Help, 16, NULL,  NULL,
	211, 46, 26, 11, (UBYTE *)"Verbose", &topaz800, IconDisplay_Verbose, 2, NULL,  NULL,
	};

struct Library *DiskfontBase = NULL;
struct Library *GadToolsBase = NULL;
struct GfxBase *GfxBase = NULL;
struct IntuitionBase *IntuitionBase = NULL;
struct LocaleBase *LocaleBase = NULL;

struct TextAttr topaz801 = { (STRPTR)"topaz.font", 8, 0, 1 };
struct TextAttr topaz800 = { (STRPTR)"topaz.font", 8, 0, 0 };

void RendWindowIconDisplay( struct Window *Win, void *vi )
{
int loop;
UWORD offx = Win->BorderLeft;
UWORD offy = Win->BorderTop;
if (Win != NULL)
	{
	BevelTags[3] = (ULONG)vi;
	DrawBevelBoxA( Win->RPort, 7+offx,18+offy,100,98, (struct TagItem *)(&BevelTags[2]));
	DrawBevelBoxA( Win->RPort, 109+offx,18+offy,100,98, (struct TagItem *)(&BevelTags[2]));
	DrawBevelBoxA( Win->RPort, 2+offx,2+offy,354,118, (struct TagItem *)(&BevelTags[0]));
	for( loop=0; loop<3; loop++)
		if (IconDisplayTexts[loop].ITextFont==NULL)
			IconDisplayTexts[loop].ITextFont=Win->WScreen->Font;
	PrintIText( Win->RPort, IconDisplayTexts, offx, offy);
	}
}

int OpenWindowIconDisplay( struct MsgPort *awmp, long awid)
{
struct Screen *Scr;
UWORD offx, offy;
UWORD loop;
struct NewGadget newgad;
struct Gadget *Gad;
struct Gadget *Gad2;
APTR Cla;
if (IconDisplayFirstRun == 0)
	{
	IconDisplayFirstRun = 1;
	}
if (IconDisplay == NULL)
	{
	Scr = LockPubScreen(NULL);
	if (NULL != Scr)
		{
		offx = Scr->WBorLeft;
		offy = Scr->WBorTop + Scr->Font->ta_YSize+1;
		if (NULL != ( IconDisplayVisualInfo = GetVisualInfoA( Scr, NULL)))
			{
			if (NULL != ( IconDisplayDrawInfo = GetScreenDrawInfo( Scr)))
				{
				IconDisplayGList = NULL;
				Gad = CreateContext( &IconDisplayGList);
				for ( loop=0 ; loop<4 ; loop++ )
					if (IconDisplayGadgetTypes[loop] != 198)
						{
						CopyMem((char * )&IconDisplayNewGadgets[loop], ( char * )&newgad, (long)sizeof( struct NewGadget ));
						newgad.ng_VisualInfo = IconDisplayVisualInfo;
						newgad.ng_LeftEdge += offx;
						newgad.ng_TopEdge += offy;
						IconDisplayGadgets[ loop ] = NULL;
						IconDisplayGadgets[ newgad.ng_GadgetID - IconDisplayFirstID ] = Gad = CreateGadgetA( IconDisplayGadgetTypes[loop], Gad, &newgad, newgad.ng_UserData );
						}
				for ( loop=0 ; loop<4 ; loop++ )
					if (IconDisplayGadgetTypes[loop] == 198)
						{
						IconDisplayGadgets[ loop ] = NULL;
						Cla = NULL;
						if (Gad)
							IconDisplayGadgets[ loop ] = Gad2 = (struct Gadget *) NewObjectA( (struct IClass *)Cla, IconDisplayNewGadgets[ loop ].ng_GadgetText, IconDisplayNewGadgets[ loop ].ng_UserData );
						}
				if (Gad != NULL)
					{
					if (NULL != (IconDisplay = OpenWindowTags( NULL, (WA_Left), 361,
									(WA_Top), 101,
									(WA_Width), 362+offx,
									(WA_Height), 124+offy,
									(WA_Title), "UpdateNewIcon - By Dominic Clifton",
									(WA_MinWidth), 150,
									(WA_MinHeight), 25,
									(WA_MaxWidth), 1200,
									(WA_MaxHeight), 1200,
									(WA_DragBar), TRUE,
									(WA_DepthGadget), TRUE,
									(WA_CloseGadget), TRUE,
									(WA_Activate), TRUE,
									(WA_Dummy+0x30), TRUE,
									(WA_SmartRefresh), TRUE,
									(WA_AutoAdjust), TRUE,
									(WA_Gadgets), IconDisplayGList,
									(WA_IDCMP),588,
									(TAG_END))))
						{
						IconDisplayAppWin = AddAppWindowA( awid, 0, IconDisplay, awmp, NULL);
						RendWindowIconDisplay(IconDisplay, IconDisplayVisualInfo );
						GT_RefreshWindow( IconDisplay, NULL);
						RefreshGList( IconDisplayGList, IconDisplay, NULL, ~0);
						UnlockPubScreen( NULL, Scr);
						return( 0L );
						}
					}
				FreeGadgets( IconDisplayGList);
				FreeScreenDrawInfo( Scr, IconDisplayDrawInfo );
				}
			FreeVisualInfo( IconDisplayVisualInfo );
			}
		UnlockPubScreen( NULL, Scr);
		}
	}
else
	{
	WindowToFront(IconDisplay);
	ActivateWindow(IconDisplay);
	return( 0L );
	}
return( 1L );
}

void CloseWindowIconDisplay( void )
{
if (IconDisplay != NULL)
	{
	if (NULL !=  IconDisplayAppWin )
		RemoveAppWindow( IconDisplayAppWin );
	FreeScreenDrawInfo( IconDisplay->WScreen, IconDisplayDrawInfo );
	IconDisplayDrawInfo = NULL;
	CloseWindow( IconDisplay);
	IconDisplay = NULL;
	FreeVisualInfo( IconDisplayVisualInfo);
	FreeGadgets( IconDisplayGList);
	}
}

int OpenLibs( void )
{
LocaleBase = (struct LocaleBase * )OpenLibrary((UBYTE *)"locale.library", 38);
if ( NULL != (DiskfontBase = OpenLibrary((UBYTE *)"diskfont.library" , 36)))
	if ( NULL != (GadToolsBase = OpenLibrary((UBYTE *)"gadtools.library" , 37)))
		if ( NULL != (GfxBase = (struct GfxBase * )OpenLibrary((UBYTE *)"graphics.library" , 37)))
			if ( NULL != (IntuitionBase = (struct IntuitionBase * )OpenLibrary((UBYTE *)"intuition.library" , 37)))
				return( 0L );
CloseLibs();
return( 1L );
}

void CloseLibs( void )
{
if (NULL != DiskfontBase )
	CloseLibrary( DiskfontBase );
if (NULL != GadToolsBase )
	CloseLibrary( GadToolsBase );
if (NULL != GfxBase )
	CloseLibrary( ( struct Library * )GfxBase );
if (NULL != IntuitionBase )
	CloseLibrary( ( struct Library * )IntuitionBase );
if (NULL != LocaleBase )
	CloseLibrary( ( struct Library * )LocaleBase );
}

int OpenDiskFonts( void )
{
	int OKSoFar = 0;
if (NULL == OpenDiskFont( &topaz801 ) )
	OKSoFar = 1;
if (NULL == OpenDiskFont( &topaz800 ) )
	OKSoFar = 1;
return ( OKSoFar );
}

