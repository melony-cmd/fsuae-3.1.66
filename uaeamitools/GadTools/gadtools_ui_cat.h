#ifndef gadtools_ui_cat_H
#define gadtools_ui_cat_H
/*
** gadtools_ui_cat.h Catalog Header File
*/

#include<exec/types.h>

struct FC_Type
{
	LONG ID;
	STRPTR Str;
};

extern const struct FC_Type _MSG_SCREENTITLE_SCR;
#define MSG_SCREENTITLE_SCR ((APTR) &_MSG_SCREENTITLE_SCR)
extern const struct FC_Type _MSG_MainWindow_WND;
#define MSG_MainWindow_WND ((APTR) &_MSG_MainWindow_WND)
extern const struct FC_Type _MSG_GID_ADD_GAD;
#define MSG_GID_ADD_GAD ((APTR) &_MSG_GID_ADD_GAD)
extern const struct FC_Type _MSG_GID_DELETE_GAD;
#define MSG_GID_DELETE_GAD ((APTR) &_MSG_GID_DELETE_GAD)
extern const struct FC_Type _MSG_GID_CLEAR_GAD;
#define MSG_GID_CLEAR_GAD ((APTR) &_MSG_GID_CLEAR_GAD)
extern const struct FC_Type _MSG_GID_PLATFORM_GAD;
#define MSG_GID_PLATFORM_GAD ((APTR) &_MSG_GID_PLATFORM_GAD)
extern const struct FC_Type _MSG_GID_PLATFORM_L0_GAD;
#define MSG_GID_PLATFORM_L0_GAD ((APTR) &_MSG_GID_PLATFORM_L0_GAD)
extern const struct FC_Type _MSG_GID_DISKLIST_GAD;
#define MSG_GID_DISKLIST_GAD ((APTR) &_MSG_GID_DISKLIST_GAD)
extern const struct FC_Type _MSG_GID_INSERTDF0_GAD;
#define MSG_GID_INSERTDF0_GAD ((APTR) &_MSG_GID_INSERTDF0_GAD)
extern const struct FC_Type _MSG_GID_INSERTDF1_GAD;
#define MSG_GID_INSERTDF1_GAD ((APTR) &_MSG_GID_INSERTDF1_GAD)
extern const struct FC_Type _MSG_GID_INSERTDF2_GAD;
#define MSG_GID_INSERTDF2_GAD ((APTR) &_MSG_GID_INSERTDF2_GAD)
extern const struct FC_Type _MSG_GID_INSERTDF3_GAD;
#define MSG_GID_INSERTDF3_GAD ((APTR) &_MSG_GID_INSERTDF3_GAD)
extern const struct FC_Type _MSG_GID_EJECTDF0_GAD;
#define MSG_GID_EJECTDF0_GAD ((APTR) &_MSG_GID_EJECTDF0_GAD)
extern const struct FC_Type _MSG_GID_EJECTDF1_GAD;
#define MSG_GID_EJECTDF1_GAD ((APTR) &_MSG_GID_EJECTDF1_GAD)
extern const struct FC_Type _MSG_GID_EJECTDF2_GAD;
#define MSG_GID_EJECTDF2_GAD ((APTR) &_MSG_GID_EJECTDF2_GAD)
extern const struct FC_Type _MSG_GID_EJECTDF3_GAD;
#define MSG_GID_EJECTDF3_GAD ((APTR) &_MSG_GID_EJECTDF3_GAD)
extern const struct FC_Type _MSG_GID_BOOT_GAD;
#define MSG_GID_BOOT_GAD ((APTR) &_MSG_GID_BOOT_GAD)
extern const struct FC_Type _MSG__GAD;
#define MSG__GAD ((APTR) &_MSG__GAD)
extern const struct FC_Type _MSG__L0_GAD;
#define MSG__L0_GAD ((APTR) &_MSG__L0_GAD)


#endif /* gadtools_ui_cat_H */
