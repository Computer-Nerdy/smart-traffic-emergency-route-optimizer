/**
 * @file dijkstra.c
 * @brief Implementation of Dijkstra's algorithm with Min-Heap for smart city routing.
 * 
 * Course: 21CSC201J - Data Structures and Algorithms
 * Institution: SRM Institute of Science and Technology, Ramapuram
 */

#include "dijkstra.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

static double get_edge_weight(const RoadEdge *edge, bool is_emergency, RouteMetric metric) {
    if (!edge) return INF_TIME;

    if (edge->incident == INCIDENT_ROAD_CLOSURE) {
        return INF_TIME;
    }

    double weight;
    switch (metric) {
        case METRIC_DYNAMIC_TRAVEL_TIME:
            weight = city_graph_calculate_travel_time(edge, is_emergency);
            break;

        case METRIC_PHYSICAL_DISTANCE:
            weight = edge->distance_km;
            break;

        case METRIC_FREE_FLOW_TIME:
            weight = (edge->distance_km / edge->speed_limit_kmh) * 60.0;
            break;

        default:
            weight = city_graph_calculate_travel_time(edge, is_emergency);
            break;
    }

    /* Dijkstra algorithm strictly requires non-negative edge costs */
    if (weight < 0.0) {
        weight = 0.0;
    }
    return weight;
}

DijkstraResult* dijkstra_compute(const CityGraph *g, int src, bool is_emergency, RouteMetric metric) {
    if (!g || src < 0 || src >= g->num_vertices || g->vertices[src].id == -1) {
        return NULL;
    }

    DijkstraResult *res = (DijkstraResult*)malloc(sizeof(DijkstraResult));
    if (!res) return NULL;

    res->source = src;
    res->num_vertices = g->num_vertices;
    res->is_emergency = is_emergency;
    res->metric = metric;

    res->dist = (double*)malloc(g->num_vertices * sizeof(double));
    res->parent = (int*)malloc(g->num_vertices * sizeof(int));
    bool *visited = (bool*)calloc(g->num_vertices, sizeof(bool));

    if (!res->dist || !res->parent || !visited) {
        if (res->dist) free(res->dist);
        if (res->parent) free(res->parent);
        if (visited) free(visited);
        free(res);
        return NULL;
    }

    for (int i = 0; i < g->num_vertices; ++i) {
        res->dist[i] = INF_TIME;
        res->parent[i] = -1;
    }

    MinHeap *heap = min_heap_create(g->num_vertices);
    if (!heap) {
        free(res->dist);
        free(res->parent);
        free(visited);
        free(res);
        return NULL;
    }

    /* Start at source */
    res->dist[src] = 0.0;
    min_heap_insert(heap, src, 0.0);

    while (!min_heap_is_empty(heap)) {
        MinHeapNode min_node = min_heap_extract_min(heap);
        int u = min_node.vertex;
        double current_dist = min_node.distance;

        if (u < 0 || u >= g->num_vertices || visited[u]) {
            continue;
        }
        visited[u] = true;

        if (current_dist >= INF_TIME / 2.0) {
            break; /* Remaining vertices unreachable */
        }

        /* Relax all outgoing edges from u */
        RoadEdge *edge = g->vertices[u].head;
        while (edge) {
            int v = edge->dest;
            if (v < 0 || v >= g->num_vertices || visited[v]) {
                edge = edge->next;
                continue;
            }

            double weight = get_edge_weight(edge, is_emergency, metric);

            if (weight < INF_TIME / 2.0 && res->dist[u] + weight < res->dist[v]) {
                res->dist[v] = res->dist[u] + weight;
                res->parent[v] = u;

                if (min_heap_contains(heap, v)) {
                    min_heap_decrease_key(heap, v, res->dist[v]);
                } else {
                    min_heap_insert(heap, v, res->dist[v]);
                }
            }
            edge = edge->next;
        }
    }

    free(visited);
    min_heap_destroy(heap);
    return res;
}

