#include "tsp.h"


#ifdef _WIN32
	#define popen _popen
	#define pclose _pclose
	#define mkdir _mkdir
#endif

double dist(instance* inst, int a, int b)
{
	return inst->distances[a][b];
}

void verbose_print(instance *inst, int vMin, const char* format, ...) {
	if(inst->verbose >= vMin){
		va_list args;
		va_start(args, format);
		vprintf(format, args);
		va_end(args);
	}
}

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
		if(inst->plotSolution == NULL){
			
			inst->plotSolution = popen("gnuplot --persist", "w");
			
			fprintf(inst->plotSolution, "set title \"Solution\"\n");
			fprintf(inst->plotSolution, "set xlabel \"X Axis\"\n");
			fprintf(inst->plotSolution, "set ylabel \"Y Axis\"\n");
			fprintf(inst->plotSolution, "set grid\n");
			fprintf(inst->plotSolution, "set term qt persist font \"Arial\"\n"); // Set font to Arial
			fprintf(inst->plotSolution, "set pointsize 0.1\n"); // Set font to Arial
			fflush(inst->plotSolution);
		}

		fprintf(inst->plotSolution, "plot [-10:10010] [-10:10010] '-' with linespoints pointtype 7\n");

		for(int i=0; i<inst->nnodes; i++)
		{
			fprintf(inst->plotSolution, "%f %f\n", inst->coord[inst->best_sol[i]].x, inst->coord[inst->best_sol[i]].y);
		}
		fprintf(inst->plotSolution, "%f %f\n", inst->coord[inst->best_sol[0]].x, inst->coord[inst->best_sol[0]].y);

		fprintf(inst->plotSolution, "e\n");

		fflush(inst->plotSolution);

	} else 
	{
		for(int i=0; i<inst->nnodes; i++)
		{
			printf("%f %f\n", inst->coord[inst->best_sol[i]].x, inst->coord[inst->best_sol[i]].y);
		}
	}
}

void show_solution_comps(instance* inst, bool useGnuplot, int** result, int ncomp)
{    
	if(useGnuplot)
	{
		if(inst->plotSolution == NULL){
			
			inst->plotSolution = popen("gnuplot --persist", "w");
			
			fprintf(inst->plotSolution, "set title \"Solution\"\n");
			fprintf(inst->plotSolution, "set xlabel \"X Axis\"\n");
			fprintf(inst->plotSolution, "set ylabel \"Y Axis\"\n");
			fprintf(inst->plotSolution, "set grid\n");
			fprintf(inst->plotSolution, "set term qt persist font \"Arial\"\n"); // Set font to Arial
			fprintf(inst->plotSolution, "set pointsize 0.1\n"); // Set font to Arial

			fflush(inst->plotSolution);
		}

		fprintf(inst->plotSolution, "plot [-10:10010] [-10:10010] '-' with linespoints pointtype 7\n");
		for(int j=1; j<ncomp+1; j++){
			for(int i=0; i<inst->nnodes && result[j][i] != -1; i++)
			{
				fprintf(inst->plotSolution, "%f %f\n", inst->coord[result[j][i]].x, inst->coord[result[j][i]].y);
			}
			if(result[j][0] != -1)
				fprintf(inst->plotSolution, "%f %f\n\n", inst->coord[result[j][0]].x, inst->coord[result[j][0]].y);
		}

		fprintf(inst->plotSolution, "e\n");

		fflush(inst->plotSolution);

	} else 
	{
		for(int i=0; i<inst->nnodes; i++)
		{
			printf("%f %f\n", inst->coord[inst->best_sol[i]].x, inst->coord[inst->best_sol[i]].y);
		}
	}
}

