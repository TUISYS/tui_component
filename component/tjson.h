/*
  Copyright (c) 2009 Dave Gamble
 
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
 
  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.
 
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#ifndef tJSON__h
#define tJSON__h

#ifdef __cplusplus
extern "C" {
#endif
//#include <string.h>
//#include <stdio.h>
//#include <math.h>
//#include <stdlib.h>
//#include <float.h>
//#include <limits.h>
//#include <ctype.h>

/* tJSON Types: */
#define tJSON_False 0
#define tJSON_True 1
#define tJSON_NULL 2
#define tJSON_Number 3
#define tJSON_String 4
#define tJSON_Array 5
#define tJSON_Object 6

#define tJSON_IsReference 256

/* The tJSON structure: */
	typedef struct tJSON {
		struct tJSON *next, *prev;	/* next/prev allow you to walk array/object chains. Alternatively, use GetArraySize/GetArrayItem/GetObjectItem */
		struct tJSON *child;	/* An array or object item will have a child pointer pointing to a chain of the items in the array/object. */

		int type;	/* The type of the item, as above. */

		char *valuestring;	/* The item's string, if type==tJSON_String */
		int valueint;	/* The item's number, if type==tJSON_Number */
		double valuedouble;	/* The item's number, if type==tJSON_Number */

		char *string;	/* The item's name string, if this item is the child of, or is in the list of subitems of an object. */
	} tJSON;

	typedef struct tJSON_Hooks {
		void *(*malloc_fn) (size_t sz);
		void (*free_fn) (void *ptr);
	} tJSON_Hooks;

/* Supply malloc, realloc and free functions to tJSON */
	extern void tJSON_InitHooks(tJSON_Hooks * hooks);

/* Supply a block of JSON, and this returns a tJSON object you can interrogate. Call tJSON_Delete when finished. */
	extern tJSON *tJSON_Parse(const char *value);
/* Render a tJSON entity to text for transfer/storage. Free the char* when finished. */
	extern char *tJSON_Print(tJSON * item);
/* Render a tJSON entity to text for transfer/storage without any formatting. Free the char* when finished. */
	extern char *tJSON_PrintUnformatted(tJSON * item);
/* Delete a tJSON entity and all subentities. */
	extern void tJSON_Delete(tJSON * c);

/* Returns the number of items in an array (or object). */
	extern int tJSON_GetArraySize(tJSON * array);
/* Retrieve item number "item" from array "array". Returns NULL if unsuccessful. */
	extern tJSON *tJSON_GetArrayItem(tJSON * array, int item);
/* Get item "string" from object. Case insensitive. */
	extern tJSON *tJSON_GetObjectItem(tJSON * object, const char *string);

/* For analysing failed parses. This returns a pointer to the parse error. You'll probably need to look a few chars back to make sense of it. Defined when tJSON_Parse() returns 0. 0 when tJSON_Parse() succeeds. */
	extern const char *tJSON_GetErrorPtr(void);

/* These calls create a tJSON item of the appropriate type. */
	extern tJSON *tJSON_CreateNull(void);
	extern tJSON *tJSON_CreateTrue(void);
	extern tJSON *tJSON_CreateFalse(void);
	extern tJSON *tJSON_CreateBool(int b);
	extern tJSON *tJSON_CreateNumber(double num);
	extern tJSON *tJSON_CreateString(const char *string);
	extern tJSON *tJSON_CreateArray(void);
	extern tJSON *tJSON_CreateObject(void);

/* These utilities create an Array of count items. */
	extern tJSON *tJSON_CreateIntArray(const int *numbers, int count);
	extern tJSON *tJSON_CreateFloatArray(const float *numbers, int count);
	extern tJSON *tJSON_CreateDoubleArray(const double *numbers, int count);
	extern tJSON *tJSON_CreateStringArray(const char **strings, int count);

/* Append item to the specified array/object. */
	extern void tJSON_AddItemToArray(tJSON * array, tJSON * item);
	extern void tJSON_AddItemToObject(tJSON * object, const char *string, tJSON * item);
/* Append reference to item to the specified array/object. Use this when you want to add an existing tJSON to a new tJSON, but don't want to corrupt your existing tJSON. */
	extern void tJSON_AddItemReferenceToArray(tJSON * array, tJSON * item);
	extern void tJSON_AddItemReferenceToObject(tJSON * object, const char *string, tJSON * item);

/* Remove/Detatch items from Arrays/Objects. */
	extern tJSON *tJSON_DetachItemFromArray(tJSON * array, int which);
	extern void tJSON_DeleteItemFromArray(tJSON * array, int which);
	extern tJSON *tJSON_DetachItemFromObject(tJSON * object, const char *string);
	extern void tJSON_DeleteItemFromObject(tJSON * object, const char *string);

/* Update array items. */
	extern void tJSON_ReplaceItemInArray(tJSON * array, int which, tJSON * newitem);
	extern void tJSON_ReplaceItemInObject(tJSON * object, const char *string, tJSON * newitem);

/* Duplicate a tJSON item */
	extern tJSON *tJSON_Duplicate(tJSON * item, int recurse);
/* Duplicate will create a new, identical tJSON item to the one you pass, in new memory that will
need to be released. With recurse!=0, it will duplicate any children connected to the item.
The item->next and ->prev pointers are always zero on return from Duplicate. */

/* ParseWithOpts allows you to require (and check) that the JSON is null terminated, and to retrieve the pointer to the final byte parsed. */
	extern tJSON *tJSON_ParseWithOpts(const char *value, const char **return_parse_end, int require_null_terminated);

	extern void tJSON_Minify(char *json);

/* Macros for creating things quickly. */
#define tJSON_AddNullToObject(object,name)		tJSON_AddItemToObject(object, name, tJSON_CreateNull())
#define tJSON_AddTrueToObject(object,name)		tJSON_AddItemToObject(object, name, tJSON_CreateTrue())
#define tJSON_AddFalseToObject(object,name)		tJSON_AddItemToObject(object, name, tJSON_CreateFalse())
#define tJSON_AddBoolToObject(object,name,b)	tJSON_AddItemToObject(object, name, tJSON_CreateBool(b))
#define tJSON_AddNumberToObject(object,name,n)	tJSON_AddItemToObject(object, name, tJSON_CreateNumber(n))
#define tJSON_AddStringToObject(object,name,s)	tJSON_AddItemToObject(object, name, tJSON_CreateString(s))

/* When assigning an integer value, it needs to be propagated to valuedouble too. */
#define tJSON_SetIntValue(object,val)			((object)?(object)->valueint=(object)->valuedouble=(val):(val))

#ifdef __cplusplus
}
#endif
#endif
