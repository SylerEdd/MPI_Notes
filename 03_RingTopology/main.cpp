// MPI_notes - App 03 - App02 in a Ring Structure
/** Using a ring structure of 4 connected processes **/

/** includes**/
#include <iostream>
#include <cstdlib>
#include <mpi.h>

// messages for communicating tasks
int COMPUTE_AVERAGE = 1;

// the world rank and size that will be useful in many functions
int world_size;
int world_rank;

// coordinator method
void coordinator(void){
    int message;

    // tell the next node that we have to start computing an average
    MPI_Send(&COMPUTE_AVERAGE, 1, MPI_INT, 1, 0, MPI_COMM_WORLD);

    // we will expect the last node to send us a message to compute our average. get it
    // and ignore it
    MPI_Recv(&message, 1, MPI_INT, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    
    // compute our average and print out the results
    srand(world_rank);
    int sum = 0;

    for (int i = 0; i < 100000; i++){
        sum += rand() % 10;
    }
    float average = sum / 100000.f;

    std::cout <<"coordinator 0 sum is: " << sum << std::endl;
    std::cout << "coordinator 0 average is: " << average << std::endl;

    //ask node one to compute its average by sending a single floating point value this
    // will add in it's average and pass to node 2 etc
    // when we get the result back we will have the full average
    MPI_Send(&average, 1, MPI_FLOAT, 1, 0, MPI_COMM_WORLD);
    MPI_Recv(&average, 1, MPI_FLOAT, world_size - 1, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    // print out the overall average by dividing by the number of nodes and exit
    std::cout << "coordinator 0 total average is: " << average / world_size << std::endl;
}



// computeAverage method 
void computeAverage(void){
    int message;
    float current_average = 0;

    // WAIT TILL WE GET THE COMPUTE MESSAGE AFTER THIS TELL THE NEXT NODE TO START
    MPI_Recv(&message, 1, MPI_INT, (world_rank + world_size - 1) % world_size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Send(&COMPUTE_AVERAGE, 1, MPI_INT, (world_rank + 1) % world_size, 0, MPI_COMM_WORLD);

    // compute average
    srand(world_rank);
    int sum = 0;
    for (int i = 0; i < 100000; i++){
        sum+= rand() % 10;
    }
    float average = sum / 100000.f;

    // print out information about our sum and average
    std::cout << "node " << world_rank << " sum is: " << sum << std::endl;
    std::cout << "node " << world_rank << " average is: " << average << std::endl;

    // at some point we will get a message that will contain a current total of the
    // overall average. take that value add our average
    // to it and pass it on to the next node
    MPI_Recv(&current_average, 1, MPI_FLOAT, (world_rank + world_size - 1) % world_size, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    current_average += average;

    std::cout << "node " << world_rank << " current average is " << current_average << std::endl;
    MPI_Send(&current_average, 1, MPI_FLOAT, (world_rank + 1) % world_size, 0, MPI_COMM_WORLD);
}

int main(int argc, char** argv) {
    // always initialize the MPI library at the start of an MPI program
    MPI_Init(NULL, NULL);

    // one of the important tasks we have to establish is how many processes are 
    // in this MPI instance. This tells us who and what we have to communicate with
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // another important job is finding out which rank we are. We can use this rank number
    // to assign seperate jobs to different mpi units
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // process zero is the coordinator
    // compute the average of 400 numbers by using ring like communication
    if(world_rank == 0){
        coordinator();
    } else{
        computeAverage();
    }

    // before ending the application, always finalize everything so MPI can shutdown properly
    MPI_Finalize();

    // standard C/C++ thing to do, return control to the OS
    return 0;
}