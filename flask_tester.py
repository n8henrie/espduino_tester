from flask import Flask, request
app = Flask(__name__)

ESPDUINO_IP = 'XXX.XXX.XXX.XXX'
INCOMING_TEST_STRING = 'This is my espduino test string!'
incoming_requests = []

@app.route('/', methods=['GET', 'POST'])
def count_requests():
    incoming_ip = request.remote_addr

    if incoming_ip == ESPDUINO_IP:
        if request.form['test_string'] == INCOMING_TEST_STRING and request.method == 'POST':
            incoming_requests.append(request.form['test_num'])
            msg = "All is peachy!"
    else:
        msg = "Hey, you're not the espduino!<br>"
        msg += str(len(incoming_requests)) + " requests so far.<br>"
        missing = set(range(100)) - set(map(int, incoming_requests))
        msg += "Missing: {}<br>".format(', '.join(map(str, missing)) if missing else "None!")
        msg += '<br>'.join(incoming_requests)
    return msg

if __name__ == '__main__':
    app.run(host='0.0.0.0')
