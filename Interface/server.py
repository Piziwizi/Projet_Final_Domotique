from flask import Flask, request, redirect, url_for, render_template, json, render_template_string, jsonify

app = Flask(__name__)

@app.route('/accueil')
def accueil():
    return render_template('accueil.html')

@app.route('/', methods=['GET', 'POST'])
def moduleChambre():

    if request.method == 'POST':
        if request.form.get('rs-range-line'):
            volume = request.form.get('rs-range-line')
            light = request.form.get('lightToggle')
            tempJson = {
                        "get": [
                            {
                            "id": "0",
                            "type": "temperature",
                            "value": volume
                            },
                            {
                            "id": "1",
                            "type": "light",
                            "value": light
                            }
                        ]
                        }
            with open("get.json", "w") as write_file:
                jsonFormattedData = json.dump(tempJson, write_file, indent=6, separators=(", ", ": "), sort_keys=True)
            return json.dumps({'volume': volume})
    return render_template('moduleChambre.html')

if __name__ == '__main__':
    app.run()
