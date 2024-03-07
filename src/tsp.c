#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "../include/tsp.h"

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

void show_solution(instance* inst, bool useGnuplot)
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
        fprintf(plot, "set term qt font \"Arial\"\n"); // Set font to Arial
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

void save_solution(instance* inst, const char* outputFileName)
{    
    // Create the "solution" directory
    #ifdef _WIN32
         _mkdir("Archive");
        _mkdir("Archive/Image");
        _mkdir("Archive/Svg");
    #else
        mkdir("Archive", 0777);
        mkdir("Archive/Image", 0777);
        mkdir("Archive/Svg", 0777);
    #endif

    // Modify the output file paths to include the "Archive/Image" and the "Archive/Svg" directories
    char pngPath[100]; // Adjust the size as needed
    char svgPath[100]; // Adjust the size as needed
    sprintf(pngPath, "Archive/Image/%s.png", outputFileName);
    sprintf(svgPath, "Archive/Svg/%s.svg", outputFileName);

    FILE *plotPNG = popen("gnuplot", "w");
    FILE *plotSVG = popen("gnuplot", "w");

    // Send Gnuplot commands directly for PNG
    fprintf(plotPNG, "set terminal png\n");
    fprintf(plotPNG, "set output \"%s\"\n", pngPath);
    fprintf(plotPNG, "set title \"Solution\"\n");
    fprintf(plotPNG, "set xlabel \"X Axis\"\n");
    fprintf(plotPNG, "set ylabel \"Y Axis\"\n");
    fprintf(plotPNG, "set grid\n");
    fprintf(plotPNG, "set term qt font \"Arial\"\n");
    fprintf(plotPNG, "plot '-' with linespoints\n");

    // Send Gnuplot commands directly for SVG
    fprintf(plotSVG, "set terminal svg\n");
    fprintf(plotSVG, "set output \"%s\"\n", svgPath);
    fprintf(plotSVG, "set title \"Solution\"\n");
    fprintf(plotSVG, "set xlabel \"X Axis\"\n");
    fprintf(plotSVG, "set ylabel \"Y Axis\"\n");
    fprintf(plotSVG, "set grid\n");
    fprintf(plotSVG, "set term qt font \"Arial\"\n");
    fprintf(plotSVG, "plot '-' with linespoints\n");

    for(int i = 0; i < inst->nnodes; i++) {
        fprintf(plotPNG, "%f %f\n", inst->coord[inst->best_sol[i]].x, inst->coord[inst->best_sol[i]].y);
        fprintf(plotSVG, "%f %f\n", inst->coord[inst->best_sol[i]].x, inst->coord[inst->best_sol[i]].y);
    }

    fprintf(plotPNG, "%f %f\n", inst->coord[inst->best_sol[0]].x, inst->coord[inst->best_sol[0]].y);
    fprintf(plotSVG, "%f %f\n", inst->coord[inst->best_sol[0]].x, inst->coord[inst->best_sol[0]].y);

    fprintf(plotPNG, "e\n");
    fflush(plotPNG);
    pclose(plotPNG);

    fprintf(plotSVG, "e\n");
    fflush(plotSVG);
    pclose(plotSVG);
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