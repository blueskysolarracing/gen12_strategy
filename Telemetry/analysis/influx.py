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

query1 = ' from(bucket: "bssr") \
  |> range(start: 2023-10-12T03:48:00Z, stop: 2023-10-12T06:48:00Z) \
  |> filter(fn: (r) => r["_measurement"] == "mqtt_consumer") \
  |> filter(fn: (r) => r["_field"] == "PPTMB") \
  |> filter(fn: (r) => r["topic"] == "bus_metrics/current/PPTMB") \
  |> aggregateWindow(every: 100ms, fn: mean, createEmpty: false) \
  |> yield(name: "mean")  \
  |> pivot(rowKey:["_time"], columnKey: ["_field"], valueColumn: "_value") \
  |> keep(columns: ["_time", "PPTMB"])'

# query2 = ' from(bucket: "bssr") \
#   |> range(start: -8h) \
#   |> filter(fn: (r) => r["_measurement"] == "mqtt_consumer") \
#   |> filter(fn: (r) => r["_field"] == "car_speed") \
#   |> aggregateWindow(every: 100ms, fn: mean, createEmpty: false) \
#   |> yield(name: "mean")  \
#   |> pivot(rowKey:["_time"], columnKey: ["_field"], valueColumn: "_value") \
#   |> keep(columns: ["_time", "car_speed"])'


# query3 = ' from(bucket: "bssr") \
#   |> range(start: 2023-10-12 03:48:00, stop: 2023-10-12 06:48:00) \
#   |> filter(fn: (r) => r["_measurement"] == "mqtt_consumer") \
#   |> filter(fn: (r) => r["_field"] == "MCMB") \
#   |> filter(fn: (r) => r["_topic"] == "bus_metrics/voltage/MCMB"  \
#   |> aggregateWindow(every: 100ms, fn: mean, createEmpty: false) \
#   |> yield(name: "mean")  \
#   |> pivot(rowKey:["_time"], columnKey: ["_field"], valueColumn: "_value") \
# '

# query3 = ' from(bucket: "bssr") \
#   |> range(start: -8h) \
#   |> filter(fn: (r) => r["_measurement"] == "mqtt_consumer") \
#   |> filter(fn: (r) => r["_field"] == "car_speed") \
#   |> aggregateWindow(every: 100ms, fn: mean, createEmpty: false) \
#   |> yield(name: "mean")  \
#   |> pivot(rowKey:["_time"], columnKey: ["_field"], valueColumn: "_value") \
#   |> keep(columns: ["_time", ""])'

#result = query_api.query(org=ORG, query=query)
result1 = query_api.query_data_frame(query1)
print(result1, "HELLO")

# df.to_csv("kai.csv")
