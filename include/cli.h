/**
 * @file cli.h
 * @brief Command Line Interface and Demonstration Controller.
 * 
 * Course: 21CSC201J - Data Structures and Algorithms
 * Institution: SRM Institute of Science and Technology, Ramapuram
 * Project: Smart Traffic & Emergency Route Optimization using Graphs & Dijkstra's Algorithm
 */

#ifndef CLI_H
#define CLI_H

#include "city_graph.h"
#include <stdbool.h>

/**
 * @brief Launches the interactive CLI menu for user experimentation.
 * @param g CityGraph pointer
 */
void cli_run_interactive_menu(CityGraph *g);

/**
 * @brief Executes a comprehensive, scripted smart city demonstration.
 * @param g CityGraph pointer
 */
void cli_run_automated_demo(CityGraph *g);

/**
 * @brief Executes rigorous automated verification test suite.
 * @return 0 if all tests pass, non-zero otherwise.
 */
int cli_run_test_suite(void);

#endif /* CLI_H */
