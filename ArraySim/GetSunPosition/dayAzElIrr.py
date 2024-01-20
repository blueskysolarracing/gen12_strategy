"""
Generate a 180 x 4 csv with columns: 
azimuth (degrees) | elevation (degrees) | irradiance (W/m^2) | time (24 hour)
"""

from pysolar.solar import *
import datetime
import sys
import csv

OUTPUT_CSV_NAME = "sun_position.csv"

def generate_irr_csv(lat: float, lon: float, start_time: str, end_time: str, utc_adjustment: float, num_timesteps: int):
    """
    Generates the 180 x 4 csv and saves it to a sun_position.csv file in the current directory
    @param lat: latitude in degrees
    @param lon: longitude in degrees
    @param start_time: start time of the simulation in "YYYY-MM-DD HH:MM:SS" format in 24 hour local time
    @param end_time: end time of the simulation in "YYYY-MM-DD HH:MM:SS" format in 24 hour local time
    @param utc_adjustment: conversion from local time to utc. Note that for Alice Springs Australia, this is -9.5
    @param num_timesteps: Number of timesteps between start_time and end_time inclusive

    Example: python dayAzElIrr.py -23.6980 133.8807 "2023-10-22 06:00:00" "2023-10-22 19:00:00" -9.5 180 180 to simulate in Alice Springs
    with a bearing of 180 degrees (pointing south) and 180 timesteps
    """

    # Local Times
    sim_start_time = datetime.datetime.strptime(start_time, '%Y-%m-%d %H:%M:%S')
    sim_end_time = datetime.datetime.strptime(end_time, '%Y-%m-%d %H:%M:%S')

    # UTC Times
    utc_time_delta = datetime.timedelta(hours=utc_adjustment)
    sim_start_time_utc = (sim_start_time + utc_time_delta).replace(tzinfo=datetime.timezone.utc)

    timestep = (sim_end_time - sim_start_time) / num_timesteps

    curr_time_utc = sim_start_time_utc
    curr_time = sim_start_time
    output_csv = []
    
    for _ in range(0, num_timesteps):
        az = get_azimuth(lat, lon, curr_time_utc)  
        el = get_altitude(lat, lon, curr_time_utc)
        irradiance = radiation.get_radiation_direct(curr_time_utc, el)

        output_csv.append([az, el, irradiance, curr_time.strftime('%Y-%m-%d %H:%M:%S')])

        curr_time_utc += timestep
        curr_time += timestep
    
    # Write data to csv
    with open(OUTPUT_CSV_NAME, 'w', newline='') as csv_file:
        # Create a CSV writer object
        csv_writer = csv.writer(csv_file)

        # Write the data to the CSV file
        csv_writer.writerows(output_csv)

if __name__ == "__main__":
    lat = float(sys.argv[1])
    lon = float(sys.argv[2])
    start_time = sys.argv[3]
    end_time = sys.argv[4]
    utc_adjustment = float(sys.argv[5])
    num_timesteps = int(sys.argv[6])

    generate_irr_csv(lat, lon, start_time, end_time, utc_adjustment, num_timesteps)
