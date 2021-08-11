# Interface communication
## json format
2 files sensor.json and control.json
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

# Internal  TODO UPDATE THE TASKS
We will be using ptheard to be able to support multiple sensors.
Tasks:
* [X] Sensor task
  * [X]  Send and get information from each sensor. 1 task by sensor
* [X] Search Sensor task
  * [X]  Create sensor task for each sensors and search from them
* [X] Save Sensor task
  * [X]  Save the sensor in a json string
* [X] Save Control task
  * [X]  Save the control in a json string
* [X] Read Control task
  * [X]  Read the control in a json string
* [X]  Interface task
  * [X]  Read and write the json interface file
* [X]  Logging task
  * [X]  Write the logging file


# Sensor communication
| Command                | Usage                        | Done |
| ---------------------- | ---------------------------- | ---- |
| SENSOR:TYPE:GET        | Get the sensor value         | X    |
| CONTROL:TYPE:SET:value | Set the control value        | X    |
| CONTROL:TYPE:GET       | Get the control value        | X    |
| BUTTON:TYPE:GET        | Get the button value         | X    |
| SYSTEM:RESET           | Reset to the defaults values |      |
| SYSTEM:START           | Start the aquisition         | X    |
| SYSTEM:RESTART         | Restart the ESP32            | X    |
| SYSTEM:STOP            | Stop the device              |      |
| SYSTEM:STATUS          | Get the status of the device |      |
| SYSTEM:GET_ID          | Get the id of the device     | X    |

ssid=HOMETS
wpa_passphrase=VerySecurePassword3!

https://thepi.io/how-to-use-your-raspberry-pi-as-a-wireless-access-point/