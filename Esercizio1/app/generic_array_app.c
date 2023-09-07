#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include "../include/generic_array.h"

#define BUFFSIZE 1024
#define COMPARESTRING compare_record_string
#define COMPARESTRINGD compared_record_string
#define COMPAREINT compare_record_int
#define COMPAREINTD compared_record_int
#define COMPAREFLOAT compare_record_float
#define COMPAREFLOATD compared_record_float

typedef struct _GenericArray GenericArray;

typedef struct Record {
  int id;
  char *field1;
  int field2;
  float field3;
} Record;

/* =============== DECLARATIONS FUNCTIONS ================= */

int compared_record_string(void* elem1, void* elem2);
int compared_record_int(void* elem1, void* elem2);
int compared_record_float(void* elem1, void* elem2);
int compare_record_string(void* elem1, void* elem2);
int compare_record_int(void* elem1, void* elem2);
int compare_record_float(void* elem1, void* elem2);

static void free_array(GenericArray* generic_array);
static void print_array(GenericArray* generic_array);
static void load_array(const char *filename, GenericArray* generic_array);
static void loadinsertion(const char *filename, GenericArrayCmp compare);
static void loadquick(const char *filename, GenericArrayCmp compare);

/* ================ COMPARATORS USED BY INSERTION SORT =================== */

int compared_record_string(void* elem1, void* elem2) {
  Record *r1 = (struct Record *)elem1;
  Record *r2 = (struct Record *)elem2;
  return (strcmp(r1->field1, r2->field1) < 0);
}

int compared_record_int(void* elem1, void* elem2) {
  Record *r1 = (struct Record *)elem1;
  Record *r2 = (struct Record *)elem2;
  return (r1->field2 < r2->field2);
}

int compared_record_float(void* elem1, void* elem2) {
  Record *r1 = (struct Record *)elem1;
  Record *r2 = (struct Record *)elem2;
  return (r1->field3 < r2->field3);
}

/* ================ COMPARATORS USED BY QUICK SORT =================== */

int compare_record_string(void* elem1, void* elem2) {
  Record *r1 = (struct Record *)elem1;
  Record *r2 = (struct Record *)elem2;
  return (strcmp(r1->field1, r2->field1) > 0);
}

int compare_record_int(void* elem1, void* elem2) {
  Record *r1 = (struct Record *)elem1;
  Record *r2 = (struct Record *)elem2;
  return (r1->field2 > r2->field2);
}

int compare_record_float(void* elem1, void* elem2) {
  Record *r1 = (struct Record *)elem1;
  Record *r2 = (struct Record *)elem2;
  return (r1->field3 > r2->field3);
}

/* ====================== FUNCTIONS ======================== */

static void free_array(GenericArray* generic_array) {
  int num_elements = GenericArray_size(generic_array), i;
  for(i = 0; i < num_elements; i++) {
    struct Record *array = (struct Record *)GenericArray_get(generic_array, i);
    free(array->field1);
    free(array);
  }
  GenericArray_free(generic_array);
}

static void print_array(GenericArray* generic_array) {
  int num_elements = GenericArray_size(generic_array), i;
  for(i = 0; i < num_elements; i++) {
    struct Record *array = (struct Record *)GenericArray_get(generic_array, i);
    printf("%d, %s, %d, %f\n",array->id, array->field1, array->field2, array->field3);
  }
}

static void load_array(const char *filename, GenericArray* generic_array) {
  char *read_line;
  char buffer[BUFFSIZE];
  FILE *fp;

  printf("READING FILE...\n");
  fp = fopen(filename, "r");
  if(fp == NULL) {
        printf("\nUnable to open file.\n");
        printf("Please check if file exists and you have read privilege.\n");
        exit(EXIT_FAILURE);
   }
    
  while(fgets(buffer, BUFFSIZE, fp) != NULL) {
    read_line = malloc((strlen(buffer) + 1) * sizeof(char));
    if(read_line == NULL) {
          printf("\nUnable to to allocate memory\n");
          printf("Please check if file exists and you have read privilege.\n");
          exit(EXIT_FAILURE);
    }
      strcpy(read_line, buffer);
      char *id_read_line = strtok(read_line, ",");
      char *string_read_line = strtok(NULL, ",");
      char *int_read_line = strtok(NULL, ",");
      char *float_read_line = strtok(NULL, ",");
      char *string_field = malloc((strlen(string_read_line) + 1) * sizeof(char));
      
      if(string_field == NULL) {
          printf("\nUnable to to allocate memory\n");
          printf("Please check if file exists and you have read privilege.\n");
          exit(EXIT_FAILURE);
      }

      int read_id = atoi(id_read_line);
      strcpy(string_field, string_read_line);
      int read_field2 = atoi(int_read_line);
      float read_field3 = (float)atof(float_read_line);
      struct Record *record_p = malloc(sizeof(struct Record));
      
      if(record_p == NULL) {
          printf("\nUnable to to allocate memory\n");
          printf("Please check if file exists and you have read privilege.\n");
          exit(EXIT_FAILURE);
      }

      record_p->id = read_id;
      record_p->field1 = string_field;
      record_p->field2 = read_field2;
      record_p->field3 = read_field3;

      GenericArray_insert(generic_array, (void *)record_p);
      free(read_line);
    
  }
  fclose(fp);
}

