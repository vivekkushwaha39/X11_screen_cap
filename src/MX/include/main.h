#ifndef MAIN_H_
#define MAIN_H_
#include <Evas.h>
#include <Ecore.h>
//#include <time.h>		// For struct tm
//#include <sys/timeb.h>	// For struct timeb
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
//////////////////////////////////////////////////
#ifndef NULL
#define NULL 0
#endif

typedef int DATA32;
typedef struct Evas_Layer Evas_Layer;
typedef struct Evas_Func Evas_Func;
struct _Evas_Modifier
{
   struct {
      int       count;
      char    **list;
   } mod;
   Evas_Modifier_Mask mask; /* ok we have a max of 64 modifiers */
};

struct _Evas_Lock
{
   struct {
      int       count;
      char    **list;
   } lock;
   Evas_Modifier_Mask mask; /* we have a max of 64 locks */
};

typedef enum _Evas_Font_Hinting_Flags
{
   EVAS_FONT_HINTING_NONE,
   EVAS_FONT_HINTING_AUTO,
   EVAS_FONT_HINTING_BYTECODE
} Evas_Font_Hinting_Flags; 

typedef struct _Evas_Object_List Evas_Object_List;
struct _Evas_Object_List
{
   Evas_Object_List  *next, *prev;
   Evas_Object_List  *last;
};


struct _Evas
{
   Evas_Object_List  _list_data;

   DATA32            magic;

   struct {
      unsigned char  inside : 1;
      int            mouse_grabbed;
      DATA32         button;
      int            x, y;

      Evas_Coord         canvas_x, canvas_y;

      struct {
	 Evas_List *in;
      } object;

   } pointer;

   struct  {
      Evas_Coord     x, y, w, h;
      unsigned char  changed : 1;
   } viewport;

   struct {
      int            w, h;
      DATA32         render_method;
      unsigned char  changed : 1;
   } output;

   int               output_validity;

   Evas_List        *damages;
   Evas_List        *obscures;

   Evas_Layer       *layers;

   Evas_Hash        *name_hash;

   unsigned char     changed : 1;
   unsigned char     walking_layers : 1;

   int               events_frozen;

   struct {
      Evas_Func *func;
      struct {
	 void *output;

	 void *context;
      } data;

      void *info;
      int   info_magic;
   } engine;

   int            delete_grabs;
   int            walking_grabs;
   Evas_List     *grabs;

   Evas_List     *font_path;

   Evas_Object   *focused;
   Evas_Modifier  modifiers;
   Evas_Lock      locks;
   unsigned int   last_timestamp;
   void          *attach_data;
   int            last_click_counter;
   Evas_Font_Hinting_Flags hinting;
};

////////////////////////////////////////////


void *EventProcThread(void *);
//static int Sleep(double sleep_millisec);

#endif /*MAIN_H_*/
