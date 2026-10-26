/**
 * @file iot_sensor.c
 * @brief Implementation of IoT Sensor Network for SRM Ramapuram Smart City.
 * 
 * Course: 21CSC201J - Data Structures and Algorithms
 * Institution: SRM Institute of Science and Technology, Ramapuram
 */

#include "iot_sensor.h"
#include <stdio.h>
#include <stdlib.h>

bool iot_sensor_init_srm_smart_network(CityGraph *g) {
    if (!g) return false;

    /* 1. Register Intersections, Landmarks & Emergency Centers */
    city_graph_add_vertex(g, NODE_SRM_RAMAPURAM,         "SRM IST Ramapuram Campus",       VERTEX_INTERSECTION,   1.0, 4.0);
    city_graph_add_vertex(g, NODE_RAMAPURAM_JN,          "Ramapuram Junction",            VERTEX_INTERSECTION,   2.0, 4.2);
    city_graph_add_vertex(g, NODE_DLF_IT_PARK,            "DLF Cybercity IT Park",         VERTEX_TRANSIT_HUB,    3.5, 3.5);
    city_graph_add_vertex(g, NODE_PORUR_ROUNDTANA,        "Porur Roundtana Bypass",        VERTEX_TRANSIT_HUB,    4.8, 5.0);
    city_graph_add_vertex(g, NODE_MIOT_HOSPITAL,          "MIOT International Hospital",   VERTEX_HOSPITAL,       3.8, 2.5);
    city_graph_add_vertex(g, NODE_KATHIPARA_FLYOVER,      "Kathipara Grade Separator",     VERTEX_TRANSIT_HUB,    6.0, 1.8);
    city_graph_add_vertex(g, NODE_GUINDY_METRO,           "Guindy Metro Station",          VERTEX_TRANSIT_HUB,    7.0, 1.5);
    city_graph_add_vertex(g, NODE_ASHOK_NAGAR_FIRE,       "Ashok Nagar Fire & Rescue",     VERTEX_FIRE_STATION,   6.5, 4.0);
    city_graph_add_vertex(g, NODE_VADAPALANI_SIGNAL,      "Vadapalani Signal Junction",    VERTEX_TRANSIT_HUB,    5.5, 5.5);
    city_graph_add_vertex(g, NODE_JAFFERKHANPET_BRIDGE,   "Jafferkhanpet Kasi Bridge",     VERTEX_INTERSECTION,   5.2, 3.0);
    city_graph_add_vertex(g, NODE_KOYAMBEDU_HUB,          "Koyambedu CMBT Hub",            VERTEX_TRANSIT_HUB,    4.5, 7.5);
    city_graph_add_vertex(g, NODE_SIMS_HOSPITAL,          "SIMS Hospital Vadapalani",      VERTEX_HOSPITAL,       5.8, 5.7);
    city_graph_add_vertex(g, NODE_GUINDY_POLICE_HQ,       "Guindy Police Control Room",    VERTEX_POLICE_STATION, 7.2, 1.2);
    city_graph_add_vertex(g, NODE_RAMAPURAM_HEALTH_CTR,   "Ramapuram Health Emergency Sub",VERTEX_HOSPITAL,       1.8, 4.5);
    city_graph_add_vertex(g, NODE_MANAPAKKAM_CORRIDOR,    "Manapakkam IT Corridor",        VERTEX_INTERSECTION,   2.8, 2.2);

    /* 2. Connect Road Segments with Physical Characteristics */
    city_graph_add_road(g, NODE_SRM_RAMAPURAM, NODE_RAMAPURAM_JN,
                        "Bharathi Salai Road", 1.2, 40.0, 2, ROAD_LOCAL, true);

    city_graph_add_road(g, NODE_RAMAPURAM_JN, NODE_DLF_IT_PARK,
                        "Mount-Poonamallee Link", 1.8, 50.0, 4, ROAD_ARTERIAL, true);

    city_graph_add_road(g, NODE_DLF_IT_PARK, NODE_PORUR_ROUNDTANA,
                        "Porur-DLF High Road", 2.2, 60.0, 6, ROAD_ARTERIAL, true);

    city_graph_add_road(g, NODE_RAMAPURAM_JN, NODE_RAMAPURAM_HEALTH_CTR,
                        "Valluvar Salai", 0.9, 35.0, 2, ROAD_LOCAL, true);

    city_graph_add_road(g, NODE_DLF_IT_PARK, NODE_MANAPAKKAM_CORRIDOR,
                        "Manapakkam Main Road", 1.5, 45.0, 4, ROAD_COLLECTOR, true);

    city_graph_add_road(g, NODE_MANAPAKKAM_CORRIDOR, NODE_MIOT_HOSPITAL,
                        "Trade Centre Avenue", 1.4, 50.0, 4, ROAD_COLLECTOR, true);

    city_graph_add_road(g, NODE_DLF_IT_PARK, NODE_MIOT_HOSPITAL,
                        "Mount-Poonamallee Expressway", 1.6, 60.0, 6, ROAD_EXPRESSWAY, true);

    city_graph_add_road(g, NODE_MIOT_HOSPITAL, NODE_KATHIPARA_FLYOVER,
                        "Butt Road - Kathipara", 2.5, 60.0, 6, ROAD_ARTERIAL, true);

    city_graph_add_road(g, NODE_KATHIPARA_FLYOVER, NODE_GUINDY_METRO,
                        "GST Road Expressway", 1.8, 70.0, 8, ROAD_EXPRESSWAY, true);

    city_graph_add_road(g, NODE_GUINDY_METRO, NODE_GUINDY_POLICE_HQ,
                        "Guindy Station Road", 0.8, 40.0, 2, ROAD_LOCAL, true);

    city_graph_add_road(g, NODE_KATHIPARA_FLYOVER, NODE_JAFFERKHANPET_BRIDGE,
                        "Inner Ring Road South", 2.0, 60.0, 6, ROAD_ARTERIAL, true);

    city_graph_add_road(g, NODE_JAFFERKHANPET_BRIDGE, NODE_ASHOK_NAGAR_FIRE,
                        "Kasi-Ashok Nagar Avenue", 1.6, 50.0, 4, ROAD_COLLECTOR, true);

    city_graph_add_road(g, NODE_ASHOK_NAGAR_FIRE, NODE_VADAPALANI_SIGNAL,
                        "100 Feet Road / Ring Road", 2.1, 60.0, 6, ROAD_ARTERIAL, true);

    city_graph_add_road(g, NODE_VADAPALANI_SIGNAL, NODE_SIMS_HOSPITAL,
                        "Jawaharlal Nehru Salai", 0.6, 45.0, 4, ROAD_ARTERIAL, true);

    city_graph_add_road(g, NODE_VADAPALANI_SIGNAL, NODE_KOYAMBEDU_HUB,
                        "Vadapalani-Koyambedu Bypass", 3.2, 70.0, 6, ROAD_EXPRESSWAY, true);

    city_graph_add_road(g, NODE_PORUR_ROUNDTANA, NODE_KOYAMBEDU_HUB,
                        "Poonamallee High Bypass", 4.5, 80.0, 6, ROAD_EXPRESSWAY, true);

    city_graph_add_road(g, NODE_RAMAPURAM_JN, NODE_VADAPALANI_SIGNAL,
                        "Arcot Road Central", 4.2, 45.0, 4, ROAD_ARTERIAL, true);

    city_graph_add_road(g, NODE_MIOT_HOSPITAL, NODE_JAFFERKHANPET_BRIDGE,
                        "Nandambakkam Canal Road", 2.3, 40.0, 2, ROAD_COLLECTOR, true);

    city_graph_add_road(g, NODE_GUINDY_METRO, NODE_ASHOK_NAGAR_FIRE,
                        "Guindy-Ashok Pillar Link", 2.6, 50.0, 4, ROAD_COLLECTOR, true);

    return true;
}

