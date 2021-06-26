# Interface communication
## json format
```javascript
{
  "get": [
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
  ],
  "set": [
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
* [ ]  Sensor task
  * [ ]  Send and get information from each sensor. 1 task by sensor
* [ ]  Sensor_Manager task
  * [ ]  Create sensor task for each sensors
* [ ]  File task
  * [ ]  Read and write the json interface file
* [ ]  Control task
  * [ ]  Send and get information from each control. 1 task by control
* [ ]  Control_manager task
  * [ ]  Create Control task for each control


# Sensor communication

To determine
