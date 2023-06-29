#ifndef gadtools_ui_H
#define gadtools_ui_H
/*
** gadtools_ui.h Header File
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
#define VER_PROGRAMNAME "<untitled>"
#define VER_AUTHOR "<unknown>"
#define VER_VERSION 1
#define VER_REVISION 0
#define VER_SVER "\0$VER: "VER_PROGRAMNAME" 1.0 "__AMIGADATE__""

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


#define CNT_MainWindow 15
#define GD_GID_ADD 0
#define GD_GID_DELETE 1
#define GD_GID_CLEAR 2
#define GD_GID_PLATFORM 3
#define GD_GID_DISKLIST 4
#define GD_GID_INSERTDF0 5
#define GD_GID_INSERTDF1 6
#define GD_GID_INSERTDF2 7
#define GD_GID_INSERTDF3 8
#define GD_GID_EJECTDF0 9
#define GD_GID_EJECTDF1 10
#define GD_GID_EJECTDF2 11
#define GD_GID_EJECTDF3 12
#define GD_GID_BOOT 13
#define GD_ 14

/* Prototypes */
int FN_GID_ADD_Clicked(void);
int FN_GID_DELETE_Clicked(void);
int FN_GID_CLEAR_Clicked(void);
int FN_GID_PLATFORM_Clicked(void);
int FN_GID_DISKLIST_Clicked(void);
int FN_GID_INSERTDF0_Clicked(void);
int FN_GID_INSERTDF1_Clicked(void);
int FN_GID_INSERTDF2_Clicked(void);
int FN_GID_INSERTDF3_Clicked(void);
int FN_GID_EJECTDF0_Clicked(void);
int FN_GID_EJECTDF1_Clicked(void);
int FN_GID_EJECTDF2_Clicked(void);
int FN_GID_EJECTDF3_Clicked(void);
int FN_GID_BOOT_Clicked(void);
int FN__Clicked(void);

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

#endif /* gadtools_ui_H */
