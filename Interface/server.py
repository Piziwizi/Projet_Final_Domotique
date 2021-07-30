from typing_extensions import Literal
from flask import Flask, request, redirect, url_for, render_template, json, render_template_string, jsonify

app = Flask(__name__)

@app.route('/accueil')
def accueil():
    return render_template('accueil.html')

@app.route('/moduleCuisine', methods=['GET', 'POST'])
def moduleCuisine():

    if request.method == 'POST':
        if request.form.get('rs-range-line'):
            temperature = request.form.get('rs-range-line')
            light = request.form.get('lightToggle')

            json_file = open("static/control.json")
            json_data = json.load(json_file)
            temperature2 = ""
            light2 = ""
            counter = 0
            for data in json_data:
                if(counter == 0):
                    temperature1 = data
                if(counter == 1):
                    light1 = data
                counter += 1

            if light is None:
                light = "0"
            else:
                light = "1"

            tempJson = [    
                            temperature1,
                            light1,
                            {
                            "id": "1",
                            "type": "temperature",
                            "value": temperature
                            },
                            {
                            "id": "1",
                            "type": "light",
                            "value": light
                            }
                        ]
            with open("static/control.json", "w") as write_file:
                jsonUnformattedData = json.dump(tempJson, write_file)
            return json.dumps({'volume': temperature})
    return render_template('moduleCuisine.html')

@app.route('/moduleSalon')
def moduleSalon():
    return render_template('moduleSalon.html')

@app.route('/', methods=['GET', 'POST'])
def moduleChambre():

    if request.method == 'POST':
        if request.form.get('rs-range-line'):
            temperature = request.form.get('rs-range-line')
            light = request.form.get('lightToggle')

            json_file = open("static/control.json")
            json_data = json.load(json_file)
            temperature2 = ""
            light2 = ""
            counter = 0
            for data in json_data:
                if(counter == 2):
                    temperature2 = data
                if(counter == 3):
                    light2 = data
                counter += 1

            if light is None:
                light = "0"
            else:
                light = "1"

            tempJson = [
                            {
                            "id": "0",
                            "type": "temperature",
                            "value": temperature
                            },
                            {
                            "id": "0",
                            "type": "light",
                            "value": light
                            },
                            temperature2,
                            light2
                        ]
            with open("static/control.json", "w") as write_file:
                jsonUnformattedData = json.dump(tempJson, write_file)
            return json.dumps({'volume': temperature})
    return render_template('moduleChambre.html')

if __name__ == '__main__':
    app.run(host='0.0.0.0' , port=5000)
