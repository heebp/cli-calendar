#include "linkedlist.h"

t_date    date_HeadNode = { 0, };
t_date    date_TailNode = { 0, };
char 			FILE_NAME[10] = "memo.dat";

void	initList(void)
{
	releaseList();
	date_HeadNode.pNext = &date_TailNode;
	date_TailNode.pPrev = &date_HeadNode;
}

int		isEmpty(void)
{
	if (date_HeadNode.pNext == &date_TailNode || date_HeadNode.pNext == NULL)
		return 1;

	return 0;
}

void	releaseList(void)
{
	if (isEmpty())
		return; 

	t_date* pTmp = date_HeadNode.pNext;
	t_date* pDelete;
	while (pTmp != &date_TailNode)
	{
		t_memo *memoTmp = pTmp->memo;
		while (memoTmp != NULL)
		{
			// printf("Delete: %s\n",memoTmp->data);
			free(memoTmp);
			memoTmp = memoTmp->pNext;
		}
		pDelete = pTmp;
		pTmp = pTmp->pNext;

		// printf("Delete: %s, %s\n",pDelete->date, pDelete->memo);
		free(pDelete);
	}

	date_HeadNode.pNext = &date_TailNode;
	date_TailNode.pPrev = &date_HeadNode;
}


t_date*	searchDateNode(char* date)
{
	t_date* dateTmp = date_HeadNode.pNext;
	int i = 0;
  while (dateTmp != NULL && dateTmp != &date_TailNode)
	{
    if (strcmp(dateTmp->date, date) == 0)
			return dateTmp;
		dateTmp = dateTmp->pNext;
	}
  return NULL;
}

t_date*	addDateNewNode(char* date)
{
	t_date* pDateNewNode = calloc(1, sizeof(t_date));
	strcpy(pDateNewNode->date, date);

	t_date* pPrevNode = date_TailNode.pPrev;
	pPrevNode->pNext = pDateNewNode;
	pDateNewNode->pPrev = pPrevNode;
	pDateNewNode->pNext = &date_TailNode;
	date_TailNode.pPrev = pDateNewNode;
	return pDateNewNode;
}

int		loadListFromFile(void)
{
	releaseList();
	FILE* fp = NULL;
	fopen_s(&fp, FILE_NAME, "rb");
	if (fp == NULL)
		return 0;

	t_date 	date = { 0 };
	t_date*	dateNode;
	while (fread(&date, sizeof(t_date), 1, fp) > 0)
	{
		dateNode = addDateNewNode(date.date);
		t_memo memo = { 0 };
		int i = 0;
		int memo_list_size = date.memo_cnt;
		t_memo* pHead = NULL;
		while (i < memo_list_size && fread(&memo, sizeof(t_memo), 1, fp) > 0)
		{
			addMemoNewNode(&pHead, dateNode, memo.data);
			memset(&memo, 0, sizeof(t_memo));
			i++;
		}
		memset(&date, 0, sizeof(t_date));
	}
	fclose(fp);
	return 1;
}

int		saveListToFile(void)
{
	FILE* fp = NULL;
	fopen_s(&fp, FILE_NAME, "wb");
	if (fp == NULL)
		return 0;

	t_date* dateTmp = date_HeadNode.pNext;
	while (dateTmp != NULL && dateTmp != &date_TailNode)
	{
		fwrite(dateTmp, sizeof(t_date), 1, fp);
    t_memo* memoTmp = dateTmp->memo;
		int	i = 0;
    while (memoTmp != NULL && i < dateTmp->memo_cnt )
    {
      fwrite(memoTmp, sizeof(t_memo), 1, fp);
      memoTmp = memoTmp->pNext;
			i++;
    }
		dateTmp = dateTmp->pNext;
	}

	fclose(fp);
	return 1;
}
int		appendListToFile(char* date)
{
	FILE* fp = NULL;
	fopen(&fp, FILE_NAME, "ab");
	t_date* pCur = searchDateNode(date);
	

}
void	printMemoByDate(t_date* date)
{
  printf("\n-----------%s 메모 목록-----------\n\n", date);

	t_memo* pCur = date->memo;
	t_memo* pNext;
	int i = 0;
	while (pCur != NULL)
	{
		printf("%d번 메모 : %s\n",i++, pCur->data);
		pCur = pCur->pNext;
	}

  printf("\n------------------------------------------\n\n");
}

