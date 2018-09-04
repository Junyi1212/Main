#ifndef SIMPLELIST_H
#define SIMPLELIST_H

#include <stdio.h>
#include <stdlib.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct SListElement
{
	struct SListElement* next; /**< Pointer to next in the list */
	struct SListElement* prev; /**< Pointer to previous in the list */
}SListElement;

/**
*******************************************************************************
*   
*   @brief Returns a pointer to the data structure stored into linked list element 
*
*   @def SIMPLELIST_ENTRY  
*
*   @param[in] __ptr     Pointer to list element
*   @param[in] __type    Type name of the data stored into list element
*   @param[in] __member  Name of the list element member in the data type
*
*   @return Pointer to __type data type
*
*   @Example:
*
*   @code
*
*   typedef struct SOwnDataElement {
*       u32                            data;
*       SListElement  listElement;
*   } SOwnDataElement;
* 
*   SListElement head;
*   SOwnDataElement* newE = malloc(sizeof(SOwnDataElement));
*   SListElement* e;
*   SOwnDataElement* i;
* 
*   // Important to init head element
*   SimpleListInit(&head);
* 
*   SimpleListAddBefore(&newE->listElement, &head);
*
*   // Following operations are needed if list member is not the first member
*   // in the own data struct.
*   e = head->listElement.next;
*   i = SIMPLELIST_ENTRY(e, SOwnDataElement, listElement);
*   
*   if (i == newE)
*   {
*   }
******************************************************************************/
#define SIMPLELIST_ENTRY(__ptr, __type, __member) \
    ((__type *)((char *)(__ptr)-(unsigned long)(&((__type *)0)->__member)))


/**
*******************************************************************************
*
*   @brief      Initialize list element.
*
*   @param[in]  element Pointer to an element to initialize
*
*   @return     Pointer to the initialized element
*
*   @ Description:
*   Initilizes given element to be used in a list.
*   It is important to init head element before any other element is added.
*   
*   @Example:
*
*   @code
*
*   typedef struct SOwnDataElement {
*       SListElement  list;
*       u32                            data;
*   } SOwnDataElement;
* 
*   SOwnDataElement* head = malloc(sizeof(SOwnDataElement));
*   SListElement head2;
* 
*   // In this example the head element is in the own element
*   SimpleListInit(&head->list);
*
*   // In this example the head element is alone
*   SimpleListInit(&head2);
******************************************************************************/
SListElement* SimpleListInit(SListElement* element);


/**
*******************************************************************************
*
*   @brief      Insert new element between two known element.
*
*   @param[in]  element  Pointer to an element to be inserted
*   @param[in]  prev     Pointer to an element which next shall be the new
*   @param[in]  next     Pointer to an element which prev shall be the new 
*
*   @return     Pointer to the new element
*
*   @ Description:
*   Mainly used in other API calls of SListElement internally.
*
*   @Example:
*
*   @code
*   @see SimpleListAddBefore
*   @see SimpleListAddAfter
* 
******************************************************************************/
SListElement* SimpleListInsert(SListElement* element, SListElement* prev, SListElement* next);


/**
*******************************************************************************
*
*   @brief      Add new element after given element.
*
*   @param[in]  element  Pointer to a new element to add to a list
*   @param[in]  head     Pointer to a a list where the new element shall be added
*
*   @return     Pointer to the new element
*
*   @ Description:
*   Useful for implementing stacks.
*
*   @ Example:
*
*   @code
*
*   typedef struct SOwnDataElement {
*       SListElement  list;
*       u32                            data;
*   } SOwnDataElement;
* 
*   SOwnDataElement* head = malloc(sizeof(SOwnDataElement));
*   SOwnDataElement* newE = malloc(sizeof(SOwnDataElement));
*
* 
*   SimpleListInit(&head->list);
* 
*   // Pointer to the element to be added, is returned
*   SimpleListAddAfter(&newE->list, &head->list) == &newE->list;
*
******************************************************************************/
SListElement* SimpleListAddAfter(SListElement* element, SListElement* head);

/**
*******************************************************************************
*
*   @brief      Add new element before given element.
*
*   @param[in]  element  Pointer to a new element to add to a list
*   @param[in]  head     Pointer to a a list where the new element shall be added
*
*   @return     Pointer to the new element
*
*   @ Description:
*   Useful for implementing queues.
* 
*   @ Example:
*
*   @code

*   typedef struct SOwnDataElement {
*       SListElement  list;
*       u32                            data;
*   } SOwnDataElement;
* 
*   SOwnDataElement* head = malloc(sizeof(SOwnDataElement));
*   SOwnDataElement* newE = malloc(sizeof(SOwnDataElement));
* 
*   // Only head element needs to be init because AddBefore operation
*   // does not use pointers from the source element
*   SimpleListInit(&head->list);
* 
*   SimpleListAddBefore(&newE->list, &head->list);
*
******************************************************************************/
SListElement* SimpleListAddBefore(SListElement* element, SListElement* head);

