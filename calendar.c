#include "calendar.h"
#include "linkedlist.h"

int month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};

void    initCalendar(t_calendar_info* now_info);
t_menu  printCalendar(t_calendar_info* now_info);
int     isLeapYear(int year);
void    setFebruary(int year);
void    SearchByDate(t_calendar_info* calendar);
t_menu  printCRUD();
void    printSelectDayCommand();
void    moveCalendar(char* command, t_calendar_info* now_info);
void    setNextMonth(t_calendar_info* calendar);
void    setPrevMonth(t_calendar_info* calendar);
t_date* addDate(t_calendar_info* calendar);
void    newMemo(t_date* date);


int   main(void)
{
  t_calendar_info now_info;
  t_menu          menu;
  
  initList();
  loadListFromFile();
  initCalendar(&now_info);
  while ((menu = printCalendar(&now_info)) != 0)
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
      SearchByDate(&now_info);
      break;

    default:
      break;
    }
  }
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
  printf("\n%d , %d, %d\n", select_date->month_wday, now_time.tm_mday, now_time.tm_wday);
  setFebruary(select_date->year);
}


int   isLeapYear(int year)
{
  if((year % 4 == 0) && (year % 100 != 0) || (year % 400 == 0))
    return 1;
  else
    return 0;
}

void  setFebruary(int year)
{
  if (isLeapYear(year))
    month[1] = 29;
  else
    month[1] = 28;
}

t_menu  printCalendar(t_calendar_info* now_info)
{
  char  input[10];
  int   num;

  printf("%d�� %d��\n", now_info->year, now_info->month_index + 1);
  printf("��\t��\tȭ\t��\t��\t��\t��\n");
  for (int i = 0; i < (now_info->month_wday % 7 ); i++)
    printf(" \t");
  for (int i = 1; i <= month[now_info->month_index]; i++)
  {
    printf("%d\t", i);
    if ((i + now_info->month_wday) % 7 == 0)
      printf("\n");
  }
  printf("\n\n");

  printf("��¥�� �Է����ּ��� \n(���� �޷� �̵� : Next/N/n ) (���� �޷� �̵� : Prev/P/p ) (����: 0)\n�Է� : ");
	scanf("%s", input);
  if (strcmp(input,"Next") == 0 || strcmp(input,"N") == 0 || strcmp(input,"n") == 0)
    return (MOVE_NEXT_MONTH);
  else if(strcmp(input,"Prev") == 0 || strcmp(input,"P") == 0 || strcmp(input,"p") == 0)
    return (MOVE_PREV_MONTH);
  else if ((num = atoi(input)) > 0  && num <= month[now_info->month_index])
  {
    now_info->day = num;
    return (SELECT_DATE);
  }
  return -1;
}

void  SearchByDate(t_calendar_info* calendar)
{
  t_menu  menu;
  char    date[11];
  int     num;
  sprintf(date, "%d-%d-%d",calendar->year, calendar->month_index + 1, calendar->day);
  t_date* selected = NULL;
  // selected = calloc(1, sizeof(t_date));
  if ((selected = searchDateNode(selected, date)) == NULL)
  {
    printf("�ش��ϴ� ��¥�� �޸� �����ϴ�.\n���ο� �޸� ����Ͻðڽ��ϱ�?(y/n):\n");
    fflush(stdin);
    char ch = getchar();
    if (ch == 'y' || ch == 'Y')
    {
      selected = addDate(calendar);
      newMemo(selected);
    }
    return;
  }
  else
  {
    printf("%s, %s\n", selected->date, selected->memo);
    printf("%p, %p\n", selected, selected->memo);
    printAllMemo(selected);
  }
  while ((menu = printCRUD()) != 0)
  {
    switch (menu)
    {
    case PRINT:
      printf("��ȸ�ϰ� ���� �޸��ȣ�� �Է��ϼ��� : ");
      int i = 0;
      scanf("%d", &num);
      printMemoByNum(selected, num);
      break;

    case REGISTER:
        newMemo(selected);
      break;

    case MODIFY:
      printf("�����ϰ� ���� �޸��ȣ�� �Է��ϼ��� : ");
      scanf("%d", &num);
      printf("���� ������ �Է��ϼ���.");
      char  content[50];
      scanf("%s", &content);
      modifyMemoByNum(selected, content, num);
      break;

    case REMOVE:
      printf("�޸� �����ϼ���");
      scanf("%d", &num);
      int condition = removeMemoByNum(selected, num);
      if (condition == 2)
      {
        printf("�޸� ��� �����Ǿ����ϴ�.\n");
        return;
      }
      else if (condition == 1)
        printf("���������� �����Ͽ����ϴ�.");
      else
        printf("�������� �ʾҽ��ϴ�.");
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

  printf("1) ��ȸ\t\t2) ���\n3) ����\t\t4) ���� 8) ���� 0) ��������\n��ɾ �Է����ּ��� : ");
  scanf("%d", &menu);
  return menu;
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
  printf("���ó�¥:%s\n�޸� �Է����ּ���", date);
  scanf("%s", memo);
}

void  newMemo(t_date* date)
{
  char  memo[50];
  writeMemo(date->date, memo);
  t_memo* pHead = date->memo;
  addMemoNewNode(&pHead, date, memo);
}

t_date*  addDate(t_calendar_info* calendar)
{
  char  date[11];
  sprintf(date, "%d-%d-%d",calendar->year, calendar->month_index + 1, calendar->day);
  t_date* newDate;
  newDate = addDateNewNode(date);
  return newDate;
}

 