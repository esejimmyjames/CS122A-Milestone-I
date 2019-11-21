import os
import spidev 
import time

from flask import Flask, render_template
from flask import jsonify

PEOPLE_FOLDER = os.path.join('static', 'people_photo')


app = Flask(__name__)
app.config['UPLOAD_FOLDER'] = PEOPLE_FOLDER


# Calculate running average
welford={'mean':0,'s':0,'numValues':0}
def welfordsAlgorithm(newLightValue):
    _numValues = welford.get('numValues')
    if _numValues is 0:
        _numValues = 1
        _mean = newLightValue
        _s = 0
    else:
        _numValues += 1
        _oldMean = welford.get('mean')
        _mean = _oldMean + (newLightValue - _oldMean) / _numValues
        _s = welford.get('s')
        _s = _s + ((newLightValue-_oldMean) * (newLightValue-_mean))
    welford['numValues'] = _numValues
    welford['mean'] = _mean
    welford['s'] = _s
    return {'Average':_mean,'Outlier':None,'Requests':_numValues}
        

@app.route('/submitLightValue/<int:lightValue>')
def submitLightValue(lightValue):
    return jsonify(welfordsAlgorithm(lightValue))

@app.route('/')
def index():
    return jsonify({'Average': welford.get('mean'),
            'Requests': welford.get('numValues')})

@app.route('/index')
def show_index():
    full_filename = os.path.join(app.config['UPLOAD_FOLDER'], 'shovon.jpg')
    return render_template("index.html", user_image = full_filename)

if __name__ == '__main__':
    app.run(debug=True,host='192.168.43.140')
