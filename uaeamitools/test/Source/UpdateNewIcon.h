/*********************************************/
/*                                           */
/*       Designer (C) Ian OConnor 1994       */
/*                                           */
/*      Designer Produced C header file      */
/*                                           */
/*********************************************/



#define IconDisplayFirstID 0
#define IconDisplay_NICopy 0
#define IconDisplay_CopyNormal 1
#define IconDisplay_Help 2
#define IconDisplay_Verbose 3

extern struct TextAttr topaz801;
extern struct TextAttr topaz800;
extern struct AppWindow *IconDisplayAppWin;
extern struct Gadget *IconDisplayGadgets[4];
extern struct Gadget *IconDisplayGList;
extern struct Window *IconDisplay;
extern APTR IconDisplayVisualInfo;
extern APTR IconDisplayDrawInfo;
extern ULONG IconDisplayGadgetTags[];
extern UWORD IconDisplayGadgetTypes[];
extern struct NewGadget IconDisplayNewGadgets[];
extern struct Library *DiskfontBase;
extern struct Library *GadToolsBase;
extern struct GfxBase *GfxBase;
extern struct IntuitionBase *IntuitionBase;
extern struct LocaleBase *LocaleBase;

extern void RendWindowIconDisplay( struct Window *Win, void *vi );
extern int OpenWindowIconDisplay( struct MsgPort *awmp, long awid);
extern void CloseWindowIconDisplay( void );
extern int OpenLibs( void );
extern void CloseLibs( void );
extern int OpenDiskFonts( void );

