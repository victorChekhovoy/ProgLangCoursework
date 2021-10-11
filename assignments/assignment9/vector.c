#include <stdlib.h>
#include "vector.h"
#include <stdio.h>
#include <assert.h>


void init(Vector *vector, int memorySize){
  vector->array = (int *)malloc(memorySize * sizeof(int));
  vector->memorySize = memorySize;
  vector->size = 0;
}

void cleanup(Vector *vector){
  if (vector->array != NULL){
  free(vector->array);
  vector->array = NULL;
  }
}

void print(Vector *vector){
  printf("Printing vector\nMemory size: %i\nSize: %i\nElements: ", vector->memorySize, vector->size);

  for (int i = 0; i < vector->size; i++){
    printf("%i, ", vector->array[i]);
  }
  printf("\n");
}

int get(Vector *vector, int location, int *value){
  if ((location < 0) || (location >= vector->size)){
    return 0;
  }
  else{
    *value = vector->array[location];
    return 1;
  }
}

int insert(Vector *vector, int location, int value){
  if ((location < 0) || (location > vector->size)){
    return 0;
  }
  if (vector->size < vector->memorySize){
    int inserting = value;
    int copying;
    for (int i = location; i <= vector->size; i++){
      get(vector, i, &copying);
      vector->array[i] = inserting;
      inserting = copying;
    }
    vector->size++;
    return 1;
  }
  else {
    int *new_array = malloc(2 * sizeof(vector->array));
    for (int i = 0; i < vector->size; i++){
      get(vector, i, &new_array[i]);
    }
    free(vector->array);
    vector->array = new_array;
    vector->memorySize *= 2;
    return insert(vector, location, value);;
  }
}

int delete(Vector *vector, int location){
  if ((location < 0) || (location > vector->size)){
    return 0;
  }
  int inserting;
  get(vector, vector->size - 1, &inserting);
  int copying;
  for (int i = (vector->size) - 1; i >= location; i--){
    get(vector, i - 1, &copying);
    insert(vector, i - 1, inserting);
    inserting = copying;
  }
  vector->size--;
  return 1;

}

int main(){
  return 1;
}
