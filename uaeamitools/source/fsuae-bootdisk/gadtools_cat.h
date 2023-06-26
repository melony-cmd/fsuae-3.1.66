#ifndef gadtools_cat_H
#define gadtools_cat_H
/*
** gadtools_cat.h Catalog Header File
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
extern const struct FC_Type _MSG_BT_ADD_GAD;
#define MSG_BT_ADD_GAD ((APTR) &_MSG_BT_ADD_GAD)
extern const struct FC_Type _MSG_BT_REMOVE_GAD;
#define MSG_BT_REMOVE_GAD ((APTR) &_MSG_BT_REMOVE_GAD)
extern const struct FC_Type _MSG__GAD;
#define MSG__GAD ((APTR) &_MSG__GAD)
extern const struct FC_Type _MSG_BT_BOOT_GAD;
#define MSG_BT_BOOT_GAD ((APTR) &_MSG_BT_BOOT_GAD)


#endif /* gadtools_cat_H */
