# GenList
A simple cross-platform Generic List implemented using C99 code. Check `demo.c` for sample usage.

### The APIs:
```c
typedef void* GENLIST;
typedef bool (*GENLIST_FOREACH_HANDLER)(int, void*); // return true to 'break'

bool GenList_Create(GENLIST*, size_t item_size, bool do_memcpy);
bool GenList_Destroy(GENLIST);

bool GenList_Add(GENLIST, void *item);
bool GenList_AddAt(GENLIST, size_t i, void *item);
bool GenList_GetAt(GENLIST, size_t i, void **item);
bool GenList_RemoveAt(GENLIST, size_t i);
bool GenList_Clear(GENLIST);

// it's OK to pass NULL like this:
// ...GetInfo(list, &count, NULL, NULL);
bool GenList_GetInfo(GENLIST, size_t *count, size_t *item_size, bool *do_memcpy);
bool GenList_ForEach(GENLIST, GENLIST_FOREACH_HANDLER);
int GenList_LastErr();
```


### About `do_memcpy`

If **`do_memcpy == false`**, GenList stores the pointer you have passed. Usually this is for **storing memory pointers that you've `malloc()`-ed**, such as dynamic-allocated char array. Before calling `GenList_Destroy()`, you should `free()` every memory pointer you've `malloc()`-ed.

If **`do_memcpy == true`**, GenList copies the item you have added into a self-allocated memory with size of `item_size`. Usually this is for **storing local variables**, such as `int`, `double`, `char[]` and so on. When you call `GenList_Destroy()`, it will  `free()` every memory it has allocated.
