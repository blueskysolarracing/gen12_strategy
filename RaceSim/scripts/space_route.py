import csv
import geopy.distance
import sys

spacing = 0.08 # in km

def space_route(route_file):
    spaced_coords = []
    with open(route_file, 'r') as file:
        csv_reader = csv.reader(file)

        coord1 = None
        coord2 = None
        last_inserted = 0
        counter = 0

        for i, line in enumerate(csv_reader):
            counter += 1
            if (i == 0):
                coord1 = tuple((line[0], line[1]))
                spaced_coords.append(line)
                continue
            
            coord2 = tuple((line[0], line[1]))
            if geopy.distance.distance(coord1, coord2).km > spacing:
                spaced_coords.append(line)
                coord1 = coord2
                last_inserted = i

        if (last_inserted != counter):
            spaced_coords.append(line)

    with open(f"{route_file.split('.csv')[0]}_spaced.csv", 'w', newline='') as file:
        csv_writer = csv.writer(file)

        csv_writer.writerows(spaced_coords)

if __name__ == "__main__":
    route_file = sys.argv[1]
    space_route(route_file)
