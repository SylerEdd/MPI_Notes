// MPI_notes - App 02 - Master/Slave convention
/** MPI example that will send a number of items using send and receive commands */

// includes to import libraries
#include <iostream>
#include <mpi.h>

// messages for communicating tasks
int COMPUTE_AVERAGE = 1;
int SEND_AVERAGE = 2;
int SHUTDOWN = 3;

// the world rank and size that will be useful in many functions
int size;
int rank;

// method that will act as the master for this program
void master(void){
    // the total average of all the averages from the nodes
    float total_average = 0;
    // an average that we receive from a node
    float average = 0;

    // ask all three nodes to compute an average
    for (int i = 1; i < size; i++){
        MPI_Send(&COMPUTE_AVERAGE, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
    }

    std::cout << "Master (0): told all slaves to compute" << std::endl;

    //ask all three nodes to send their average to us
    for (int i = 1; i < size; i++){
        MPI_Send(&SEND_AVERAGE, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        MPI_Recv(&average, 1, MPI_FLOAT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        total_average += average;
    }

    // take the average of averages and display the result
    std::cout << "Master (0): average result from all slaves is: " << total_average / (size - 1) << std::endl;
}

// method that will act as the slave for this program
void slave(void){
    // the message type that we have received
    int message_type = 0;
    float average = 0;

    // keep looping until we receive a shutdown message
    MPI_Recv(&message_type, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

    while(message_type != SHUTDOWN){
        if(message_type == COMPUTE_AVERAGE){
            std::cout << "Slave" << rank << ": calculating average..." << std::endl;
            //get the average of 100 random numbers
            srand(rank);
            int sum = 0;
            for (int i = 0; i < 100; i++){
                sum += rand() % 10;
            }
            average = sum / 100.f;

            std::cout << "Slave" << rank << ": sum of 100 ints is " << sum << std::endl;
            std::cout << "Slave" << rank << ": average of 100 ints is " << average << std::endl;
            std::cout << "Slave" << rank << ": calculated average" << std::endl;
        } else if(message_type == SEND_AVERAGE){
            MPI_Send(&average, 1, MPI_FLOAT, 0, 0, MPI_COMM_WORLD);
            std::cout << "Slave" << rank << ": sent average" << std::endl;
        }
        MPI_Recv(&message_type, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    std::cout << "Slave" << rank << ": shutting down" << std::endl;
}

int main(int argc, char **argv){
    // always initialize the MPI library at the start of an MPI program
    MPI_Init(NULL, NULL);

    // on of the important tasks we have to establish is how many processes are 
    // in this MPI instance. This tells us who and what we have to communcate with
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // another important job is finding out which rank we are. We can use this rank number
    // to assign separate jobs to different MPI units
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    // process zero is the master, all other processes are slaves
    // call the appropriate function depending on which rank we are
    if(rank == 0){
        master();
    }else{
        slave();
    }

    // before ending the application, always finalize everything so MPI can shut down properly
    MPI_Finalize();

    //standard C/C++ thing to do, return 0 to the OS
    return 0;
}

