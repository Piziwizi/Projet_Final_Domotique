# Interface communication
## json format
sudo apt install libjson-c-dev
2 files sensor.c and control.c
```javascript
[
  {
    "id": "0",
    "type": "temperature",
    "value": "25.0"
  },
  {
    "id": "1",
    "type": "light",
    "value": "1"
  }
]
```

# Internal
We will be using ptheard to be able to support multiple sensors.
Tasks:
* [ ] Sensor task
  * [ ]  Send and get information from each sensor. 1 task by sensor
* [ ]  Sensor_Manager task
  * [ ]  Create sensor task for each sensors
* [ ]  Interface task
  * [ ]  Read and write the json interface file
* [ ]  Control task
  * [ ]  Send and get information from each control. 1 task by control
* [ ]  Control_manager task
  * [ ]  Create Control task for each control


# Sensor communication
| Command              | Usage                           |
| -------------------- | ------------------------------- |
| SENSOR:TYPE:GET      | Get the sensor value            |
| CONTROL:TYPE:SET     | Set the control value           |
| CONTROL:TYPE:GET     | Get the control value           |
| REFRESH:SET          | Set the refresh period          |
| REFRESH:GET          | Get the refresh period          |
| SYSTEM:RESET         | Reset to the defaults values    |
| SYSTEM:START         | Start the aquisition            |
| SYSTEM:RESTART       | Restart the ESP32               |
| SYSTEM:STOP          | Stop the device                 |
| SYSTEM:WIFI:PASSWORD | New password                    |
| SYSTEM:WIFI:SSID     | New SSID                        |
| SYSTEM:WIFI:APPLY    | Apply the new password and SSID |
| SYSTEM:STATUS        | Get the status of the device    |

Telnet

If time ssh
