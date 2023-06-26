/* Programmname                                                               */

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
	#include <string.h>

/*----------------------------------------------------------------------------*/
/* Konstanten & Makros                                                        */
/*----------------------------------------------------------------------------*/

	#define GID_LIST        1001
	#define GID_STRING      1002
	#define GID_ADD         1003
	#define GID_REMOVE      1004
	#define GID_SORT        1005
	#define GID_LOAD        1006
	#define GID_SAVE        1007
	#define GID_QUIT        1008

	#ifndef __amigaos4__
	 #define NewList(list) ((list)->lh_Head = (struct Node *)&(list)->lh_Tail,(list)->lh_TailPred = (struct Node *)&(list)->lh_Head,(list)->lh_Tail = NULL)
	 #define GetHead(list) ((list) && (list)->lh_Head && (list)->lh_Head->ln_Succ ? (list)->lh_Head : NULL)
	 #define GetTail(list) ((list) && (list)->lh_TailPred && (list)->lh_TailPred->ln_Pred ? (list)->lh_TailPred : NULL)
	 #define GetSucc(node) ((node) && (node)->ln_Succ->ln_Succ ? (node)->ln_Succ : NULL)
	 #define GetPred(node) ((node) && (node)->ln_Pred->ln_Pred ? (node)->ln_Pred : NULL)
	#endif

/*----------------------------------------------------------------------------*/
/* Typdefinitionen                                                            */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* Globale Variablen                                                          */
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/
/* ListView-Liste aus Datei laden                                             */
/*----------------------------------------------------------------------------*/
BOOL load_list (char *name,struct List *list)	{
	BPTR file;
	BOOL ok = FALSE;
	char line[256];
	struct Node *node;
	long l;

	if (file = Open (name,MODE_OLDFILE)) {
		while (FGets (file,line,256)) {
			if (l = strlen(line))	{
				l --;
				if (line[l] == '\n')
					line[l] = 0;
			}

			if (node = AllocVec (sizeof(struct Node) + strlen(line) + 1,MEMF_CLEAR)) {
				node->ln_Name = (STRPTR)(node + 1);
				strcpy (node->ln_Name,line);
				AddTail (list,node);
			}
		}
		ok = TRUE;
		Close (file);
	}

	return (ok);
}

/*----------------------------------------------------------------------------*/
/* ListView-Liste in Datei speichern                                          */
/*----------------------------------------------------------------------------*/
BOOL save_list (char *name,struct List *list) {
  BPTR file;
  BOOL ok = FALSE;
  struct Node *node;
  if (file = Open (name,MODE_NEWFILE)) {
	  for (node = GetHead (list); node; node = GetSucc (node))  {
		  FPuts (file,node->ln_Name);
		  FPutC (file,'\n');
		}
	  ok = TRUE;
	  Close (file);
	}
  return (ok);
}

/*----------------------------------------------------------------------------*/
/* Zwei ListView-Nodes vergleichen (f�r Sort)                                 */
/*----------------------------------------------------------------------------*/
int compare_lvnodes (struct Node **n1,struct Node **n2) {
  return (Stricmp ((*n1)->ln_Name,(*n2)->ln_Name));
}

/*----------------------------------------------------------------------------*/
/* ListView-Liste sortieren                                                   */
/*----------------------------------------------------------------------------*/
void sort_list (struct List *list) {
	struct Node *node;
	struct Node **array;
	long n,i;

	n = 0;
	for (node = GetHead (list); node; node = GetSucc (node)) { n ++; }

	if (array = AllocVec (n * sizeof(struct Node *),0))	{
		for (i = 0; i < n; i++) {
			array[i] = RemHead (list);
    }

		qsort (array,n,sizeof(struct Node *),(void *)compare_lvnodes);

		for (i = 0; i < n; i++) {
		  AddTail (list,array[i]);
    }

		FreeVec (array);
		}
	}

