/**
 * @file min_heap.h
 * @brief Min-Heap Priority Queue implementation for Dijkstra's Algorithm.
 * 
 * Course: 21CSC201J - Data Structures and Algorithms
 * Institution: SRM Institute of Science and Technology, Ramapuram
 * Project: Smart Traffic & Emergency Route Optimization using Graphs & Dijkstra's Algorithm
 */

#ifndef MIN_HEAP_H
#define MIN_HEAP_H

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief Structure representing an element in the Min-Heap.
 */
typedef struct MinHeapNode {
    int vertex;         /**< Vertex / Intersection index */
    double distance;    /**< Current shortest travel time / weight from source */
} MinHeapNode;

/**
 * @brief Indexed Min-Heap structure for O(log V) priority queue operations.
 */
typedef struct MinHeap {
    int capacity;       /**< Total capacity of the heap */
    int size;           /**< Current number of elements in the heap */
    int *pos;           /**< Position map: pos[v] stores the heap array index of vertex v */
    MinHeapNode **array;/**< Array of pointers to heap nodes */
} MinHeap;

/**
 * @brief Creates and initializes an indexed Min-Heap of given capacity.
 * @param capacity Maximum number of vertices (V)
 * @return Pointer to the allocated MinHeap, or NULL on allocation failure.
 */
MinHeap* min_heap_create(int capacity);

/**
 * @brief Checks whether the min-heap is empty.
 * @param min_heap Pointer to MinHeap
 * @return true if size == 0, false otherwise.
 */
bool min_heap_is_empty(const MinHeap *min_heap);

/**
 * @brief Checks if a given vertex is currently present in the Min-Heap.
 * @param min_heap Pointer to MinHeap
 * @param v Vertex index
 * @return true if vertex v is in the heap, false otherwise.
 */
bool min_heap_contains(const MinHeap *min_heap, int v);

/**
 * @brief Extracts the node with the minimum distance (root of min-heap).
 * @param min_heap Pointer to MinHeap
 * @return MinHeapNode containing vertex and its minimum distance.
 */
MinHeapNode min_heap_extract_min(MinHeap *min_heap);

/**
 * @brief Decreases the distance key of a vertex in the min-heap and restores heap property.
 * @param min_heap Pointer to MinHeap
 * @param v Vertex index whose distance is being updated
 * @param dist New decreased distance
 */
void min_heap_decrease_key(MinHeap *min_heap, int v, double dist);

/**
 * @brief Inserts or initializes a vertex into the min-heap.
 * @param min_heap Pointer to MinHeap
 * @param v Vertex index
 * @param dist Initial distance
 */
void min_heap_insert(MinHeap *min_heap, int v, double dist);

/**
 * @brief Frees all memory allocated for the Min-Heap.
 * @param min_heap Pointer to MinHeap
 */
void min_heap_destroy(MinHeap *min_heap);

#endif /* MIN_HEAP_H */
