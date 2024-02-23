"""
Given the result csv from a simulation, find the average and maximum of motor power
"""

import pandas as pd
import numpy as np
import sys
import argparse
from datetime import datetime, time

parser = argparse.ArgumentParser()
parser.add_argument('-p', '--path', type=str, default='./in.csv', help='Path to sim result csv (default: ./in.csv)')
parser.add_argument('-m', '--motor', type=str, default='Motor_Power(W)', help='Motor Power column name (default: Motor_Power(W))')
parser.add_argument('-t', '--time', type=str, default='time', help='Time column name (default: time)')

def t1_before_t2(t1:str, t2:datetime) -> bool:
    hours, minutes, seconds = map(int, t1.split(':'))

    # Convert the timestamps to seconds since midnight
    total_seconds1 = hours * 3600 + minutes * 60 + seconds
    total_seconds2 = t2.hour * 3600 + t2.minute * 60 + t2.second

    # Compare the timestamps
    if total_seconds1 < total_seconds2:
        return True
    else:
        return False

# Start and end times of each day (second day onwards)
start_time_str = '09:00:00'  
end_time_str = '17:00:00'  

def get_average_power(csv_path, array_col_name, datetime_col_name):
    df = pd.read_csv(csv_path)

    y = df[array_col_name].values

    max_power = float('-inf')
    x = list(pd.to_datetime(df[datetime_col_name], format="%a %b %d %H:%M:%S %Y"))
    #x = list(pd.to_datetime(df[datetime_col_name], format="%Y-%m-%d %H:%M:%S%z", utc=True) + pd.Timedelta(hours=9, minutes=30))

    assert len(x) == len(y)

    total_area = 0.0
    start_idx = 0
    end_idx = -1
    negative_zone = False
    total_time = 0.0 # in hours
    num_points = len(x)
    first_day = True
    end_loop = False

    print("Total number of points: ", num_points)
    while(end_idx < num_points-1):
        end_idx += 1

        # Take area once we reach an overnight stop
        if (t1_before_t2(end_time_str, x[end_idx])):
            if (end_idx > start_idx):
                first_datetime = x[start_idx]
                hours_from_start = np.array([(dt - first_datetime).total_seconds() / 3600 for dt in x[start_idx:end_idx]])
                total_area += np.trapz(y[start_idx:end_idx], hours_from_start)
                total_time += (x[end_idx-1]-x[start_idx]).total_seconds() / 3600
            elif (negative_zone):
                start_idx = end_idx

        # Skip overnight stops
        while ((not t1_before_t2(start_time_str, x[end_idx]) and not first_day) or t1_before_t2(end_time_str, x[end_idx])):
            end_idx += 1
            start_idx = end_idx
            first_day = False
            if (end_idx >= num_points):
                end_loop = True
                break
        
        if (end_loop):
            break
        # Find max power
        if (y[end_idx] > max_power):
            max_power = y[end_idx]

    # Add remaining area
    if (end_idx > start_idx and end_idx <= num_points):
        if (t1_before_t2(start_time_str, x[end_idx-1]) and not t1_before_t2(end_time_str, x[end_idx-1])):
            first_datetime = x[start_idx]
            hours_from_start = np.array([(dt - first_datetime).total_seconds() / 3600 for dt in x[start_idx:end_idx]])
            total_area += np.trapz(y[start_idx:end_idx], hours_from_start)
            total_time += (x[end_idx-1]-x[start_idx]).total_seconds() / 3600

    average_power = total_area / total_time
    print(total_area, total_time, max_power, average_power)

if __name__ == "__main__":
    args = parser.parse_args()
    csv_path = args.path
    motor_power_column_name = args.motor
    time_column_name = args.time
    get_average_power(csv_path, motor_power_column_name, time_column_name)


    