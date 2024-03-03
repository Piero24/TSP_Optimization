#include "vrp.h"
#include <stdio.h>

/**
 * @brief Generates a data file with node coordinates for 
 * the specified instance and saves it to the given filename.
 * 
 * @param filename The name of the file to write the data to.
 * @param inst A pointer to the instance structure containing node coordinates.
 */
void generateDataFile(const char* filename, instance* inst)
{
    FILE* file = fopen(filename, "w");
    if (file == NULL)
    {
        perror("Error opening file");
        exit(1);
    }

    // Write data to the file
    for(int i=0; i<inst->nnodes; i++)
    {
        fprintf(file, "%d %d\n", inst->xcoord[i], inst->ycoord[i]);
    }

    fclose(file);
}

void print_solution(instance* inst, bool useGnuplot)
{
    if(useGnuplot)
    {
        // Generate data file
        const char* dataFilename = "data.txt";
        generateDataFile(dataFilename, inst);

        // Create Gnuplot script file
        FILE* scriptFile = fopen("plot_script.plt", "w");

        if (scriptFile == NULL)
        {
            perror("Error opening script file");
            exit(1);
        }

        // Write Gnuplot commands to script file
        fprintf(scriptFile, "set title \"Solution\"\n");
        fprintf(scriptFile, "set xlabel \"X Axis\"\n");
        fprintf(scriptFile, "set ylabel \"Y Axis\"\n");
        fprintf(scriptFile, "set grid\n");
        fprintf(scriptFile, "plot 'data.txt' with linespoints title \"Nodes\"\n");

        fclose(scriptFile);

        // Execute Gnuplot script
        system("gnuplot plot_script.plt");

        // Optionally, remove generated files
        remove(dataFilename);
        remove("plot_script.plt");
    } else
    {
        for(int i=0; i<inst->nnodes; i++)
        {
            printf("#%d x=%d y=%d\n", i, inst->xcoord[i], inst->ycoord[i]);
        }
    }
}

void free_instance(instance* inst){}