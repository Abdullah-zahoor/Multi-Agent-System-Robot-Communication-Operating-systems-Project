#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <semaphore.h>
#include <pthread.h>
#include <vector>

using namespace std;

// Constants for setting up the simulation environment
const int NumBots = 50; // Total number of robots in the simulation
const int RoomSize = 100; // Size of the room (100x100 units)
const int ExitX = 50; // X-coordinate of the exit
const int ExitY = 0; // Y-coordinate of the exit
const int NeighborDist = 5; // Maximum distance to consider another robot as a neighbor
const int ShmSize = (NumBots * 3 + 1) * sizeof(int); // Size of shared memory block
const int TrueWidth = 21; // Actual width of the exit (for comparison purposes)

// Structure to represent each robot's data
struct BotData {
    int x, y; // Position of the robot
    int width; // Robot's estimated width of the exit
    int* shm; // Pointer to shared memory
    sem_t* sem; // Pointer to semaphore for synchronization
};

// Function to calculate Euclidean distance between two points (x1, y1) and (x2, y2)
int calcDist(int x1, int y1, int x2, int y2) {
    return sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
}

// Function to estimate the exit width based on the robot's distance from the exit
int calcWidth(int dist, int trueW) {
    // Adjust accuracy based on distance
    double acc = (dist <= 5) ? 0.95 : (dist <= 10) ? 0.88 : max(0.75, 1 - (dist - 10) * 0.01);
    // Random fluctuation in the estimated width
    int fluct = (rand() % 21) - 10; // Fluctuation range: -10 to +10
    return static_cast<int>(trueW * acc) + fluct;
}

// Function to print the grid showing robot positions in the room
void showGrid(int* shm) {
    const int GridSize = 20; // Size of the grid for visual display
    const int Scale = RoomSize / GridSize; // Scaling factor to fit the room into the grid
    vector<vector<char>> grid(GridSize, vector<char>(GridSize, '.')); // Initialize grid with empty cells

    // Place robots on the grid based on their scaled positions
    for (int i = 0; i < NumBots; ++i) {
        int x = shm[i * 3] / Scale;
        int y = shm[i * 3 + 1] / Scale;
        if (x < GridSize && y < GridSize) grid[y][x] = 'R'; // Mark robot positions
    }

    // Print the grid
    for (const auto& row : grid) {
        for (char cell : row) cout << cell << " ";
        cout << endl;
    }
}

// Function executed by each robot thread
void* botTask(void* arg) {
    BotData* data = (BotData*) arg;
    // Introduce a delay based on the robot's initial X position
    sleep(data->x % 5);
    
    // Randomly assign a new position to the robot within the room
    data->x = rand() % RoomSize;
    data->y = rand() % RoomSize;
    
    // Calculate the estimated width based on the new position
    data->width = calcWidth(calcDist(data->x, data->y, ExitX, ExitY), TrueWidth);

    // Access shared memory to write robot data
    sem_wait(data->sem); // Wait on semaphore to ensure exclusive access
    data->shm[0] = data->x;
    data->shm[1] = data->y;
    data->shm[2] = data->width;
    sem_post(data->sem); // Release semaphore

    cout << "Bot (" << data->x << ", " << data->y << ") task done" << endl;
    pthread_exit(NULL);
}

// Main function to set up and execute the robot simulation
int main() {
    // Create a unique key for shared memory
    key_t key = ftok("shmfile", 65);
    
    // Allocate shared memory
    int shmid = shmget(key, ShmSize, 0666|IPC_CREAT);
    int* shm = (int*) shmat(shmid, (void*)0, 0);

    // Initialize semaphore for synchronizing access to shared memory
    sem_t sem;
    sem_init(&sem, 1, 1);

    srand(time(NULL));
    pid_t pid;
    pthread_t th;

    // Create child processes for each robot
    for (int i = 0; i < NumBots; ++i) {
        pid = fork();
        if (pid == 0) {
            // In child process: set up and execute robot task
            srand(getpid());
            BotData data;
            data.shm = shm + i * 3;
            data.sem = &sem;
            pthread_create(&th, NULL, botTask, (void*)&data);
            pthread_join(th, NULL);
            cout << "Bot " << i << " process done" << endl;
            exit(0);
        }
    }

    // Wait for all child processes to complete
    for (int i = 0; i < NumBots; ++i) wait(NULL);

    // Aggregate and display results after all robots have completed their tasks
    cout << "Waiting 10 seconds before aggregation..." << endl;
    sleep(10);

    int totalWidth = 0;
    // Calculate and display individual and global average width estimates
    for (int i = 0; i < NumBots; ++i) {
        int x = shm[i * 3], y = shm[i * 3 + 1], width = shm[i * 3 + 2], neighbors = 1;
        for (int j = 0; j < NumBots; ++j) {
            if (i != j) {
                int nx = shm[j * 3], ny = shm[j * 3 + 1];
                if (calcDist(x, y, nx, ny) <= NeighborDist) {
                    width += shm[j * 3 + 2];
                    neighbors++;
                }
            }
        }
        int avgWidth = width / neighbors;
        totalWidth += avgWidth;
        cout << "Bot [" << i << "] at (" << x << ", " << y << ") estimate: " << avgWidth << endl;
    }

    int avgGlobalWidth = totalWidth / NumBots;
    cout << "\nGlobal average width: " << avgGlobalWidth << endl;
    cout << "True width: " << TrueWidth << endl;
    cout << "Width difference: " << abs(avgGlobalWidth - TrueWidth) << endl;
    cout << "\nBot positions:" << endl;
    showGrid(shm);

    // Cleanup shared resources
    sem_destroy(&sem);
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

