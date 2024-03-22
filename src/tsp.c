#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <time.h>

#include "../include/tsp.h"
#include "../include/parser.h"

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
        FILE *plot = popen("gnuplot --persist", "w");
        
        fprintf(plot, "set title \"Solution\"\n");
        fprintf(plot, "set xlabel \"X Axis\"\n");
        fprintf(plot, "set ylabel \"Y Axis\"\n");
        fprintf(plot, "set grid\n");
        fprintf(plot, "set term qt font \"Arial\"\n"); // Set font to Arial
        fprintf(plot, "set pointsize 0.1\n"); // Set font to Arial
        fprintf(plot, "plot '-' with linespoints pointtype 7\n");

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

void save_solution(instance* inst)
{    
    char img_directory_name[50];
    sprintf(img_directory_name, "Archive/Image/%s", inst->algorithm_name);

    char svg_directory_name[50];
    sprintf(svg_directory_name, "Archive/Svg/%s", inst->algorithm_name);

    // Create the "solution" directory
    #ifdef _WIN32
         _mkdir("Archive");
        _mkdir("Archive/Image");
        _mkdir(img_directory_name);
        _mkdir("Archive/Svg");
        _mkdir(svg_directory_name);
    #else
        mkdir("Archive", 0777);
        mkdir("Archive/Image", 0777);
        mkdir(img_directory_name, 0777);
        mkdir("Archive/Svg", 0777);
        mkdir(svg_directory_name, 0777);
    #endif

    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    char date_str[20];
    strftime(date_str, sizeof(date_str), "%y-%m-%d", tm); // Format: YY-MM-DD
    // strftime(date_str, sizeof(date_str), "%y-%m-%d_%H:%M", tm); // Format: YY-MM-DD HH:MM

    char *file_name = getFileName(inst->input_file);

    // Modify the output file paths to include the "Archive/Image" and the "Archive/Svg" directories
    char pngPath[100];
    char svgPath[100];
    sprintf(pngPath, "Archive/Image/%s/%s_%s.png", inst->algorithm_name, file_name, date_str);
    sprintf(svgPath, "Archive/Svg/%s/%s_%s.svg", inst->algorithm_name, file_name, date_str);

    FILE *plotPNG = popen("gnuplot", "w");
    FILE *plotSVG = popen("gnuplot", "w");

    // Send Gnuplot commands directly for PNG
    fprintf(plotPNG, "set terminal png\n");
    fprintf(plotPNG, "set output \"%s\"\n", pngPath);
    fprintf(plotPNG, "set title \"Solution\"\n");
    fprintf(plotPNG, "set xlabel \"X Axis\"\n");
    fprintf(plotPNG, "set ylabel \"Y Axis\"\n");
    fprintf(plotPNG, "set grid\n");
    fprintf(plotPNG, "plot '-' with linespoints\n");

    // Send Gnuplot commands directly for SVG
    fprintf(plotSVG, "set terminal svg\n");
    fprintf(plotSVG, "set output \"%s\"\n", svgPath);
    fprintf(plotSVG, "set title \"Solution\"\n");
    fprintf(plotSVG, "set xlabel \"X Axis\"\n");
    fprintf(plotSVG, "set ylabel \"Y Axis\"\n");
    fprintf(plotSVG, "set grid\n");
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

double randomDouble(double min, double max)
{
    return min + (rand() / (double)RAND_MAX) * (max - min);
}

char* fileGenerator(int n)
{
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);

    char date_str[20];
    strftime(date_str, sizeof(date_str), "%Y-%m-%d", tm); // Format: YYYYMMDD

    // Create the file name
    char file_name[50];
    snprintf(file_name, sizeof(file_name), "Resource/pr%d-%s.tsp", n, date_str);

    // Open the file
    FILE *fp = fopen(file_name, "w");
    if (fp == NULL)
    {
        printf("Error opening file.\n");
        return NULL;
    }

    char f_name[50];
    snprintf(f_name, sizeof(f_name), "pr %d %s", n, date_str);

    fprintf(fp, "NAME : %s\n", f_name);
    fprintf(fp, "COMMENT : %d-city problem (Random Generated)\n", n);
    fprintf(fp, "TYPE : TSP\n");
    fprintf(fp, "DIMENSION : %d\n", n);
    fprintf(fp, "EDGE_WEIGHT_TYPE : EUC_2D\n");
    fprintf(fp, "NODE_COORD_SECTION\n");

    for (int i = 1; i <= n; i++)
    {
        double x = randomDouble(1.0, 9.999999);
        double y = randomDouble(1.0, 9.999999);
        fprintf(fp, "%d %.4f %.4f\n", i, x, y);
    }

    fclose(fp);

    // Dynamically allocate memory for the file name
    char *dynamic_file_name = malloc(strlen(file_name) + 1);
    strcpy(dynamic_file_name, file_name);

    return dynamic_file_name;
}