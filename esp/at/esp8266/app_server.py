# 用户测试POST请求 （http实际上就是tcp传输）

from flask import Flask, request

app = Flask(__name__)

@app.route('/', methods=['get','post'])
def index():
    print(request.data)
    return 'gett'

if __name__ == "__main__":
    app.run(host='0.0.0.0', port=80)