void show_costs(instance* inst, point* costs, int n, bool alg)
{    
	if(inst->plotCosts == NULL){

		inst->plotCosts = popen("gnuplot --persist", "w");
		
		fprintf(inst->plotCosts, "set title \"Solution\"\n");
		fprintf(inst->plotCosts, "set xlabel \"X Axis\"\n");
		fprintf(inst->plotCosts, "set ylabel \"Y Axis\"\n");
		fprintf(inst->plotCosts, "set grid\n");
		fprintf(inst->plotCosts, "set term qt persist font \"Arial\"\n"); // Set font to Arial
		fprintf(inst->plotCosts, "set pointsize 0.5\n"); // Set font to Arial

		fflush(inst->plotCosts);
	}

	fprintf(inst->plotCosts, "set arrow 1 from 0,%f to %f,%f nohead lc \"red\"\n", inst->zbest, costs[n-1].x, inst->zbest);

	if(alg)
		fprintf(inst->plotCosts, "plot '-' title \"%s\" with linespoints pointtype 7 linecolor ", inst->algorithm_name);
	else
		fprintf(inst->plotCosts, "plot '-' title \"%s\" with linespoints pointtype 7 linecolor ", inst->opt_name);
	
	if(alg && strcmp(inst->algorithm_name, "Nearest Neighborhood")){
		fprintf(inst->plotCosts, "\"blue\"\n");
	}else if(!alg && strcmp(inst->opt_name, "2-Opt")){
		fprintf(inst->plotCosts, "\"green\"\n");
	}else if(!alg && strcmp(inst->opt_name, "Tabu Search")){
		fprintf(inst->plotCosts, "\"light magenta\"\n");
	}else if(!alg && strcmp(inst->opt_name, "Variable Neighborhood Search")){
		fprintf(inst->plotCosts, "\"purple\"\n");
	}
	
	for(int i=0; i<n; i++)
	{
		fprintf(inst->plotCosts, "%f %f\n", costs[i].x, costs[i].y);
	}

	fprintf(inst->plotCosts, "e\n");

	fflush(inst->plotCosts);
}

void save_solution(instance* inst)
{    
	char img_directory_name[50];
	sprintf(img_directory_name, "Archive/Image/%s", inst->algorithm_name);

	char svg_directory_name[50];
	sprintf(svg_directory_name, "Archive/Svg/%s", inst->algorithm_name);

	// Create the "solution" directory
	mkdir("Archive", 0777);
	mkdir("Archive/Image", 0777);
	mkdir(img_directory_name, 0777);
	mkdir("Archive/Svg", 0777);
	mkdir(svg_directory_name, 0777);

	time_t t = time(NULL);
	struct tm *tm = localtime(&t);

	char date_str[20];
	strftime(date_str, sizeof(date_str), "%y-%m-%d", tm); // Format: YY-MM-DD
	// strftime(date_str, sizeof(date_str), "%y-%m-%d_%H:%M", tm); // Format: YY-MM-DD HH:MM

	char *file_name = getFileName(inst->input_file, ".tsp");

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
	fprintf(plotPNG, "plot [-10:10010] [-10:10010] '-' with linespoints\n");

	// Send Gnuplot commands directly for SVG
	fprintf(plotSVG, "set terminal svg\n");
	fprintf(plotSVG, "set output \"%s\"\n", svgPath);
	fprintf(plotSVG, "set title \"Solution\"\n");
	fprintf(plotSVG, "set xlabel \"X Axis\"\n");
	fprintf(plotSVG, "set ylabel \"Y Axis\"\n");
	fprintf(plotSVG, "set grid\n");
	fprintf(plotSVG, "plot [-10:10010] [-10:10010] '-' with linespoints\n");

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
    // printf("%s\n",file_name);
    FILE *fp = fopen(file_name, "w");
    if (fp == NULL)
    {
        perror("Error opening file.\n");
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
		double x = randomDouble(0, 10000);
		double y = randomDouble(0, 10000);
		fprintf(fp, "%d %.4f %.4f\n", i, x, y);
	}

	fclose(fp);

	// Dynamically allocate memory for the file name
	char *dynamic_file_name = malloc(strlen(file_name) + 1);
	strcpy(dynamic_file_name, file_name);

	return dynamic_file_name;
}

char *getFileName(const char *filePath, const char *extension)
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
	//const char *extension = ".tsp";
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