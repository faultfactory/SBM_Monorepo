# Data Pipeline

This folder contains the details on getting data out and any automated analysis that may be done. At the moment this is manual operation but in time potentially can be automated to remove this burden from the team.

## Logging
This folder contains the required tools for conversion and analysis for CAN data coming from the logger.
[An SD Card Can Logger](https://www.tindie.com/products/akpc806a/can-bus-logger-with-sd-card/) was purchased for the team. Based on a sample file format provided by the supplier.
A manual for the device is provided [here.](https://github.com/akpc806a/CAN_Logger/raw/master/Doc/CAN%20Logger%20(manual).pdf)<br><br>

## Conversion Scripts

```canlogger2mat.py``` is the current data logger to matlab conversion script. It will automatically parse the current .dbc file in the repository and using that information it will convert the raw CAN bytes to a .mat file for the team in engineering units that can be analyzed.

### Using the script

This requires an installation of Python3 and pip package manager for Python 3.

#### Preparation
- Install python3, however you choose to. 
- Install pip.
- Install the required packages for the script with ``` pip install -r requrements.txt ```

#### Running the script
From the environment you've used to install python3 type this:<br>
```python3 canlogger2mat.py <path to csv file>```<br>
It should have dropped a .mat file in the same directory. There is a test log file in the directory to test this with to validate you have done the process correctly. 




# TODO: 

- Add the ability to pull the .dbc from a specific git commit hash. 
- Deal with frame ids not in the .dbc. Signals we would not have the ability to tell
- Change output file names so the system can be used for a directory of multiple files.
