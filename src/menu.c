#include "menu.h"


int manageMenu(instance *inst)
{
    int n = 0;
    
    // default   
    initialization(inst);
    
    file_menu(inst->input_file, &n);
    clearScreen();

    algorithm_menu(inst->algorithm_name);
    clearScreen();

    optimization_menu(inst->opt_name);
    clearScreen();

    options_menu(&inst->verbose, &inst->time_limit, &inst->random_seed, &inst->show_gnuplot);
    srand(inst->random_seed);
    clearScreen();

    if (n > 0) 
    {
        char* name = fileGenerator(n, inst->random_seed);
        strcpy(inst->input_file, name);
        // Free the dynamically allocated memory
        free(name);
    }

    parameterPrint(inst);
    //! Why if free the pointer t_limit the program crashes?
    //! TSP_Optimization(84625,0x1d94e5c40) malloc: *** error for object 0x1046533f4: pointer being freed was not allocated
    //! TSP_Optimization(84625,0x1d94e5c40) malloc: *** set a breakpoint in malloc_error_break to debug
    //! zsh: abort      ./TSP_Optimization
    // free(t_limit);
    return 0;
}

void clearScreen() {
    #ifdef _WIN32
        system("cls"); // For Windows
    #else
        system("clear"); // For Unix/Linux
    #endif
}

int file_menu(char *file_name, int *n)
{
    char input[200];
    char *token;
    int i = 0;
    char *input_array[2];
    int err_flag = 0;

    while (1) 
    {   
        clearScreen();
        welcomeMessage();
        printf("\nCHOOSE INPUT FILE\n");
        printf("Example: If you want to use your own file \"-f Resource/pr1000v1.tsp\" or if you want to generate a file \"-g 1257\"\n\n");
        printf("-> (-f) Insert input file path\n");
        printf("-> (-g) Generate a random input file with n points\n");
        printf("-> (-h) Show help menu\n");
        printf("-> (-q) Exit\n");

        if (err_flag)
        {
            printf("\n");
            char error_info[] = "ERROR: Invalid input. Add a valid input.";
            printCentered(error_info, ' ');
            printf("\n");
            err_flag = 0;
        }
        printf("\nEnter your choice: ");

        fgets(input, 200, stdin);
        input[strlen(input) - 1] = '\0';
        token = strtok(input, " ");

        while (token != NULL)
        {
            input_array[i++] = token;
            token = strtok (NULL, " ");
        }

        if (strstr(input_array[0], "-f") != NULL) 
        {
            strcpy(file_name, input_array[1]);
            break;

        } else if (strstr(input_array[0], "-g") != NULL) 
        {
            if (atoi(input_array[1]) != 0) 
            {
                *n = atoi(input_array[1]);
            }
            break;

        } else if (strstr(input_array[0], "-h") != NULL) 
        {
            showHelpMenu(1);

        } else if (strstr(input_array[0], "-q") != NULL) 
        {
            quitMessage();

        } else 
        {
            err_flag = 1;
        }
    }
    return 0;
}

int algorithm_menu(char *algorithm_name)
{
    int err_flag = 0;

    while (1) 
    {
        clearScreen();
        welcomeMessage();
        printf("\nCHOOSE THE ALGORITHM\n");
        printf("Select the algorithm you want to apply to the file.\n\n");
        
        printf("1 - Random\n");
        printf("2 - Nearest Neighbor\n");
        printf("3 - Cplex\n");
        printf("\n9 - Show help menu\n");
        printf("0 - Exit\n");
        
        if (err_flag)
        {
            printf("\n");
            char error_info[] = "ERROR: Invalid input. Add a valid input.";
            printCentered(error_info, ' ');
            printf("\n");
            err_flag = 0;
        }

        printf("\nEnter your choice: ");

        int choice;
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n'); // Clear input buffer
            err_flag = 1;
        }

        switch (choice)
        {
            case 1:
                strcpy(algorithm_name, "Random");
                return 0;

            case 2:
                strcpy(algorithm_name, "Nearest Neighbor");
                return 0;
            
            case 3:
                strcpy(algorithm_name, "CPLEX");
                return 0;
            
            case 4:
                strcpy(algorithm_name, "Benders' Loop");
                return 0;
            
            case 5:
                strcpy(algorithm_name, "Glued Benders' Loop");
                return 0;

            case 9:
                while (getchar() != '\n'); 
                showHelpMenu(2);
                break;

            case 0:
                quitMessage();
                break;

            default:
                err_flag = 1;
        }
    }
    return 0;
}

