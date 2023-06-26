#ifndef gadtools_H
#define gadtools_H
/*
** gadtools.h Header File
*/

#include<exec/types.h>
#include<intuition/classusr.h>

#define GetString( g )  ((( struct StringInfo * )g->SpecialInfo )->Buffer  )
#define GetInteger( g ) ((( struct StringInfo * )g->SpecialInfo )->LongInt )
#define TOOL_HORIZ 0
#define TOOL_VERT 1

#define DRAG_ID MAKE_ID('D','R','A','G')

#define COLORWHEEL_KIND 10000
#define DATATYPE_KIND 10001
#define GRADIENTSLIDER_KIND 10002
#define PAGE_KIND 10003
#define POPUPMENU_KIND 10004
#define PROGRESS_KIND 10005
#define TAPEDECK_KIND 10006
#define TEXTFIELD_KIND 10007
#define TOOLIMAGE_KIND 10008

#define TREEVIEW_KIND 10009
#define VER_PROGRAMNAME ""
#define VER_AUTHOR ""
#define VER_VERSION 0
#define VER_REVISION 0
#define VER_SVER "\0$VER: "VER_PROGRAMNAME" 0.0 "__AMIGADATE__""

#define WA_CenterX (WA_Dummy + 0x100)
#define WA_CenterY (WA_Dummy + 0x101)
#define WA_RelRight (WA_Dummy + 0x102)
#define WA_RelBottom (WA_Dummy + 0x103)
#define WA_RelWidth (WA_Dummy + 0x104)
#define WA_RelHeight (WA_Dummy + 0x105)
#define WA_ToolDragBar (WA_Dummy + 0x106)
#define WA_FontAdaptive (WA_Dummy + 0x107)
#define GTBERR_OK 0
#define GTBERR_CREATEPOINTER 15
#define GTBERR_CREATEIMAGE 16
#define GTBERR_OPENLIB_POPUPMENU 104
#define GTBERR_OPENSCREEN 200
#define GTBERR_VISUALINFO 201
#define GTBERR_SCRDRAWINFO 202
#define GTBERR_CREATECONTEXT 300
#define GTBERR_CREATEGADGET 301
#define GTBERR_CREATEMENUS 302
#define GTBERR_OPENWINDOW 303

extern TEXT ScrTitle[];
extern TEXT ScrPubName[];
extern UWORD ScrWidth,ScrHeight,ScrDepth;
extern ULONG ScrDisplayID;
extern struct Screen *Scr;
extern struct DrawInfo *DrawInfo;
extern APTR VisualInfo;
extern struct TextAttr *Font, Attr;

extern struct Window *MainWindowWnd;
extern struct Gadget *MainWindowGList;
extern ULONG MainWindowWTags[];
extern struct IntuiMessage MainWindowMsg;
extern struct Gadget *MainWindowGadgets[];
#define WND_MainWindow 0

#define CNTWND_WINDOWS 2

extern ULONG MainWindowGTags[];
extern struct SmartBitMapIRGB ToolBarPalette[];
#define CNTMN_MENUS 1


#define CNT_MainWindow 5
#define GD_BT_ADD 0
#define GD_BT_REMOVE 1
#define GD_ 2
#define GD_CY_PLATFORM 3
#define GD_BT_BOOT 4

/* Prototypes */
int FN_BT_ADD_Clicked(void);
int FN_BT_REMOVE_Clicked(void);
int FN__Clicked(void);
int FN_CY_PLATFORM_Clicked(void);
int FN_BT_BOOT_Clicked(void);

int HandleIDCMP_MainWindow(void);
void Render_MainWindow(void);
int FN_CloseWindow_MainWindow(void);
UWORD OpenWindow_MainWindow(void);
void CloseWindow_MainWindow(void);

UWORD SetupScreen(UBYTE);
void CloseDownScreen(void);
UWORD InitApplication(void);
void FreeApplication(void);

struct Gadget *CreateGadgetExA(UWORD,UWORD,UWORD,UWORD,struct Gadget *,struct TagItem *);
void DeleteGadgetExA(struct Gadget *,struct TagItem *);
struct Window *OpenWindowTagsEx(ULONG, ...);
void CloseWindowEx(struct Window *);
void CalcLeftTop(UWORD *,UWORD *,struct TagItem *);
void CalcRightBottom(UWORD *,UWORD *,struct TagItem *);
UWORD AdaptX(UWORD);
UWORD AdaptY(UWORD);
void CalcAdaptivity(UWORD,UWORD);
STRPTR GetCatString(APTR);

#endif /* gadtools_H */
