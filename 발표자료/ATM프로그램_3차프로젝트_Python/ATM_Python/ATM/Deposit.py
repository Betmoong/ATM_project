# 입금

import pymysql
import Except
import time

def deposit():
    print("===========================================")
    print("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup")
    button = input("입금하실 계좌의 은행을 선택해주세요 : ")

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
    check = Except.acc_discord(acc_number, my_bank_name)
    if check == 1:return

    # 계좌 정지 여부 확인
    if Except.acc_stop(acc_number) == 3:
        print("\n정지된 계좌입니다.\n관리자에게 문의해주세요.\n")
        print("잠시 후 초기화면으로 돌아갑니다...")
        time.sleep(2)
        print("clear")
        return

    # 비밀번호 검사 메소드 호출 (비밀번호 반환 or (오류 or 비밀번호 3회 틀렸을 시 0반환))
    acc_password = Except.check_password(acc_number)
    if acc_password == 0:
        print("잠시 후 초기화면으로 돌아갑니다...")
        time.sleep(2)
        print("clear")
        return

    temp2 = input("입금액을 입력해주세요 : ")
    try:
       money = int(temp2)

    except:
        print("clear")
        print("올바른 값을 입력해주세요!\n")
        return

    if money < 0:
        print("\n잘못 입력하셨습니다. 정확한 값을 입력해주세요!\n")
        return

    print("clear") # 콘솔창 초기화

    print("\n===========================")
    print("은행명칭 : {}\n계좌번호 : {}\n입금액 : {}원".format(my_bank_name, acc_number, money))
    print("===========================")
    print("위 정보가 맞으면 1번(입금), 틀리면 2번(취소)을 입력해주세요.")
    temp3 = input("1번[입금], 2번[취소] : ")
    try:
       okay = int(temp3)

    except:
        print("clear")
        print("올바른 값을 입력해주세요!\n")
        return

    if okay == 1:
        conn = pymysql.connect(host="localhost", user="root", password="cjch5391", db="atm_python", charset="utf8mb4")
        curs = conn.cursor()

        sql1 = "update bank set balance=balance + %s where bank_name = %s and account_num = %s and acc_password = %s"
        curs.execute(sql1, (money, my_bank_name, acc_number, acc_password))
        conn.commit()
        print("\n입금이 완료되었습니다!\n잠시 후 거래명세표가 출력됩니다...\n")
        time.sleep(1.5)

        # 현재 잔액 반환 함수 사용
        balance = Except.lack_of_balance(acc_number)

        # 거래명세표 출력
        Except.soai(my_bank_name, "예금입금", money, balance, 0, acc_number)
        print("거래가 완료되었습니다.\n잠시 후 초기화면으로 돌아갑니다.\n")
        conn.close()
        time.sleep(2)
        return

    if okay == 2:
        print("\n2번[취소]를 누르셨습니다.\n초기화면으로 돌아갑니다.\n")
        return


    else:
        print("잘못 입력하셨습니다.\n정확한 값을 입력해주세요.\n")
        return
