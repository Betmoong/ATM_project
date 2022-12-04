# 계좌생성

import pymysql
import time
import random

def create_acc():
    print("===========================================")
    print("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup")
    button = input("만드실 은행을 선택해주세요 : ")

    if button == "1":
        my_bank_name = "kakao"
        print("\n{}번 [{}] 은행을 선택하셨습니다.".format(button, my_bank_name))

    elif button == "2":
        my_bank_name = "woori"
        print("\n{}번 [{}] 은행을 선택하셨습니다.".format(button, my_bank_name))

    elif button == "3":
        my_bank_name = "shinhan"
        print("\n{}번 [{}] 은행을 선택하셨습니다.".format(button, my_bank_name))

    elif button == "4":
        my_bank_name = "hana"
        print("\n{}번 [{}] 은행을 선택하셨습니다.".format(button, my_bank_name))

    elif button == "5":
        my_bank_name = "nonghyup"
        print("\n{}번 [{}] 은행을 선택하셨습니다.".format(button, my_bank_name))

    else:
        print("\n올바른 값을 입력해주세요!")
        return

    # 신규 계좌번호(난수) 생성
    acc_number = random.randrange(11111111, 99999999) # (11111111 ~ 99999999) 범위
    print("신규 계좌번호 : {}".format(acc_number))
    name = input("이름을 입력해주세요 : ")
    temp1 = input("계좌 비밀번호를 입력해주세요 : ")
    try:
        acc_password = int(temp1)

    except:
        print("clear")
        print("올바른 값을 입력해주세요!\n")
        return

    # 계좌 비밀번호를 음수로 입력했을 경우
    if acc_password < 0:
        print("\n잘못 입력하셨습니다. 정확한 값을 입력해주세요!\n")
        return

    print("\n===========================")
    print("은행명칭 : {}\n계좌번호 : {}\n이름 : {}\n비밀번호 : {}".format(my_bank_name, acc_number, name, acc_password))
    print("===========================")
    print("위 정보가 맞으면 1번(계좌생성), 틀리면 2번(취소)을 입력해주세요.")
    okay = input("1번[계좌 생성], 2번[취소] : ")

    if okay == "1":
        conn = pymysql.connect(host="localhost", user="root", password="cjch5391", db="atm_python", charset="utf8mb4")
        curs = conn.cursor()

        sql = "insert into bank(bank_name, name, account_num, acc_password, balance) values(%s, %s, %s, %s, %s)"

        curs.execute(sql, (my_bank_name, name, acc_number, acc_password, 0))
        conn.commit()

        print("\n계좌생성이 완료되었습니다!\n잠시 후 초기화면으로 돌아갑니다...")
        time.sleep(2) # 2초간 지연
        print("clear") # 콘솔창 초기화

    elif okay == "2":
        print("\n취소버튼을 누르셨습니다.\n잠시 후 초기화면으로 돌아갑니다...")
        time.sleep(1.5)
        return

    else:
        print("\n잘못 입력하셨습니다.\n잠시 후 초기화면으로 돌아갑니다...")
        time.sleep(1.5)
        return


