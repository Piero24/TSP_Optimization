#include "../include/parser.h"

int parse_args(int argc, char** argv, instance* inst)
{
	int n = 0;
	int help = 0;

	// default   
	strcpy(inst->algorithm_name, "NULL");
    strcpy(inst->opt_name, "NULL");
	inst->show_gnuplot = 0;

	inst->old_benders = 0;
	strcpy(inst->input_file, "NULL");
	inst->random_seed = 93846529; 
	inst->num_threads = 0;
	inst->time_limit = DBL_MAX; 	//! CPX_INFBOUND
	inst->cutoff = DBL_MAX; 		//! CPX_INFBOUND
	inst->integer_costs = 0;
	inst->verbose = 0;

	inst->available_memory = 12000;   	// available memory, in MB, for Cplex execution (e.g., 12000)
	inst->max_nodes = -1; 				// max n. of branching nodes in the final run (-1 unlimited)        

	int model_type = 0;
	int opt_type = 0;

	for ( int i = 1; i < argc; i++ ) 
	{ 
		if ( strcmp(argv[i],"-file") == 0 ) { strcpy(inst->input_file,argv[++i]); continue; } 					// input file
		if ( strcmp(argv[i],"-input") == 0 ) { strcpy(inst->input_file,argv[++i]); continue; } 					// input file
		if ( strcmp(argv[i],"-f") == 0 ) { strcpy(inst->input_file,argv[++i]); continue; } 						// input file
		
        if ( strcmp(argv[i],"-time_limit") == 0 ) { inst->time_limit = atof(argv[++i]); continue; }				// total time limit
		if ( strcmp(argv[i],"-tl") == 0 ) { inst->time_limit = atof(argv[++i]); continue; }		        		// total time limit
		if ( strcmp(argv[i],"-t") == 0 ) { inst->time_limit = atof(argv[++i]); continue; }		        		// total time limit

        if ( strcmp(argv[i],"-model_type") == 0 ) { model_type = atoi(argv[++i]); continue; } 					// model type
		if ( strcmp(argv[i],"-model") == 0 ) { model_type = atoi(argv[++i]); continue; } 						// model type
		if ( strcmp(argv[i],"-alg") == 0 ) { model_type = atoi(argv[++i]); continue; }							// model type
		
		if ( strcmp(argv[i],"-opt") == 0 ) { opt_type = atoi(argv[++i]); continue; }							// optimization type
		if ( strcmp(argv[i],"-2opt") == 0 ) { opt_type = atoi(argv[++i]); continue; }							// optimization type
		if ( strcmp(argv[i],"-optimization") == 0 ) { opt_type = atoi(argv[++i]); continue; }					// optimization type

		if ( strcmp(argv[i],"-seed") == 0 ) { inst->random_seed += abs(atoi(argv[++i])); continue; } 			// random seed
		if ( strcmp(argv[i],"-r") == 0 ) { inst->random_seed += abs(atoi(argv[++i])); continue; } 				// random seed

		if ( strcmp(argv[i],"-verbose") == 0 ) { inst->verbose = abs(atoi(argv[++i])); continue; } 				// verbose
		if ( strcmp(argv[i],"-v") == 0 ) { inst->verbose = abs(atoi(argv[++i])); continue; } 					// verbose
		
		if ( strcmp(argv[i],"-generate") == 0 ) { n = abs(atoi(argv[++i])); continue; } 						// generated file
		if ( strcmp(argv[i],"-g") == 0 ) { n = abs(atoi(argv[++i])); continue; } 								// generated file
		if (n > 0) {char* name = fileGenerator(n); strcpy(inst->input_file, name); free(name); continue;}
		
		if ( strcmp(argv[i],"-show") == 0 ) { inst->show_gnuplot = abs(atoi(argv[++i])); continue; } 			// generated file
		if ( strcmp(argv[i],"-plot") == 0 ) { inst->show_gnuplot = abs(atoi(argv[++i])); continue; } 			// generated file
		if ( strcmp(argv[i],"-gnuplot") == 0 ) { inst->show_gnuplot = abs(atoi(argv[++i])); continue; } 		// generated file
		if ( strcmp(argv[i],"-s") == 0 ) { inst->show_gnuplot = abs(atoi(argv[++i])); continue; } 				// generated file

		// if ( strcmp(argv[i],"-old_benders") == 0 ) { inst->old_benders = atoi(argv[++i]); continue; } 		// old benders
		// if ( strcmp(argv[i],"-threads") == 0 ) { inst->num_threads = atoi(argv[++i]); continue; } 			// n. threads
		// if ( strcmp(argv[i],"-memory") == 0 ) { inst->available_memory = atoi(argv[++i]); continue; }		// available memory (in MB)
		// if ( strcmp(argv[i],"-node_file") == 0 ) { strcpy(inst->node_file,argv[++i]); continue; }			// cplex's node file
		// if ( strcmp(argv[i],"-max_nodes") == 0 ) { inst->max_nodes = atoi(argv[++i]); continue; } 			// max n. of nodes
		// if ( strcmp(argv[i],"-cutoff") == 0 ) { inst->cutoff = atof(argv[++i]); continue; }					// master cutoff
		// if ( strcmp(argv[i],"-int") == 0 ) { inst->integer_costs = 1; continue; } 							// inteher costs
		
		if ( strcmp(argv[i],"-help") == 0 ) { help = 1; continue; } 											// help
		if ( strcmp(argv[i],"--help") == 0 ) { help = 1; continue; }  											// help
		if ( strcmp(argv[i],"-h") == 0 ) { help = 1; continue; } 												// help
    }  

	if (help)
	{
		showHelpMenu(5);
	}

	intToModelName(inst, model_type);
	intToOptName(inst, opt_type);

	parameterPrint(inst);
    return 0;
}

