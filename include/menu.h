#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "tsp.h"
#include "parser.h"

/**
 * @brief Manages the main menu for initializing instance parameters.
 * 
 * This function initializes various parameters of the instance structure 
 * based on user inputs through menus.
 * 
 * @param inst A pointer to the instance structure to be initialized.
 * @return Returns 0 upon successful execution.
 */
int manage_menu(instance *inst);

/**
 * @brief Clears the console screen.
 * 
 * This function clears the console screen, providing a clean 
 * display for output.
 */
void clearScreen();

/**
 * @brief Manages the file menu for selecting input options.
 * 
 * This function presents a menu for selecting input options, allowing 
 * the user to choose between providing a file path or generating a 
 * random input file with a specified number of points.
 * 
 * @param file_name The name of the input file to be selected.
 * @param n A pointer to an integer representing the number of points 
 * for a generated input file.
 * @return Returns 0 upon successful execution.
 */
int file_menu(char *file_name, int *n);

/**
 * @brief Manages the algorithm selection menu.
 * 
 * This function presents a menu for selecting the algorithm to be 
 * applied to the file.
 * 
 * @param algorithm_name A pointer to a character array where the 
 * selected algorithm name will be stored.
 * @return Returns 0 upon successful execution.
 */
int algorithm_menu(char *algorithm_name);

/**
 * @brief Manages the optimization method selection menu.
 * 
 * This function presents a menu for selecting the optimizer 
 * to be applied to the solution found by the algorithm.
 * 
 * @param opt_name A pointer to a character array where the 
 * selected optimizer name will be stored.
 * @return Returns 0 upon successful execution.
 */
int optimization_menu(char *opt_name);

/**
 * @brief Manages the options menu for configuring various parameters.
 * 
 * This function presents a menu for configuring options such as verbosity level, time limit, 
 * random seed, and whether to show with gnuplot.
 * 
 * @param verbose A pointer to an integer representing the verbosity level (0 to 100).
 * @param time_limit A pointer to a double representing the time limit.
 * @param random_seed A pointer to an integer representing the random seed.
 * @param show_gnuplot A pointer to an integer indicating whether to show with gnuplot.
 * @return Returns 0 upon successful execution.
 */
int options_menu(int *verbose, double *time_limit, int *random_seed, int *show_gnuplot);

/**
 * @brief Retrieves the width of the terminal window.
 * 
 * This function determines the width of the terminal window in characters.
 * 
 * @return Returns the width of the terminal window.
 */
int getTerminalWidth();

/**
 * @brief Prints the given text centered within the terminal window, 
 * surrounded by the specified character.
 * 
 * This function prints the provided text centered within the terminal window, 
 * with the surrounding character repeated to fill the remaining space on 
 * both sides of the text.
 * 
 * @param text The text to be printed.
 * @param c The character used to surround the text.
 */
void printCentered(const char *text, char c);

/**
 * @brief Prints a horizontal line using the specified character 
 * across the terminal window.
 * 
 * This function prints a horizontal line across the terminal 
 * window using the provided character.
 * 
 * @param c The character used to draw the horizontal line.
 */
void printHorizontalLine(char c);

/**
 * @brief Displays the welcome message for the TSP optimization program.
 * 
 * This function displays the welcome message for the TSP optimization 
 * program, including authors' information, lecturer's name, and academic year.
 */
void welcomeMessage();

/**
 * @brief Displays the help menu for providing additional information
 * based on the specified type.
 * 
 * This function displays a help menu providing additional information 
 * based on the specified type, which corresponds to different
 * sections of the program.
 * 
 * @param type An integer representing the type of help menu to be displayed.
 */
void showHelpMenu(int type);

/**
 * @brief Displays a message indicating that the program has been closed.
 * 
 * This function clears the screen, displays a farewell message, 
 * and exits the program.
 */
void quitMessage();

#endif /* MENU_H */