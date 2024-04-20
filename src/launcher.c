#include "launcher.h"

int manage_launcher(instance *inst, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return 1;
    }

    char line[256];
    int nonEmptyLines = 0;
    const char *files_csv[nonEmptyLines + 1];
    int num_files = 0;

    while (fgets(line, sizeof(line), file))
    {
        // Skip lines starting with "//"
        if (strncmp(line, "//", 2) != 0)
        {
            nonEmptyLines++;
        }
    }
    rewind(file);

    while (fgets(line, sizeof(line), file))
    {
        // Skip lines starting with "//"
        if (strncmp(line, "//", 2) != 0)
        {
            char *token;
            char *delim = " \n";
            // Assuming a maximum of 256 words per line
            char *words[256];
            // Start with 1 to reserve the first cell for an empty string
            int argc = 1;

            token = strtok(line, delim);
            
            while (token != NULL && argc < 256)
            {
                printf("%s\n", token);
                words[argc++] = token;
                token = strtok(NULL, delim);
            }

            parse_args(argc, words, inst);
            const char *file_name = execute_workflow(inst);
            printf("File name: %s\n", file_name);
            exit(0);

            // Allocates memory and copies string
            files_csv[num_files] = strdup(file_name);
            if (files_csv[num_files] == NULL)
            {
                fprintf(stderr, "Memory allocation failed\n");
                exit(EXIT_FAILURE);
            }
            num_files++;

            // Later, you can access the file names from files_csv array
            for (int i = 0; i < num_files; i++) {
                printf("File %d: %s\n", i + 1, files_csv[i]);
            }

            // Free memory allocated by strdup
            for (int i = 0; i < num_files; i++) {
                free((void *)files_csv[i]);
            }
        }
    }

    return 0;
}

const char* execute_workflow(instance *inst)
{
    char csv_path[50];
    char date_str[20];

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(date_str, sizeof(date_str), "%y-%m-%d", tm); // Format: YY-MM-DD

    if (strcmp(inst->opt_name, "None") != 0)
    {
        sprintf(csv_path, "Archive/tmp/%s_%s_%s.csv", inst->algorithm_name, inst->opt_name, date_str);

    } else
    {
        sprintf(csv_path, "Archive/tmp/%s_%s.csv", inst->algorithm_name, date_str);
    }

    // Create the "solution" directory
    mkdir("Archive", 0777);
    mkdir("Archive/tmp", 0777);

    // Open file for writing
    if(inst->output_csv == NULL)
    {
        inst->output_csv = fopen(csv_path, "w");
        // Header
        fprintf(inst->output_csv, "1; %s %s\n", inst->algorithm_name, inst->opt_name);
        fflush(inst->output_csv);
    }
    // fprintf(file, "John, 25, New York\n");

    read_input(inst);
    apply_algorithm(inst);

    //show_solution(inst, true);
    save_solution(inst);
    fflush(inst->output_csv);
    fclose(inst->output_csv);
    
    free_instance(inst);
    return csv_path;
}