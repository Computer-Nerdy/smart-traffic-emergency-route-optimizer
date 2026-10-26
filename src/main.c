/**
 * @file main.c
 * @brief Main Entry Point for Smart Traffic & Emergency Route Optimization System.
 * 
 * Course: 21CSC201J - Data Structures and Algorithms
 * Institution: SRM Institute of Science and Technology, Ramapuram
 * Department: Computer Science and Business Systems (CSBS-IOT/SCSE)
 * Semester: III Semester / II Year, October 2026
 */

#include "cli.h"
#include "city_graph.h"
#include "iot_sensor.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    /* If test mode requested */
    if (argc > 1 && (strcmp(argv[1], "--test") == 0 || strcmp(argv[1], "-t") == 0)) {
        return cli_run_test_suite();
    }

    CityGraph *city = city_graph_create(50);
    if (!city) {
        fprintf(stderr, "Fatal Error: Failed to allocate city road network graph memory.\n");
        return 1;
    }

    /* Initialize with SRM Ramapuram Smart Network */
    iot_sensor_init_srm_smart_network(city);

    if (argc > 1 && (strcmp(argv[1], "--demo") == 0 || strcmp(argv[1], "-d") == 0)) {
        cli_run_automated_demo(city);
        city_graph_destroy(city);
        return 0;
    }

    if (argc > 1 && (strcmp(argv[1], "--all") == 0 || strcmp(argv[1], "-a") == 0)) {
        cli_run_test_suite();
        cli_run_automated_demo(city);
        city_graph_destroy(city);
        return 0;
    }

    if (argc > 1 && (strcmp(argv[1], "--interactive") == 0 || strcmp(argv[1], "-i") == 0)) {
        cli_run_interactive_menu(city);
        city_graph_destroy(city);
        return 0;
    }

    /* Default when executed without arguments: Run test suite & automated demonstration */
    cli_run_test_suite();
    cli_run_automated_demo(city);

    city_graph_destroy(city);
    return 0;
}
