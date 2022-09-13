# Using this Repo

## Setup
 - Enable Developer Mode on Windows 10.
    - [Instructions here.](https://docs.microsoft.com/en-us/windows/apps/get-started/enable-your-device-for-development)
- Install git for Windows. **During installation make sure to Enable Symbolic Links.**  
    - [Download page here.](https://git-scm.com/download/win) Get the 64 bit installer. 
- Install the Arduino IDE. Don't get the fancy new one. Get the standard 1.8.x
    - [Download page here.](https://www.arduino.cc/en/software)
- Install the board packages for the Feather M4 CAN.
    - [Instructions and address provided here.](https://learn.adafruit.com/adafruit-feather-m4-can-express/arduino-ide-setup) Should be self explanatory.
    - You should not need a special driver for this. Should just plug in and be visible after selecting a COM Port. 
    - Verify you can connect to the board and see output on the Serial Monitor in the Arduino IDE.
- Install the CAN library for the Feather M4 CAN. It is titled ```CAN Adafruit Fork```.
    - [Instructions on adding libraries provided here.](https://docs.arduino.cc/software/ide-v1/tutorials/installing-libraries)
- Optional: Install VSCode with reccomended packages:
    - ```vsciot-vscode.vscode-arduino```
    - ```ms-vscode.cpptools```
    - ```eamodio.gitlens```
- In order to contribute code you will need to set up a github account and and SSH Key to authenticate for pushing to the repository.
    - [Here](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/generating-a-new-ssh-key-and-adding-it-to-the-ssh-agent) and [here](https://docs.github.com/en/authentication/connecting-to-github-with-ssh/adding-a-new-ssh-key-to-your-github-account#adding-a-new-ssh-key-to-your-account) describe how you generate and integrate the SSH Key.
    - I also suggestion following the suggestions [here](https://docs.github.com/en/account-and-profile/setting-up-and-managing-your-personal-account-on-github/managing-email-preferences/setting-your-commit-email-address) for using the provided github noreply email address for your ```user.email```. 
- Clone this repository. Again use cmd or powershell. Not git bash or WSL.

Newbies, reach out of this is not complete enough. This is all very straightforward to me but may be daunting to those with less time in the trenches. 
## Arduino, VSCode and Some Silly Restrictions:
Arduino abstracts a lot of SW development pain away from us but it has some silly things going on such as a forced directory structure.
It keeps things simplified for software noobs so I suggest to stick with it. Other embedded programming frameworks are complicated.
Amoung other things, here's what it always wants to see:
- Parent directory and Sketch file name match. 
- In the parent directory, another directory ```src``` exists that contains code that is part of the sketch but not an official or installed library.
<br>

Because the parent directory and sketch file have the aformenetioned name requirements, you can't compile multiple sketches using the same ```src``` directory. That means we have to have the CAN interface files in the ```src``` directory of each sketch directory. Instead of making copies, we leverage symbolic links which are just shortcuts pointing to another location. This is why Developer Mode is required. It's a newer windows feature.

Another pan point is the VSCode Arduino plugin. When opening a folder, VScode will create the ```.vscode``` directory for certain settings. An ```arduino.json``` will be created defining some settings for the board type and including the sketch files. So if you're using VSCode for compilation and serial monitor, just open the folder with the ```.ino``` file. <br>
If you don't intend to use the Arduino features of VSCode, don't worry about it. It's finicky for uploading.

# Making a New Node for the Network: 

1. Leave the CAN_Node_Example as-is. That's a template. Review it carefully and get a feel for what it's doing. As the team asks me questions on how to execute something, I would be making my updates and changes there.
2. When you're ready to create a new node, make a copy of that directory to the root of the repo. This will copy the required symbolic link as well.
3. Rename the Sketch file and the Folder you've copied to reflect the device you're making code for.
4. Make sure you're using git from cmd or powershell (i.e. Not git bash or WSL). Line endings get swapped between OS's in git.
5. Do not touch the ```can_conv``` directory in the root of the repo. You'll break the symbolic links.
6. When editing the .dbc file, only use Vector CANDb++ editor and only add signals and messages.
     - Code is automatically generated based on signal and message names along with the message ID values. Changing these will break code referencing that generated code. 
     - It is okay to re-arrange existing signals or add signals within a message as the generated code abstracts that away. 
     - Do not move signals to another message unless the team has buy-in and you are ready to change code to accomodate the move.