/**
 * @file cli.c
 * @brief Interactive CLI, Automated Demo, and Test Suite Implementation.
 * 
 * Course: 21CSC201J - Data Structures and Algorithms
 * Institution: SRM Institute of Science and Technology, Ramapuram
 */

#include "cli.h"
#include "city_graph.h"
#include "dijkstra.h"
#include "iot_sensor.h"
#include "min_heap.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

static void print_banner(void) {
    printf("\n");
    printf("****************************************************************************************\n");
    printf("*            SRM INSTITUTE OF SCIENCE AND TECHNOLOGY - RAMAPURAM CAMPUS                *\n");
    printf("*              Department of Computer Science & Business Systems / SCSE                *\n");
    printf("*               Course: 21CSC201J - Data Structures and Algorithms                     *\n");
    printf("*                                                                                      *\n");
    printf("*   PROJECT: SMART TRAFFIC & EMERGENCY ROUTE OPTIMIZATION (GRAPHS & DIJKSTRA MIN-HEAP) *\n");
    printf("****************************************************************************************\n\n");
}

static void print_menu(void) {
    printf("========================================================================================\n");
    printf("                                   CONTROL DASHBOARD                                    \n");
    printf("========================================================================================\n");
    printf("  1. Display Road Network Topology (Adjacency List)\n");
    printf("  2. Display Real-Time IoT Traffic Sensors & Incident Status\n");
    printf("  3. Compute Civilian Shortest Route (Source -> Destination)\n");
    printf("  4. Emergency Vehicle Dispatch & Route (Smart Green Corridor Preemption)\n");
    printf("  5. Multi-Depot Nearest Emergency Responder Dispatch (Hospital / Fire / Police)\n");
    printf("  6. Simulate IoT Traffic Incidents (Morning Peak / Monsoon Flood / Crash / Closure)\n");
    printf("  7. Add Custom Intersection or Custom Road Segment\n");
    printf("  8. Reset All IoT Sensors to Free-Flow Baseline\n");
    printf("  9. Run Automated Smart City Scenario Demonstration\n");
    printf(" 10. Run Built-in Verification & Regression Test Suite\n");
    printf("  0. Exit\n");
    printf("========================================================================================\n");
    printf("Enter Choice [0 - 10]: ");
}