int optimization_menu(char *opt_name)
{
    int err_flag = 0;
    
    while (1) 
    {
        clearScreen();
        welcomeMessage();
        printf("\nCHOOSE THE OPTIMIZATION METHOD\n");
        printf("Select the optimizer you want to apply to the solution found by the algorithm.\n\n");
        printf("1 - None\n");
        printf("2 - 2-Opt\n");
        printf("3 - Tabu Search (+ 2-Opt)\n");
        printf("4 - VNS\n");
        printf("\n9 - Show help menu\n");
        printf("0 - Exit\n");

        if (err_flag)
        {
            printf("\n");
            char error_info[] = "ERROR: Invalid input. Add a valid input.";
            printCentered(error_info, ' ');
            printf("\n");
            err_flag = 0;
        }

        printf("\nEnter your choice: ");
        
        int choice;
        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n'); // Clear input buffer
            err_flag = 1;
        }

        switch (choice)
        {
            case 1:
                strcpy(opt_name, "None");
                return 0;

            case 2:
                strcpy(opt_name, "2-Opt");
                return 0;

            case 3:
                strcpy(opt_name, "Tabu Search");
                return 0;
            
            case 4:
                strcpy(opt_name, "Variable Neighborhood Search");
                return 0;

            case 9:
                while (getchar() != '\n'); 
                showHelpMenu(3);
                break;

            case 0:
                quitMessage();
                break;

            default:
                err_flag = 1;
        }
    }
    return 0;
}

int options_menu(int *verbose, double *timeLimit, int *randomSeed, int *showGnuplot)
{
    char input[500];
    char *token;
    int i = 0;
    char *input_array[10];
    int err_flag = 0;

    // Clear input buffer
    while (getchar() != '\n'); 

    while (1)
    {
        int h = 0;

        clearScreen();
        welcomeMessage();
        printf("\nCHOOSE ONE OR MORE OPTIONS (Optionals)\n");
        printf("WARNING: you have to put one or all the options (e.g., -v 60 -t 25000 -r 5432 -s 3), the algorithm will start after you press ENTER\n\n");
        printf("-> (-v) Verbose from 0 to 100\n");
        printf("-> (-t) Time limit\n");
        printf("-> (-r) Random seed\n");
        printf("-> (-s) Show with gnuplot (\"-s 0\" to show without slowing down, \"-s 2\" to show with slowing down of 2 sec)\n");
        printf("-> (-h) Show help menu\n");
        printf("-> (-q) Exit\n\n");

        if (err_flag)
        {
            printf("\n");
            char error_info[] = "ERROR: Invalid input. Add a valid input.";
            printCentered(error_info, ' ');
            printf("\n");
            err_flag = 0;
        }

        printf("Enter your choice: ");

        fgets(input, 500, stdin);
        input[strlen(input) - 1] = '\0';
        token = strtok(input, " ");

        while (token != NULL)
        {
            input_array[i++] = token;
            token = strtok (NULL, " ");
        }

        if (i == 1) {i = 2;}

        for (int j = 0; j < i-1; j++)
        {
            if (strstr(input_array[j], "-v") != NULL)
            {
                *verbose = atoi(input_array[j+1]);

            } else if (strstr(input_array[j], "-t") != NULL)
            {
                *timeLimit = atoi(input_array[j+1]);

            } else if (strstr(input_array[j], "-r") != NULL)
            {
                *randomSeed = atoi(input_array[j+1]);

            } else if (strstr(input_array[j], "-s") != NULL)
            {
                *showGnuplot = atoi(input_array[j+1]);

            } else if (strstr(input_array[j], "-h") != NULL)
            {
                showHelpMenu(4);
                h = 1;
                break;

            } else if (strstr(input_array[j], "-q") != NULL)
            {
                quitMessage();
            }
        }

        if (h == 0)
        {
           return 0;  
        }
    }
}

int getTerminalWidth()
{
    #ifdef _WIN32
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        return csbi.srWindow.Right - csbi.srWindow.Left + 1;
    #else
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        return w.ws_col;
    #endif
}

void printCentered(const char *text, char c)
{
    int termWidth = getTerminalWidth();
    int textWidth = strlen(text);
    int leadingSpaces = (termWidth - textWidth - 2) / 2; // Subtracting 2 for the characters on both sides

    // Print leading characters
    for (int i = 0; i < leadingSpaces; i++)
    {
        putchar(c);
    }

    // Print text
    printf("%s", text);

    if (c != ' ')
    {
        // Print trailing characters
        for (int i = 0; i < leadingSpaces; i++)
        {
            putchar(c);
        }

        // If the width is odd, print an additional trailing character
        if ((termWidth - textWidth - 2) % 2 != 0)
        {
            putchar(c);
        }
    }
}

void printHorizontalLine(char c)
{
    int termWidth = getTerminalWidth();
    for (int i = 0; i < termWidth; i++)
    {
        putchar(c);
    }
    putchar('\n');
}

