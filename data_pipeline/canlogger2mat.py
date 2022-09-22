# -*- coding: utf-8 -*-
"""
Created on Sun Sep 11 22:34:52 2022

@author: faultfactory
"""

#!/usr/bin/env python3

import cantools
import requests
import sys
import h5py
import hdf5storage
import csv
import numpy as np

def make_array_name(message):
    return str(hex(message.frame_id)) + '_' + str(message.name)


# total arguments
n = len(sys.argv)
if n == 1:
    print("Script requires 1 or 2 arguments:")
    print("Argument 1: Path to csv file for conversion")
    print("(In Development)If the second argument is not provided, the current main branch .dbc will be used from github")
    print("(In Development)Argument 2: git hash for .dbc file used to record data")
    sys.exit()  # You screwed up. This relationship can't go on and I'm leaving you. 
    

if n>1:
    print("File path provided: " + sys.argv[1])
    
data_file_path = sys.argv[1]

# grab the dbc file from the web and load it into to a string in local memory. 
dbc_string = requests.get('https://raw.githubusercontent.com/faultfactory/SBM_Monorepo/main/dbc/sbm_network_definition.dbc').text

database = cantools.database.Database()
database.add_dbc_string(dbc_string)

csvfile = open(data_file_path, newline='')
csvreader = csv.reader(csvfile, delimiter=' ', quotechar='|')
next(csvreader) # The first line has a garbage header layout. 


array_output_dict={}
    
for row in csvreader:
    line = row[0].split(",")
    time_value_s = float(line[0])/1000.
    # find the cantools message object that has this frame_id
    msg_obj = database.get_message_by_frame_id(int(line[1],16)) 
    data_field = bytes(int(oct,16) for oct in line[2:10])
    # python dict containing decoded data in message. 
    msg_data= database.decode_message(int(line[1],16),data_field)
    
    ## create array output information
    array_prefix = "CAN_"+(make_array_name(msg_obj))
    time_array_name = array_prefix+"_Time_s"
    if time_array_name in array_output_dict:  # if the message has already been written to,
        array_output_dict[time_array_name].append(time_value_s)  # then append timestamp to it. 
        for data_field in msg_data.items(): 
            data_array_name = array_prefix + '_' + data_field[0]
            array_output_dict[data_array_name].append(data_field[1])  # then append message data to it. 
    else:                                     # if it has not been written to
        array_output_dict[time_array_name] = list([time_value_s])  ## create the field
        for data_field in msg_data.items(): 
            data_array_name = array_prefix + '_' + data_field[0]
            array_output_dict[data_array_name]= list([data_field[1]])  # then create the data fields.
    
# convert all fields to numpy arrays for scipy. 
for item in array_output_dict.items():
    array_output_dict[item[0]] = np.float32(item[1])

hdf5storage.savemat('converted_from_data_logger.mat',array_output_dict)

