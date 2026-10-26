/**
 * @file dijkstra.h
 * @brief Dijkstra's Algorithm with Min-Heap for Dynamic Smart City Routing.
 * 
 * Course: 21CSC201J - Data Structures and Algorithms
 * Institution: SRM Institute of Science and Technology, Ramapuram
 * Project: Smart Traffic & Emergency Route Optimization using Graphs & Dijkstra's Algorithm
 */

#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "city_graph.h"
#include "min_heap.h"
#include <stdbool.h>

/**
 * @brief Routing optimization objective metric.
 */
typedef enum {
    METRIC_DYNAMIC_TRAVEL_TIME = 0, /**< Shortest travel time using live IoT traffic */
    METRIC_PHYSICAL_DISTANCE,       /**< Shortest physical road distance in km */
    METRIC_FREE_FLOW_TIME           /**< Theoretical fastest time assuming no congestion */
} RouteMetric;

/**
 * @brief Stores shortest path trees and distances calculated by Dijkstra's algorithm.
 */
typedef struct DijkstraResult {
    int source;             /**< Origin vertex ID */
    int num_vertices;       /**< Number of vertices in graph */
    double *dist;           /**< Array of shortest distances/times from source */
    int *parent;            /**< Predecessor array for path reconstruction */
    bool is_emergency;      /**< Emergency vehicle priority flag */
    RouteMetric metric;     /**< Routing objective applied */
} DijkstraResult;

/**
 * @brief Executes Dijkstra's algorithm with Min-Heap priority queue.
 * @param g Pointer to CityGraph
 * @param src Source vertex ID
 * @param is_emergency Whether routing for an emergency vehicle (ambulance, fire engine, police)
 * @param metric Optimization objective (travel time vs distance)
 * @return Pointer to allocated DijkstraResult, or NULL on error.
 */
DijkstraResult* dijkstra_compute(const CityGraph *g, int src, bool is_emergency, RouteMetric metric);

/**
 * @brief Reconstructs the vertex path from source to destination.
 * @param res DijkstraResult pointer
 * @param dest Destination vertex ID
 * @param path_out Array to store sequence of vertex IDs
 * @param max_path_len Maximum capacity of path_out buffer
 * @param path_len_out Pointer to store actual count of vertices in path
 * @return true if valid path exists, false if unreachable.
 */
bool dijkstra_get_path(const DijkstraResult *res, int dest, int *path_out, int max_path_len, int *path_len_out);

/**
 * @brief Prints detailed turn-by-turn navigation instructions for a route.
 * @param g Pointer to CityGraph
 * @param res Pointer to DijkstraResult
 * @param dest Destination vertex ID
 */
void dijkstra_print_path_details(const CityGraph *g, const DijkstraResult *res, int dest);

/**
 * @brief Prints shortest path distances/times to all reachable vertices.
 * @param g Pointer to CityGraph
 * @param res Pointer to DijkstraResult
 */
void dijkstra_print_all_shortest_paths(const CityGraph *g, const DijkstraResult *res);

/**
 * @brief Finds the fastest emergency responder (e.g. nearest Hospital or Fire Station)
 *        to an incident location using Dijkstra's algorithm.
 * @param g Pointer to CityGraph
 * @param incident_vertex Vertex ID of the incident / patient location
 * @param responder_type Type of responder needed (e.g. VERTEX_HOSPITAL or VERTEX_FIRE_STATION)
 * @param best_responder_id Output pointer for chosen responder vertex ID
 * @param response_time_min Output pointer for estimated time of arrival in minutes
 * @return Pointer to DijkstraResult computed from the selected responder, or NULL if none reachable.
 */
DijkstraResult* dijkstra_find_nearest_emergency_responder(const CityGraph *g, int incident_vertex,
                                                          VertexType responder_type,
                                                          int *best_responder_id,
                                                          double *response_time_min);

/**
 * @brief Activates smart IoT "Green Corridor" signal preemption along a path.
 * @param g Pointer to CityGraph
 * @param path Array of vertex IDs along the route
 * @param path_len Number of vertices in the route
 */
void dijkstra_activate_green_corridor_along_path(CityGraph *g, const int *path, int path_len);

/**
 * @brief Deactivates all smart IoT Green Corridors across the entire network.
 * @param g Pointer to CityGraph
 */
void dijkstra_clear_green_corridors(CityGraph *g);

/**
 * @brief Deallocates memory used by DijkstraResult.
 * @param res Pointer to DijkstraResult
 */
void dijkstra_result_destroy(DijkstraResult *res);

#endif /* DIJKSTRA_H */