void iot_sensor_simulate_morning_rush(CityGraph *g) {
    if (!g) return;

    printf("\n[IoT SIMULATION] >>> Surging Morning Peak IT Corridor Traffic (DLF / Mount-Poonamallee / Arcot Rd) <<<\n");

    /* Heavy tech park congestion */
    city_graph_update_iot_sensor(g, NODE_RAMAPURAM_JN, NODE_DLF_IT_PARK, 3.8, INCIDENT_CONGESTION, false, true);
    city_graph_update_iot_sensor(g, NODE_DLF_IT_PARK, NODE_PORUR_ROUNDTANA, 4.2, INCIDENT_CONGESTION, false, true);
    city_graph_update_iot_sensor(g, NODE_DLF_IT_PARK, NODE_MIOT_HOSPITAL, 3.5, INCIDENT_CONGESTION, false, true);
    city_graph_update_iot_sensor(g, NODE_RAMAPURAM_JN, NODE_VADAPALANI_SIGNAL, 4.0, INCIDENT_CONGESTION, false, true);
    city_graph_update_iot_sensor(g, NODE_MIOT_HOSPITAL, NODE_KATHIPARA_FLYOVER, 2.8, INCIDENT_CONGESTION, false, true);
    city_graph_update_iot_sensor(g, NODE_KATHIPARA_FLYOVER, NODE_JAFFERKHANPET_BRIDGE, 3.0, INCIDENT_CONGESTION, false, true);
}

