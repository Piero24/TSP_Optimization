#include "launcher.h"

#define MAX_HEADER_LENGTH 350

int manage_launcher(instance *inst, const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return 1;
    }

    char line[256];
    char tmp_csv_name[50];
    char header_names[MAX_HEADER_LENGTH];
    strcpy(header_names, "");
    
    int nonEmptyLines = 0;

    char csv_path[50];
    char date_str[20];

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    strftime(date_str, sizeof(date_str), "%y-%m-%d_%H-%M", tm); // Format: YY-MM-DD_HH-MM
    sprintf(csv_path, "Archive/Csv/%s.csv", date_str);

    while (fgets(line, sizeof(line), file))
    {
        // Skip lines starting with "//"
        if (strncmp(line, "//", 2) != 0 && line[0] != '\0' && line[0] != '\n')
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

            strncat(header_names, inst->algorithm_name, MAX_HEADER_LENGTH - strlen(header_names) - 1);
            strncat(header_names, " - ", MAX_HEADER_LENGTH - strlen(header_names) - 1);
            strncat(header_names, inst->opt_name, MAX_HEADER_LENGTH - strlen(header_names) - 1);
            strncat(header_names, "; ", MAX_HEADER_LENGTH - strlen(header_names) - 1);

            if(inst->output_csv == NULL)
            {
                // Create the "solution" directory
                mkdir("Archive", 0777);
                mkdir("Archive/Csv", 0777);

                strcpy(inst->name_csv, csv_path);
                parameterPrint(inst);

                strcpy(inst->name_csv, "Archive/Csv/tmp.csv");
                strcpy(tmp_csv_name, "Archive/Csv/tmp.csv");
                inst->output_csv = fopen(inst->name_csv, "a");
            }

            execute_workflow(inst);
            nonEmptyLines++;
        }
    }

    fclose(file);

    if (strlen(header_names) >= 2)
        header_names[strlen(header_names) - 2] = '\n';

    FILE *final_csv = fopen(csv_path, "a");

    if (!file)
    {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return 1;
    }

    // Max length of header_names + max length of nonEmptyLines + separator length
    char header_and_lines[MAX_HEADER_LENGTH + 20];
    sprintf(header_and_lines, "%d; %s", nonEmptyLines, header_names);

    // Write header_and_lines to the CSV file
    fprintf(final_csv, "%s", header_and_lines);

    FILE *tmp_file = fopen(tmp_csv_name, "r");

    if (!tmp_file)
    {
        fprintf(stderr, "Error: Could not open file %s\n", tmp_csv_name);
        return 1;
    }

    // Copy rows from tmp_file to final_csv
    char row[256]; // Assuming max length of a row
    while (fgets(row, sizeof(row), tmp_file))
    {
        fprintf(final_csv, "%s", row);
    }

    fflush(final_csv);
    fclose(final_csv);

    fflush(tmp_file);
    fclose(tmp_file);

    if (remove(tmp_csv_name) != 0)
    {
        printf("Error: Unable to delete the file %s.\n", tmp_csv_name);
    }

    runPythonScript();
    return 0;
}

int execute_workflow(instance *inst)
{
    char csv_path[150];
    
    if (inst->output_csv == NULL)
    {
        char date_str[20];

        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        strftime(date_str, sizeof(date_str), "%y-%m-%d", tm); // Format: YY-MM-DD

        if (strcmp(inst->opt_name, "None") != 0)
        {
            sprintf(csv_path, "Archive/Csv/%s_%s_%s.csv", inst->algorithm_name, inst->opt_name, date_str);

        } else
        {
            sprintf(csv_path, "Archive/Csv/%s_%s.csv", inst->algorithm_name, date_str);
        }

        // Create the "solution" directory
        mkdir("Archive", 0777);
        mkdir("Archive/Csv", 0777);

        strcpy(inst->name_csv, csv_path);
    }

    // Open file for writing
    if(inst->output_csv == NULL)
    {
        inst->output_csv = fopen(inst->name_csv, "w");
        // Header
        fprintf(inst->output_csv, "1; %s %s\n", inst->algorithm_name, inst->opt_name);
        fflush(inst->output_csv);

        parameterPrint(inst);
    }

    read_input(inst);
    apply_algorithm(inst);

    if (strcmp(inst->algorithm_name, "Random") == 0 || strcmp(inst->algorithm_name, "Nearest Neighbor") == 0)
        show_solution(inst, true);
    
    save_solution(inst);
    fflush(inst->output_csv);
    fclose(inst->output_csv);
    
    free_instance(inst);
    return 0;
}

int runPythonScript()
{
    char *env_value;
    // Get the value of the VIRTUAL_ENV environment variable
    env_value = getenv("VIRTUAL_ENV");

    // Check if the environment variable is set
    if (env_value != NULL)
    {
        // printf("A virtual environment is active: %s\n", env_value);
        printf("Python Script here\n");
        // if (system("python3 Documents/HW_Plot/perfprof.py -D , -T 3600 -S 2 -M 20 Documents/HW_Plot/lagr.csv pp.pdf -P 'all instances, shift 2 sec.s'") != 0)
        // {
        //     printf("Failed to run the command.\n");
        //     return 1;
        // }

    }
    
    return 0;
}