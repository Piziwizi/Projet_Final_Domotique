from typing_extensions import Literal
from flask import Flask, request, redirect, url_for, render_template, json, render_template_string, jsonify

app = Flask(__name__)

@app.route('/accueil')
def accueil():
    return render_template('accueil.html')

@app.route('/moduleCuisine')
def moduleCuisine():
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

            if light is None:
                light = "off";

            tempJson = [
                            {
                            "id": "0",
                            "type": "temperature",
                            "value": temperature
                            },
                            {
                            "id": "1",
                            "type": "light",
                            "value": light
                            }
                        ]
            with open("control.json", "w") as write_file:
                jsonUnformattedData = json.dump(tempJson, write_file)
            return json.dumps({'volume': temperature})
    return render_template('moduleChambre.html')

if __name__ == '__main__':
    app.run(host='0.0.0.0' , port=5000)