bool dijkstra_get_path(const DijkstraResult *res, int dest, int *path_out, int max_path_len, int *path_len_out) {
    if (!res || !path_out || !path_len_out || max_path_len <= 0) return false;
    if (dest < 0 || dest >= res->num_vertices) return false;

    if (dest == res->source) {
        path_out[0] = dest;
        *path_len_out = 1;
        return true;
    }

    if (res->dist[dest] >= INF_TIME / 2.0 || res->parent[dest] == -1) {
        *path_len_out = 0;
        return false; /* Unreachable */
    }

    /* Allocate dynamic temp buffer sized to vertex count */
    int *temp = (int*)malloc(res->num_vertices * sizeof(int));
    if (!temp) return false;

    int count = 0;
    int curr = dest;

    while (curr != -1 && count < res->num_vertices) {
        temp[count++] = curr;
        if (curr == res->source) break;
        curr = res->parent[curr];
    }

    /* Verify path successfully reached source */
    if (count == 0 || temp[count - 1] != res->source || count > max_path_len) {
        free(temp);
        *path_len_out = 0;
        return false;
    }

    /* Reverse path into path_out */
    for (int i = 0; i < count; ++i) {
        path_out[i] = temp[count - 1 - i];
    }
    *path_len_out = count;
    free(temp);
    return true;
}

void dijkstra_print_path_details(const CityGraph *g, const DijkstraResult *res, int dest) {
    if (!g || !res) return;
    if (dest < 0 || dest >= g->num_vertices) {
        printf("Error: Destination ID %d is out of range.\n", dest);
        return;
    }

    int path[512];
    int path_len = 0;

    printf("\n----------------------------------------------------------------------------------------\n");
    printf("ROUTING MISSION: [%s] -> [%s]\n",
           g->vertices[res->source].name, g->vertices[dest].name);
    printf("Priority Mode: %s | Optimization Metric: %s\n",
           res->is_emergency ? "EMERGENCY (SIRENS & SIGNAL PREEMPTION)" : "CIVILIAN VEHICLE",
           res->metric == METRIC_DYNAMIC_TRAVEL_TIME ? "Dynamic Real-Time IoT Travel Time" :
           (res->metric == METRIC_PHYSICAL_DISTANCE ? "Shortest Physical Distance" : "Free-Flow Time"));
    printf("----------------------------------------------------------------------------------------\n");

    if (!dijkstra_get_path(res, dest, path, 512, &path_len)) {
        printf("[!] NO VIABLE ROUTE FOUND: Destination is currently disconnected or blocked by road closures.\n");
        printf("----------------------------------------------------------------------------------------\n\n");
        return;
    }

    double total_distance_km = 0.0;
    double total_travel_time_min = 0.0;

    printf("\nTURN-BY-TURN NAVIGATION AND SENSOR TELEMETRY:\n");
    for (int i = 0; i < path_len - 1; ++i) {
        int u = path[i];
        int v = path[i + 1];
        RoadEdge *edge = city_graph_get_edge(g, u, v);
        if (!edge) continue;

        double seg_time = city_graph_calculate_travel_time(edge, res->is_emergency);
        total_distance_km += edge->distance_km;
        total_travel_time_min += seg_time;

        printf("  Step %2d: [%s] ──► [%s]\n", i + 1, g->vertices[u].name, g->vertices[v].name);
        printf("           Via: %-22s | Dist: %.2f km | Speed Limit: %.0f km/h | Lanes: %d\n",
               edge->road_name, edge->distance_km, edge->speed_limit_kmh, edge->lanes);
        printf("           IoT Congestion: %4.1fx | Avg Speed: %4.1f km/h | Incident: %s\n",
               edge->congestion_factor, edge->sensor_avg_speed, incident_type_to_string(edge->incident));
        printf("           Segment Travel Time: %.2f mins | Green Corridor: %s\n",
               seg_time, edge->emergency_corridor ? "ACTIVE (Preempted Signals)" : "STANDARD");
        printf("           --------------------------------------------------------------------\n");
    }

    printf("\nSUMMARY OF COMPUTED OPTIMAL ROUTE:\n");
    printf("  ► Path Sequence: ");
    for (int i = 0; i < path_len; ++i) {
        printf("%s%s", g->vertices[path[i]].name, (i < path_len - 1) ? " -> " : "\n");
    }
    printf("  ► Total Road Distance    : %.2f km\n", total_distance_km);
    printf("  ► Estimated Travel Time  : %.2f minutes (%.1f seconds)\n",
           total_travel_time_min, total_travel_time_min * 60.0);
    if (total_travel_time_min > 0.001) {
        printf("  ► Average Journey Speed  : %.2f km/h\n", (total_distance_km / (total_travel_time_min / 60.0)));
    }
    printf("----------------------------------------------------------------------------------------\n\n");
}

