/**
 * @file city_graph.h
 * @brief Graph data structures and IoT road network models.
 * 
 * Course: 21CSC201J - Data Structures and Algorithms
 * Institution: SRM Institute of Science and Technology, Ramapuram
 * Project: Smart Traffic & Emergency Route Optimization using Graphs & Dijkstra's Algorithm
 */

#ifndef CITY_GRAPH_H
#define CITY_GRAPH_H

#include <stdbool.h>
#include <stddef.h>

#define MAX_NAME_LEN 64
#define INF_TIME 1e9

/**
 * @brief Vertex classifications in the smart city network.
 */
typedef enum {
    VERTEX_INTERSECTION = 0,
    VERTEX_HOSPITAL,
    VERTEX_FIRE_STATION,
    VERTEX_POLICE_STATION,
    VERTEX_TRANSIT_HUB,
    VERTEX_INCIDENT_ZONE
} VertexType;

/**
 * @brief Road hierarchy and functional tier.
 */
typedef enum {
    ROAD_EXPRESSWAY = 0,
    ROAD_ARTERIAL,
    ROAD_COLLECTOR,
    ROAD_LOCAL
} RoadTier;

/**
 * @brief IoT detected road incident types.
 */
typedef enum {
    INCIDENT_NONE = 0,
    INCIDENT_CONGESTION,
    INCIDENT_ACCIDENT,
    INCIDENT_ROADWORK,
    INCIDENT_WATERLOGGING,
    INCIDENT_ROAD_CLOSURE
} IncidentType;

/**
 * @brief Adjacency list edge representing a directional road segment.
 */
typedef struct RoadEdge {
    int dest;                       /**< Destination vertex index */
    char road_name[MAX_NAME_LEN];   /**< Name of the road/avenue */
    double distance_km;             /**< Physical length in kilometers */
    double speed_limit_kmh;         /**< Design speed limit in km/h */
    int lanes;                      /**< Number of active traffic lanes */
    RoadTier tier;                  /**< Functional road classification */
    
    /* IoT Sensor Telemetry */
    double congestion_factor;       /**< 1.0 = Free Flow, 1.5 = Moderate, 2.8 = Heavy, 5.0+ = Gridlock */
    IncidentType incident;          /**< Live incident reported by IoT roadside units */
    bool emergency_corridor;        /**< Dynamic Green Corridor signal preemption enabled */
    double sensor_avg_speed;        /**< Real-time speed from IoT radar/loop detectors */
    int vehicle_count;              /**< Real-time vehicle count per minute from smart cameras */

    struct RoadEdge *next;          /**< Pointer to next edge in adjacency list */
} RoadEdge;

/**
 * @brief Node representing an intersection, landmark, or emergency depot.
 */
typedef struct Vertex {
    int id;                         /**< Unique numerical ID (0 to V-1) */
    char name[MAX_NAME_LEN];        /**< Name of intersection or landmark */
    VertexType type;                /**< Categorization of vertex */
    double coord_x;                 /**< 2D coordinate for map visualization (km) */
    double coord_y;                 /**< 2D coordinate for map visualization (km) */
    RoadEdge *head;                 /**< Head of adjacency list of outgoing roads */
} Vertex;

/**
 * @brief City Road Network Graph represented as an Adjacency List.
 */
typedef struct CityGraph {
    int num_vertices;               /**< Number of registered vertices */
    int capacity;                   /**< Maximum allocated vertices */
    int num_edges;                  /**< Total directed road segments */
    Vertex *vertices;               /**< Array of vertices */
} CityGraph;

/**
 * @brief Creates a new city road network graph.
 * @param capacity Maximum number of vertices to support
 * @return Pointer to CityGraph, or NULL on failure.
 */
CityGraph* city_graph_create(int capacity);

/**
 * @brief Adds an intersection or landmark vertex to the graph.
 * @param g Graph pointer
 * @param id Vertex ID
 * @param name Landmark/Intersection name
 * @param type Classification (e.g. Hospital, Fire Station)
 * @param x Coordinate X
 * @param y Coordinate Y
 * @return true on success, false if invalid or duplicate.
 */
bool city_graph_add_vertex(CityGraph *g, int id, const char *name, VertexType type, double x, double y);

/**
 * @brief Searches for a vertex ID by its string name.
 * @param g Graph pointer
 * @param name Vertex name to search
 * @return Vertex ID, or -1 if not found.
 */
int city_graph_find_vertex_by_name(const CityGraph *g, const char *name);

/**
 * @brief Adds a road segment between two vertices.
 * @param g Graph pointer
 * @param src Source vertex ID
 * @param dest Destination vertex ID
 * @param road_name Road title
 * @param distance_km Physical distance in km
 * @param speed_limit_kmh Speed limit in km/h
 * @param lanes Number of lanes
 * @param tier Classification tier
 * @param bidirectional If true, adds reverse road segment dest -> src
 * @return true on success, false otherwise.
 */
bool city_graph_add_road(CityGraph *g, int src, int dest, const char *road_name,
                         double distance_km, double speed_limit_kmh, int lanes,
                         RoadTier tier, bool bidirectional);

/**
 * @brief Updates IoT sensor telemetry for a road segment.
 * @param g Graph pointer
 * @param src Source vertex ID
 * @param dest Destination vertex ID
 * @param congestion_factor Traffic multiplier (1.0 to 10.0)
 * @param incident Incident category
 * @param emergency_corridor Whether green corridor is activated
 * @param bidirectional Whether to update both directions
 * @return true if updated, false if edge not found.
 */
bool city_graph_update_iot_sensor(CityGraph *g, int src, int dest,
                                  double congestion_factor, IncidentType incident,
                                  bool emergency_corridor, bool bidirectional);

/**
 * @brief Computes dynamic travel time in minutes for a road edge.
 * @param edge Road segment
 * @param is_emergency_vehicle If true, applies emergency priority and green corridor discount
 * @return Estimated travel time in minutes.
 */
double city_graph_calculate_travel_time(const RoadEdge *edge, bool is_emergency_vehicle);

/**
 * @brief Finds the edge pointer between src and dest.
 * @param g Graph pointer
 * @param src Source vertex ID
 * @param dest Destination vertex ID
 * @return RoadEdge pointer or NULL if not found.
 */
RoadEdge* city_graph_get_edge(const CityGraph *g, int src, int dest);

/**
 * @brief Displays the city road network adjacency list and connection topology.
 * @param g Graph pointer
 */
void city_graph_print(const CityGraph *g);

/**
 * @brief Displays the real-time IoT traffic status across all road segments.
 * @param g Graph pointer
 */
void city_graph_print_sensor_status(const CityGraph *g);

/**
 * @brief Returns the human-readable string for a VertexType.
 */
const char* vertex_type_to_string(VertexType type);

/**
 * @brief Returns the human-readable string for an IncidentType.
 */
const char* incident_type_to_string(IncidentType incident);

/**
 * @brief Returns the human-readable string for a RoadTier.
 */
const char* road_tier_to_string(RoadTier tier);

/**
 * @brief Deallocates all vertices and edge lists in the graph.
 * @param g Graph pointer
 */
void city_graph_destroy(CityGraph *g);

#endif /* CITY_GRAPH_H */