void parameterPrint(instance* inst)
{
	readInputParameters(inst);

	clearScreen();
	welcomeMessage();
	printf("\n");

	char info[] = "      SELECTED PARAMETERS      ";
    printCentered(info, ' ');
	printf("\n");

	char info_2[] = "*******************************";
	printCentered(info_2, ' ');
	printf("\n");

	char *file_name = getFileName(inst->input_file);
	printf("- File Name:              %s\n", file_name);
	printf("- File Comment:           %s\n", inst->file_comment);
	printf("- Total N° of nodes:      %d\n\n", inst->nnodes);

	printf("- Algorithm Name:         %s\n", inst->algorithm_name);
	printf("- Optimization Name:      %s\n\n", inst->opt_name);
	
	const char* time_limit_string = getTimeLimitString(inst->time_limit);
	printf("- Time Limit:             %s\n", time_limit_string);

	// printf("-old_benders %d\n", inst->old_benders);  
	// printf("-threads %d\n", inst->num_threads);  
	// printf("-max_nodes %d\n", inst->max_nodes); 
	// printf("-memory %d\n", inst->available_memory); 
	// printf("-int %d\n", inst->integer_costs); 
	// printf("-node_file %s\n", inst->node_file);
	// printf("-cutoff %lf\n", inst->cutoff); 
	// printf("\nenter -help or --help for help\n");

	printf("- Seed:                   %d\n\n", inst->random_seed);

	printf("- Verbose:                %d\n", inst->verbose);
	printf("- Show Real Time Plot:    %d\n", inst->show_gnuplot);

	printf("\n");
	printHorizontalLine('*');
}

int intToModelName(instance* inst, int type)
{
	switch (type)
	{
		case 1:
			strcpy(inst->algorithm_name, "Nearest Neighbor");
			break;

		case 2:
			strcpy(inst->algorithm_name, "Variable Neighborhood Search");
			break;

		case 3:
			strcpy(inst->algorithm_name, "Algo 3");
			break;

		default:
			// Seed the random number generator
			srand(time(NULL));
			// Generate a random number between 1 and 3
			type = rand() % 3 + 1;
			intToModelName(inst, type);
			break;
	}
	return 0;
}

int intToOptName(instance* inst, int type)
{
	switch (type)
	{
		case 1:
			strcpy(inst->opt_name, "2-Opt");
			break;

		case 2:
			strcpy(inst->opt_name, "Tabu Search");
			break;

		case 3:
			strcpy(inst->opt_name, "Opt 3");
			break;

		default:
			strcpy(inst->opt_name, "None");
			break;
	}
	return 0;
}

void print_error(const char *err)
{
    printf("\n\n ERROR: %s \n\n", err);
    fflush(NULL);
    exit(1);
}