/*----------------------------------------------------------------------------*/
/* Alle Nodes einer List freigeben                                            */
/*----------------------------------------------------------------------------*/
void free_list (struct List *list) {
	struct Node *node;

	while (node = RemHead (list)) {
		FreeVec (node);
  }
}

/*----------------------------------------------------------------------------*/
/* Node Nummer n aus der Liste ermitteln                                      */
/*----------------------------------------------------------------------------*/
struct Node *get_node (struct List *list,ULONG num) {
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
/* Nummer einer Node in der Liste ermitteln                                   */
/*----------------------------------------------------------------------------*/
ULONG node_number (struct List *list,struct Node *node_to_find)	{
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
/* Eine Node mit neuem Text ersetzen                                          */
/*----------------------------------------------------------------------------*/
struct Node *replace_node_text (struct List *list,struct Node *node,char *text) {
	struct Node *pred;
	struct Node *new;

	if (new = AllocVec (sizeof(struct Node) + strlen(text) + 1,MEMF_CLEAR))	{
		new->ln_Name = (STRPTR)(new + 1);
		strcpy (new->ln_Name,text);

		pred = node->ln_Pred;

		Remove (node);
		FreeVec (node);

		Insert (list,new,pred);
	}

	return (new);
}

/*----------------------------------------------------------------------------*/
/* Hauptprogramm                                                              */
/*----------------------------------------------------------------------------*/
int main (void)	{
	struct Screen *scr;
	struct Window *win;
	struct NewGadget ng;
	ULONG winsig;
	ULONG sigs;
	BOOL cont;
	struct Gadget *glist;
	struct Gadget *gad;
	struct Gadget *lvgad;
	struct Gadget *strgad;
	struct FileRequester *filereq;
	struct List lvlist;
	struct Node *node = NULL;
	struct Node *nextnode;
	ULONG fontw,fonth;
	ULONG winw,winh;
	struct IntuiMessage *imsg;
	ULONG size;
	char *text;
	ULONG num;

	NewList (&lvlist);

	if (scr = LockPubScreen (NULL))	{
		winw = scr->Width  * 4 / 10;
		winh = scr->Height * 8 / 10;

		filereq = AllocAslRequestTags (ASL_FileRequest,
			ASLFR_InitialLeftEdge,(scr->Width - winw) / 2,
			ASLFR_InitialTopEdge,(scr->Height - winh) / 2,
			ASLFR_InitialWidth,winw,
			ASLFR_InitialHeight,winh,
			TAG_END);

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
		ng.ng_Width      = 3*16 * fontw + 3*8 + 2*4;
		ng.ng_GadgetText = NULL;
		ng.ng_GadgetID   = GID_STRING;
		gad = CreateGadget (STRING_KIND,gad,&ng,GA_Disabled,TRUE,TAG_END);
		strgad = gad;

		ng.ng_TopEdge    = winw;
		ng.ng_Height    += winh;      /* including string height */
		ng.ng_GadgetID   = GID_LIST;
		gad = CreateGadget (LISTVIEW_KIND,gad,&ng,GTLV_Labels,&lvlist,GTLV_ShowSelected,gad,TAG_END);
		lvgad = gad;

		ng.ng_TopEdge   += ng.ng_Height + 4;
		ng.ng_Width      = 16 * fontw + 8;
		ng.ng_Height     = fonth + 6;
		ng.ng_GadgetText = "Add";
		ng.ng_GadgetID   = GID_ADD;
		gad = CreateGadget (BUTTON_KIND,gad,&ng,TAG_END);

		ng.ng_LeftEdge  += ng.ng_Width + 4;
		ng.ng_GadgetText = "Remove";
		ng.ng_GadgetID   = GID_REMOVE;
		gad = CreateGadget (BUTTON_KIND,gad,&ng,TAG_END);

		ng.ng_LeftEdge  += ng.ng_Width + 4;
		ng.ng_GadgetText = "Sort";
		ng.ng_GadgetID   = GID_SORT;
		gad = CreateGadget (BUTTON_KIND,gad,&ng,TAG_END);

		ng.ng_TopEdge   += ng.ng_Height + 4;
		ng.ng_LeftEdge   = scr->WBorLeft + 4;
		ng.ng_GadgetText = "Load";
		ng.ng_GadgetID   = GID_LOAD;
		gad = CreateGadget (BUTTON_KIND,gad,&ng,TAG_END);

		ng.ng_LeftEdge  += ng.ng_Width + 4;
		ng.ng_GadgetText = "Save";
		ng.ng_GadgetID   = GID_SAVE;
		gad = CreateGadget (BUTTON_KIND,gad,&ng,TAG_END);

		ng.ng_LeftEdge  += ng.ng_Width + 4;
		ng.ng_GadgetText = "Quit";
		ng.ng_GadgetID   = GID_QUIT;
		gad = CreateGadget (BUTTON_KIND,gad,&ng,TAG_END);

		if (gad) {
			winw = ng.ng_LeftEdge + ng.ng_Width + 4 + scr->WBorRight;
			winh = ng.ng_TopEdge + ng.ng_Height + 4 + scr->WBorBottom;

			if (win = OpenWindowTags (NULL,
					WA_Title,"My first GadTools program",
					WA_Width,winw,
					WA_Height,winh,
					WA_Left,(scr->Width - winw) / 2,
					WA_Top,(scr->Height - winh) / 2,
					WA_Flags,WFLG_CLOSEGADGET | WFLG_DRAGBAR | WFLG_DEPTHGADGET | WFLG_ACTIVATE,
					WA_IDCMP,IDCMP_CLOSEWINDOW | IDCMP_VANILLAKEY | IDCMP_REFRESHWINDOW | BUTTONIDCMP | STRINGIDCMP | LISTVIEWIDCMP,
					WA_PubScreen,scr,
					WA_Gadgets,glist,
					TAG_END))
				{
				GT_RefreshWindow (win,NULL);

				winsig = 1L << win->UserPort->mp_SigBit;

				cont = TRUE;
				do	{
					sigs = Wait (winsig | SIGBREAKF_CTRL_C);

					if (sigs & SIGBREAKF_CTRL_C)
						cont = FALSE;

					if (sigs & winsig) {
						while (imsg = GT_GetIMsg (win->UserPort))	{
							switch (imsg->Class) {

							case IDCMP_GADGETUP:
								gad = (struct Gadget *) imsg->IAddress;

								switch (gad->GadgetID) {
								  case GID_LOAD:
								  	if (AslRequestTags (filereq, ASLFR_Window,win, ASLFR_DoSaveMode,FALSE, TAG_END)) {
								  		size = strlen(filereq->fr_Drawer) + strlen(filereq->fr_File) + 4;
								  		if (text = AllocVec (size,MEMF_CLEAR)) {
								  			strcpy (text,filereq->fr_Drawer);
								  			AddPart (text,filereq->fr_File,size);
								  			GT_SetGadgetAttrs (lvgad,win,NULL,GTLV_Labels,-1,TAG_END);
								  			free_list  (&lvlist);
								  			load_list (text,&lvlist);
								  			GT_SetGadgetAttrs (lvgad,win,NULL,GTLV_Labels,&lvlist,GTLV_Selected,-1,TAG_END);
								  			GT_SetGadgetAttrs (strgad,win,NULL,GTST_String,"",GA_Disabled,TRUE,TAG_END);
								  			node = NULL;
								  			FreeVec (text);
								  		}
								  	}
								  	break;

								  case GID_SAVE:                
								  	if (AslRequestTags (filereq,ASLFR_Window,win,ASLFR_DoSaveMode,TRUE,TAG_END)) {
								  		size = strlen(filereq->fr_Drawer) + strlen(filereq->fr_File) + 4;
								  		if (text = AllocVec (size,MEMF_CLEAR)) {
								  			strcpy (text,filereq->fr_Drawer);
								  			AddPart (text,filereq->fr_File,size);
								  			save_list (text,&lvlist);
								  			FreeVec (text);
								  		}
								  	}
								  	break;

								case GID_SORT:
									GT_SetGadgetAttrs (lvgad,win,NULL,GTLV_Labels,-1,TAG_END);
									sort_list (&lvlist);
									GT_SetGadgetAttrs (lvgad,win,NULL,GTLV_Labels,&lvlist,GTLV_Selected,-1,TAG_END);
									GT_SetGadgetAttrs (strgad,win,NULL,GTST_String,"",GA_Disabled,TRUE,TAG_END);
									node = NULL;
									break;

								case GID_QUIT:
									cont = FALSE;
									break;

								case GID_LIST:
									GT_GetGadgetAttrs (lvgad,win,NULL,GTLV_Selected,&num,TAG_END);
									if (node = get_node (&lvlist,num)) {
										GT_SetGadgetAttrs (strgad,win,NULL,GTST_String,node->ln_Name,GA_Disabled,FALSE,TAG_END);
										ActivateGadget (strgad,win,NULL);
									}
									break;

								case GID_STRING:
									if (node) {
										GT_GetGadgetAttrs (strgad,win,NULL,GTST_String,&text,TAG_END);
										GT_SetGadgetAttrs (lvgad,win,NULL,GTLV_Labels,-1,TAG_END);
										node = replace_node_text (&lvlist,node,text);
										GT_SetGadgetAttrs (lvgad,win,NULL,GTLV_Labels,&lvlist,TAG_END);
									}
									break;

								case GID_REMOVE:
									if (node)	{
										GT_SetGadgetAttrs (lvgad,win,NULL,GTLV_Labels,-1,TAG_END);
										nextnode = GetSucc (node);
										Remove (node);
										FreeVec (node);
										node = nextnode;
										if (node)	{
											GT_SetGadgetAttrs (lvgad,win,NULL,GTLV_Labels,&lvlist,TAG_END);
											GT_SetGadgetAttrs (strgad,win,NULL,GTST_String,node->ln_Name,GA_Disabled,FALSE,TAG_END);
											ActivateGadget (strgad,win,NULL);
										} else {
											GT_SetGadgetAttrs (lvgad,win,NULL,GTLV_Labels,&lvlist,GTLV_Selected,-1,TAG_END);
											GT_SetGadgetAttrs (strgad,win,NULL,GTST_String,"",GA_Disabled,TRUE,TAG_END);
										}
									}
									break;

								case GID_ADD:
									nextnode = node;
									if (node = AllocVec (sizeof(struct Node) + 1,MEMF_CLEAR))	{
										node->ln_Name = (STRPTR)(node + 1);
										GT_SetGadgetAttrs (lvgad,win,NULL,GTLV_Labels,-1,TAG_END);
										if (nextnode)
											Insert (&lvlist,node,nextnode);
										else
											AddTail (&lvlist,node);
										GT_SetGadgetAttrs (lvgad,win,NULL,GTLV_Labels,&lvlist,GTLV_Selected,node_number(&lvlist,node),TAG_END);
										GT_SetGadgetAttrs (strgad,win,NULL,GTST_String,"",GA_Disabled,FALSE,TAG_END);
										ActivateGadget (strgad,win,NULL);
									}
									break;
								}
								break;

							case IDCMP_VANILLAKEY:
								if (imsg->Code == 0x1b) /* Esc */
									cont = FALSE;
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
				}
			else
				Printf ("cannot open window\n");
			}
		else
			Printf ("cannot create objects\n");

		FreeGadgets (glist);
		FreeAslRequest (filereq);

		UnlockPubScreen (NULL,scr);
		}

	free_list (&lvlist);

	return (0);
}

/*----------------------------------------------------------------------------*/
/* Ende des Quelltextes                                                       */
/*----------------------------------------------------------------------------*/
