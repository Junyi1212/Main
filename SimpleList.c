#include "SimpleList.h"


SListElement* SimpleListInit(SListElement* element)
{
	return element->next = element->prev = element;
} 

SListElement* SimpleListInsert(SListElement* element, SListElement* prev, SListElement* next)
{
	next->prev = element;
	prev->next = element;

	element->next = next;
	element->prev = prev;

	return element;
}


SListElement* SimpleListAddAfter(SListElement* element, SListElement* head)
{
	return SimpleListInsert(element, head, head->next);
} 

SListElement* SimpleListAddBefore(SListElement* element, SListElement* head)
{
	return SimpleListInsert(element, head->prev, head);
}

SListElement* SimpleListRemove(SListElement* const element)
{
	SListElement* next = element->next;
	SListElement* prev = element->prev;

	next->prev = prev;
	prev->next = next;

	element->next = NULL;
	element->prev = NULL;

	return element;
}

bool SimpleListEmpty(const SListElement* const head)
{
	return head == head->next;
}

size_t SimpleListSize(SListElement* head)
{
	size_t size = 0;
	SListElement* e = head->next;

	for (; e != head; ++size, e = e->next)
	{
	}

	return size;
}

SListElement* SimpleListMoveBefore(SListElement* element, SListElement* head)
{
	SListElement* next = element->next;
	SListElement* prev = element->prev;

	next->prev = prev;
	prev->next = next;

	return SimpleListAddBefore(element, head);
}

SListElement* SimpleListMoveAfter(SListElement* element, SListElement* head)
{
	SListElement* next = element->next;
	SListElement* prev = element->prev;

	next->prev = prev;
	prev->next = next;

	return SimpleListAddAfter(element, head);
}