void welcomeMessage()
{
    //printHorizontalLine('-');
    char welcome_message[] = "   WELCOME TO THE TSP OPTIMIZATION PROGRAM   ";
    printCentered(welcome_message , '-');
    printf("\n\n");
    //printHorizontalLine('-');
    //printf("\n");

    char info[] = "Authors: A. Pietrobon, A. Felline     Lecturer: M. Fischetti";
    printCentered(info, ' ');
    printf("\n\n");

	char info_2[] = "Operation Research 2 - A.Y. 2023-2024";
    printCentered(info_2 , ' ');
    printf("\n");

    printHorizontalLine('-');
}

void showHelpMenu(int type)
{
    char input[500];

    clearScreen();
    welcomeMessage();
    printf("\n");

    char helpMessage[] = "HELP MENU";
    printCentered(helpMessage, ' ');
    printf("\n");

    char infoHelp[] = "Here you can find a more detailed response to your questions.";
    printCentered(infoHelp, ' ');
    printf("\n\n");
    
    switch (type)
    {
    case 1:
        printf("\nINPUT FILE HELP MENU'\n");
        printf("There are 2 possible parameters that are accepted:\n\n");
        printf("-> (-f) When you already have a .tsp file you can use this to apply the algorithm to the file. (e.g. -f Resource/pr1002.tsp)\n");
        printf("-> (-g) If you don't have a file .tsp you can generate a file with a custom number of nodes. (e.g. -g 1500)\n");
        break;

    case 2:
        printf("\nALGORITHM HELP MENU'\n");
        printf("Select the algorithm you want to apply to the file:\n\n");
        printf("1 - Random: Simply move 1/4 of the nodes in a random way.\n");
        printf("2 - Nearest Neighbor: Select first the node with a lowest cost. Where the cost is the distance from the current node the next nodes.\n");
        printf("3 - Cplex: Use of Cplex library\n");
        break;

    case 3:
        printf("\nOPTIMIZATION METHOD HELP MENU'\n");
        printf("Select the optimizer you want to apply to the solution found by the algorithm:\n\n");
        printf("1 - None (default): No optimization it going to apply.\n");
        printf("2 - 2-Opt: Swap the nodes on the path that have a crossing pair of arcs.\n");
        printf("3 - Tabu Search (+ 2-Opt): After 2-Opt it perform a research in the neighborhood of the solution to find a better solution.\n");
        printf("4 - VNS: Perform a research in the neighborhood of the solution to find a better solution.\n");
        break;
    
    case 4:
        printf("\nOPTIONS HELP MENU' (Optionals)\n");
        printf("More options to see more information about what the algorithms and the optimization methods do:\n\n");
        printf("-> (-v) Verbose (from 0 to 100): Show more info during the execution like the swapping of nodes and the current best solution found. (-v 60)\n");
        printf("-> (-t) Time limit: For prevent the algorithm to run for an infinite amount of time (-t 600) in seconds. If stop it return the best solution found until that moment.\n");
        printf("-> (-r) Random seed: Accept a personalized random seed for the generation of the nodes. (-r 1234)\n");
        printf("-> (-s) Show GnuPlot: If activate show the real time plot execution of the algorithm graphically (-s 0: to show without slowing down, -s 2: show with slowing down of 2 sec)\n");
        break;
    
     case 5:
        printf("\nINLINE PARSER HELP MENU'\n");
        printf("-> Input Files: -f: to use your own file (e.g. -f Resource/pr1020.tsp), -g to generate a new file with N nodes. (e.g. -g 1000)\n");
        printf("-> Possible Algorithms: 1: Random, 2: Nearest Neighbor 3: Cplex (e.g. -algo 2)\n");
        printf("-> Possible Optimization Methods: 1: None, 2: 2-Opt, 3: Tabu Search (+ 2-Opt), 4: VNS. (e.g. -opt 2)\n");
        printf("-> Other Parameters (Optionals): -v: (from 0 to 100) show more details (-v 60), -t: time limit (-t 600), -r: personal seed (-r 579), -s: show real time plot (-s 5)\\nn");
        printf("Example: ./TSP_Optimization -file Resource/pr10v1.tsp -model 1 -opt 1 -v 50 -t 120\n");
        exit(0);

    default:
        break;
    }

    printf("Press ENTER to return: ");
    // Clear input buffer
    // while (getchar() != '\n'); 
    fgets(input, 500, stdin);
}

void quitMessage()
{
    clearScreen();
    welcomeMessage();

    printf("\n");
    char quit_info[] = "Program closed correctly.";
    printCentered(quit_info, ' ');
    printf("\n\n");

    exit(0);
}