import influxdb_client
import pandas as pd
BUCKET = "bssr"
ORG = "BSSR"
TOKEN = "K1h0Bk9xIbuJiihHAywIXFE_IKnpO_81NWaiRFLCokXC5PSUMDPwwIJQmR7-bnBKfCejTS_gCy-A8YfmIAyeHQ=="
URL = "http://localhost:8086"

client = influxdb_client.InfluxDBClient(
  url=URL,
  token=TOKEN,
  org=ORG
)

query_api = client.query_api()

"""Extract array power, motor power, car speed"""
pptmb_current_query = ' from(bucket: "bssr") \
  |> range(start: 2023-10-21T01:00:00Z, stop: 2023-10-27T08:00:00Z) \
  |> filter(fn: (r) => r["_measurement"] == "mqtt_consumer") \
  |> filter(fn: (r) => r["_field"] == "PPTMB") \
  |> filter(fn: (r) => r["topic"] == "bus_metrics/current/PPTMB")\
  |> aggregateWindow(every: 1000ms, fn: mean, createEmpty: false) \
  |> yield(name: "mean")  \
  |> pivot(rowKey:["_time"], columnKey: ["_field"], valueColumn: "_value") \
  |> keep(columns: ["_time", "PPTMB"])'

pptmb_voltage_query = ' from(bucket: "bssr") \
  |> range(start: 2023-10-21T01:00:00Z, stop: 2023-10-27T08:00:00Z) \
  |> filter(fn: (r) => r["_measurement"] == "mqtt_consumer") \
  |> filter(fn: (r) => r["_field"] == "PPTMB") \
  |> filter(fn: (r) => r["topic"] == "bus_metrics/voltage/PPTMB")\
  |> aggregateWindow(every: 1000ms, fn: mean, createEmpty: false) \
  |> yield(name: "mean")  \
  |> pivot(rowKey:["_time"], columnKey: ["_field"], valueColumn: "_value") \
  |> keep(columns: ["_time", "PPTMB"])'

mcmb_voltage_query = ' from(bucket: "bssr") \
  |> range(start: 2023-10-21T01:00:00Z, stop: 2023-10-27T08:00:00Z) \
  |> filter(fn: (r) => r["_measurement"] == "mqtt_consumer") \
  |> filter(fn: (r) => r["_field"] == "MCMB") \
  |> filter(fn: (r) => r["topic"] == "bus_metrics/voltage/MCMB")\
  |> aggregateWindow(every: 1000ms, fn: mean, createEmpty: false) \
  |> yield(name: "mean")  \
  |> pivot(rowKey:["_time"], columnKey: ["_field"], valueColumn: "_value") \
  |> keep(columns: ["_time", "MCMB"])'

mcmb_current_query = ' from(bucket: "bssr") \
  |> range(start: 2023-10-21T01:00:00Z, stop: 2023-10-27T08:00:00Z) \
  |> filter(fn: (r) => r["_measurement"] == "mqtt_consumer") \
  |> filter(fn: (r) => r["_field"] == "MCMB") \
  |> filter(fn: (r) => r["topic"] == "bus_metrics/current/MCMB")\
  |> aggregateWindow(every: 1000ms, fn: mean, createEmpty: false) \
  |> yield(name: "mean")  \
  |> pivot(rowKey:["_time"], columnKey: ["_field"], valueColumn: "_value") \
  |> keep(columns: ["_time", "MCMB"])'

speed_query = ' from(bucket: "bssr") \
  |> range(start: 2023-10-20T01:00:00Z, stop: 2023-10-27T08:00:00Z) \
  |> filter(fn: (r) => r["_measurement"] == "mqtt_consumer") \
  |> filter(fn: (r) => r["_field"] == "car_speed") \
  |> filter(fn: (r) => r["topic"] == "mcmb/car_speed")\
  |> aggregateWindow(every: 1000ms, fn: mean, createEmpty: false) \
  |> yield(name: "mean")  \
  |> pivot(rowKey:["_time"], columnKey: ["_field"], valueColumn: "_value") \
  |> keep(columns: ["_time", "MCMB"])'

pptmb_current = query_api.query_data_frame(pptmb_current_query)
pptmb_voltage = query_api.query_data_frame(pptmb_voltage_query)
mcmb_current=  query_api.query_data_frame(mcmb_current_query)
mcmb_voltage = query_api.query_data_frame(mcmb_voltage_query)
car_speed = query_api.query_data_frame(speed_query)

speed_df = car_speed[0]
pptmb_voltage_df = pptmb_voltage[0]
pptmb_current_df = pptmb_current[0]
mcmb_current_df = mcmb_current[0]
mcmb_voltage_df = mcmb_voltage[0]

speed_df.to_csv('race_car_speed.csv')
pptmb_voltage_df.to_csv('pptmb_voltage.csv')
pptmb_current_df.to_csv('pptmb_current.csv')
mcmb_current_df.to_csv('mcmb_current.csv')
mcmb_voltage_df.to_csv('mcmb_voltage.csv')