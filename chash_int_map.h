#ifndef __CHASH_INT_MAP_H__
#define __CHASH_INT_MAP_H__
#include <inttypes.h>

typedef void* CHashIntMap;

CHashIntMap CHashIntMapAlloc();

void CHashIntMapFree(CHashIntMap hash_map);

void CHashIntMapSet(CHashIntMap hash_map, int64_t key, void* value);

void CHashIntMapRemoveByKey(CHashIntMap hash_map, int64_t key);

void CHashIntMapRemoveByValue(CHashIntMap hash_map, void* value);

void* CHashIntMapGet(CHashIntMap hash_map, int64_t key);

#endif
