from flask import Flask, render_template, jsonify, request

app = Flask(__name__)

from pymongo import MongoClient

client = MongoClient('mongodb://test:test@localhost', 27017)
# client = MongoClient('localhost', 27017)
db = client.bank


## HTML을 주는 부분
@app.route('/')
def home():
    return render_template('index.html')


## API 역할을 하는 부분
@app.route('/createAccount', methods=['POST'])
def write_client_info():
    bank_name_receive = request.form['bankName_give']
    acc_number_receive = request.form['accNumber_give']
    name_receive = request.form['name_give']
    password_receive = request.form['password_give']

    doc = {
        'bankName': bank_name_receive,
        'accNumber': acc_number_receive,
        'name': name_receive,
        'password': password_receive,
        'balance': 0,
        'error': 0
    }

    db.atm.insert_one(doc)
    return jsonify({'msg': '계좌 생성이 완료되었습니다!'})


@app.route('/depositChangeInfo', methods=['POST'])
def changeDP_client_info():
    acc_number_receive = request.form['accNumber_give']
    money_receive = int(request.form['money_give'])

    target_acc = db.atm.find_one({'accNumber': acc_number_receive})
    current_balance = target_acc['balance']

    new_balance = current_balance + money_receive

    db.atm.update_one({'accNumber': acc_number_receive}, {'$set': {'balance': new_balance}})

    return jsonify({'msg': '입금이 완료되었습니다!'})


# 비밀번호 error 카운트
@app.route('/checkPasswordError', methods=['POST'])
def check_password_error():
    acc_number_receive = request.form['accNumber_give']
    acc_password_receive = request.form['password_give']

    target_acc = db.atm.find_one({'accNumber': acc_number_receive})
    password = target_acc['password']
    current_error_count = target_acc['error']
    error_return = -1

    if current_error_count > 4:
        return jsonify({'acc_password': error_return}, {'receive_password': acc_password_receive})

    elif password != acc_password_receive:
        db.atm.update_one({'accNumber': acc_number_receive}, {'$set': {'error': current_error_count + 1}})
        return jsonify({'acc_password': password}, {'receive_password': acc_password_receive})

    elif password == acc_password_receive:
        return jsonify({'acc_password': password}, {'receive_password': acc_password_receive})


# 현재 계좌에 저장된 error 횟수 반환
@app.route('/getError', methods=['GET'])
def get_error_count():
    bank_name_receive = request.args.get('bankName_give')
    acc_number_receive = request.args.get('accNumber_give')

    currentError = db.atm.find_one({'accNumber': acc_number_receive, 'bankName': bank_name_receive})['error']

    return jsonify({'error': currentError})


# 계좌에 저장된 error 횟수 초기화
@app.route('/initPasswordError', methods=['POST'])
def init_password_error():
    acc_number_receive = request.form['accNumber_give']

    target_acc = db.atm.find_one({'accNumber': acc_number_receive})['accNumber']

    db.atm.update_one({'accNumber': target_acc}, {'$set': {'error': 0}})

    return jsonify({'msg': 'error 초기화 완료!'})


@app.route('/withdrawChangeInfo', methods=['POST'])
def changeWD_client_info():
    acc_number_receive = request.form['accNumber_give']
    money_receive = int(request.form['money_give'])

    target_acc = db.atm.find_one({'accNumber': acc_number_receive})
    current_balance = target_acc['balance']

    new_balance = current_balance - money_receive

    db.atm.update_one({'accNumber': acc_number_receive}, {'$set': {'balance': new_balance}})

    return jsonify({'msg': '출금이 완료되었습니다!'})


@app.route('/remitChangeInfo', methods=['POST'])
def changeRM_client_info():
    my_acc_number_receive = request.form['myAccNumber_give']
    your_acc_number_receive = request.form['yourAccNumber_give']
    my_bank_name_receive = request.form['myBankName_give']
    your_bank_name_receive = request.form['yourBankName_give']
    money_receive = int(request.form['money_give'])

    target_my_acc = db.atm.find_one({'accNumber': my_acc_number_receive})
    current_my_balance = target_my_acc['balance']

    target_your_acc = db.atm.find_one({'accNumber': your_acc_number_receive})
    current_your_balance = target_your_acc['balance']

    if my_bank_name_receive == your_bank_name_receive:
        new_my_balance = current_my_balance - money_receive
        new_your_balance = current_your_balance + money_receive
        db.atm.update_one({'accNumber': my_acc_number_receive}, {'$set': {'balance': new_my_balance}})
        db.atm.update_one({'accNumber': your_acc_number_receive}, {'$set': {'balance': new_your_balance}})
        return jsonify({'msg': '송금이 완료되었습니다!'})

    else:
        new_my_balance = (current_my_balance - money_receive) - 500
        new_your_balance = current_your_balance + money_receive
        db.atm.update_one({'accNumber': my_acc_number_receive}, {'$set': {'balance': new_my_balance}})
        db.atm.update_one({'accNumber': your_acc_number_receive}, {'$set': {'balance': new_your_balance}})
        return jsonify({'msg': '송금이 완료되었습니다!'})


