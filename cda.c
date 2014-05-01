/*
 * file: cda.c
 *
 * implementation of dynamic arrays in c.
 * designed to be effecient with larger objects.
 *
 * It works by using an array of void pointers.
 * This array can grow with realloc. 
 * This array do not shrink.
 * However, the user can release unnessecery elements. 
 *
 *license:BSD
 *
 * Author : Sanjib Pradhan
 * */


/********************         cda.h        *******************/
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#ifndef CDA_H
#define CDA_H
struct list_ptr{
	void * list[0];
};
typedef struct list_ptr * list_ptr;
struct cda{
	list_ptr  list_ptr;
	int length;
	int size;
};
typedef struct cda * cda;

/* 
 * macro: cda_create
 * creates a dynamic array. initially the size of the array is set to 512.
 * argumetns:
 * 	type: data type
 * returns: cda object for success. NULL for failure.
 */	
#define cda_create(type) create_cda(sizeof(type), DEFAULT_INIT_LENGTH)
/* 
 * macro: cda_init
 * creates a dynamic array. takes the initial size of the array as argument.
 * argumetns:
 * 	type: data type i
 * 	init_length: initial size of table.
 * returns: cda object for success. NULL for failure.
 */	
#define cda_init(type, init_length) create_cda(sizeof(type), init_length)
/* 
 * function: cda_destroy
 * releases a dynamic array.
 * argumetns:
 * 	cda_obj: cda object
 * returns: void.
 */	
void cda_destroy(cda cda_obj);

/* 
 * function: cda_set 
 * sets an element in the dynamic array.
 * argumetns:
 * 	cda_obj: cda object
 * 	index: index of the array
 * 	data: to be copied into the array.
 * returns: zero for success. non zzero for failure.
 */	
int cda_set(cda cda_obj, int index, void * data);
/* 
 * function: cda_get 
 * gets an element of the dynamic array.
 * argumetns:
 * 	cda_obj: cda object
 * 	index: index of the array
 * returns: pointer to the element or NULL.
 */	
void * cda_get(cda cda_obj, int index);

/* 
 * function: cda_release 
 * releases the memory of an element in the dynamic array.
 * argumetns:
 * 	cda_obj: cda object
 * 	index: index of the array
 * returns: void.
 */	
void cda_release(cda cda_obj, int index);
/* 
 * function: cda_status 
 * returns the status of an element in the dynamic element.
 * argumetns:
 * 	cda_obj: cda object
 * 	index: index of the array
 * returns: zero, if memory has been allocated for the element. else, nonzero.
 */	
int cda_status(cda cda_obj, int index);
#endif

/********************         cda.c        *******************/
//#include <cda.h>
static int DEFAULT_INIT_LENGTH = 512;
cda create_cda(unsigned int size, unsigned int init_length)
{
	cda cda_obj = NULL;
	cda_obj = (cda)calloc(1, sizeof(struct cda)); 
	if(NULL == cda_obj) 
		return NULL;
	if(0 >= (int)init_length)
		cda_obj->length = DEFAULT_INIT_LENGTH;
	else
		cda_obj->length = init_length;
	cda_obj->list_ptr = (list_ptr)calloc(cda_obj->length, sizeof(void *));
	if(NULL == cda_obj->list_ptr)
	{
		free(cda_obj);
		return NULL;
	} 
	cda_obj->size = size;
	return cda_obj;
}
void cda_destroy(cda cda_obj)
{
	if(NULL == cda_obj) 
		return -1;
	if(NULL == cda_obj->list_ptr) 
		return -2;
	
	int i;	
	for(i = 0; i < cda_obj->length; i++)
	{
		if(cda_obj->list_ptr->list[i])
			free(cda_obj->list_ptr->list[i]);	
	}

	free(cda_obj->list_ptr);
	free(cda_obj);
	return 0;	
}
static int cda_expand(cda cda_obj, int index)
{
	int new_length = 0;
	int i;
	for(i = 0; (i < 16) && (new_length < index); i++)
	{
		new_length = index * 2; 
	}
	if(16 == i) 
		return -1;
	list_ptr  temp = cda_obj->list_ptr;
	cda_obj->list_ptr = NULL;
	cda_obj->list_ptr = (list_ptr)calloc(new_length, sizeof(void *));
	if(NULL == cda_obj->list_ptr)
	{
		cda_obj->list_ptr = temp;
		return -2;
	}
	memcpy(cda_obj->list_ptr, temp, sizeof(void *) * cda_obj->length);
	free(temp);
	cda_obj->length = new_length; 
	return 0;
}
int cda_set(cda cda_obj, int index, void * data)
{
	if( cda_obj->length <= index)
	{
		int ret = 0;
		if(ret = cda_expand(cda_obj, index)) 
			return ret;
	}
	if(NULL == cda_obj->list_ptr->list[index])
	{
		cda_obj->list_ptr->list[index] = malloc(cda_obj->size);
		if(NULL == cda_obj->list_ptr->list[index]) 
			return -3;
	}
	memcpy(cda_obj->list_ptr->list[index], data, cda_obj->size);
	return 0;
}
void * cda_get(cda cda_obj, int index)
{
	if( cda_obj->length <= index)
	{
		if(cda_expand(cda_obj, index)) 
			return NULL;
	}
	if(NULL == cda_obj->list_ptr->list[index])
		cda_obj->list_ptr->list[index] = calloc(1, cda_obj->size);
	return cda_obj->list_ptr->list[index];	
}

void cda_release(cda cda_obj, int index)
{
	if( cda_obj->length <= index) 
		return;
	if(cda_obj->list_ptr->list[index])
	{
		free(cda_obj->list_ptr->list[index]);
		cda_obj->list_ptr->list[index] = NULL;
	}
}
int cda_status(cda cda_obj, int index)
{
	if( cda_obj->length <= index) 
		return 1;
	if(cda_obj->list_ptr->list[index]) 
		return 0;
	return 2;

}
/********************     tester       *******************/
#include <string.h>
//#include <cda.h>

#define LEN 2
main()
{
	struct dummy{char s[16];} dummy; 
	struct dummy * dummy_ptr = NULL; 
	strcpy(dummy.s, "dummy data");
	cda cda_obj = cda_init( struct dummy , LEN);
	int i;
	int ret;
	for(i = 0; i < 20; i++)
	{
		if(ret = cda_set(cda_obj, i, &dummy)) 
			printf("%d: cda_set failed. ret = %d\n", i, ret);
	}	
	printf("###############################################\n");
	for(i = 0; i < 30; i++)
	{
		if(NULL == cda_get(cda_obj, i)) printf ("%d: cda_get return failed\n", i);
	}	
	printf("###############################################\n");
	for(i = 0; i < 40; i++)
	{
		printf("iterating 1.      ");
		if(cda_status(cda_obj, i));
			dummy_ptr = NULL;
			dummy_ptr = cda_get(cda_obj, i);
			if(dummy_ptr)
				printf("data = %s\n", cda_get(cda_obj, i));
		printf("\n");
	}	
	printf("###############################################\n");
	for(i = 5; i < 10; i++)
	{
		cda_release(cda_obj, i);
	}	
	printf("###############################################\n");
	for(i = 0; i < 40; i++)
	{
		printf("iterating 2.      ");
		if(cda_status(cda_obj, i));
			dummy_ptr = NULL;
			dummy_ptr = cda_get(cda_obj, i);
			if(dummy_ptr)
				printf("data = %s\n", cda_get(cda_obj, i));
		printf("\n");
	}	
	cda_destroy(cda_obj);
}
