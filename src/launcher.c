#include "launcher.h"

void instExtractor(instance *inst, char *line)
{
    char *token;
    char *delim = " \n";
    char *words[384];
    // Start with 1 to reserve the first cell for an empty string
    int argc = 1;
    char line_copy[256];
    strcpy(line_copy, line);
    token = strtok(line_copy, delim);

    while (token != NULL && argc < 256)
    {
        // printf("%s\n", token);
        words[argc++] = token;
        token = strtok(NULL, delim);
    }

    parse_args(argc, words, inst);
}

void headerGenerator(char *header_and_lines, int tuple_count, Tuple *tuples)
{
    char header_names[MAX_HEADER_LENGTH];
    strcpy(header_names, "");

    for (int i = 0; i < tuple_count; i++)
    {
        strncat(header_names, tuples[i].algorithm, MAX_HEADER_LENGTH - strlen(header_names) - 1);
        strncat(header_names, " - ", MAX_HEADER_LENGTH - strlen(header_names) - 1);

        if (strcmp(tuples[i].optimizer, "None") != 0)
            strncat(header_names, tuples[i].optimizer, MAX_HEADER_LENGTH - strlen(header_names) - 1);

        strncat(header_names, ", ", MAX_HEADER_LENGTH - strlen(header_names) - 1);
    }

    if (strlen(header_names) >= 2)
        header_names[strlen(header_names) - 2] = '\0';

    sprintf(header_and_lines, " %d, %s", tuple_count, header_names);
}

void finalGenerator(char *csv_path) {

    char date_str[20];

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    // Format: YY-MM-DD_HH-MM
    strftime(date_str, sizeof(date_str), "%y-%m-%d_%H-%M", tm);
    sprintf(csv_path, "Archive/Csv/%s.csv", date_str);

    // Create the "csv" directory
    mkdir("Archive", 0777);
    mkdir("Archive/Csv", 0777);
}

int manageLauncher(instance *inst, const char *filename)
{
    char line[256];

    char* files[MAX_FILES];
    int file_count = 0;

    Tuple tuples[MAX_FILES];
    int tuple_count = 0;

    char parameters[256];

    FILE *file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        return 1;
    }

    while (fgets(line, sizeof(line), file))
    {
        // Skip lines starting with "//"
        if (strncmp(line, "//", 2) != 0 && line[0] != '\0' && line[0] != '\n')
        {
            instExtractor(inst, line);

            if (strcmp(inst->input_file, "NULL") != 0 && file_count < MAX_FILES) {
                files[file_count] = strdup(inst->input_file);
                file_count++;
            }

            if (strcmp(inst->algorithm_name, "Undefined") != 0 && tuple_count < MAX_FILES*5) {
                strcpy(tuples[tuple_count].algorithm, inst->algorithm_name);
                strcpy(tuples[tuple_count].optimizer, inst->opt_name);
                tuple_count++;
            }

            if (strcmp(inst->input_file, "NULL") == 0 && strcmp(inst->algorithm_name, "Undefined") == 0) {
                strcpy(parameters, line);
            }
        }
    }

    fclose(file);

    char csv_path[50];
    finalGenerator(csv_path);

    char header_and_lines[MAX_HEADER_LENGTH + 20];
    headerGenerator(header_and_lines, tuple_count, tuples);

    FILE *final_csv = fopen(csv_path, "a");

    if (!final_csv)
    {
        fprintf(stderr, "Error: Could not open file %s\n", csv_path);
        return 1;
    }

    // Write header to the CSV file
    fprintf(final_csv, "%s", header_and_lines);
    fflush(final_csv);

    initialization(inst);
    instExtractor(inst, parameters);

    char *out_file_name = getFileName(csv_path, ".csv");
    strcpy(inst->name_csv, out_file_name);

    for (int i = 0; i < file_count; i++)
    {
        strcpy(inst->input_file, files[i]);

        char row[MAX_HEADER_LENGTH];
        strcpy(row, "");
        char *file_name = getFileName(inst->input_file, ".tsp");
        strncat(row, file_name, MAX_HEADER_LENGTH - strlen(row) - 1);
        strncat(row, ", ", MAX_HEADER_LENGTH - strlen(row) - 1);

        for (int j = 0; j < tuple_count; j++)
        {
            strcpy(inst->algorithm_name, tuples[j].algorithm);
            strcpy(inst->opt_name, tuples[j].optimizer);

            execute_workflow(inst);

            char zbest_str[20];
            snprintf(zbest_str, sizeof(zbest_str), "%f", inst->zbest);
            strncat(row, zbest_str, MAX_HEADER_LENGTH - strlen(row) - 1);
            strncat(row, ", ", MAX_HEADER_LENGTH - strlen(row) - 1);
        }
        
        if (strlen(row) >= 2)
            row[strlen(row) - 2] = '\0';

        fprintf(final_csv, "\n%s", row);
        fflush(final_csv);
        free(files[i]);
    }

    fflush(final_csv);
    fclose(final_csv);

    runPythonScript(inst, csv_path);
    free_instance(inst);
    return 0;
}

int execute_workflow(instance *inst)
{
    parameterPrint(inst);

    read_input(inst);
    apply_algorithm(inst);

    //show_solution(inst, true);
    save_solution(inst);
    
    //free_instance(inst);
    return 0;
}

int runPythonScript(instance *inst, char* csv_path)
{
    char *env_value;
    // Get the value of the VIRTUAL_ENV environment variable
    env_value = getenv("VIRTUAL_ENV");
    // printf("A virtual environment is active: %s\n", env_value);

    // Check if the environment variable is set
    if (env_value != NULL)
    {
        // Create the "pdf" directory
        mkdir("Archive/Pdf", 0777);
        char *out_file_name = getFileName(csv_path, ".csv");

        char sys_call[350];
        sprintf(sys_call, "python3 src/Comparator/perfprof.py -D , -T 3600 -S 2 -M 20 %s Archive/Pdf/%s.pdf -P 'all instances, shift 2 sec.s'", csv_path, out_file_name);

        if (system(sys_call) != 0)
        {
            printf("Failed to run the command.\n");
            return 1;
        }
    }
    return 0;
}