# 변경 비밀번호 DB에 반영(계좌번호, 변경 비밀번호를 클라이언트에서 받음)
@app.route('/ChangeAccPassword', methods=['POST'])
def change_acc_password():
    acc_number_receive = request.form['accNumber_give']
    new_password_receive = request.form['password_give']

    new_password = new_password_receive

    target_acc = db.atm.find_one({'accNumber': acc_number_receive})
    current_password = target_acc['password']

    # 새로운 비밀번호 입력값이 기존 비밀번호와 같을 경우
    if current_password == new_password:
        return jsonify({'result': 'false', 'msg': '새로운 비밀번호를 입력해주세요!'})
    else:
        db.atm.update_one({'accNumber': acc_number_receive}, {'$set': {'password': new_password}})
        return jsonify({'msg': '비밀번호 변경이 완료되었습니다!'})


# 은행 이름과 계좌번호 유효값 확인, 입력 받은 정보와 일치 하는 계좌 소유자의 이름 반환(입금 part)
@app.route('/deposit', methods=['GET'])
def read_name():
    bank_name_receive = request.args.get('bankName_give')
    acc_number_receive = request.args.get('accNumber_give')

    name = db.atm.find_one({'accNumber': acc_number_receive, 'bankName': bank_name_receive})['name']

    return jsonify({'accName': name})


# 거래 후 잔액 반환(입금 part)
@app.route('/balance', methods=['GET'])
def read_balance():
    bank_name_receive = request.args.get('bankName_give')
    acc_number_receive = request.args.get('accNumber_give')

    currentBalance = db.atm.find_one({'accNumber': acc_number_receive, 'bankName': bank_name_receive})['balance']

    return jsonify({'balance': currentBalance})


# 은행 이름, 계좌번호, 비밀번호 유효값 확인 후 현재 잔액 반환(출금 part)
@app.route('/withdraw', methods=['GET'])
def check_info():
    bank_name_receive = request.args.get('bankName_give')
    acc_number_receive = request.args.get('accNumber_give')
    acc_password_receive = request.args.get('accPassword_give')

    name = \
        db.atm.find_one(
            {'accNumber': acc_number_receive, 'bankName': bank_name_receive, 'password': acc_password_receive})[
            'name']
    currentBalance = \
        db.atm.find_one(
            {'accNumber': acc_number_receive, 'bankName': bank_name_receive, 'password': acc_password_receive})[
            'balance']

    return jsonify({'name': name}, {'balance': currentBalance})


# 수취인 은행, 계좌번호가 DB에 저장된 값과 일치한 지 확인(송금 part)
@app.route('/checkRemit', methods=['GET'])
def check_remit():
    my_acc_number_receive = request.args.get('myAccNumber_give')
    your_acc_number_receive = request.args.get('yourAccNumber_give')
    your_bank_name_receive = request.args.get('yourBankName_give')

    my_acc_number = db.atm.find_one({'accNumber': my_acc_number_receive})['accNumber']
    your_acc_number = db.atm.find_one({'accNumber': your_acc_number_receive, 'bankName': your_bank_name_receive})[
        'accNumber']
    your_name = db.atm.find_one({'accNumber': your_acc_number_receive, 'bankName': your_bank_name_receive})['name']

    return jsonify({'my_acc_number': my_acc_number}, {'your_acc_number': your_acc_number}, {'your_name': your_name})


# 관리자 비밀번호 확인
@app.route('/getAdminPassword', methods=['GET'])
def get_admin_password():
    admin_password_receive = request.args.get('admin_password_give')

    password = db.atm.find_one({'name': 'admin', 'password':  admin_password_receive})['password']

    return jsonify({'admin_password': password})


# 관리자 비밀번호 확인
@app.route('/viewAccInfo', methods=['GET'])
def view_acc_info():
    db_acc_info = list(db.atm.find({'name': {'$ne': 'admin'}}, {'_id': False}))
    return jsonify({'acc_info': db_acc_info})


if __name__ == '__main__':
    app.run('0.0.0.0', port=5000, debug=True)
