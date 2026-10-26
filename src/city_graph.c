/**
 * @file city_graph.c
 * @brief Implementation of City Road Network Graph with IoT Traffic Telemetry.
 * 
 * Course: 21CSC201J - Data Structures and Algorithms
 * Institution: SRM Institute of Science and Technology, Ramapuram
 */

#include "city_graph.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const char* vertex_type_to_string(VertexType type) {
    switch (type) {
        case VERTEX_INTERSECTION:    return "Intersection";
        case VERTEX_HOSPITAL:        return "Hospital / Trauma Center";
        case VERTEX_FIRE_STATION:    return "Fire Station";
        case VERTEX_POLICE_STATION:  return "Police Station";
        case VERTEX_TRANSIT_HUB:     return "Transit Hub";
        case VERTEX_INCIDENT_ZONE:   return "Incident Zone";
        default:                     return "Unknown";
    }
}

const char* incident_type_to_string(IncidentType incident) {
    switch (incident) {
        case INCIDENT_NONE:         return "Clear / Normal";
        case INCIDENT_CONGESTION:   return "Traffic Congestion";
        case INCIDENT_ACCIDENT:     return "Vehicle Collision";
        case INCIDENT_ROADWORK:     return "Construction / Roadwork";
        case INCIDENT_WATERLOGGING: return "Monsoon Waterlogging";
        case INCIDENT_ROAD_CLOSURE: return "Road Blocked / Closed";
        default:                    return "Unknown Incident";
    }
}

const char* road_tier_to_string(RoadTier tier) {
    switch (tier) {
        case ROAD_EXPRESSWAY: return "Expressway / Flyover";
        case ROAD_ARTERIAL:   return "Arterial Road";
        case ROAD_COLLECTOR:  return "Collector Road";
        case ROAD_LOCAL:      return "Local Street";
        default:              return "Road";
    }
}

CityGraph* city_graph_create(int capacity) {
    if (capacity <= 0) return NULL;

    CityGraph *g = (CityGraph*)malloc(sizeof(CityGraph));
    if (!g) return NULL;

    g->capacity = capacity;
    g->num_vertices = 0;
    g->num_edges = 0;

    g->vertices = (Vertex*)malloc(capacity * sizeof(Vertex));
    if (!g->vertices) {
        free(g);
        return NULL;
    }

    for (int i = 0; i < capacity; ++i) {
        g->vertices[i].id = -1;
        g->vertices[i].name[0] = '\0';
        g->vertices[i].type = VERTEX_INTERSECTION;
        g->vertices[i].coord_x = 0.0;
        g->vertices[i].coord_y = 0.0;
        g->vertices[i].head = NULL;
    }

    return g;
}

bool city_graph_add_vertex(CityGraph *g, int id, const char *name, VertexType type, double x, double y) {
    if (!g || id < 0 || !name) return false;

    /* Dynamically grow vertex capacity if needed */
    if (id >= g->capacity) {
        int new_cap = (id >= g->capacity * 2) ? id + 16 : g->capacity * 2;
        Vertex *new_vertices = (Vertex*)realloc(g->vertices, new_cap * sizeof(Vertex));
        if (!new_vertices) return false;

        for (int i = g->capacity; i < new_cap; ++i) {
            new_vertices[i].id = -1;
            new_vertices[i].name[0] = '\0';
            new_vertices[i].type = VERTEX_INTERSECTION;
            new_vertices[i].coord_x = 0.0;
            new_vertices[i].coord_y = 0.0;
            new_vertices[i].head = NULL;
        }

        g->vertices = new_vertices;
        g->capacity = new_cap;
    }

    /* Check if already registered */
    if (g->vertices[id].id != -1) {
        return false;
    }

    g->vertices[id].id = id;
    strncpy(g->vertices[id].name, name, MAX_NAME_LEN - 1);
    g->vertices[id].name[MAX_NAME_LEN - 1] = '\0';
    g->vertices[id].type = type;
    g->vertices[id].coord_x = x;
    g->vertices[id].coord_y = y;
    g->vertices[id].head = NULL;

    if (id >= g->num_vertices) {
        g->num_vertices = id + 1;
    }

    return true;
}

int city_graph_find_vertex_by_name(const CityGraph *g, const char *name) {
    if (!g || !name) return -1;
    for (int i = 0; i < g->num_vertices; ++i) {
        if (g->vertices[i].id != -1 && strcmp(g->vertices[i].name, name) == 0) {
            return i;
        }
    }
    return -1;
}

