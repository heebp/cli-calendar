#ifndef CALENDAR_H
# define CALENDAR_H

# ifndef MEMO_SIZE
#  define MEMO_SIZE 150
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
	EXIT, CONTINUE, PRINT, REGISTER, MODIFY, REMOVE, MOVE_NEXT_MONTH, MOVE_PREV_MONTH, SELECT_DATE, SAVE, PRINT_ALL
} t_menu;

void    initCalendar(t_calendar_info* now_info);
t_menu  printUI(t_calendar_info* now_info);
void    printCalendar(t_calendar_info* now_info);
int     isLeapYear(int year);
void    setFebruary(int year);
t_date* getDate(t_calendar_info* calendar);
void    selectByDate(t_date* selected);
t_menu  printCRUD();
void    printSelectDayCommand();
void    moveCalendar(char* command, t_calendar_info* now_info);
void    setNextMonth(t_calendar_info* calendar);
void    setPrevMonth(t_calendar_info* calendar);
t_date* addDate(t_calendar_info* calendar);
void    newMemo(t_date* date);

#endif