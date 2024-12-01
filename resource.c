#include "defs.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Resource functions */

/**
 * Creates a new `Resource` object.
 *
 * Allocates memory for a new `Resource` and initializes its fields.
 * The `name` is dynamically allocated.
 *
 * @param[out] resource      Pointer to the `Resource*` to be allocated and initialized.
 * @param[in]  name          Name of the resource (the string is copied).
 * @param[in]  amount        Initial amount of the resource.
 * @param[in]  max_capacity  Maximum capacity of the resource.
 */
void resource_create(Resource **resource, const char *name, int amount, int max_capacity) {
    *resource = (Resource*) malloc(sizeof(Resource));
    (*resource)->amount = amount;
    (*resource)->max_capacity = max_capacity;
    (*resource)->name = (char*)malloc(sizeof(strlen(*name)));
    strcpy((*resource)->name, name);

}

/**
 * Destroys a `Resource` object.
 *
 * Frees all memory associated with the `Resource`.
 *
 * @param[in,out] resource  Pointer to the `Resource` to be destroyed.
 */
void resource_destroy(Resource *resource) {
    if(resource != NULL){
        free(resource);
    }
}

/* ResourceAmount functions */

/**
 * Initializes a `ResourceAmount` structure.
 *
 * Associates a `Resource` with a specific `amount`.
 *
 * @param[out] resource_amount  Pointer to the `ResourceAmount` to initialize.
 * @param[in]  resource         Pointer to the `Resource`.
 * @param[in]  amount           The amount associated with the `Resource`.
 */
void resource_amount_init(ResourceAmount *resource_amount, Resource *resource, int amount) {
    resource_amount->resource = resource;
    resource_amount->amount = amount;
}

/**
 * Initializes the `ResourceArray`.
 *
 * Allocates memory for the array of `Resource*` pointers and sets initial values.
 *
 * @param[out] array  Pointer to the `ResourceArray` to initialize.
 */
void resource_array_init(ResourceArray *array) {
    array->resources = (Resource**)calloc(1, sizeof(Resource*));
    array->capacity = 1;
    array->size = 0;
}

/**
 * Cleans up the `ResourceArray` by destroying all resources and freeing memory.
 *
 * Iterates through the array, calls `resource_destroy` on each `Resource`,
 * and frees the array memory.
 *
 * @param[in,out] array  Pointer to the `ResourceArray` to clean.
 */
void resource_array_clean(ResourceArray *array) {
    if (array == NULL || array->resources == NULL) {
        return;
    }

    for (int i = 0; i < array->size; i++) {
        resource_destroy(array->resources[i]);
        array->resources[i] = NULL;
    }

    free(array->resources);
    array->resources = NULL;
    array->capacity = 0;
    array->size = 0;
}

/**
 * Adds a `Resource` to the `ResourceArray`, resizing if necessary (doubling the size).
 *
 * Resizes the array when the capacity is reached and adds the new `Resource`.
 * Use of realloc is NOT permitted.
 * 
 * @param[in,out] array     Pointer to the `ResourceArray`.
 * @param[in]     resource  Pointer to the `Resource` to add.
 */
void resource_array_add(ResourceArray *array, Resource *resource) {
    
    if(array->capacity < array->size + 1){
       
        //if not enough capacity, allocate new memory 
        Resource **newArray = (Resource**) calloc((array->size * 2), sizeof(Resource*));
        array->capacity = array->size * 2; //increment the capacity
        
        //copy everything from old memory to new memory
        for(int i = 0; i < array->size; i++){
            newArray[i] = array->resources[i];
        }
        
        //clean old memory
        free(array->resources);
        //set pointer to the new array
        array->resources = newArray;
    }

    //add the new resource to the new memory
    array->resources[array->size] = resource;        
    //increment the size
    array->size++;
}