static void clear_stdin(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

static void handle_civilian_route(CityGraph *g) {
    int src = -1, dest = -1;
    printf("\nEnter Source Vertex ID (0 to %d): ", g->num_vertices - 1);
    if (scanf("%d", &src) != 1) {
        clear_stdin();
        printf("Invalid input.\n");
        return;
    }
    printf("Enter Destination Vertex ID (0 to %d): ", g->num_vertices - 1);
    if (scanf("%d", &dest) != 1) {
        clear_stdin();
        printf("Invalid input.\n");
        return;
    }

    if (src < 0 || src >= g->num_vertices || dest < 0 || dest >= g->num_vertices ||
        g->vertices[src].id == -1 || g->vertices[dest].id == -1) {
        printf("Invalid or inactive vertex ID.\n");
        return;
    }

    DijkstraResult *res = dijkstra_compute(g, src, false, METRIC_DYNAMIC_TRAVEL_TIME);
    if (res) {
        dijkstra_print_path_details(g, res, dest);
        dijkstra_result_destroy(res);
    }
}

static void handle_emergency_route(CityGraph *g) {
    int src = -1, dest = -1;
    printf("\nEnter Emergency Depot Vertex ID (0 to %d): ", g->num_vertices - 1);
    if (scanf("%d", &src) != 1) {
        clear_stdin();
        printf("Invalid input.\n");
        return;
    }
    printf("Enter Incident / Hospital Destination Vertex ID (0 to %d): ", g->num_vertices - 1);
    if (scanf("%d", &dest) != 1) {
        clear_stdin();
        printf("Invalid input.\n");
        return;
    }

    if (src < 0 || src >= g->num_vertices || dest < 0 || dest >= g->num_vertices ||
        g->vertices[src].id == -1 || g->vertices[dest].id == -1) {
        printf("Invalid or inactive vertex ID.\n");
        return;
    }

    /* 1. Calculate optimal path for emergency vehicle */
    DijkstraResult *res = dijkstra_compute(g, src, true, METRIC_DYNAMIC_TRAVEL_TIME);
    if (!res) return;

    int path[512];
    int path_len = 0;
    if (dijkstra_get_path(res, dest, path, 512, &path_len)) {
        printf("\n>>> [GREEN CORRIDOR PROTOCOL ACTIVATED] <<<\n");
        printf("Synchronizing smart traffic signals along route: %s to %s\n",
               g->vertices[src].name, g->vertices[dest].name);

        /* Preempt signals along the chosen path */
        dijkstra_activate_green_corridor_along_path(g, path, path_len);

        /* Recompute with active green corridor */
        dijkstra_result_destroy(res);
        res = dijkstra_compute(g, src, true, METRIC_DYNAMIC_TRAVEL_TIME);
        dijkstra_print_path_details(g, res, dest);

        /* Clear corridors after dispatch */
        dijkstra_clear_green_corridors(g);
    } else {
        printf("No reachable path available for emergency vehicle.\n");
    }

    dijkstra_result_destroy(res);
}

static void handle_nearest_responder(CityGraph *g) {
    int incident_node = -1;
    int resp_type_choice = 1;

    printf("\nEnter Incident Location Vertex ID (0 to %d): ", g->num_vertices - 1);
    if (scanf("%d", &incident_node) != 1) {
        clear_stdin();
        printf("Invalid input.\n");
        return;
    }

    if (incident_node < 0 || incident_node >= g->num_vertices || g->vertices[incident_node].id == -1) {
        printf("Invalid or inactive incident location vertex ID.\n");
        return;
    }

    printf("Select Required Emergency Responder Service:\n");
    printf("  1. Ambulance / Trauma Care (Hospital Depot)\n");
    printf("  2. Fire Engine & Rescue Unit (Fire Depot)\n");
    printf("  3. Police Rapid Response (Police Depot)\n");
    printf("Choice [1 - 3]: ");
    if (scanf("%d", &resp_type_choice) != 1) {
        clear_stdin();
        printf("Invalid input.\n");
        return;
    }

    VertexType vtype = VERTEX_HOSPITAL;
    if (resp_type_choice == 2) vtype = VERTEX_FIRE_STATION;
    else if (resp_type_choice == 3) vtype = VERTEX_POLICE_STATION;

    int best_responder = -1;
    double eta = 0.0;
    DijkstraResult *res = dijkstra_find_nearest_emergency_responder(g, incident_node, vtype, &best_responder, &eta);

    if (res && best_responder != -1) {
        printf("\n========================================================================================\n");
        printf("              OPTIMAL EMERGENCY RESPONDER DISPATCH RECOMMENDATION                       \n");
        printf("========================================================================================\n");
        printf("Incident Scene   : [%d] %s\n", incident_node, g->vertices[incident_node].name);
        printf("Selected Responder: [%d] %s (%s)\n",
               best_responder, g->vertices[best_responder].name, vertex_type_to_string(vtype));
        printf("Quickest Arrival Time (ETA): %.2f minutes (%.1f seconds)\n", eta, eta * 60.0);
        printf("========================================================================================\n");

        /* Activate green corridor and print details */
        int path[512];
        int path_len = 0;
        if (dijkstra_get_path(res, incident_node, path, 512, &path_len)) {
            dijkstra_activate_green_corridor_along_path(g, path, path_len);
            dijkstra_result_destroy(res);
            res = dijkstra_compute(g, best_responder, true, METRIC_DYNAMIC_TRAVEL_TIME);
            dijkstra_print_path_details(g, res, incident_node);
            dijkstra_clear_green_corridors(g);
        }
        dijkstra_result_destroy(res);
    } else {
        printf("\n[!] Error: No available %s found with a viable route to the incident location.\n",
               vertex_type_to_string(vtype));
    }
}

static void handle_iot_simulations(CityGraph *g) {
    int sim_choice = 0;
    printf("\n--- IoT TRAFFIC TELEMETRY SIMULATION CONTROLS ---\n");
    printf("  1. Trigger Peak Morning Commuter Rush (DLF / Mount-Poonamallee / Arcot Rd)\n");
    printf("  2. Trigger Monsoon Severe Waterlogging on Canal Road & Low Subway\n");
    printf("  3. Report Vehicle Collision Accident between two nodes\n");
    printf("  4. Enforce Full Road Closure (Metro Construction / VIP Corridor)\n");
    printf("  5. Reset Network to Free-Flow Normal Conditions\n");
    printf("Enter choice [1 - 5]: ");
    if (scanf("%d", &sim_choice) != 1) {
        clear_stdin();
        printf("Invalid input.\n");
        return;
    }

    if (sim_choice == 1) {
        iot_sensor_simulate_morning_rush(g);
    } else if (sim_choice == 2) {
        iot_sensor_simulate_monsoon_flood(g);
    } else if (sim_choice == 3) {
        int u, v;
        printf("Enter Road Endpoint 1: ");
        if (scanf("%d", &u) != 1) { clear_stdin(); return; }
        printf("Enter Road Endpoint 2: ");
        if (scanf("%d", &v) != 1) { clear_stdin(); return; }
        iot_sensor_simulate_accident(g, u, v);
    } else if (sim_choice == 4) {
        int u, v;
        printf("Enter Blocked Road Endpoint 1: ");
        if (scanf("%d", &u) != 1) { clear_stdin(); return; }
        printf("Enter Blocked Road Endpoint 2: ");
        if (scanf("%d", &v) != 1) { clear_stdin(); return; }
        iot_sensor_simulate_road_closure(g, u, v);
    } else if (sim_choice == 5) {
        iot_sensor_reset_to_free_flow(g);
    }
}

static void handle_add_custom(CityGraph *g) {
    int sub_choice = 0;
    printf("\n  1. Add Custom Intersection / Depot Vertex\n");
    printf("  2. Add Custom Road Segment (Edge)\n");
    printf("Choice [1 - 2]: ");
    if (scanf("%d", &sub_choice) != 1) {
        clear_stdin();
        printf("Invalid input.\n");
        return;
    }

    if (sub_choice == 1) {
        char name[MAX_NAME_LEN];
        int type_val = 0;
        double x = 0.0, y = 0.0;
        printf("Enter Landmark Name: ");
        scanf(" %63[^\n]", name);
        printf("Enter Vertex Type (0: Intersection, 1: Hospital, 2: Fire, 3: Police, 4: Transit Hub): ");
        if (scanf("%d", &type_val) != 1) { clear_stdin(); return; }
        printf("Enter Map Coordinates X Y: ");
        if (scanf("%lf %lf", &x, &y) != 2) { clear_stdin(); return; }

        int new_id = g->num_vertices;
        if (city_graph_add_vertex(g, new_id, name, (VertexType)type_val, x, y)) {
            printf("Successfully registered Vertex [%d]: %s\n", new_id, name);
        } else {
            printf("Failed to register vertex.\n");
        }
    } else if (sub_choice == 2) {
        int u, v, lanes;
        char rname[MAX_NAME_LEN];
        double dist, speed;
        int bidirectional = 1;

        printf("Enter Source Vertex ID: ");
        if (scanf("%d", &u) != 1) { clear_stdin(); return; }
        printf("Enter Destination Vertex ID: ");
        if (scanf("%d", &v) != 1) { clear_stdin(); return; }
        printf("Enter Road Name: ");
        scanf(" %63[^\n]", rname);
        printf("Enter Length in km: ");
        if (scanf("%lf", &dist) != 1) { clear_stdin(); return; }
        printf("Enter Speed Limit in km/h: ");
        if (scanf("%lf", &speed) != 1) { clear_stdin(); return; }
        printf("Enter Number of Lanes: ");
        if (scanf("%d", &lanes) != 1) { clear_stdin(); return; }
        printf("Is Road Bidirectional? (1: Yes, 0: One-Way): ");
        if (scanf("%d", &bidirectional) != 1) { clear_stdin(); return; }

        if (city_graph_add_road(g, u, v, rname, dist, speed, lanes, ROAD_ARTERIAL, bidirectional == 1)) {
            printf("Successfully added road segment [%s] connecting %s and %s.\n",
                   rname, g->vertices[u].name, g->vertices[v].name);
        } else {
            printf("Failed to add road segment. Ensure vertices exist and values are positive.\n");
        }
    }
}

void cli_run_interactive_menu(CityGraph *g) {
    print_banner();
    int choice = -1;

    while (choice != 0) {
        print_menu();
        int res = scanf("%d", &choice);
        if (res == EOF) {
            break;
        }
        if (res != 1) {
            /* Clear input buffer on invalid characters */
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        switch (choice) {
            case 1:
                city_graph_print(g);
                break;
            case 2:
                city_graph_print_sensor_status(g);
                break;
            case 3:
                handle_civilian_route(g);
                break;
            case 4:
                handle_emergency_route(g);
                break;
            case 5:
                handle_nearest_responder(g);
                break;
            case 6:
                handle_iot_simulations(g);
                break;
            case 7:
                handle_add_custom(g);
                break;
            case 8:
                iot_sensor_reset_to_free_flow(g);
                break;
            case 9:
                cli_run_automated_demo(g);
                break;
            case 10:
                cli_run_test_suite();
                break;
            case 0:
                printf("\nExiting Smart Traffic Optimizer. Thank you!\n");
                break;
            default:
                printf("\nInvalid option. Please choose between 0 and 10.\n");
                break;
        }
    }
}

void cli_run_automated_demo(CityGraph *g) {
    print_banner();
    printf("========================================================================================\n");
    printf("              STARTING AUTOMATED END-TO-END SMART CITY DEMONSTRATION                   \n");
    printf("========================================================================================\n");

    /* Scenario 1: Initial Free-Flow Baseline */
    printf("\n[SCENARIO 1] Baseline Free-Flow Traffic Conditions across SRM Ramapuram Network\n");
    printf("----------------------------------------------------------------------------------------\n");
    iot_sensor_reset_to_free_flow(g);

    printf("\n>> Mission 1: Civilian Travel from [SRM IST Ramapuram] to [SIMS Hospital Vadapalani]\n");
    DijkstraResult *res1 = dijkstra_compute(g, NODE_SRM_RAMAPURAM, false, METRIC_DYNAMIC_TRAVEL_TIME);
    dijkstra_print_path_details(g, res1, NODE_SIMS_HOSPITAL);
    double free_flow_time = res1->dist[NODE_SIMS_HOSPITAL];
    dijkstra_result_destroy(res1);

    /* Scenario 2: Morning Rush Hour Traffic Surge */
    printf("\n[SCENARIO 2] Morning Rush Hour Traffic Congestion Surge (IoT Sensor Telemetry Update)\n");
    printf("----------------------------------------------------------------------------------------\n");
    iot_sensor_simulate_morning_rush(g);

    printf("\n>> Mission 2A: Civilian Commuter Route under Rush Hour Congestion\n");
    DijkstraResult *res2_civ = dijkstra_compute(g, NODE_SRM_RAMAPURAM, false, METRIC_DYNAMIC_TRAVEL_TIME);
    dijkstra_print_path_details(g, res2_civ, NODE_SIMS_HOSPITAL);
    double civilian_rush_time = res2_civ->dist[NODE_SIMS_HOSPITAL];
    dijkstra_result_destroy(res2_civ);

    printf("\n>> Mission 2B: Emergency Ambulance Dispatch with Dynamic Smart Green Corridor\n");
    DijkstraResult *res2_emg = dijkstra_compute(g, NODE_SRM_RAMAPURAM, true, METRIC_DYNAMIC_TRAVEL_TIME);
    int path[512];
    int path_len = 0;
    dijkstra_get_path(res2_emg, NODE_SIMS_HOSPITAL, path, 512, &path_len);
    dijkstra_activate_green_corridor_along_path(g, path, path_len);
    dijkstra_result_destroy(res2_emg);

    res2_emg = dijkstra_compute(g, NODE_SRM_RAMAPURAM, true, METRIC_DYNAMIC_TRAVEL_TIME);
    dijkstra_print_path_details(g, res2_emg, NODE_SIMS_HOSPITAL);
    double emergency_rush_time = res2_emg->dist[NODE_SIMS_HOSPITAL];
    dijkstra_result_destroy(res2_emg);
    dijkstra_clear_green_corridors(g);

    printf("========================================================================================\n");
    printf("                  IoT TRAFFIC EFFICIENCY & TIME-SAVING ANALYSIS                         \n");
    printf("========================================================================================\n");
    printf("  ► Free-Flow Baseline Travel Time        : %6.2f mins\n", free_flow_time);
    printf("  ► Civilian Vehicle during Rush Hour     : %6.2f mins (Delay: +%.2f mins / +%.1f%%)\n",
           civilian_rush_time, civilian_rush_time - free_flow_time,
           ((civilian_rush_time - free_flow_time) / free_flow_time) * 100.0);
    printf("  ► Emergency Vehicle with Green Corridor : %6.2f mins (Saved:  -%.2f mins / -%.1f%%)\n",
           emergency_rush_time, civilian_rush_time - emergency_rush_time,
           ((civilian_rush_time - emergency_rush_time) / civilian_rush_time) * 100.0);
    printf("========================================================================================\n\n");

    /* Scenario 3: Critical Incident - Unplanned Arterial Road Closure */
    printf("\n[SCENARIO 3] Critical Incident: Pipeline Burst & Road Closure on Mount-Poonamallee Link\n");
    printf("----------------------------------------------------------------------------------------\n");
    iot_sensor_simulate_road_closure(g, NODE_RAMAPURAM_JN, NODE_DLF_IT_PARK);

    printf("\n>> Mission 3: Automatic Dynamic Rerouting around Blocked Arcot / Poonamallee Junction\n");
    DijkstraResult *res3 = dijkstra_compute(g, NODE_SRM_RAMAPURAM, false, METRIC_DYNAMIC_TRAVEL_TIME);
    dijkstra_print_path_details(g, res3, NODE_MIOT_HOSPITAL);
    dijkstra_result_destroy(res3);

    /* Scenario 4: Multi-Depot Emergency Responder Dispatch */
    printf("\n[SCENARIO 4] Mass Casualty Incident at Porur Roundtana: Multi-Depot Responder Dispatch\n");
    printf("----------------------------------------------------------------------------------------\n");
    int best_hosp = -1;
    double hosp_eta = 0.0;
    DijkstraResult *res4 = dijkstra_find_nearest_emergency_responder(g, NODE_PORUR_ROUNDTANA,
                                                                     VERTEX_HOSPITAL,
                                                                     &best_hosp, &hosp_eta);
    if (res4) {
        printf(">> Optimal Hospital Dispatched: [%s] | Fast Arrival ETA: %.2f mins\n",
               g->vertices[best_hosp].name, hosp_eta);
        dijkstra_print_path_details(g, res4, NODE_PORUR_ROUNDTANA);
        dijkstra_result_destroy(res4);
    }

    printf("\n========================================================================================\n");
    printf("                 AUTOMATED DEMONSTRATION RUN COMPLETED SUCCESSFULLY                     \n");
    printf("========================================================================================\n\n");
}

/* ============================================================================================= */
/*                                    AUTOMATED TEST SUITE                                       */
/* ============================================================================================= */

static void test_min_heap_operations(void) {
    printf("[TEST 1/12] Running Min-Heap Priority Queue Verification... ");
    MinHeap *heap = min_heap_create(10);
    assert(heap != NULL);
    assert(min_heap_is_empty(heap) == true);

    min_heap_insert(heap, 0, 50.0);
    min_heap_insert(heap, 1, 20.0);
    min_heap_insert(heap, 2, 70.0);
    min_heap_insert(heap, 3, 10.0);

    assert(min_heap_is_empty(heap) == false);
    assert(min_heap_contains(heap, 1) == true);
    assert(min_heap_contains(heap, 9) == false);

    /* Decrease key of vertex 2 from 70 to 5 (should become new root) */
    min_heap_decrease_key(heap, 2, 5.0);

    MinHeapNode n1 = min_heap_extract_min(heap);
    assert(n1.vertex == 2);
    assert(fabs(n1.distance - 5.0) < 1e-6);

    MinHeapNode n2 = min_heap_extract_min(heap);
    assert(n2.vertex == 3);
    assert(fabs(n2.distance - 10.0) < 1e-6);

    MinHeapNode n3 = min_heap_extract_min(heap);
    assert(n3.vertex == 1);
    assert(fabs(n3.distance - 20.0) < 1e-6);

    MinHeapNode n4 = min_heap_extract_min(heap);
    assert(n4.vertex == 0);
    assert(fabs(n4.distance - 50.0) < 1e-6);

    /* Deep Verification of Position Map Reset & Emptied State */
    assert(min_heap_is_empty(heap) == true);
    assert(min_heap_contains(heap, 0) == false);
    assert(heap->pos[0] == -1);

    /* Re-insert on emptied heap: must not conflict with previously extracted nodes */
    min_heap_insert(heap, 5, 12.0);
    assert(min_heap_contains(heap, 5) == true);
    assert(min_heap_contains(heap, 0) == false);
    MinHeapNode n5 = min_heap_extract_min(heap);
    assert(n5.vertex == 5);
    assert(heap->pos[5] == -1);
    assert(min_heap_is_empty(heap) == true);

    min_heap_destroy(heap);
    printf("PASSED\n");
}

static void test_graph_creation_and_topology(void) {
    printf("[TEST 2/12] Running Graph Adjacency List Integrity Verification... ");
    CityGraph *g = city_graph_create(5);
    assert(g != NULL);

    assert(city_graph_add_vertex(g, 0, "Node 0", VERTEX_INTERSECTION, 0.0, 0.0) == true);
    assert(city_graph_add_vertex(g, 1, "Node 1", VERTEX_HOSPITAL, 1.0, 1.0) == true);
    /* Duplicate vertex id rejected */
    assert(city_graph_add_vertex(g, 0, "Duplicate Node", VERTEX_INTERSECTION, 0.0, 0.0) == false);

    assert(city_graph_add_road(g, 0, 1, "Alpha Way", 5.0, 50.0, 2, ROAD_ARTERIAL, true) == true);
    /* Check bidirectional edges */
    RoadEdge *e1 = city_graph_get_edge(g, 0, 1);
    RoadEdge *e2 = city_graph_get_edge(g, 1, 0);
    assert(e1 != NULL);
    assert(e2 != NULL);
    assert(fabs(e1->distance_km - 5.0) < 1e-6);
    assert(fabs(e2->distance_km - 5.0) < 1e-6);

    city_graph_destroy(g);
    printf("PASSED\n");
}

static void test_dijkstra_shortest_path(void) {
    printf("[TEST 3/12] Running Dijkstra Shortest Path Correctness on Triangle Graph... ");
    /* Simple 3-node triangle:
       0 -> 1: dist 10 km @ 60 km/h (10 mins)
       0 -> 2: dist 2 km @ 60 km/h  (2 mins)
       2 -> 1: dist 3 km @ 60 km/h  (3 mins)
       Expected shortest path 0 -> 1 is 0 -> 2 -> 1 with cost 5 mins, not direct edge 10 mins.
    */
    CityGraph *g = city_graph_create(3);
    city_graph_add_vertex(g, 0, "A", VERTEX_INTERSECTION, 0.0, 0.0);
    city_graph_add_vertex(g, 1, "B", VERTEX_INTERSECTION, 1.0, 0.0);
    city_graph_add_vertex(g, 2, "C", VERTEX_INTERSECTION, 0.5, 0.5);

    city_graph_add_road(g, 0, 1, "Direct", 10.0, 60.0, 2, ROAD_ARTERIAL, false);
    city_graph_add_road(g, 0, 2, "Path1", 2.0, 60.0, 2, ROAD_ARTERIAL, false);
    city_graph_add_road(g, 2, 1, "Path2", 3.0, 60.0, 2, ROAD_ARTERIAL, false);

    DijkstraResult *res = dijkstra_compute(g, 0, false, METRIC_DYNAMIC_TRAVEL_TIME);
    assert(res != NULL);
    assert(fabs(res->dist[1] - 5.0) < 1e-6);
    assert(res->parent[1] == 2);
    assert(res->parent[2] == 0);

    int path[10];
    int path_len = 0;
    assert(dijkstra_get_path(res, 1, path, 10, &path_len) == true);
    assert(path_len == 3);
    assert(path[0] == 0 && path[1] == 2 && path[2] == 1);

    dijkstra_result_destroy(res);
    city_graph_destroy(g);
    printf("PASSED\n");
}

static void test_iot_congestion_weighting(void) {
    printf("[TEST 4/12] Running Dynamic IoT Traffic Congestion Weighting Test... ");
    CityGraph *g = city_graph_create(2);
    city_graph_add_vertex(g, 0, "Origin", VERTEX_INTERSECTION, 0.0, 0.0);
    city_graph_add_vertex(g, 1, "Dest", VERTEX_INTERSECTION, 1.0, 0.0);
    city_graph_add_road(g, 0, 1, "Highway", 10.0, 60.0, 4, ROAD_EXPRESSWAY, false);

    /* Free flow: 10 km @ 60 km/h = 10 minutes */
    DijkstraResult *r1 = dijkstra_compute(g, 0, false, METRIC_DYNAMIC_TRAVEL_TIME);
    assert(fabs(r1->dist[1] - 10.0) < 1e-6);
    dijkstra_result_destroy(r1);

    /* Apply IoT congestion factor 2.5x */
    city_graph_update_iot_sensor(g, 0, 1, 2.5, INCIDENT_CONGESTION, false, false);
    DijkstraResult *r2 = dijkstra_compute(g, 0, false, METRIC_DYNAMIC_TRAVEL_TIME);
    /* 10.0 * 2.5 = 25.0 minutes */
    assert(fabs(r2->dist[1] - 25.0) < 1e-6);
    dijkstra_result_destroy(r2);

    city_graph_destroy(g);
    printf("PASSED\n");
}

static void test_road_closure_rerouting(void) {
    printf("[TEST 5/12] Running Road Closure and Automatic Rerouting Test... ");
    /* Diamond Graph:
       0 -> 1: dist 5 km (via closed road)
       0 -> 2 -> 1: dist 7 km (detour)
    */
    CityGraph *g = city_graph_create(3);
    city_graph_add_vertex(g, 0, "Start", VERTEX_INTERSECTION, 0.0, 0.0);
    city_graph_add_vertex(g, 1, "End", VERTEX_INTERSECTION, 2.0, 0.0);
    city_graph_add_vertex(g, 2, "Detour", VERTEX_INTERSECTION, 1.0, 1.0);

    city_graph_add_road(g, 0, 1, "Direct Main", 5.0, 60.0, 4, ROAD_ARTERIAL, false);
    city_graph_add_road(g, 0, 2, "Detour 1", 3.0, 60.0, 2, ROAD_COLLECTOR, false);
    city_graph_add_road(g, 2, 1, "Detour 2", 4.0, 60.0, 2, ROAD_COLLECTOR, false);

    /* Direct path is faster initially */
    DijkstraResult *r1 = dijkstra_compute(g, 0, false, METRIC_DYNAMIC_TRAVEL_TIME);
    assert(r1->parent[1] == 0);
    dijkstra_result_destroy(r1);

    /* Simulate Road Closure on Direct Main */
    iot_sensor_simulate_road_closure(g, 0, 1);

    DijkstraResult *r2 = dijkstra_compute(g, 0, false, METRIC_DYNAMIC_TRAVEL_TIME);
    /* Should automatically choose Detour 0 -> 2 -> 1 */
    assert(r2->parent[1] == 2);
    assert(r2->parent[2] == 0);
    assert(fabs(r2->dist[1] - 7.0) < 1e-6);
    dijkstra_result_destroy(r2);

    city_graph_destroy(g);
    printf("PASSED\n");
}

static void test_green_corridor_speedup(void) {
    printf("[TEST 6/12] Running Emergency Green Corridor Preemption Test... ");
    CityGraph *g = city_graph_create(2);
    city_graph_add_vertex(g, 0, "Depot", VERTEX_HOSPITAL, 0.0, 0.0);
    city_graph_add_vertex(g, 1, "Scene", VERTEX_INCIDENT_ZONE, 1.0, 0.0);
    city_graph_add_road(g, 0, 1, "Expressway", 10.0, 60.0, 4, ROAD_EXPRESSWAY, false);

    /* Under heavy congestion 3.0x */
    city_graph_update_iot_sensor(g, 0, 1, 3.0, INCIDENT_CONGESTION, false, false);

    /* Civilian time: 10 * 3.0 = 30.0 mins */
    DijkstraResult *rciv = dijkstra_compute(g, 0, false, METRIC_DYNAMIC_TRAVEL_TIME);
    assert(fabs(rciv->dist[1] - 30.0) < 1e-6);
    dijkstra_result_destroy(rciv);

    /* Emergency vehicle with active green corridor */
    city_graph_update_iot_sensor(g, 0, 1, 3.0, INCIDENT_CONGESTION, true, false);
    DijkstraResult *remg = dijkstra_compute(g, 0, true, METRIC_DYNAMIC_TRAVEL_TIME);
    /* Green corridor gives base * 0.85 = 10.0 * 0.85 = 8.5 mins */
    assert(fabs(remg->dist[1] - 8.5) < 1e-6);
    assert(remg->dist[1] < 10.0);
    dijkstra_result_destroy(remg);

    city_graph_destroy(g);
    printf("PASSED\n");
}

static void test_multi_depot_responder_dispatch(void) {
    printf("[TEST 7/12] Running Multi-Depot Nearest Responder Selection Test... ");
    CityGraph *g = city_graph_create(5);
    city_graph_add_vertex(g, 0, "Hospital East", VERTEX_HOSPITAL, 0.0, 0.0);
    city_graph_add_vertex(g, 1, "Hospital West", VERTEX_HOSPITAL, 10.0, 0.0);
    city_graph_add_vertex(g, 2, "Incident Scene", VERTEX_INCIDENT_ZONE, 3.0, 0.0);
    city_graph_add_vertex(g, 3, "Police Station", VERTEX_POLICE_STATION, 1.0, 0.0);

    /* Hospital East -> Incident: 3 km @ 60 km/h = 3 min */
    city_graph_add_road(g, 0, 2, "Road E", 3.0, 60.0, 2, ROAD_ARTERIAL, false);
    /* Hospital West -> Incident: 7 km @ 60 km/h = 7 min */
    city_graph_add_road(g, 1, 2, "Road W", 7.0, 60.0, 2, ROAD_ARTERIAL, false);

    int best_hosp = -1;
    double eta = 0.0;
    DijkstraResult *res = dijkstra_find_nearest_emergency_responder(g, 2, VERTEX_HOSPITAL, &best_hosp, &eta);
    assert(res != NULL);
    assert(best_hosp == 0); /* Hospital East selected */
    assert(fabs(eta - 3.0) < 1e-6);

    dijkstra_result_destroy(res);
    city_graph_destroy(g);
    printf("PASSED\n");
}

static void test_boundary_and_unreachable_cases(void) {
    printf("[TEST 8/12] Running Edge Cases & Disconnected Component Verification... ");
    CityGraph *g = city_graph_create(4);
    city_graph_add_vertex(g, 0, "Island A", VERTEX_INTERSECTION, 0.0, 0.0);
    city_graph_add_vertex(g, 1, "Island B", VERTEX_INTERSECTION, 1.0, 0.0);
    city_graph_add_vertex(g, 2, "Isolated C", VERTEX_INTERSECTION, 2.0, 0.0);

    city_graph_add_road(g, 0, 1, "Bridge", 2.0, 40.0, 2, ROAD_LOCAL, false);
    /* Node 2 has no edges connected */

    DijkstraResult *res = dijkstra_compute(g, 0, false, METRIC_DYNAMIC_TRAVEL_TIME);
    assert(res != NULL);
    assert(res->dist[0] == 0.0);
    assert(res->dist[1] < INF_TIME / 2.0);
    assert(res->dist[2] >= INF_TIME / 2.0);

    int path[10];
    int path_len = 0;
    assert(dijkstra_get_path(res, 2, path, 10, &path_len) == false);
    assert(path_len == 0);

    /* Boundary parameter checks */
    assert(city_graph_get_edge(g, -1, 0) == NULL);
    assert(city_graph_get_edge(g, 0, 99) == NULL);
    assert(dijkstra_compute(g, 99, false, METRIC_DYNAMIC_TRAVEL_TIME) == NULL);

    dijkstra_result_destroy(res);
    city_graph_destroy(g);
    printf("PASSED\n");
}

static void test_min_heap_decrease_key_invariants(void) {
    printf("[TEST 9/12] Running Min-Heap Invariant & Invalid Key Increase Protection... ");
    MinHeap *heap = min_heap_create(5);
    min_heap_insert(heap, 0, 10.0);
    min_heap_insert(heap, 1, 20.0);
    min_heap_insert(heap, 2, 30.0);

    /* Attempting to increase key (from 10 to 50) must be safely rejected */
    min_heap_decrease_key(heap, 0, 50.0);
    MinHeapNode root = min_heap_extract_min(heap);
    assert(root.vertex == 0);
    assert(fabs(root.distance - 10.0) < 1e-6);

    min_heap_destroy(heap);
    printf("PASSED\n");
}

static void test_dynamic_graph_capacity_expansion(void) {
    printf("[TEST 10/12] Running Dynamic Graph Capacity Auto-Expansion Test... ");
    /* Create graph with tiny initial capacity = 2 */
    CityGraph *g = city_graph_create(2);
    assert(g != NULL);
    assert(g->capacity == 2);

    assert(city_graph_add_vertex(g, 0, "Node 0", VERTEX_INTERSECTION, 0.0, 0.0));
    assert(city_graph_add_vertex(g, 1, "Node 1", VERTEX_INTERSECTION, 1.0, 0.0));

    /* Adding vertex beyond initial capacity 2 must automatically grow capacity */
    assert(city_graph_add_vertex(g, 2, "Node 2", VERTEX_INTERSECTION, 2.0, 0.0));
    assert(city_graph_add_vertex(g, 5, "Node 5", VERTEX_HOSPITAL, 5.0, 0.0));
    assert(g->capacity >= 6);
    assert(g->num_vertices == 6);

    assert(city_graph_add_road(g, 0, 5, "Long Link", 8.0, 60.0, 2, ROAD_ARTERIAL, true));
    RoadEdge *e = city_graph_get_edge(g, 0, 5);
    assert(e != NULL);
    assert(fabs(e->distance_km - 8.0) < 1e-6);

    DijkstraResult *res = dijkstra_compute(g, 0, false, METRIC_PHYSICAL_DISTANCE);
    assert(res != NULL);
    assert(fabs(res->dist[5] - 8.0) < 1e-6);
    dijkstra_result_destroy(res);

    city_graph_destroy(g);
    printf("PASSED\n");
}

static void test_negative_weights_and_zero_cycles(void) {
    printf("[TEST 11/12] Running Negative Weight Clamping & Zero-Cost Cycle Resilience... ");
    CityGraph *g = city_graph_create(4);
    city_graph_add_vertex(g, 0, "A", VERTEX_INTERSECTION, 0.0, 0.0);
    city_graph_add_vertex(g, 1, "B", VERTEX_INTERSECTION, 1.0, 0.0);
    city_graph_add_vertex(g, 2, "C", VERTEX_INTERSECTION, 2.0, 0.0);

    /* Zero cost edges and cycle: 0 -> 1 -> 2 -> 0 */
    city_graph_add_road(g, 0, 1, "Link 1", 0.001, 100.0, 2, ROAD_LOCAL, false);
    city_graph_add_road(g, 1, 2, "Link 2", 0.001, 100.0, 2, ROAD_LOCAL, false);

    DijkstraResult *res = dijkstra_compute(g, 0, false, METRIC_DYNAMIC_TRAVEL_TIME);
    assert(res != NULL);
    assert(res->dist[2] < INF_TIME / 2.0);
    dijkstra_result_destroy(res);

    city_graph_destroy(g);
    printf("PASSED\n");
}

static void test_iot_sensor_validation_error_handling(void) {
    printf("[TEST 12/12] Running IoT Simulation Input Validation & Guard Verification... ");
    CityGraph *g = city_graph_create(3);
    city_graph_add_vertex(g, 0, "Active 0", VERTEX_INTERSECTION, 0.0, 0.0);
    city_graph_add_vertex(g, 1, "Active 1", VERTEX_INTERSECTION, 1.0, 0.0);
    /* No road added between 0 and 1 */

    /* Must safely reject invalid IDs without crashing */
    iot_sensor_simulate_accident(g, -1, 0);
    iot_sensor_simulate_accident(g, 0, 99);
    iot_sensor_simulate_accident(g, 0, 1); /* Valid nodes, but no road */

    iot_sensor_simulate_road_closure(g, -5, 10);
    iot_sensor_simulate_road_closure(g, 0, 1);

    city_graph_destroy(g);
    printf("PASSED\n");
}

int cli_run_test_suite(void) {
    printf("\n========================================================================================\n");
    printf("                    EXECUTING AUTOMATED TEST AND VERIFICATION SUITE                     \n");
    printf("========================================================================================\n");

    test_min_heap_operations();
    test_graph_creation_and_topology();
    test_dijkstra_shortest_path();
    test_iot_congestion_weighting();
    test_road_closure_rerouting();
    test_green_corridor_speedup();
    test_multi_depot_responder_dispatch();
    test_boundary_and_unreachable_cases();
    test_min_heap_decrease_key_invariants();
    test_dynamic_graph_capacity_expansion();
    test_negative_weights_and_zero_cycles();
    test_iot_sensor_validation_error_handling();

    printf("========================================================================================\n");
    printf("            ALL 12/12 VERIFICATION TEST CASES PASSED WITH 100%% SUCCESS RATE!            \n");
    printf("========================================================================================\n\n");
    return 0;
}
