#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

t_date    date_HeadNode = { 0, };
t_date    date_TailNode = { 0, };

t_date**  idxListDate = NULL;

void initList(void)
{
	releaseList();
	date_HeadNode.pNext = &date_TailNode;
	date_TailNode.pPrev = &date_HeadNode;
}

int isEmpty(void)
{
	if (date_HeadNode.pNext == &date_TailNode ||
		date_HeadNode.pNext == NULL)
		return 1;

	return 0;
}

void releaseList(void)
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
			printf("Delete: %s\n",
				memoTmp->data);
			free(memoTmp);
			memoTmp = memoTmp->pNext;
		}
		pDelete = pTmp;
		pTmp = pTmp->pNext;

		printf("Delete: %s, %s\n",
			pDelete->date, pDelete->memo);
		free(pDelete);
	}

	date_HeadNode.pNext = &date_TailNode;
	date_TailNode.pPrev = &date_HeadNode;
}

t_date*  searchDateNode(t_date* selected, char* date)
{
	t_date* dateTmp = date_HeadNode.pNext;
	int i = 0;
  while (dateTmp != NULL && dateTmp != &date_TailNode)
	{
		printf("searchDateNode %d¹ø¤Š, %s³¯\n",i++, dateTmp->date);
    if(strcmp(dateTmp->date, date) == 0)
		{
			// memcpy(selected, dateTmp, sizeof(t_date));
			return dateTmp;
		}
		dateTmp = dateTmp->pNext;
	}
  return NULL;
}


t_date* addDateNewNode(char* date)
{
	t_date* pDateNewNode = calloc(1, sizeof(t_date));
	printf("\naddDateNewNode : %p\n", pDateNewNode);
	strcpy(pDateNewNode->date, date);

	t_date* pPrevNode = date_TailNode.pPrev;
	pPrevNode->pNext = pDateNewNode;
	pDateNewNode->pPrev = pPrevNode;
	pDateNewNode->pNext = &date_TailNode;
	date_TailNode.pPrev = pDateNewNode;
	return pDateNewNode;
}

int loadListFromFile(void)
{
	releaseList();
	FILE* fp = NULL;
	fopen_s(&fp, "listData.dat", "rb");
	if (fp == NULL)
		return 0;

	t_date 	date = { 0 };
	t_date*	dateNode;
	while(fread(&date, sizeof(t_date), 1, fp) > 0)
	{
		dateNode = addDateNewNode(date.date);
		t_memo memo = { 0 };
		int i = 0;
		int memo_list_size = date.memo_cnt;
		printf("loadListFromFile -> date : %s, memo_cnt : %d\n",dateNode->date, memo_list_size);
		t_memo* pHead = NULL;
		while(i < memo_list_size && fread(&memo, sizeof(t_memo), 1, fp) > 0)
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

int saveListToFile(void)
{
	FILE* fp = NULL;
	fopen_s(&fp, "listData.dat", "wb");
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
			printf("write success content : %s\n", memoTmp->data);
      fwrite(memoTmp, sizeof(t_memo), 1, fp);
      memoTmp = memoTmp->pNext;
			i++;
    }
		printf("save_cnt : %d, date_tmp_cnt : %d\n", i, dateTmp->memo_cnt);
		dateTmp = dateTmp->pNext;
	}

	fclose(fp);
	return 1;
}

void		printAllMemo(t_date* date)
{
	t_memo* pCur = date->memo;
	t_memo* pPrev;
	t_memo* pNext;
	int i = 0;
	while(pCur != NULL)
	{
		printf("%d: %s\n",i++, pCur->data);
		pCur = pCur->pNext;
	}
}

void	printMemoByNum(t_date* date, int num)
{
  t_memo* pCur = date->memo;
	int i = 0;
  while(pCur != NULL && i < num)
  {
    pCur = pCur->pNext;
    i++;
  }
  printf("%s\n", pCur->data);
}

void addMemoNewNode(t_memo** pHead, t_date* date, char* content)
{
	t_memo* pMemoNewNode = calloc(1, sizeof(t_memo));

	strcpy(pMemoNewNode->data, content);
	printf("addMemoNewNode -> debug :%s\n", pMemoNewNode->data);

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
	printf("addMemoNewNode -> debug :%s\n", pMemoNewNode->data);
	date->memo_cnt++;
	date->memo = *pHead;
	printf("date->memo : %p, pMemoNewNode : %p\n", date->memo, pMemoNewNode);
	printf("after date->memo : %p, pMemoNewNode : %p\n", date->memo, pMemoNewNode);
	printf("date: %s,addMemoNewNode -> debug :%s, date_memo_cnt : %d\n",date->date, date->memo->data, date->memo_cnt);
}

int modifyMemoByNum(t_date* date, char* content, int num)
{
	t_memo* pCur = date->memo;
	t_memo* pNextNode;
	t_memo* pPrevNode = pCur;
	t_memo* pDateNewNode = calloc(1, sizeof(t_memo));
	sprintf(pDateNewNode->data, content);
	int index = 0;
	while(pCur != NULL && index < num)
	{
		if (index == num)
		{
			pDateNewNode->pNext = pCur->pNext;
			pDateNewNode = pCur->pPrev;
			pCur = pDateNewNode;
			return 1;
		}
		pCur = pCur->pNext;
		index++;
	}
}

int	removeDateNode(t_date* date)
{
	t_date* pCur = date;
	t_date* pNextNode;
	t_date* pPrevNode;
	pNextNode = pCur->pNext;
	pPrevNode = pCur->pPrev;

	pNextNode->pPrev = pCur->pPrev;
	pPrevNode->pNext = pCur->pNext;
	return 1;
}

int removeMemoByNum(t_date* date, int num)
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
	else
	{
		while(pCur != NULL && index <= num)
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
	}
	return 0;
}