char *getFileName(const char *filePath)
{
    const char *fileName = strrchr(filePath, '/');
    if (fileName != NULL)
	{
        fileName++;
	} else 
	{
        fileName = filePath;
    }

    // Check if the file name ends with ".tsp"
    const char *extension = ".tsp";
    size_t lenFileName = strlen(fileName);
    size_t lenExtension = strlen(extension);

    if (lenFileName > lenExtension && strcmp(fileName + lenFileName - lenExtension, extension) == 0)
	{
        lenFileName -= lenExtension;
        while (lenFileName > 0 && fileName[lenFileName - 1] == '.')
		{
            lenFileName--;
        }
    }

    char *result = malloc(lenFileName + 1);
    if (result == NULL)
	{
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    strncpy(result, fileName, lenFileName);
    result[lenFileName] = '\0';
    return result;
}

const char* getTimeLimitString(double time_limit) {

    if (time_limit == DBL_MAX ) //! || timeLimit == CPX_INFBOUND) 
	{
        return "Inf";

    } else
	{
        static char buffer[100];
        snprintf(buffer, sizeof(buffer), "%lf", time_limit);
        return buffer;
    }
}

























int readInputParameters(instance* inst)
{
	FILE *fin = fopen(inst->input_file, "r");
	if ( fin == NULL ) 
		print_error(" input file not found!");
	
	inst->nnodes = -1;
	strcpy(inst->file_comment, "NULL");
	char line[180];
	char *par_name; 
	char *token1;

	int active_section = 0; // =1 NODE_COORD_SECTION, =2 DEMAND_SECTION, =3 DEPOT_SECTION 
	
	int do_print = ( inst->verbose >= 100 );

	while ( fgets(line, sizeof(line), fin) != NULL )
	{
		if ( strlen(line) <= 1 ) continue; // skip empty lines
	    par_name = strtok(line, " :");

		if ( strncmp(par_name, "NAME", 4) == 0 ) 
		{
			active_section = 0;
			continue;
		}

		if ( strncmp(par_name, "COMMENT", 7) == 0 ) 
		{
			active_section = 0;   
			token1 = strtok(NULL, "");  
			if ( inst->verbose >= 10 ) 
			{
				char* trimmed_comment = token1;

				// Check if the first two characters are ": "
				if (strncmp(trimmed_comment, ": ", 2) == 0) {
					trimmed_comment += 2; // Move the pointer forward by two characters
				}

				// Remove the trailing '\n' if present
				size_t len = strlen(trimmed_comment);
				if (len > 0 && trimmed_comment[len - 1] == '\n') {
					trimmed_comment[len - 1] = '\0'; // Replace '\n' with '\0'
				}

				strcpy(inst->file_comment, trimmed_comment);
			}
			continue;
		}

		if ( strncmp(par_name, "DIMENSION", 9) == 0 ) 
		{
			if ( inst->nnodes >= 0 ) print_error(" repeated DIMENSION section in input file");
			token1 = strtok(NULL, " :");
			inst->nnodes = atoi(token1);
			if ( do_print ) printf(" ... nnodes %d\n", inst->nnodes); 
			//inst->demand = (double *) calloc(inst->nnodes, sizeof(double));
			inst->coord = (point *) calloc(inst->nnodes, sizeof(point));

			inst->distances = (double **)malloc(inst->nnodes * sizeof(double *));
			for (int i = 0; i < inst->nnodes; i++)
				inst->distances[i] = (double *)malloc(inst->nnodes * sizeof(double));
				
			inst->best_sol = (int *) calloc(inst->nnodes, sizeof(int));

			active_section = 0;  
			continue;
		}
	}

	fclose(fin); 
	return 0;
}

int read_input(instance* inst)
{
                            
	FILE *fin = fopen(inst->input_file, "r");
	if ( fin == NULL ) 
		print_error(" input file not found!");
	
	inst->nnodes = -1;
	inst->depot = -1;  
	inst->nveh = -1;

	char line[180];
	char *par_name;   
	char *token1;
	char *token2;
	
	int active_section = 0; // =1 NODE_COORD_SECTION, =2 DEMAND_SECTION, =3 DEPOT_SECTION 
	
	int do_print = ( inst->verbose >= 100 );

	while ( fgets(line, sizeof(line), fin) != NULL ) 
	{
		if ( inst->verbose >= 100 ) { printf("%s",line); fflush(NULL); }
		if ( strlen(line) <= 1 ) continue; // skip empty lines
	    par_name = strtok(line, " :");
		if ( inst->verbose >= 100 ) { printf("parameter \"%s\" ",par_name); fflush(NULL); }

		if ( strncmp(par_name, "NAME", 4) == 0 ) 
		{
			active_section = 0;
			continue;
		}

		if ( strncmp(par_name, "COMMENT", 7) == 0 ) 
		{
			active_section = 0;   
			token1 = strtok(NULL, "");  
			//if ( inst->verbose >= 10 ) printf(" ... solving instance %s with model %s\n\n", token1, inst->algorithm_name);
			continue;
		}   
		
		if ( strncmp(par_name, "TYPE", 4) == 0 ) 
		{
			token1 = strtok(NULL, " :");  
			if ( strncmp(token1, "TSP", 3) != 0 ) print_error(" format error:  only TYPE == TSP implemented so far!!!!!!"); 
			active_section = 0;
			continue;
		}
		

		if ( strncmp(par_name, "DIMENSION", 9) == 0 ) 
		{
			if ( inst->nnodes >= 0 ) print_error(" repeated DIMENSION section in input file");
			token1 = strtok(NULL, " :");
			inst->nnodes = atoi(token1);
			if ( do_print ) printf(" ... nnodes %d\n", inst->nnodes); 
			//inst->demand = (double *) calloc(inst->nnodes, sizeof(double));
			inst->coord = (point *) calloc(inst->nnodes, sizeof(point));

			inst->distances = (double **)malloc(inst->nnodes * sizeof(double *));
			for (int i = 0; i < inst->nnodes; i++)
				inst->distances[i] = (double *)malloc(inst->nnodes * sizeof(double));
				
			inst->best_sol = (int *) calloc(inst->nnodes, sizeof(int));

			active_section = 0;  
			continue;
		}

		if ( strncmp(par_name, "CAPACITY", 8) == 0 ) 
		{
			token1 = strtok(NULL, " :");
			inst->capacity = atof(token1);
			if ( do_print ) printf(" ... vehicle capacity %lf\n", inst->capacity); 
			active_section = 0;
			continue;
		}


		if ( strncmp(par_name, "VEHICLES", 8) == 0 ) 
		{
			token1 = strtok(NULL, " :");
			inst->nveh = atoi(token1);
			if ( do_print ) printf(" ... n. vehicles %d\n", inst->nveh);  
			active_section = 0;
			continue;
		}


		if ( strncmp(par_name, "EDGE_WEIGHT_TYPE", 16) == 0 ) 
		{
			token1 = strtok(NULL, " :");
			if ( strncmp(token1, "EUC_2D", 6) != 0 ) print_error(" format error:  only EDGE_WEIGHT_TYPE == EUC_2D implemented so far!!!!!!"); 
			active_section = 0;
			continue;
		}            
		
		if ( strncmp(par_name, "NODE_COORD_SECTION", 18) == 0 ) 
		{
			if ( inst->nnodes <= 0 ) print_error(" ... DIMENSION section should appear before NODE_COORD_SECTION section");
			active_section = 1;   
			continue;
		}
		
		/*
		if ( strncmp(par_name, "DEMAND_SECTION", 14) == 0 ) 
		{
			if ( inst->nnodes <= 0 ) print_error(" ... DIMENSION section should appear before DEMAND_SECTION section");
			active_section = 2;
			continue;
		}
		*/

		if ( strncmp(par_name, "DEPOT_SECTION", 13) == 0 )  
		{
			if ( inst->depot >= 0 ) print_error(" ... DEPOT_SECTION repeated??");
			active_section = 3;   
			continue;
		}

		
		if ( strncmp(par_name, "EOF", 3) == 0 ) 
		{
			active_section = 0;
			break;
		}
		
		if ( active_section == 1 ) // within NODE_COORD_SECTION
		{
			int i = atoi(par_name) - 1; 
			if ( i < 0 || i >= inst->nnodes ) print_error(" ... unknown node in NODE_COORD_SECTION section");     
			token1 = strtok(NULL, " :,");
			token2 = strtok(NULL, " :,");
			inst->coord[i].x = atof(token1);
			inst->coord[i].y = atof(token2);

			for(int j=i;j>=0;j--){
				inst->distances[i][j] = sqrt( pow((inst->coord[i].x - inst->coord[j].x), 2) + pow(inst->coord[i].y - inst->coord[j].y, 2) );
				inst->distances[j][i] = inst->distances[i][j];
			}
			
			if ( do_print ) printf(" ... node %4d at coordinates ( %15.7lf , %15.7lf )\n", i+1, inst->coord[i].x, inst->coord[i].y); 
			continue;
		}    
		  
		// if ( active_section == 2 ) // within DEMAND_SECTION
		// {
		// 	int i = atoi(par_name) - 1; 
		// 	if ( i < 0 || i >= inst->nnodes ) print_error(" ... unknown node in NODE_COORD_SECTION section");     
		// 	token1 = strtok(NULL, " :,");
		// 	inst->demand[i] = atof(token1);
		// 	if ( do_print ) printf(" ... node %4d has demand %10.5lf\n", i+1, inst->demand[i]); 
		// 	continue;
		// }  

		if ( active_section == 3 ) // within DEPOT_SECTION
		{
			int i = atoi(par_name) - 1; 
			if ( i < 0 || i >= inst->nnodes ) continue;
			if ( inst->depot >= 0 ) print_error(" ... multiple depots not supported in DEPOT_SECTION");     
			inst->depot = i;
			if ( do_print ) printf(" ... depot node %d\n", inst->depot+1); 
			continue;
		}  
		
		printf(" final active section %d\n", active_section);
		print_error(" ... wrong format for the current simplified parser!!!!!!!!!");     
		    
	}  

	inst->tstart = clock();

	fclose(fin); 
    return 0;
}