void iot_sensor_simulate_monsoon_flood(CityGraph *g) {
    if (!g) return;

    printf("\n[IoT SIMULATION] >>> Monsoon Alert: Waterlogging on Low-Lying Canal & Subway Roads <<<\n");

    /* Flooding on canal road & Jafferkhanpet Kasi subway */
    city_graph_update_iot_sensor(g, NODE_MIOT_HOSPITAL, NODE_JAFFERKHANPET_BRIDGE, 5.5, INCIDENT_WATERLOGGING, false, true);
    city_graph_update_iot_sensor(g, NODE_JAFFERKHANPET_BRIDGE, NODE_ASHOK_NAGAR_FIRE, 4.0, INCIDENT_WATERLOGGING, false, true);
}

void iot_sensor_simulate_accident(CityGraph *g, int src, int dest) {
    if (!g || src < 0 || src >= g->num_vertices || dest < 0 || dest >= g->num_vertices) {
        printf("\n[!] Error: Invalid intersection IDs (%d, %d).\n", src, dest);
        return;
    }

    if (g->vertices[src].id == -1 || g->vertices[dest].id == -1) {
        printf("\n[!] Error: Specified intersection is not active.\n");
        return;
    }

    if (!city_graph_get_edge(g, src, dest)) {
        printf("\n[!] Error: No direct road segment connects [%s] and [%s].\n",
               g->vertices[src].name, g->vertices[dest].name);
        return;
    }

    printf("\n[IoT SIMULATION] >>> Severe Traffic Incident: Collision Reported between [%s] and [%s] <<<\n",
           g->vertices[src].name, g->vertices[dest].name);

    city_graph_update_iot_sensor(g, src, dest, 6.0, INCIDENT_ACCIDENT, false, true);
}

void iot_sensor_simulate_road_closure(CityGraph *g, int src, int dest) {
    if (!g || src < 0 || src >= g->num_vertices || dest < 0 || dest >= g->num_vertices) {
        printf("\n[!] Error: Invalid intersection IDs (%d, %d).\n", src, dest);
        return;
    }

    if (g->vertices[src].id == -1 || g->vertices[dest].id == -1) {
        printf("\n[!] Error: Specified intersection is not active.\n");
        return;
    }

    if (!city_graph_get_edge(g, src, dest)) {
        printf("\n[!] Error: No direct road segment connects [%s] and [%s].\n",
               g->vertices[src].name, g->vertices[dest].name);
        return;
    }

    printf("\n[IoT SIMULATION] >>> CRITICAL EMERGENCY ALERT: Road Closed between [%s] and [%s] <<<\n",
           g->vertices[src].name, g->vertices[dest].name);

    city_graph_update_iot_sensor(g, src, dest, 999.0, INCIDENT_ROAD_CLOSURE, false, true);
}

void iot_sensor_reset_to_free_flow(CityGraph *g) {
    if (!g) return;

    for (int i = 0; i < g->num_vertices; ++i) {
        RoadEdge *edge = g->vertices[i].head;
        while (edge) {
            edge->congestion_factor = 1.0;
            edge->incident = INCIDENT_NONE;
            edge->emergency_corridor = false;
            edge->sensor_avg_speed = edge->speed_limit_kmh;
            edge->vehicle_count = 15 * edge->lanes;
            edge = edge->next;
        }
    }
    printf("\n[IoT SIMULATION] Network sensors reset to normal free-flow baseline conditions.\n");
}
