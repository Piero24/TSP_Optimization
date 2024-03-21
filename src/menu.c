#include "../include/menu.h"

int manage_menu(instance *inst)
{
    int n = 0;
    // default   
    strcpy(inst->algorithm_name, "NULL");
    strcpy(inst->opt_name, "NULL");
    inst->show_gnuplot = 0;
	inst->old_benders = 0;
	strcpy(inst->input_file, "NULL");
	inst->random_seed = 93846529; 
	inst->num_threads = 0;
	inst->time_limit = DBL_MAX; 	//CPX_INFBOUND
	inst->cutoff = DBL_MAX; 	//CPX_INFBOUND
	inst->integer_costs = 0;
	inst->verbose = 0;

	inst->available_memory = 12000;   	// available memory, in MB, for Cplex execution (e.g., 12000)
	inst->max_nodes = -1; 				// max n. of branching nodes in the final run (-1 unlimited)        

    
    file_menu(inst->input_file, &n);
    clearScreen();

    algorithm_menu(inst->algorithm_name);
    clearScreen();

    optimization_menu(inst->opt_name);
    clearScreen();

    options_menu(&inst->verbose, &inst->time_limit, &inst->random_seed, &inst->show_gnuplot);
    clearScreen();

    if (n > 0) 
    {
        char* name = fileGenerator(n);
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
            printCentered(error_info);
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
        printf("3 - Variable Neighborhood Search\n");
        printf("\n9 - Show help menu\n");
        printf("0 - Exit\n");
        
        if (err_flag)
        {
            printf("\n");
            char error_info[] = "ERROR: Invalid input. Add a valid input.";
            printCentered(error_info);
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
                strcpy(algorithm_name, "Variable Neighborhood Search");
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
        printf("2 - 2opt\n");
        printf("3 - Tabù (+ 2opt)\n");
        printf("\n9 - Show help menu\n");
        printf("0 - Exit\n");

        if (err_flag)
        {
            printf("\n");
            char error_info[] = "ERROR: Invalid input. Add a valid input.";
            printCentered(error_info);
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
                strcpy(opt_name, "2opt");
                return 0;

            case 3:
                strcpy(opt_name, "Tabù (+ 2opt)");
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
        printf("\nCHOOSE ONE OR MORE OPTIONS\n");
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
            printCentered(error_info);
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
        return csbi.dwSize.X;
    #else
        struct winsize w;
        ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
        return w.ws_col;
    #endif
}

void printCentered(const char *text)
{
    int termWidth = getTerminalWidth();
    int textWidth = strlen(text);
    int leadingSpaces = (termWidth - textWidth) / 2;

    for (int i = 0; i < leadingSpaces; i++)
    {
        putchar(' ');
    }
    printf("%s", text);
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
    printHorizontalLine('-');
    char welcome_message[] = "WELCOME TO THE TSP OPTIMIZATION PROGRAM";
    printCentered(welcome_message);
    printf("\n");
    printHorizontalLine('-');
    //printf("\n");

    char info[] = "Authors: A. Pietrobon, A. Felline     Lecturer: M. Fischetti";
    printCentered(info);
    printf("\n\n");

	char info_2[] = "Operation Research 2 - A.Y. 2023-2024";
    printCentered(info_2);
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
    printCentered(helpMessage);
    printf("\n");

    char infoHelp[] = "Here you can find a more detailed response to your questions.";
    printCentered(infoHelp);
    printf("\n\n");
    
    switch (type)
    {
    case 1:
        // ! Add help menu for file_menu
        /* code */
        break;

    case 2:
        // ! Add help menu for algorithm_menu
        /* code */
        break;

    case 3:
        // ! Add help menu for optimization_menu
        /* code */
        break;
    
    case 4:
        // ! Add help menu for options_menu
        /* code */
        break;
    
     case 5:
        // ! Add help menu for the parser
        /* code */
        break;

    default:
        break;
    }

    printf("Press ENTER to continue: ");
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
    printCentered(quit_info);
    printf("\n\n");

    exit(0);
}