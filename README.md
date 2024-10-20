# RobotArmSimulator
Robot Arm Simulator in SDL2. Watch as the Robot Types a String of Letters with a Keyboard using its Robot Arms. 
![61e61a06b208c6f77434c3697e42f78b](https://github.com/user-attachments/assets/17046770-58ef-4146-8fa0-bf528ec09450 | width=100)
## Usage

```RobotArmSimulator [FILE_PATH]``` \
If [FILE_PATH] is not set, then it will default to res/default.txt

## How to Compile
**Required Dependencies: SDL2, SDL2_ttf** \
**Disclaimer:** Currently, this compilation tutorial will use Visual Studio 2022 and Windows. However, if you choose not to, you can take the source/res files and try to compile it with other methods.
### Setup
1. Open the "**Edit the system environment variables**" setting
2. Click "**Edit the system environment variables**"
3. Create a new environment variable with the name **SDL2** and the value as **the path of your SDL2 and SDL2_ttf folders**
### Building
1. Open the .sln file
2. Run "**Build Solution**"
3. Copy the res folder and the SDL2 and SDL2_ttf .dll's to the build directory
4. Enjoy