void	printMemoByNum(t_date* date, int num)
{
  t_memo* pCur = date->memo;
	int i = 0;
  while (pCur != NULL && i < num)
  {
    pCur = pCur->pNext;
    i++;
  }
  printf("%s\n", pCur->data);
}

void	addMemoNewNode(t_memo** pHead, t_date* date, char* content)
{
	t_memo* pMemoNewNode = calloc(1, sizeof(t_memo));

	strcpy(pMemoNewNode->data, content);

	if (*pHead == NULL)
	{
		*pHead = pMemoNewNode;
	}
	else
	{
		t_memo* pCur = *pHead;
		while (pCur->pNext != NULL)
		{
			pCur = pCur->pNext;
		}
		pCur->pNext = pMemoNewNode;
	}
	date->memo_cnt++;
	date->memo = *pHead;
}

int		modifyMemoByNum(t_date* date, char* content, int num)
{
	t_memo* pCur = date->memo;
	t_memo* pNextNode = pCur;
	t_memo* pPrevNode = pCur;
	t_memo* pDateNewNode = calloc(1, sizeof(t_memo));
	sprintf(pDateNewNode->data, content);
	int index = 0;
	if (num == 0)
	{
		pDateNewNode->pNext = date->memo->pNext;
		free(date->memo);
		date->memo = pDateNewNode;
		return 1;
	}
	while (pCur != NULL && index <= num)
	{
		if (index == num)
		{
			pDateNewNode->pNext = pCur->pNext;
			pPrevNode->pNext = pDateNewNode;
			pCur = pDateNewNode;
			return 1;
		}
		pPrevNode = pCur;
		pCur = pCur->pNext;
		index++;
	}
	return 0;
}

int		removeDateNode(t_date* date)
{
	t_date* pCur = date;
	t_date* pNextNode;
	t_date* pPrevNode;
	pNextNode = pCur->pNext;
	pPrevNode = pCur->pPrev;

	pNextNode->pPrev = pCur->pPrev;
	pPrevNode->pNext = pCur->pNext;
	free(pCur);
	return 1;
}

int		removeMemoByNum(t_date* date, int num)
{
	t_memo* pCur = date->memo;
	t_memo* pNextNode = pCur;
	t_memo* pPrevNode = pCur;
	int index = 0;
	if (num == 0 && date->memo_cnt == 1)
	{
		removeDateNode(date);
		return 2;
	}
	if (num == 0)
	{
		date->memo = pCur->pNext;
		date->memo_cnt--;
		return 1;
	}
	while (pCur != NULL && index <= num)
	{
		if (index == num)
		{
			pPrevNode->pNext = pCur->pNext;
			date->memo_cnt--;
			if (index != 0)
				free(pCur);
			return 1;
		}
		pPrevNode = pCur;
		pCur = pCur->pNext;
		index++;
	}
	return 0;
}

void	nodeCopy(t_date* pLeft, t_date* pRight)
{
	pLeft->memo_cnt = pRight->memo_cnt;
	pLeft->memo = pRight->memo;
	strcpy_s(pLeft->date, sizeof(pLeft->date), pRight->date);
}

void	swapNode(t_date* pLeft, t_date* pRight)
{
	t_date tmp = *pLeft;
	nodeCopy(pLeft, pRight);
	nodeCopy(pRight, &tmp);
}

void	sortNodeByDate(void)
{
	if (isEmpty())
		return;

	t_date* pTmp = date_HeadNode.pNext;
	t_date* pSelected = NULL;
	t_date* pCmp = NULL;
	while (pTmp != NULL && pTmp != date_TailNode.pPrev)
	{
		pSelected = pTmp;
		pCmp = pTmp->pNext;
		while (pCmp != NULL && pCmp != &date_TailNode)
		{
			if (strcmp(pSelected->date, pCmp->date) > 0)
				pSelected = pCmp;

			pCmp = pCmp->pNext;
		}

		if (pTmp != pSelected)
			swapNode(pTmp, pSelected);

		pSelected = NULL;
		pTmp = pTmp->pNext;
	}
}

void	printAll(void)
{
  sortNodeByDate();
	t_date* dateTmp = date_HeadNode.pNext;
	int i = 0;
  while (dateTmp != NULL && dateTmp != &date_TailNode)
	{
		printf("%d번.",++i);
		printMemoByDate(dateTmp);
		dateTmp = dateTmp->pNext;
	}	
}