#ifndef CALENDAR_H
# define CALENDAR_H

# ifndef MAX
#  define MAX 100
# endif

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

typedef struct  s_calendar_info
{
  int year;
  int month_index;
  int month_wday;
  int day;
} t_calendar_info;

typedef enum    s_menu
{
	EXIT, PRINT, REGISTER, MODIFY, REMOVE, MOVE_NEXT_MONTH, MOVE_PREV_MONTH, SELECT_DATE, SAVE
} t_menu;


#endif