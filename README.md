## Reactor Simulation

This is a project of mine that I am working on in my freetime. I am hoping to learn a lot of nuclear physics from it.
The idea of this project is to make a free nuclear fission reactor simulator that is very realistic, one that anyone can quickly download and play around with.
In the future, I hope to add a graphical interface to this project, as the terminal can be pretty underwhelming.

## Running the simulation
### Windows
#### Prerequisites (won't be covered here)
- CMake (minimum 3.10): If you are unsure, here is a tutorial: [CMake Installation Tutorial - Windows](https://medium.com/@WamiqRaza/how-to-install-and-run-cmake-on-windows-692258b07b6f)
- A C++ compiler (like MinGW): If you are unsure, here is a tutorial: [MinGW Installation Tutorial - Windows](https://www.geeksforgeeks.org/installing-mingw-tools-for-c-c-and-changing-environment-variable/)

#### Steps
1. Download the ZIP from GitHub. Save it on your desktop and unzip it, there should now be a folder named 'ReactorSimulation-main' on your desktop. ![Download ZIP](https://github.com/user-attachments/assets/94ecd51d-4c85-4ae4-a398-1b402423c5ac)

2. Open the folder and click the address bar (shown in the picture), and copy the path. It should be something like 'C:\Users\User\Desktop\ReactorSimulation-main ![explorer_cmd-1-3560022720](https://github.com/user-attachments/assets/9b6d4b81-a8a3-4ee2-8431-8d8b107d028e)

3. Open a command prompt by going to the search bar and searching for 'cmd' or 'Command Prompt'. ![Screenshot from 2024-08-15 10-43-39](https://github.com/user-attachments/assets/595fbd23-c3a4-4ce6-ad32-eba7f3c3b615)

4. In the window that appears, type these commands one at a time: 
```bash
cd {path that you copied just before this}
mkdir build
cd build
cmake ..
make
```

5. Once the program has finished compiling, you just need to type this final command into the command prompt:
```bash
reactor.exe
```

### Linux
#### Prerequisites (won't be covered here)
- CMake (minimum 3.10): If you are unsure, here is a tutorial: [CMake (and GCC) Installation Guide - Linux](https://www.linuxfordevices.com/tutorials/linux/install-cmake-on-linux)
- A C++ Compiler (like GCC): If you are unsure, the previous tutorial covered this too.

#### Steps
1. Do steps 1 and 2 as windows does. The address bar is slightly different on Linux, but pretty similar still.

2. Open a terminal and run the same commands as shown in step 4 of windows.

3. Once the program finishes compiling, you need to type this into the terminal:
```bash
./reactor
```
