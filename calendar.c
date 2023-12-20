#include "calendar.h"
#include "linkedlist.h"

int month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
int existenceDay[32];

int main(void)
{
  t_calendar_info now_info;
  t_menu          menu;
  t_date*         selected = NULL;
  
  init();
  loadListFromFile();
  initCalendar(&now_info);
  while ((menu = printUI(&now_info)) != EXIT)
  {
    switch (menu)
    {
    case MOVE_NEXT_MONTH:
      setNextMonth(&now_info);
      break;

    case MOVE_PREV_MONTH:
      setPrevMonth(&now_info);
      break;

    case SELECT_DATE:
      if ((selected = getDate(&now_info)) == NULL)
        continue;
      selectByDate(selected);
      break;

    case PRINT_ALL:
      printAll();
	    printf("계속하시려면 아무 키나 눌러주세요.");
      fflush(stdin);
      getchar();
      break;

    default:
      break;
    }
  }
  releaseList();
  return (0);
}

void  initCalendar(t_calendar_info* select_date)
{
  struct tm now_time;
  time_t    t;
  t = time(NULL);
  now_time = *localtime(&t);
  select_date->year = now_time.tm_year+1900;
  select_date->month_index = now_time.tm_mon;
  now_time.tm_mday = 1;
  mktime(&now_time);
  select_date->month_wday = now_time.tm_wday;
  setFebruary(select_date->year);
}

void  searchExistence(t_calendar_info* now_info)
{
  char  date[8];
  sprintf(date, "%04d-%02d",now_info->year, now_info->month_index + 1);
	memset(existenceDay, 0, sizeof(existenceDay));
  getExistence(date, existenceDay);
}

void  printCalendar(t_calendar_info* now_info)
{
  searchExistence(now_info);
  printf("\n--------------------%d년 %d월--------------------\n\n", now_info->year, now_info->month_index + 1);
  printf("일\t월\t화\t수\t목\t금\t토\n");
  for (int i = 0; i < (now_info->month_wday % 7 ); i++)
    printf(" \t");
  for (int i = 1; i <= month[now_info->month_index]; i++)
  {
    if(existenceDay[i] == 1)
      printf("%d*\t", i);
    else
      printf("%d\t", i);

    if ((i + now_info->month_wday) % 7 == 0)
      printf("\n");
  }
  printf("\n\n");
}

t_menu  printUI(t_calendar_info* now_info)
{
  int   num = -1;
  char  input[10];
  printCalendar(now_info);
  printf("메모를 쓸 날짜를 입력해주세요. (1 ~ %d)\n(저번 달로 이동 : Prev/P/p )\t(다음 달로 이동 : Next/N/n ) \n(모든 메모 조회 : all/a)\t(종료: 0)\n입력 : "
          , month[now_info->month_index]);
  fflush(stdin);
	scanf("%s", input);
  if (strcmp(input,"0") == 0)
    return (EXIT);
  if (strcmp(input,"Next") == 0 ||
      strcmp(input,"N") == 0 || 
      strcmp(input,"n") == 0)
    return (MOVE_NEXT_MONTH);
  if (strcmp(input,"Prev") == 0 || 
            strcmp(input,"P") == 0 || 
            strcmp(input,"p") == 0)
    return (MOVE_PREV_MONTH);
  if (strcmp(input,"all") == 0 ||
            strcmp(input,"a") == 0)
    return (PRINT_ALL);
  if ((num = atoi(input)) > 0  && 
      num <= month[now_info->month_index])
  {
    now_info->day = num;
    return (SELECT_DATE);
  }
  return (CONTINUE);
}

t_date* getDate(t_calendar_info* calendar)
{
  char    date[11];
  t_date* selected = NULL;
  sprintf(date, "%04d-%02d-%02d",calendar->year, calendar->month_index + 1, calendar->day);
  if ((selected = searchDateNode(date)) != NULL)
  {
    printMemoByDate(selected);
    return selected;
  }
  else
  {
    printf("해당하는 날짜에 메모가 없습니다.\n새로운 메모를 등록하시겠습니까?(y/n):\n");
    fflush(stdin);
    char ch = getchar();
    if (ch == 'y' || ch == 'Y')
    {
      selected = addDate(calendar);
      newMemo(selected);
      saveListToFile();
      printMemoByDate(selected);
      return (selected);
    }
    return (NULL);
  }
}

