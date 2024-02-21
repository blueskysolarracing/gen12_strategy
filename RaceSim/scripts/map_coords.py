from geopy.distance import geodesic
from geopy import Point
import pandas as pd
import numpy as np

BASE_ROUTE_PATH = "../data/luts/wsc_2023/static/baseroute_nonspaced.csv"
CAR_SPEED_PATH = "./Car_speed.csv"
KPH_TO_MPS = 0.277778
format_specifier = '%Y-%m-%d %H:%M:%S%z'

def kph_to_mps(kph):
    return kph * KPH_TO_MPS

def main():
    """
    Assign a timestamp and speed to each coordinate
    """
    route_df = pd.read_csv(BASE_ROUTE_PATH)
    speed_df = pd.read_csv(CAR_SPEED_PATH)

    cur_speed_idx = 0
    time_column = [speed_df.iloc[0][0]]

    for index, row in route_df.iloc[:-1].iterrows():
        lat1 = row[0]
        lon1 = row[1]
        alt1 = row[2]

        lat2 = route_df.iloc[index+1][0]
        lon2 = route_df.iloc[index+1][1]
        alt2 = route_df.iloc[index+1][2]

        distance_to_cover = geodesic((lat1, lon1, alt1), (lat2, lon2, alt1)).meters
        distance_to_cover = np.sqrt(distance_to_cover**2 + (alt2-alt1)**2)

        # Find the range of speeds that cover the distance
        covered_distance = 0
        while (covered_distance < distance_to_cover):
            if (speed_df.iloc[cur_speed_idx][1] < 0):
                continue
            covered_distance += kph_to_mps(speed_df.iloc[cur_speed_idx][1])
            cur_speed_idx += 1
        if (index == 4700):
            print(speed_df.iloc[cur_speed_idx][0])
            return

        cur_speed_idx -= 1
        time_column.append(speed_df.iloc[cur_speed_idx][0])
        print(index, cur_speed_idx)

    route_df['time'] = time_column
    route_df.to_csv("./route_with_time.csv", index=False)
    
if __name__ == "__main__":
    main()




        