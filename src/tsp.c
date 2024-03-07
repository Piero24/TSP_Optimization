#include "../include/tsp.h"
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Generates a data file with node coordinates for 
 * the specified instance and saves it to the given filename.
 * 
 * @param filename The name of the file to write the data to.
 * @param inst A pointer to the instance structure containing node coordinates.
 */
void generateDataFile(const char* filename, instance* inst)
{
    FILE* file = fopen("data.txt", "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    // Write data to the file
    for(int i=0; i<inst->nnodes; i++)
    {
        fprintf(file, "%f %f\n", inst->coord[inst->best_sol[i]].x, inst->coord[inst->best_sol[i]].y);
    }
    fprintf(file, "%f %f\n", inst->coord[0].x, inst->coord[0].y);

    fclose(file);
}

void print_solution(instance* inst, bool useGnuplot)
{    
    if(useGnuplot)
    {
        /*
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
        //system("gnuplot plot_script.plt");
        system("gnuplot --persist plot_script.plt 2> gnuplot_error.log");

        // Optionally, remove generated files
        //remove(dataFilename);
        //remove("plot_script.plt");
        */
        FILE *plot = popen("gnuplot --persist", "w");
        
        fprintf(plot, "set title \"Solution\"\n");
        fprintf(plot, "set xlabel \"X Axis\"\n");
        fprintf(plot, "set ylabel \"Y Axis\"\n");
        fprintf(plot, "set grid\n");
        fprintf(plot, "plot '-' with linespoints\n");

        for(int i=0; i<inst->nnodes; i++)
        {
            fprintf(plot, "%f %f\n", inst->coord[inst->best_sol[i]].x, inst->coord[inst->best_sol[i]].y);
        }
        fprintf(plot, "%f %f\n", inst->coord[inst->best_sol[0]].x, inst->coord[inst->best_sol[0]].y);
        
        pclose(plot);
    } else 
    {
        for(int i=0; i<inst->nnodes; i++)
        {
            printf("%f %f\n", inst->coord[inst->best_sol[i]].x, inst->coord[inst->best_sol[i]].y);
        }
    }
}

void free_instance(instance* inst)
{
    //free(inst->demand);
    free(inst->coord);

    for(int i=0; i<inst->nnodes; i++)
        free(inst->distances[i]);

    free(inst->distances);
    free(inst->best_sol);
}