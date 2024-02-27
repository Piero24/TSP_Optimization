typedef struct{
    int nnodes;
} instance;

int parse_args(int argc, char** argv, instance* inst);
int read_input(instance* inst);
int apply_algorithm(instance* inst);
void print_solution(instance* inst);
void free_instance(instance* inst);