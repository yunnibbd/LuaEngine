#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define HASH_AREA 1024

struct HashNode {
	int64_t key;
	void* value;
	struct HashNode* next;
};

typedef struct StructCHashIntMap {
	struct HashNode* hash_set[HASH_AREA];
} *CHashIntMap;

CHashIntMap CHashIntMapAlloc() {
	CHashIntMap hash_map = malloc(sizeof(struct StructCHashIntMap));
	memset(hash_map, 0, sizeof(struct StructCHashIntMap));

	return hash_map;
}

void CHashIntMapFree(CHashIntMap hash_map) {
	for (int i = 0; i < HASH_AREA; ++i) {
		struct HashNode** walk = &hash_map->hash_set[i];
		while (*walk) {
			struct HashNode* node = *walk;
			*walk = node->next;
			node->next = NULL;
			free(node);
		}
	}
	free(hash_map);
}

void CHashIntMapSet(CHashIntMap hash_map, int64_t key, void* value) {
	int index = key % HASH_AREA;
	struct HashNode** walk = &hash_map->hash_set[index];
	while (*walk) {
		if ((*walk)->key == key) {
			(*walk)->value = value;
			return;
		}
		walk = &((*walk)->next);
	}

	struct HashNode* node = malloc(sizeof(struct HashNode));
	node->key = key;
	node->value = value;
	node->next = NULL;
	*walk = node;
}

void CHashIntMapRemoveByKey(CHashIntMap hash_map, int64_t key) {
	int index = key % HASH_AREA;
	struct HashNode** walk = &hash_map->hash_set[index];
	while (*walk) {
		if ((*walk)->key == key) {
			struct HashNode* node = (*walk);
			*walk = node->next;
			node->next = NULL;
			free(node);
			return;
		}
		walk = &((*walk)->next);
	}
}

void CHashIntMapRemoveByValue(CHashIntMap hash_map, void* value) {
	for (int i = 0; i < HASH_AREA; ++i) {
		struct HashNode** walk = &hash_map->hash_set[i];
		while (*walk) {
			if ((*walk)->value == value) {
				struct HashNode* node = *walk;
				*walk = node->next;
				node->next = NULL;
				free(node);
			}
			else {
				walk = &((*walk)->next);
			}
		}
	}
}

void* CHashIntMapGet(CHashIntMap hash_map, int64_t key) {
	int index = key % HASH_AREA;
	struct HashNode** walk = &hash_map->hash_set[index];
	while (*walk) {
		if ((*walk)->key == key) {
			return (*walk)->value;
		}
		walk = &((*walk)->next);
	}

	return NULL;
}