static void loadinsertion(const char *filename, GenericArrayCmp compare) {
  GenericArray* generic_array = GenericArray_create(compare);
  struct timeval start, stop;
  double secs = 0;
  gettimeofday(&start, NULL); 
  load_array(filename, generic_array);
  gettimeofday(&stop, NULL); 
  secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
  printf("\nFILE LOADED IN : %0.2fs\n", secs);

  secs = 0;
  int l = 0;
  int r = GenericArray_size(generic_array);
  
 	gettimeofday(&start, NULL);
 	InsertionSort(generic_array, l, r-1);
 	gettimeofday(&stop, NULL);
 	secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
  print_array(generic_array);
  printf("\nOK, YOUR ARRAY WAS ORDERED To: %0.2fs\n", secs);
  free_array(generic_array);
}

static void loadquick(const char *filename, GenericArrayCmp compare) {
  GenericArray* generic_array = GenericArray_create(compare);
  struct timeval start, stop;
  double secs = 0;

  gettimeofday(&start, NULL); 
  load_array(filename, generic_array);
  gettimeofday(&stop, NULL); 
  secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
  printf("\nFILE LOADED IN : %0.2fs\n", secs);

  secs = 0;
  int l = 0;
  int r = GenericArray_size(generic_array);
  
  printf("\nPLEASE WAIT.. QUICK SORT WILL BE LOADED SOON..\n");
 	gettimeofday(&start, NULL);
 	QuickSort(generic_array, l, r-1);
 	gettimeofday(&stop, NULL);
 	secs = (double)(stop.tv_usec - start.tv_usec) / 1000000 + (double)(stop.tv_sec - start.tv_sec);
  print_array(generic_array);
  printf("\nOK, YOUR ARRAY WAS ORDERED TO: %0.2fs\n", secs);
  free_array(generic_array);
}

int main(int argc, char const *argv[]) {

  if(argc < 2) {
    printf("\n");
    printf("ERROR: incorrect number of arguments\n");
    exit(EXIT_FAILURE);
  }
  
	int algorithm;
	int algorithm_ord;

	printf("\n");
	printf("HOW WOULD YOU LIKE TO ORDER YOUR ALGORITHM ?\n\n");
	printf("TYPE 1 TO ORDER BY STRING \n");
	printf("TYPE 2 TO ORDER BY INT \n");
	printf("TYPE 3 TO ORDER BY FLOAT \n");
	printf("\nCHOOSE A NUMBER : ");
	
	scanf("%d", &algorithm);

	    switch(algorithm) {
        case 1:      
        	    
		    printf("\n");
		    printf("CHOOSE THE ORDER: \n\n");
		    printf("TYPE 1 TO ORDER BY BINARY INSERTION SORT \n");
		    printf("TYPE 2 TO ORDER BY QUICK SORT \n");
		    printf("\nCHOOSE A NUMBER : ");
		    scanf("%d", &algorithm_ord);
		    
          switch(algorithm_ord) {
            case 1: 
              loadinsertion(argv[1], COMPARESTRINGD);	
            break;
              
            case 2: 
              loadquick(argv[1], COMPARESTRING);	
            break;

            default:
              printf("YOU TYPED AN INCORRECT NUMBER\n");
              exit(-1);
            break;
          }
        break;

        case 2:

		    printf("\n");
		    printf("CHOOSE THE ORDER: \n\n");
		    printf("TYPE 1 TO ORDER BY BINARY INSERTION SORT \n");
		    printf("TYPE 2 TO ORDER BY QUICK SORT \n");
		    printf("\nCHOOSE A NUMBER : ");
		    scanf("%d", &algorithm_ord);
		    
          switch(algorithm_ord) {
            case 1: 
              loadinsertion(argv[1], COMPAREINTD);	
            break;
              
            case 2: 
              loadquick(argv[1], COMPAREINT);	
            break;

            default:
              printf("YOU TYPED AN INCORRECT NUMBER\n");
              exit(-1);
            break;
          }
        break;

        case 3:
  	    
        printf("\n");
		    printf("CHOOSE THE ORDER: \n\n");
		    printf("TYPE 1 TO ORDER BY BINARY INSERTION SORT \n");
		    printf("TYPE 2 TO ORDER BY QUICK SORT \n");
		    printf("\nCHOOSE A NUMBER : ");
		    scanf("%d", &algorithm_ord);
		    
		      switch(algorithm_ord) {
		    	  case 1: 
              loadinsertion(argv[1], COMPAREFLOATD);	
  		      break;
  		    	
  		      case 2: 
              loadquick(argv[1], COMPAREFLOAT);	
		        break;

		        default:
               printf("YOU TYPED AN INCORRECT NUMBER\n");
               exit(-1);
            break;
		    }
        break;

        default:
          printf("YOU TYPED AN INCORRECT NUMBER\n");
          exit(-1);
        break;
    }
      
  return (EXIT_SUCCESS);

}