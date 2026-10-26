/**
 * @file min_heap.c
 * @brief Implementation of Indexed Min-Heap Priority Queue.
 *
 * Course: 21CSC201J - Data Structures and Algorithms
 * SRM Institute of Science and Technology, Ramapuram
 */

#include "min_heap.h"
#include <stdio.h>
#include <stdlib.h>

static MinHeapNode* create_node(int v, double dist) {
    MinHeapNode* node = (MinHeapNode*)malloc(sizeof(MinHeapNode));
    if (!node) return NULL;
    node->vertex = v;
    node->distance = dist;
    return node;
}

MinHeap* min_heap_create(int capacity) {
    if (capacity <= 0) return NULL;

    MinHeap* min_heap = (MinHeap*)malloc(sizeof(MinHeap));
    if (!min_heap) return NULL;

    min_heap->capacity = capacity;
    min_heap->size = 0;

    min_heap->pos = (int*)malloc(capacity * sizeof(int));
    if (!min_heap->pos) {
        free(min_heap);
        return NULL;
    }

    min_heap->array = (MinHeapNode**)malloc(capacity * sizeof(MinHeapNode*));
    if (!min_heap->array) {
        free(min_heap->pos);
        free(min_heap);
        return NULL;
    }

    for (int i = 0; i < capacity; ++i) {
        min_heap->pos[i] = -1;
        min_heap->array[i] = NULL;
    }

    return min_heap;
}

bool min_heap_is_empty(const MinHeap *min_heap) {
    return (!min_heap || min_heap->size == 0);
}

bool min_heap_contains(const MinHeap *min_heap, int v) {
    if (!min_heap || v < 0 || v >= min_heap->capacity) return false;
    int p = min_heap->pos[v];
    return (p >= 0 && p < min_heap->size && min_heap->array[p] != NULL && min_heap->array[p]->vertex == v);
}

static void swap_nodes(MinHeap *min_heap, int i, int j) {
    MinHeapNode *temp = min_heap->array[i];
    min_heap->array[i] = min_heap->array[j];
    min_heap->array[j] = temp;

    /* Update position map */
    min_heap->pos[min_heap->array[i]->vertex] = i;
    min_heap->pos[min_heap->array[j]->vertex] = j;
}

static void min_heapify(MinHeap *min_heap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < min_heap->size && 
        min_heap->array[left]->distance < min_heap->array[smallest]->distance) {
        smallest = left;
    }

    if (right < min_heap->size && 
        min_heap->array[right]->distance < min_heap->array[smallest]->distance) {
        smallest = right;
    }

    if (smallest != idx) {
        swap_nodes(min_heap, idx, smallest);
        min_heapify(min_heap, smallest);
    }
}

MinHeapNode min_heap_extract_min(MinHeap *min_heap) {
    MinHeapNode empty = { -1, -1.0 };
    if (min_heap_is_empty(min_heap)) return empty;

    MinHeapNode *root = min_heap->array[0];
    MinHeapNode result = *root;

    /* Pos map for extracted vertex must always become -1 */
    min_heap->pos[root->vertex] = -1;

    if (min_heap->size > 1) {
        /* Move the last node to root position */
        MinHeapNode *last_node = min_heap->array[min_heap->size - 1];
        min_heap->array[0] = last_node;
        min_heap->pos[last_node->vertex] = 0;
        min_heap->array[min_heap->size - 1] = NULL;
        min_heap->size--;
        min_heapify(min_heap, 0);
    } else {
        min_heap->array[0] = NULL;
        min_heap->size = 0;
    }

    free(root);
    return result;
}

void min_heap_decrease_key(MinHeap *min_heap, int v, double dist) {
    if (!min_heap || v < 0 || v >= min_heap->capacity) return;

    int i = min_heap->pos[v];
    if (i < 0 || i >= min_heap->size || !min_heap->array[i]) return;

    /* Priority queue decrease-key only allows equal or smaller distances */
    if (dist > min_heap->array[i]->distance) return;

    min_heap->array[i]->distance = dist;

    /* Bubble up while parent distance is greater */
    while (i > 0 && min_heap->array[i]->distance < min_heap->array[(i - 1) / 2]->distance) {
        swap_nodes(min_heap, i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}

void min_heap_insert(MinHeap *min_heap, int v, double dist) {
    if (!min_heap || v < 0 || v >= min_heap->capacity) return;
    if (min_heap->size >= min_heap->capacity) return;

    /* If already exists, just decrease key */
    if (min_heap_contains(min_heap, v)) {
        if (dist < min_heap->array[min_heap->pos[v]]->distance) {
            min_heap_decrease_key(min_heap, v, dist);
        }
        return;
    }

    int i = min_heap->size;
    min_heap->array[i] = create_node(v, dist);
    if (!min_heap->array[i]) return;

    min_heap->pos[v] = i;
    min_heap->size++;

    /* Bubble up */
    while (i > 0 && min_heap->array[i]->distance < min_heap->array[(i - 1) / 2]->distance) {
        swap_nodes(min_heap, i, (i - 1) / 2);
        i = (i - 1) / 2;
    }
}

void min_heap_destroy(MinHeap *min_heap) {
    if (!min_heap) return;

    for (int i = 0; i < min_heap->capacity; ++i) {
        if (min_heap->array[i]) {
            free(min_heap->array[i]);
            min_heap->array[i] = NULL;
        }
    }

    free(min_heap->array);
    free(min_heap->pos);
    free(min_heap);
}
