#include "ListExample.h"


SFileListElement *g_fileList = NULL;
bool elementInsert(char cur_value, EFilePriority cur_pri)
{
	SFileListElement *element;
	if (NULL == g_fileList)
	{
		printf("g_fileList not init!\n");
		return false;
	}

	element = (SFileListElement *)g_fileList->list.next;
	while (element != g_fileList)
	{
		//遍历一遍整个list，检查是否有重复的value
		if (cur_value == element->fileInfo->value)
		{
			printf("%c has already inserted!\n", cur_value);
			return false;
		}
		element = (SFileListElement *)element->list.next;
	}

	element = (SFileListElement*)malloc(sizeof(SFileListElement));
	if (NULL == element)
	{
		printf("malloc faile!\n");
		return false;
	}

	element->fileInfo = (SFileInfo*)malloc(sizeof(SFileInfo));
	if (NULL == element->fileInfo)
	{
		printf("malloc failed!\n");
		return false;
	}
	memset(element->fileInfo, 0, sizeof(SFileInfo));

	element->fileInfo->value = cur_value;
	element->fileInfo->priority = cur_pri;
	snprintf(element->fileInfo->name, FILENAME_MAX_LENGTH, "The filename-%c", cur_value);

	printf("value=%c, priority=%d, name=%s\n", element->fileInfo->value, element->fileInfo->priority, element->fileInfo->name);
	SimpleListAddBefore(&element->list, &g_fileList->list);
}

void AllPrint(void)
{
	int i = 0;

	if (g_fileList != NULL)
	{
		SFileListElement *element = (SFileListElement *)g_fileList->list.next;

		printf("Print all element!\n");
		printf("|count|file name        |priority|value|\n");
		printf("+-----+-----------------+-------+------|\n");

		while (element != g_fileList)
		{
			printf("|%4d |%-15s|%5d|%5c|\n",i,
				element->fileInfo->name,
				element->fileInfo->priority,
				element->fileInfo->value);

			element = (SFileListElement *)element->list.next;
			i++;
		}
	}
}

void sortList(void)
{
	//冒泡排序
	SFileInfo *fileInfoTemp;
	SFileListElement *ele_i;
	SFileListElement *ele_j;
	printf("%s\n",__FUNCTION__);
	for (ele_i = (SFileListElement *)g_fileList->list.next; 
		ele_i != g_fileList; 
		ele_i = (SFileListElement *)ele_i->list.next)
	{
		for (ele_j = (SFileListElement *)ele_i->list.next;
			ele_j != g_fileList; 
			ele_j = (SFileListElement *)ele_j->list.next)
		{
			if (ele_i->fileInfo->priority > ele_j->fileInfo->priority)
			{
				fileInfoTemp = ele_i->fileInfo;
				ele_i->fileInfo = ele_j->fileInfo;
				ele_j->fileInfo = fileInfoTemp;
			}
		}
	}
}

void ListTest1(void)
{
	printf("%s\n",__FUNCTION__);
	g_fileList = (SFileListElement*)malloc(sizeof(SFileListElement));
	if (NULL == g_fileList)
	{
		printf("malloc fail\n");
		return -1;
	}

	g_fileList->fileInfo = NULL;
	SimpleListInit((SListElement *)g_fileList);

	elementInsert('a', EFilePriority_Normal);
	elementInsert('b', EFilePriority_High);
	elementInsert('c', EFilePriority_Low);
	elementInsert('d', EFilePriority_Normal);
	elementInsert('e', EFilePriority_High);
	elementInsert('f', EFilePriority_Normal);
	elementInsert('g', EFilePriority_High);
	elementInsert('h', EFilePriority_Normal);
	elementInsert('i', EFilePriority_Normal);
	elementInsert('j', EFilePriority_Normal);

	AllPrint();
	sortList();
	AllPrint();
       return 0;
}

