"""
Calculate motor power, array power throughout the race
"""

import pandas as pd

def main():
    mcmb_current_df = pd.read_csv("mcmb_current.csv")
    mcmb_voltage_df = pd.read_csv("mcmb_voltage.csv")
    car_speed_df = pd.read_csv("race_car_speed.csv")

    pptmb_current_df = pd.read_csv("pptmb_current.csv")
    pptmb_voltage_df = pd.read_csv("pptmb_voltage.csv")


    motor_power_df = mcmb_current_df["_value"].multiply(mcmb_voltage_df["_value"])
    array_power_df = pptmb_current_df["PPTMB"].multiply(pptmb_voltage_df["_value"])

    motor_power_df = pd.DataFrame({'Motor_Power(W)': motor_power_df, 'time': mcmb_current_df["_time"]})
    array_power_df = pd.DataFrame({'Array_Power(W)': array_power_df, 'time': pptmb_current_df["_time"]})
    car_speed_df = car_speed_df[['_time', '_value']]
    motor_power_df.to_csv("Motor_power1.csv", index=False)
    array_power_df.to_csv("Array_Power.csv", index=False)
    car_speed_df.to_csv("Car_speed.csv", index=False)

if __name__ == "__main__":
    main()
