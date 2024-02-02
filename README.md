Overview
The Robot Simulation Program offers an advanced simulation environment in which 50 automated robots calculate the estimated width of an exit within a virtual 100x100 room. Each robot, functioning as an individual process, computes an estimate by gauging its proximity to the exit. This sophisticated software not only provides a visual layout of the robot's positions but also aggregates a collective average of their width estimations.
System Requirements
Operating System: Unix-based (e.g., Linux, macOS)
Compiler: C++ (GCC recommended)
Environment: POSIX compliant for threading and semaphore functionalities
Installation Guide
This installation guide provides detailed steps to compile and run the Robot Simulation Program on a Unix-based operating system. Ensure that you have a C++ compiler and a POSIX-compliant environment before proceeding.
Prerequisites
Before you begin, verify that your system meets the following requirements:
Unix-based operating system (Linux or macOS preferred)
GNU C++ Compiler (g++) installed
Access to terminal or command-line interface
POSIX-compliant environment for threading and semaphore support
Step 1: Source Code Preparation
Obtain Source Code: Acquire the robot_simulation.cpp file which contains the source code for the robot simulation.
Save Source Code: Place the robot_simulation.cpp file in a directory of your choice. This location will be where you compile and run the program.
Step 2: Compilation
Open your terminal and navigate to the directory where the robot_simulation.cpp file is saved.
Navigate to Source Directory:
bash
Copy code
cd path/to/source_code_directory
Replace path/to/source_code_directory with the actual path to the directory containing your robot_simulation.cpp file.
Compile the Program:
Enter the following command to compile the source code into an executable:
bash
Copy code
g++ -o robot_simulation robot_simulation.cpp -lpthread -lrt
Here’s what each part of the command does:
g++: Calls the GNU C++ Compiler.
-o robot_simulation: Specifies the output file name for the compiled program.
robot_simulation.cpp: The source code file you are compiling.
-lpthread: Links the pthread library for threading support.
-lrt: Links the real-time library which may be required for some system functions.
Compilation Confirmation:
After running the command, if there are no errors, it will complete silently, and you should see a new file named robot_simulation in the directory. This file is your compiled executable.
Step 3: Execution Instructions
With the program compiled, you are now ready to run the simulation.
Launching the Simulation:
In the terminal, ensure you’re in the same directory as the robot_simulation executable. Run the program with the following command:

bash
Copy code
./robot_simulation
This command executes the robot_simulation program you compiled in the previous step.
Program Execution:
Upon execution, the program will start the robot simulation. Follow any on-screen instructions to interact with the program.
Additional Tips
Permissions: If you encounter a permission error when trying to execute the program, you may need to change the executable permissions of the file with the command chmod +x robot_simulation.
Troubleshooting: If there are any errors during compilation, check that all dependencies are installed and that the source code file does not contain any errors.

Features and Functionalities
⦁	Robot Processes: Each of the 50 robots operates as a separate process within the simulation.
⦁	Estimation Mechanism: Robots estimate the exit width based on their calculated distance from a fixed exit location.
⦁	Neighborhood Algorithm: Estimates from adjacent robots (within a 5-unit range) are considered for a refined average estimation.
⦁	Aggregate Analysis: Post individual calculations, the system determines and exhibits the overall average estimated width.
⦁	Graphical Display: A concise grid showcases the robots' distribution within the simulated space.
⦁	Operational Delay: Introduces a 10-second pause prior to data consolidation, mimicking real-time processing conditions.
Output Details
⦁	Robot Metrics: Each robot's estimated width and coordinates are displayed.
⦁	Average Width Computation: The program computes and presents a global average of the robots' estimations.
⦁	Exit Width Evaluation: It contrasts the actual width of the exit with the computed average, detailing the variance.
⦁	Grid Visualization: Robots are depicted within a condensed 20x20 grid overview.
Important Notes
Inter-process Coordination: Shared memory and semaphores are instrumental for the synchronization and communication between processes.
Visual Grid Constraints: The grid is a reduced representation and might not precisely illustrate robot clusters due to its scaled nature.
Troubleshooting
⦁	Compilation Issues: Confirm the accuracy of the compilation command and the presence of necessary libraries.
⦁	Operational Discrepancies: Ensure the OS is POSIX-compliant, supporting the requisite threads and semaphores.
Support and Feedback
Should you require further support or wish to provide feedback, please reach out to the development team or our customer support.