RoadEdge* city_graph_get_edge(const CityGraph *g, int src, int dest) {
    if (!g || src < 0 || src >= g->num_vertices || dest < 0 || dest >= g->num_vertices) {
        return NULL;
    }

    RoadEdge *curr = g->vertices[src].head;
    while (curr) {
        if (curr->dest == dest) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

static bool add_single_directed_edge(CityGraph *g, int src, int dest, const char *road_name,
                                     double distance_km, double speed_limit_kmh, int lanes,
                                     RoadTier tier) {
    /* Check if edge already exists */
    RoadEdge *existing = city_graph_get_edge(g, src, dest);
    if (existing) {
        /* Update parameters instead of duplicating */
        strncpy(existing->road_name, road_name, MAX_NAME_LEN - 1);
        existing->road_name[MAX_NAME_LEN - 1] = '\0';
        existing->distance_km = distance_km;
        existing->speed_limit_kmh = speed_limit_kmh;
        existing->lanes = lanes;
        existing->tier = tier;
        return true;
    }

    RoadEdge *edge = (RoadEdge*)malloc(sizeof(RoadEdge));
    if (!edge) return false;

    edge->dest = dest;
    strncpy(edge->road_name, road_name, MAX_NAME_LEN - 1);
    edge->road_name[MAX_NAME_LEN - 1] = '\0';
    edge->distance_km = distance_km;
    edge->speed_limit_kmh = speed_limit_kmh;
    edge->lanes = lanes;
    edge->tier = tier;
    edge->congestion_factor = 1.0;
    edge->incident = INCIDENT_NONE;
    edge->emergency_corridor = false;
    edge->sensor_avg_speed = speed_limit_kmh;
    edge->vehicle_count = 15 * lanes;

    edge->next = g->vertices[src].head;
    g->vertices[src].head = edge;
    g->num_edges++;

    return true;
}

bool city_graph_add_road(CityGraph *g, int src, int dest, const char *road_name,
                         double distance_km, double speed_limit_kmh, int lanes,
                         RoadTier tier, bool bidirectional) {
    if (!g || !road_name) return false;
    if (src < 0 || src >= g->num_vertices || dest < 0 || dest >= g->num_vertices) return false;
    if (g->vertices[src].id == -1 || g->vertices[dest].id == -1) return false;
    if (distance_km <= 0.0 || speed_limit_kmh <= 0.0 || lanes <= 0) return false;

    bool ok1 = add_single_directed_edge(g, src, dest, road_name, distance_km, speed_limit_kmh, lanes, tier);
    if (!ok1) return false;

    if (bidirectional) {
        bool ok2 = add_single_directed_edge(g, dest, src, road_name, distance_km, speed_limit_kmh, lanes, tier);
        if (!ok2) return false;
    }

    return true;
}

bool city_graph_update_iot_sensor(CityGraph *g, int src, int dest,
                                  double congestion_factor, IncidentType incident,
                                  bool emergency_corridor, bool bidirectional) {
    if (!g) return false;
    if (src < 0 || src >= g->num_vertices || dest < 0 || dest >= g->num_vertices) return false;
    if (g->vertices[src].id == -1 || g->vertices[dest].id == -1) return false;

    RoadEdge *edge1 = city_graph_get_edge(g, src, dest);
    if (!edge1) return false;

    if (congestion_factor < 1.0) congestion_factor = 1.0;
    edge1->congestion_factor = congestion_factor;
    edge1->incident = incident;
    edge1->emergency_corridor = emergency_corridor;
    edge1->sensor_avg_speed = edge1->speed_limit_kmh / congestion_factor;
    edge1->vehicle_count = (int)(25.0 * edge1->lanes * congestion_factor);

    if (bidirectional) {
        RoadEdge *edge2 = city_graph_get_edge(g, dest, src);
        if (edge2) {
            edge2->congestion_factor = congestion_factor;
            edge2->incident = incident;
            edge2->emergency_corridor = emergency_corridor;
            edge2->sensor_avg_speed = edge2->speed_limit_kmh / congestion_factor;
            edge2->vehicle_count = edge1->vehicle_count;
        }
    }

    return true;
}

double city_graph_calculate_travel_time(const RoadEdge *edge, bool is_emergency_vehicle) {
    if (!edge) return INF_TIME;

    /* Base free-flow travel time in minutes: (distance / speed) * 60 */
    double base_time = (edge->distance_km / edge->speed_limit_kmh) * 60.0;

    /* If the road is completely closed due to accident or blockage */
    if (edge->incident == INCIDENT_ROAD_CLOSURE) {
        return INF_TIME;
    }

    if (is_emergency_vehicle) {
        double travel_time;
        if (edge->emergency_corridor) {
            /* Active Green Corridor: Traffic signals synchronized green, traffic cleared */
            travel_time = base_time * 0.85; /* Travel time improved past free-flow baseline */
        } else {
            /* Standard emergency siren response through congestion */
            /* Siren clears partial traffic; congestion penalty scaled down by 65% */
            double emergency_factor = 1.0 + (edge->congestion_factor - 1.0) * 0.35;
            travel_time = base_time * emergency_factor;
        }

        /* Moderate delay for physical obstacles even under green corridor */
        if (edge->incident == INCIDENT_ACCIDENT) {
            travel_time += 2.0; /* minor delay to navigate around crash site */
        } else if (edge->incident == INCIDENT_WATERLOGGING) {
            travel_time += 3.0; /* cautious driving in water */
        } else if (edge->incident == INCIDENT_ROADWORK) {
            travel_time += 1.5; /* narrow lane */
        }

        return travel_time;
    } else {
        /* Standard Civilian Vehicle */
        double travel_time = base_time * edge->congestion_factor;

        /* Incident-induced queues and bottlenecks */
        if (edge->incident == INCIDENT_ACCIDENT) {
            travel_time += 10.0; /* massive queue behind crash */
        } else if (edge->incident == INCIDENT_WATERLOGGING) {
            travel_time += 15.0; /* flooded road crawling speed */
        } else if (edge->incident == INCIDENT_ROADWORK) {
            travel_time += 6.0;  /* lane constriction bottleneck */
        }

        return travel_time;
    }
}

void city_graph_print(const CityGraph *g) {
    if (!g) return;

    printf("\n========================================================================================\n");
    printf("                  SMART CITY ROAD NETWORK TOPOLOGY (ADJACENCY LIST)                     \n");
    printf("========================================================================================\n");
    printf("Total Intersections (Vertices): %d | Total Road Segments (Directed Edges): %d\n",
           g->num_vertices, g->num_edges);
    printf("----------------------------------------------------------------------------------------\n");

    for (int i = 0; i < g->num_vertices; ++i) {
        if (g->vertices[i].id == -1) continue;

        printf("[%2d] %-28s (%s) @ (%.1f, %.1f km)\n",
               g->vertices[i].id,
               g->vertices[i].name,
               vertex_type_to_string(g->vertices[i].type),
               g->vertices[i].coord_x,
               g->vertices[i].coord_y);

        RoadEdge *edge = g->vertices[i].head;
        if (!edge) {
            printf("      └── (No outgoing roads connected)\n");
        }
        while (edge) {
            printf("      ├──► [%2d] %-24s | %-20s | %.2f km | %d lanes | %3.0f km/h | Cong: %.1fx\n",
                   edge->dest,
                   g->vertices[edge->dest].name,
                   edge->road_name,
                   edge->distance_km,
                   edge->lanes,
                   edge->speed_limit_kmh,
                   edge->congestion_factor);
            edge = edge->next;
        }
    }
    printf("========================================================================================\n\n");
}

void city_graph_print_sensor_status(const CityGraph *g) {
    if (!g) return;

    printf("\n========================================================================================\n");
    printf("                       REAL-TIME IoT ROAD SENSOR TELEMETRY                              \n");
    printf("========================================================================================\n");
    printf("%-4s %-22s -> %-22s %-8s %-12s %-18s %-10s\n",
           "ID", "Origin", "Destination", "Cong.", "Avg Speed", "Incident Status", "Corridor");
    printf("----------------------------------------------------------------------------------------\n");

    int count = 0;
    for (int i = 0; i < g->num_vertices; ++i) {
        RoadEdge *edge = g->vertices[i].head;
        while (edge) {
            printf("[%2d] %-22.22s -> %-22.22s %5.1fx  %4.1f km/h   %-18s %-10s\n",
                   count++,
                   g->vertices[i].name,
                   g->vertices[edge->dest].name,
                   edge->congestion_factor,
                   edge->sensor_avg_speed,
                   incident_type_to_string(edge->incident),
                   edge->emergency_corridor ? "ACTIVE (GREEN)" : "INACTIVE");
            edge = edge->next;
        }
    }
    printf("========================================================================================\n\n");
}

void city_graph_destroy(CityGraph *g) {
    if (!g) return;

    for (int i = 0; i < g->capacity; ++i) {
        RoadEdge *edge = g->vertices[i].head;
        while (edge) {
            RoadEdge *next = edge->next;
            free(edge);
            edge = next;
        }
    }

    free(g->vertices);
    free(g);
}
