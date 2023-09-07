#ifndef GENERIC_ARRAY_H
#define GENERIC_ARRAY_H

/* 
 * An array of any number of elements of any kind, ordered in non descending order
 * according to a specific precedence relation 
 * 
 */
   
/* Definition of the opaque type */
typedef struct _GenericArray GenericArray;

typedef int (*GenericArrayCmp)(void*, void*);

/* ============================= UTILS ====================================== */

/* Create new Generic Array */
GenericArray* GenericArray_create(GenericArrayCmp);

/* Returns:
 * - 1 if the array is empty
 * - 0 if the array is not empty 
 */
int GenericArray_empty(GenericArray* generic_array);

/* Returns the capacity of the Generic Array */
int GenericArray_capacity(GenericArray* generic_array);

/* Returns the number of elements currently stored in the Generic Array */
unsigned long GenericArray_size(GenericArray* generic_array);

/* Deallocates the Generic Array */
void GenericArray_free(GenericArray* generic_array);

/* It accepts as input a pointer to an Generic Array and an integer "i" 
   and it returns the pointer to the i-th element of the Generic Array */
void* GenericArray_get(GenericArray* generic_array, int i);

/* Insert an element inside the Generic Array */
void GenericArray_insert(GenericArray* generic_array, void* element);

/* Method that doubles the size of the Generic Array */
void GenericArray_check_and_realloc(GenericArray* generic_array);

/* Reverses the order of elements in the Generic Array */
void GenericArray_reverse(GenericArray* generic_array);

/* =========================== COMPARATORS ================================== */

int compare_string (void* elem1, void* elem2);
int compare_int (void* elem1, void* elem2);
int compare_float (void* elem1, void* elem2);

/* ======================= INSERTION SORT FUNCTION ========================== */

void InsertionSort(GenericArray* generic_array, int low, int high);
int BinarySearch(GenericArray* generic_array, int low, int high, void* element);

/* ======================== QUICK SORT FUNCTION ============================= */

void QuickSort(GenericArray* generic_array, int low, int high);
int partition_random(GenericArray* generic_array,int low, int high);
int partition(GenericArray* generic_array, int low, int high);
void swap(void** a, void** b);

#endif