void  selectByDate(t_date* selected)
{
  t_menu  menu;
  int     num;
  while ((menu = printCRUD()) != EXIT)
  {
    switch (menu)
    {
    case PRINT:
      printf("조회하고 싶은 메모번호를 입력하세요 : ");
      int i = 0;
      scanf("%d", &num);
      printMemoByNum(selected, num);
      break;

    case REGISTER:
      newMemo(selected);
      saveListToFile();
      break;

    case MODIFY:
      printf("변경하고 싶은 메모번호를 입력하세요 : ");
      scanf("%d", &num);
      printf("수정 내용을 입력하세요.");
      char  content[MEMO_SIZE];
      scanf(" %[^\n]", &content);
      if(modifyMemoByNum(selected, content, num))
      {
        printf("성공적으로 수정되었습니다.\n");
        saveListToFile();
      }
      else
        printf("실패하였습니다.\n");
      break;

    case REMOVE:
      printf("메모를 선택하세요: ");
      scanf("%d", &num);
      int condition = removeMemoByNum(selected, num);
      if (condition == 2)
      {
        printf("메모가 모두 삭제되었습니다.\n");
        return;
      }
      else if (condition == 1)
      {
        printf("성공적으로 삭제하였습니다.\n");
        saveListToFile();
      }
      else
        printf("삭제되지 않았습니다.\n");
      break;

    case SAVE:
      saveListToFile();
      break;

    default:
      break;
    }
  }
  
}
t_menu  printCRUD()
{
  t_menu  menu;

  printf("1) 조회\t\t2) 등록\n3) 변경\t\t4) 삭제 0) 이전으로\n명령어를 입력해주세요 : ");
  scanf("%d", &menu);
  if (menu == 1)
    return PRINT;
  if (menu == 2)
    return REGISTER;
  if (menu == 3)
    return MODIFY;
  if (menu == 4)
    return REMOVE;
  if (menu == 0)
    return EXIT;
}

void  setNextMonth(t_calendar_info* calendar)
{
  calendar->month_wday = (calendar->month_wday + month[calendar->month_index]) % 7;
  if (calendar->month_index >= 11)
  {
    calendar->year = calendar->year + 1;
    calendar->month_index = 0;
    setFebruary(calendar->year);
  }
  else
    calendar->month_index = calendar->month_index + 1;
}

void  setPrevMonth(t_calendar_info* calendar)
{
  if (calendar->month_index <= 0)
  {
    calendar->year = calendar->year - 1;
    calendar->month_index = 11;
    setFebruary(calendar->year);
  }
  else
    calendar->month_index = calendar->month_index - 1;
  calendar->month_wday = ((calendar->month_wday - month[calendar->month_index]) % (7) + 7);
}

void  writeMemo(char* date, char* memo)
{
  printf("선택날짜:%s\n메모를 입력해주세요: ", date);
  scanf(" %[^\n]", memo);
}

void  newMemo(t_date* date)
{
  char  memo[MEMO_SIZE];
  writeMemo(date->date, memo);
  t_memo* pHead = date->memo;
  addMemoNewNode(&pHead, date, memo);
}

t_date*  addDate(t_calendar_info* calendar)
{
  char  date[11];
  sprintf(date, "%04d-%02d-%02d",calendar->year, calendar->month_index + 1, calendar->day);
  t_date* newDate;
  newDate = addDateNewNode(date);
  return (newDate);
}

int   isLeapYear(int year)
{
  if((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))
    return (1);
  else
    return (0);
}

void  setFebruary(int year)
{
  if (isLeapYear(year))
    month[1] = 29;
  else
    month[1] = 28;
}
