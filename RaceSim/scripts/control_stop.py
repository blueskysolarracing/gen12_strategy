import csv
import geopy.distance

def get_control_stops(base_route_path, control_stop_coords):
    """
    Find indices of control stops in base_route_path given a list of (lat, lon) coordinates
    """
    control_stop_dict = {}
    for i, c_stop in enumerate(control_stop_coords):
        control_stop_dict[c_stop] = [float("inf"), 0] # [minimum distance, index in base route csv]

    coords_tuple = []

    # Collect all route coordinates
    with open(base_route_path, 'r') as file:
        reader = csv.reader(file)
        for i, row in enumerate(reader):
            coords_tuple.append((row[0], row[1]))

    for i, coord in enumerate(coords_tuple):
        for key, value in control_stop_dict.items():
            dist = geopy.distance.distance(coord, key).km
            if (dist < value[0]):
                control_stop_dict[key][1] = i
                value[0] = dist

    for value in control_stop_dict.values():
        print(value[1])

if __name__ == "__main__":
    get_control_stops("../data/luts/wsc_2023/static/baseroute_nonspaced.csv", [(-14.450993, 132.267918), (-16.307058, 133.385789), (-19.657750, 134.188500), (-21.378798, 133.985635), (-23.708611, 133.875556), (-25.839111, 133.315722), (-29.011056, 134.754667), (-30.969861, 135.749000), (-32.509194, 137.796722)])