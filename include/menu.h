#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "../include/tsp.h"
#include "../include/parser.h"

/**
 * @brief 
 * 
 * @param inst A pointer to the instance structure to be initialized.
 * 
 * @return
 */
int manage_menu(instance *inst);

/**
 * @brief 
 */
void clearScreen();

/**
 * @brief 
 * 
 * @param file_name
 * @param n
 * 
 * @return
 */
int file_menu(char *file_name, int *n);

/**
 * @brief 
 * 
 * @param algorithm_name
 * 
 * @return
 */
int algorithm_menu(char *algorithm_name);

/**
 * @brief 
 * 
 * @param opt_name
 * 
 * @return
 */
int optimization_menu(char *opt_name);

/**
 * @brief 
 * 
 * @param verbose
 * @param time_limit
 * @param random_seed
 * @param show_gnuplot
 * 
 * @return
 */
int options_menu(int *verbose, double *time_limit, int *random_seed, int *show_gnuplot);

/**
 * @brief 
 * 
 * @return
 */
int getTerminalWidth();

/**
 * @brief 
 * 
 * @param text
 * @param c
 */
void printCentered(const char *text, char c);

/**
 * @brief 
 * 
 * @param c
 */
void printHorizontalLine(char c);

/**
 * @brief 
 */
void welcomeMessage();

/**
 * @brief 
 * 
 * @param type
 */
void showHelpMenu(int type);

/**
 * @brief 
 */
void quitMessage();

#endif /* MENU_H */