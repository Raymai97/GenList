// GenList demo by Raymai97
//
// If `do_memcpy == false`, GenList stores the pointer you have passed.
// Usually this is for storing memory pointers that you've `malloc()`-ed,
// such as dynamic-allocated char array.
// Before calling `GenList_Destroy()`, you should `free()` every memory 
// pointer you've `malloc()`-ed.

// If `do_memcpy == true`, GenList copies the item you have added into a
// self-allocated memory with size of `item_size`. Usually this is for storing
// local variables, such as `int`, `double`, `char[]` and so on. When you call
// `GenList_Destroy()`, it will `free()` every memory it has allocated.

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "GenList.h"

#define INPUT_BUF_SIZE	(99)

char input[INPUT_BUF_SIZE];

bool GetInput() {
	// If !fgets, user entered EOF (CTRL+Z on Windows, CTRL+D on Linux)
	if (!fgets(input, INPUT_BUF_SIZE, stdin)) { return false; }
	// If got '\n' in input, set it as '\0',
	// else the input is too long.
	char *pNL = strchr(input, '\n');
	if (pNL) { *pNL = '\0'; }
	else {
		char c;
		while (c = fgetc(stdin) != '\n' && c != EOF);
	}
	return true;
}

void Test1() {
	// Let's say we want to store a series of char array with
	// size of INPUT_BUF_SIZE, we can set 'do_memcpy' to true
	// so GenList can take care the rest for us.
	printf("\nGenList Test1...\n");
	GENLIST list = NULL;
	GenList_Create(&list, sizeof(char) * INPUT_BUF_SIZE, true);
	printf("Enter string to append into GenList, or EOF to stop: \n");
	while (GetInput()) {
		GenList_Add(list, input);
	}
	size_t count = 0;
	GenList_GetInfo(list, &count, NULL, NULL);
	printf("\n");
	printf("Item count = %u \n", count);
	for (size_t i = 0; i < count; ++i) {
		char item[INPUT_BUF_SIZE] = {0};
		GenList_GetAt(list, i, (void**)item);
		printf("[%2u] %s \n", i, item);
	}
	GenList_Destroy(list);
}

bool ForEach_Free(int i, void *item, void *userdata) {
	free(item);
	return false;
}

void Test2() {
	// Sometimes you just need to store pointer, and manage
	// the memory by yourself. You can use GenList for this
	// by setting 'do_memcpy' to false.
	printf("\nGenList Test2...\n");
	GENLIST list = NULL;
	GenList_Create(&list, sizeof(void*), false);
	printf("Enter string to append into GenList, or EOF to stop: \n");
	while (GetInput()) {
		char *buf = malloc(strlen(input) + 1); // you malloc() them
		strcpy(buf, input);
		GenList_Add(list, buf);
	}
	size_t count = 0;
	GenList_GetInfo(list, &count, NULL, NULL);
	printf("Item count = %u \n", count);
	for (size_t i = 0; i < count; ++i) {
		char *item = NULL;
		GenList_GetAt(list, i, (void**)&item);
		printf("[%2u] %s\n", i, item);
	}
	GenList_ForEach(list, ForEach_Free, NULL); // you free() them
	GenList_Destroy(list);
}

void Test3() {
	printf("\nGenList Test3...\n");
	char buf[10240] = {0};
	GENLIST list;
	GenList_Create(&list, 10240, true);
	printf("Adding 10000 items into GenList, each 10KB... ");
	for (size_t i = 0; i < 10000; ++i) {
		GenList_Add(list, buf);
	}
	printf("OK!\n");
	printf("Press ENTER to destroy.\n");
	GetInput();
	GenList_Destroy(list);
	printf("Done...\n");
}

void Test4() {
	printf("\nGenList Test4...\n");
	char buf[10240] = {0};
	GENLIST list;
	GenList_Create(&list, sizeof(void*), false);
	printf("Adding 10000 items into GenList, each 10KB... ");
	for (size_t i = 0; i < 10000; ++i) {
		char *mem = malloc(10240); // you malloc() them
		memcpy(mem, buf, 10240);
		GenList_Add(list, (void**)mem);
	}
	printf("OK!\n");
	printf("Press ENTER to destroy.\n");
	GetInput();
	GenList_ForEach(list, ForEach_Free, NULL); // you free() them
	GenList_Destroy(list);
	printf("Done...\n");
}

void Test5_PrintList(GENLIST list) {
	size_t num = 0, count = 0;
	GenList_GetInfo(list, &count, NULL, NULL);
	for (size_t i = 0; i < count; ++i) {
		GenList_GetAt(list, i, (void**)&num);
		printf("[%2i] %i \n", i, num);
	}
}

void Test5() {
	printf("\nGenList Test5...\n");
	GENLIST list;
	GenList_Create(&list, sizeof(int), true);
	int num = 0;
	num = 100;	GenList_Add(list, &num);
	num = 101;	GenList_Add(list, &num);
	num = 102;	GenList_Add(list, &num);
	printf("Before AddAt: \n");
	Test5_PrintList(list);
	GetInput();
	num = 203;	GenList_AddAt(list, 3, &num);
	num = 200;	GenList_AddAt(list, 0, &num);
	num = 202;	GenList_AddAt(list, 2, &num);
	printf("After AddAt: \n");
	Test5_PrintList(list);
	GetInput();
	GenList_Destroy(list);
}

int main() {
	while (true) {
		printf("\nWelcome to GenList Demo\n");
		printf("[1] Test 1 (do_memcpy = true) \n");
		printf("[2] Test 2 (do_memcpy = false) \n");
		printf("[3] Test 3 (do_memcpy = true, 10KB per item) \n");
		printf("[4] Test 4 (do_memcpy = false, 10KB per item) \n");
		printf("[5] Test 5 (do_memcpy = true, AddAt...) \n");
		printf("To exit, enter EOF by Ctrl+Z (Windows) or Ctrl+D (Linux).\n");
		printf("Enter: ");
		if (GetInput()) {
			if (strcmp(input, "1") == 0) { Test1(); }
			if (strcmp(input, "2") == 0) { Test2(); }
			if (strcmp(input, "3") == 0) { Test3(); }
			if (strcmp(input, "4") == 0) { Test4(); }
			if (strcmp(input, "5") == 0) { Test5(); }
		}
		else { break; }
	}
	return 0;
}