void dijkstra_print_all_shortest_paths(const CityGraph *g, const DijkstraResult *res) {
    if (!g || !res) return;

    printf("\n========================================================================================\n");
    printf("            ALL SHORTEST PATHS FROM ORIGIN: [%-25s]           \n", g->vertices[res->source].name);
    printf("========================================================================================\n");
    printf("%-4s %-28s %-16s %-10s %s\n",
           "ID", "Destination Landmark", "Cost (Min/Km)", "Status", "Full Predecessor Path");
    printf("----------------------------------------------------------------------------------------\n");

    int path[512];
    int path_len = 0;

    for (int i = 0; i < g->num_vertices; ++i) {
        if (g->vertices[i].id == -1) continue;

        if (dijkstra_get_path(res, i, path, 512, &path_len)) {
            char metric_unit[16];
            if (res->metric == METRIC_PHYSICAL_DISTANCE) {
                snprintf(metric_unit, sizeof(metric_unit), "%.2f km", res->dist[i]);
            } else {
                snprintf(metric_unit, sizeof(metric_unit), "%.2f min", res->dist[i]);
            }

            printf("[%2d] %-28s %-16s %-10s ",
                   i, g->vertices[i].name, metric_unit, "REACHABLE");

            for (int k = 0; k < path_len; ++k) {
                printf("%s%s", g->vertices[path[k]].name, (k < path_len - 1) ? " -> " : "");
            }
            printf("\n");
        } else {
            printf("[%2d] %-28s %-16s %-10s (No viable path)\n",
                   i, g->vertices[i].name, "INF", "BLOCKED");
        }
    }
    printf("========================================================================================\n\n");
}

DijkstraResult* dijkstra_find_nearest_emergency_responder(const CityGraph *g, int incident_vertex,
                                                          VertexType responder_type,
                                                          int *best_responder_id,
                                                          double *response_time_min) {
    if (!g || incident_vertex < 0 || incident_vertex >= g->num_vertices || g->vertices[incident_vertex].id == -1) {
        return NULL;
    }

    int best_id = -1;
    double min_eta = INF_TIME;
    DijkstraResult *best_res = NULL;

    for (int i = 0; i < g->num_vertices; ++i) {
        if (g->vertices[i].id != -1 && g->vertices[i].type == responder_type) {
            DijkstraResult *res = dijkstra_compute(g, i, true, METRIC_DYNAMIC_TRAVEL_TIME);
            if (res) {
                double eta = res->dist[incident_vertex];
                if (eta < min_eta) {
                    min_eta = eta;
                    best_id = i;
                    if (best_res) {
                        dijkstra_result_destroy(best_res);
                    }
                    best_res = res;
                } else {
                    dijkstra_result_destroy(res);
                }
            }
        }
    }

    if (best_id != -1 && min_eta < INF_TIME / 2.0) {
        if (best_responder_id) *best_responder_id = best_id;
        if (response_time_min) *response_time_min = min_eta;
        return best_res;
    }

    if (best_res) {
        dijkstra_result_destroy(best_res);
    }
    return NULL;
}

void dijkstra_activate_green_corridor_along_path(CityGraph *g, const int *path, int path_len) {
    if (!g || !path || path_len < 2) return;

    for (int i = 0; i < path_len - 1; ++i) {
        RoadEdge *edge = city_graph_get_edge(g, path[i], path[i + 1]);
        if (edge) {
            edge->emergency_corridor = true;
        }
    }
}

void dijkstra_clear_green_corridors(CityGraph *g) {
    if (!g) return;

    for (int i = 0; i < g->num_vertices; ++i) {
        RoadEdge *edge = g->vertices[i].head;
        while (edge) {
            edge->emergency_corridor = false;
            edge = edge->next;
        }
    }
}

void dijkstra_result_destroy(DijkstraResult *res) {
    if (!res) return;
    if (res->dist) free(res->dist);
    if (res->parent) free(res->parent);
    free(res);
}
