#ifndef LINKEDLIST_H
# define LINKEDLIST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct  s_memo
{
  char            data[50];
  struct s_memo*  pNext;
} t_memo;

typedef struct  s_date
{
  char            date[11];
  int             memo_cnt;
  struct s_memo*  memo;
  struct s_date*  pNext;
  struct s_date*  pPrev;
} t_date;

void    initList(void);
void    releaseList(void);
t_date* addDateNewNode(char* date);
void    addMemoNewNode(t_memo** pHead, t_date* date, char* content);
void    printMemoByDate(t_date* date);
void    printMemoByNum(t_date* date, int num);
t_date* searchDateNode(char* date);
int     modifyMemoByNum(t_date* date, char* content, int num);
int     removeMemoByNum(t_date* date, int num);
int     loadListFromFile(void);
int     saveListToFile(void);
void    sortNodeByDate(void);
void    printAll(void);

#endif