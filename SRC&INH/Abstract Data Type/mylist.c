#include "stdlib.h"
#include "string.h"
#include "mylist.h"


void list_init(List *list,void (*destroy)(void *data)){
  list->size = 0;
	list->destroy = destroy;
	list->head = NULL_P;
	list->tail = NULL_P;
	
	return;
}

void list_destroy(List *list){
  void *data;
	
  while(list_size(list) > 0){
	  if(list_rem_next(list,NULL_P,(void**)&data)==0 && list->destroy!=NULL_P){
		  list->destroy(data);
		}
	}
	memset(list,0,sizeof(List));
	
	return;
}

int list_ins_next(List *list,ListElmt *element,const void *data){
  ListElmt *new_element;
	
	if((new_element = (ListElmt*)malloc(sizeof(ListElmt))) == NULL_P){
	  return -1;
	}
  new_element->data = (void*)data;
	if(element == NULL_P){
	  if(list_size(list) == 0){
		  list->tail = new_element;
		}
		new_element->next = list->head;
		list->head = new_element;
	}
	else{
	  if(element->next == NULL_P){
		  list->tail = new_element;
		}
		new_element->next = element->next;
		element->next = new_element;
	}
	list->size++;
	
	return 0;
}

int list_rem_next(List *list,ListElmt *element,void **data){
  ListElmt *old_element;
	
	if(list_size(list) == 0){
	  return -1;
	}
  if(element == NULL_P){
	  *data = list->head->data;
		old_element = list->head;
		list->head = list->head->next;
		if(list_size(list) == 1){
		  list->tail = NULL_P;
		}
	}
	else{
	  if(element->next == NULL_P){
		  return -1;
		}
		*data = element->next->data;
		old_element = element->next;
		element->next = element->next->next;
		if(element->next == NULL_P){
		  list->tail = element;
		}
	}
	free(old_element);
	list->size--;
	
	return 0;
}
