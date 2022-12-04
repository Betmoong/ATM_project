# 잔액조회

import pymysql
import Except
import time

def check_the_balance():
    print("===========================================")
    print("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup")
    button = input("조회하실 계좌의 은행을 선택해주세요 : ")

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

    temp1 = input("계좌번호를 입력해주세요 : ")
    try:
        acc_number = int(temp1)

    except:
        print("clear")
        print("올바른 값을 입력해주세요!\n")
        return

    # 계좌 정보 일치 확인(불일치시 1 반환)
    check = Except.acc_discord(acc_number)
    if check == 1: return

    # 비밀번호 검사 메소드 호출 (비밀번호 반환 or (오류 or 비밀번호 3회 틀렸을 시 0반환))
    acc_password = Except.check_password(acc_number)
    if acc_password == 0:
        print("잠시 후 초기화면으로 돌아갑니다...")
        time.sleep(2)
        print("clear")
        return

    conn = pymysql.connect(host="localhost", user="root", password="cjch5391", db="atm_python", charset="utf8mb4")
    curs = conn.cursor()

    sql = "select * from bank where account_num = %s and acc_password = %s"
    curs.execute(sql, (acc_number, acc_password))

    rs = curs.fetchall()
    print("\n은행명\t이름\t\t계좌번호\t[잔액]")
    print("{}\t{}\t{}\t{}원\n".format(rs[0][0], rs[0][1], rs[0][2], rs[0][4]))

    okay = input("1번[거래종료], 아무키[초기화면] : ")
    print()

    if okay == "1":
        print("거래가 종료되었습니다.")
        exit()

    else:
        print("clear")
        return