/**
*******************************************************************************
*
*   @brief      Removes given element from list.
*
*   @param[in]  element Pointer to a element in a list to to be removed
*
*   @return     Pointer to the removed element
*
*   @ Description:
*   Element is removed from the list. However element itself is not freed/deleted.
* 
*   @ Errors:   
*   None
* 
*   @ Restrictions:
*   None
*
*   @ Example:
*
*   @code
*
*
*   typedef struct SOwnDataElement {
*       u32                            data1;
*       SListElement  list; // List element can be anywhere in the struct
*       u32                            data2;
*       SListElement  list2; // Own element can be member of another list too
*       u32                            data3;
*   } SOwnDataElement;
* 
*   SListElement head;
*   SListElement head2;
*   SListElement* e;
*   SOwnDataElement* i = (SOwnDataElement*)malloc(sizeof(SOwnDataElement));
*   
*   SimpleListInit(&head);
*   SimpleListInit(&head2);
*   
*   // Add element to both lists but use different list element of the own structure
*   SimpleListAddBefore(&newE->list, &head);
*   SimpleListAddBefore(&newE->list2, &head2);
* 
*   // Remove all list elements
*   while (!SimpleListEmpty(&head))
*   {
*       e = head->list.next;
*       // Use SIMPLELIST_ENTRY macro since list element not the first member
*       i = SIMPLELIST_ENTRY(e, SOwnDataElement, list); // Get pointer to SOwnDataElement instance
*       // ... use the data of the instance
*       SimpleListRemove(&e->list);
*   }
*
*   // Remove all list2 elements
*   while (!SimpleListEmpty(&head->list2))
*   {
*       e = head->list2.next;
*       // Use SIMPLELIST_ENTRY macro since list element not the first member
*       i = SIMPLELIST_ENTRY(e, SOwnDataElement, list2); // Get pointer to SOwnDataElement instance
*       // ... use the data of the instance
*       SimpleListRemove(&e->list2);
*   }
*
******************************************************************************/
SListElement* SimpleListRemove(SListElement* const element);

/**
*******************************************************************************
*
*   @brief      Checks if the list is empty.
*
*   @param[in]  head  Pointer to list head element
*
*   @ Description:
*   List is considered to be empty if head element does not point to another
*   element thus meaning that there may be data in the head but nowhere else.
* 
*   @ Example:
*
*   @code
*
*   typedef struct SOwnDataElement {
*       SListElement  list;
*       u32                            data;
*   } SOwnDataElement;
* 
*   SListElement head;
*
*   SOwnDataElement* new = malloc(sizeof(SOwnDataElement));
* 
*   SimpleListInit(&head);
* 
*   SimpleListAddBefore(&new->list, &head);
* 
*   if (!SimpleListEmpty(&head))
*   {     
*       SimpleListRemove(&new->list);
*       free(new);
*   }
* 
*   if (SimpleListEmpty(&head))
*   {
*       printf("List is now empty (only head left)");
*   }
*
*
******************************************************************************/
bool SimpleListEmpty(const SListElement* const head);

/**
*******************************************************************************
*
*   @brief      Returns the number of element in the list.
*
*   @param[in]  head  Pointer to list head element
*
*   @ Description:
*   Element count does not contain the head element.
*
*   @ Example:
*
*   @code
*
*   typedef struct SOwnDataElement {
*       SListElement  list;
*       u32                            data;
*   } SOwnDataElement;
* 
*   SListElement head;
*
*   SOwnDataElement* new1 = malloc(sizeof(SOwnDataElement));
*   SOwnDataElement* new2 = malloc(sizeof(SOwnDataElement));
*   SOwnDataElement* new3 = malloc(sizeof(SOwnDataElement));
*   size_t size;
* 
*   SimpleListInit(&head);
* 
*   SimpleListAddBefore(&new1->list, &head);
*   SimpleListAddBefore(&new2->list, &head);
*   SimpleListAddBefore(&new3->list, &head);
* 
*   size = SimpleListSize(&head);
*
******************************************************************************/
size_t SimpleListSize(SListElement* head);


/**
*******************************************************************************
*
*   @brief      Moves given element to before another element.
*
*   @param[in]  element   Pointer to an element to move to another list
*   @param[in]  head      Pointer to a list where the new element shall be moved
*
*   @return     Pointer to the moved element
*
*   @ Description:
*   Moves given element from the list it currently is in to another list before
*   defined element. The moved element is returned.
*
*
*   @ Example:
*
*   @code
*
*   typedef struct SOwnDataElement {
*       SListElement  list;
*       u32                            data;
*   } SOwnDataElement;
* 
*   SOwnDataElement*              head = malloc(sizeof(SOwnDataElement));
*   SListElement head2;
*
*   SOwnDataElement* newE = malloc(sizeof(SOwnDataElement));
* 
*   SimpleListInit(&head->list);
*   SimpleListInit(&head2);
* 
*   // Note the difference bewteen usage of the different head elements
*   SimpleListAddBefore(&newE->list, &head->list);
*   SimpleListMoveBefore(&newE->list, &head2);
* 
* 
******************************************************************************/
SListElement* SimpleListMoveBefore(SListElement* element, SListElement* head);

/**
*******************************************************************************
*
*   @brief      Moves given element to after another element.
*
*   @param[in]  element   Pointer to an element to move to another list
*   @param[in]  head      Pointer to a list where the new element shall be moved
*
*   @return     Pointer to the new element
*
*   @ Description:
*   Moves given element from the list it currently is in to another list after
*   defined element. The moved element is returned.
* 
*   @ Example:
*
*   @code
*
*   typedef struct SOwnDataElement {
*       SListElement  list;
*       u32                            data;
*   } SOwnDataElement;
* 
*   SListElement head;
*   SListElement head2;
*   SOwnDataElement* newE = malloc(sizeof(SOwnDataElement));
* 
*   SimpleListInit(&head);
*   SimpleListInit(&head2);
* 
*   SimpleListAddBefore(&newE->list, &head);
* 
*   // Element shall be moved to beginning of list2 from list1
*   SimpleListMoveAfter(&newE->list, &head2) == &newE->list;
* 
* 
******************************************************************************/
SListElement* SimpleListMoveAfter(SListElement* element, SListElement* head);

#ifdef __cplusplus
}
#endif

#endif /* SIMPLELIST_H */