#include <iostream>
#include <mpi.h>
int world_size, world_rank;

int main(int argc, char** argv){
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    std::cout << "Hello " << world_size << ", World: "
              << world_rank << std::endl;

    // finalize MPI and return control to the OS;

    MPI_Finalize();
    return 0;
}