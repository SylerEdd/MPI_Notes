// MPI_notes - App01 - Basic Introduction to MPI
#include <iostream>
#include <mpi.h>

int main(int argc, char** argv) {
    // first thing that we must do at the start of an MPI program
    // initialize the MPI library
    MPI_Init(NULL, NULL);

    // one of the important tasks we have to establish is how many processes are
    // in this MPI instance. This tells us who and what we have to communicate with
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    std::cout << "world size is: " << world_size << std::endl;

    // another important job is finding out which rank we are. We can use this rank number
    // to assign seperate jobs to different mpi units.
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);
    std::cout << "world rank is: " << world_rank << std::endl;

    //we will try to send a message from rank 0 that is the nodes rank number
    // and we will ask the receiving process to print out that number
    if(world_rank == 0) {
        MPI_Send(&world_rank, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);
        MPI_Send(&world_rank, 1, MPI_INT, 2, 0, MPI_COMM_WORLD);
        MPI_Send(&world_rank, 1, MPI_INT, 3, 0, MPI_COMM_WORLD);
    } else{
        int received_data = 2000;
        MPI_Recv(&received_data, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        std::cout << "rank: " << world_rank << " received data from rank: " << received_data << std::endl;
    }

    // before ending the application , always finaliae everything so MPI can shut down properly
    MPI_Finalize();

    // simple output just to verify that everything is working for us
    std::cout << "it works!" << std::endl;

    //return status 0 to the OS
    return 0;
}