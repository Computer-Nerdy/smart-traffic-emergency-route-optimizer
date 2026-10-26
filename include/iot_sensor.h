/**
 * @file iot_sensor.h
 * @brief IoT Sensor Simulation and Smart City Network Initialization.
 * 
 * Course: 21CSC201J - Data Structures and Algorithms
 * Institution: SRM Institute of Science and Technology, Ramapuram
 * Project: Smart Traffic & Emergency Route Optimization using Graphs & Dijkstra's Algorithm
 */

#ifndef IOT_SENSOR_H
#define IOT_SENSOR_H

#include "city_graph.h"

/**
 * @brief Pre-configured landmarks and vertices in the SRM Ramapuram Smart Network.
 */
#define NODE_SRM_RAMAPURAM          0
#define NODE_RAMAPURAM_JN           1
#define NODE_DLF_IT_PARK            2
#define NODE_PORUR_ROUNDTANA        3
#define NODE_MIOT_HOSPITAL          4
#define NODE_KATHIPARA_FLYOVER      5
#define NODE_GUINDY_METRO           6
#define NODE_ASHOK_NAGAR_FIRE       7
#define NODE_VADAPALANI_SIGNAL      8
#define NODE_JAFFERKHANPET_BRIDGE   9
#define NODE_KOYAMBEDU_HUB         10
#define NODE_SIMS_HOSPITAL         11
#define NODE_GUINDY_POLICE_HQ      12
#define NODE_RAMAPURAM_HEALTH_CTR  13
#define NODE_MANAPAKKAM_CORRIDOR   14

#define TOTAL_SRM_PRESET_NODES     15

/**
 * @brief Builds the complete SRM Ramapuram & Chennai Smart Corridor graph network.
 * @param g CityGraph pointer (allocated with capacity >= 15)
 * @return true on success, false on error.
 */
bool iot_sensor_init_srm_smart_network(CityGraph *g);

/**
 * @brief Simulates peak morning commuter rush hour (heavy tech corridor traffic).
 * @param g CityGraph pointer
 */
void iot_sensor_simulate_morning_rush(CityGraph *g);

/**
 * @brief Simulates Chennai monsoon waterlogging on low-lying arterial roads.
 * @param g CityGraph pointer
 */
void iot_sensor_simulate_monsoon_flood(CityGraph *g);

/**
 * @brief Simulates a major multi-vehicle accident on a critical expressway/arterial road.
 * @param g CityGraph pointer
 * @param src Origin intersection of road
 * @param dest Destination intersection of road
 */
void iot_sensor_simulate_accident(CityGraph *g, int src, int dest);

/**
 * @brief Simulates complete road closure (e.g. metro pipeline burst / VIP security closure).
 * @param g CityGraph pointer
 * @param src Origin intersection of road
 * @param dest Destination intersection of road
 */
void iot_sensor_simulate_road_closure(CityGraph *g, int src, int dest);

/**
 * @brief Resets all IoT sensors across the city to free-flow baseline conditions.
 * @param g CityGraph pointer
 */
void iot_sensor_reset_to_free_flow(CityGraph *g);

#endif /* IOT_SENSOR_H */
