# 송금

import pymysql
import Except
import time

def remit():
    print("===========================================")
    print("타 은행으로 송금시 [수수료 500원]이 부과됩니다.\n")
    print("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup")
    button = input("본인 계좌의 은행을 선택해주세요 : ")

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

    temp1 = input("본인 계좌번호를 입력해주세요 : ")
    try:
       acc_number = int(temp1)

    except:
        print("clear")
        print("올바른 값을 입력해주세요!\n")
        return

    # 계좌 정보 일치 확인(불일치시 1 반환)
    check = Except.acc_discord(acc_number, my_bank_name)
    if check == 1: return

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

    # 현재 잔액 반환 함수 사용
    balance1 = Except.lack_of_balance(acc_number)
    print("*** 송금 가능 잔액 : [{}]원 ***".format(balance1))
    temp2 = input("송금액을 입력해주세요 : ")
    try:
        money = int(temp2)

    except:
        print("clear")
        print("올바른 값을 입력해주세요!\n")
        return

    # 송금액을 음수로 입력하였을 경우
    if money < 0:
        print("\n잘못 입력하셨습니다. 정확한 값을 입력해주세요!\n")
        return

    print("clear")  # 콘솔창 초기화

    # 송금 잔액이 부족할 경우
    if (balance1 - money) < 0:
        print("\n[계좌 잔액이 부족합니다.]\n")
        print("===========================")
        print("처리결과 : 잔액부족")
        print("요청금액 : {}원".format(money))
        print("현재 잔액 : {}원".format(balance1))
        print("===========================")
        print("\n잠시 후 초기화면으로 돌아갑니다.\n")
        time.sleep(2.5)
        return

    print("\n==========본인==========")
    print("은행명칭 : {}\n계좌번호 : {}\n출금액 : {}원".format(my_bank_name, acc_number, money))
    print("========================")
    print("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup")
    button = input("송금 받으실 분의 계좌의 은행을 선택해주세요 : ")

    if button == "1":
        your_bank_name = "kakao"
        print("\n{}번 [{}] 은행을 선택하셨습니다.".format(button, your_bank_name))

    elif button == "2":
        your_bank_name = "woori"
        print("\n{}번 [{}] 은행을 선택하셨습니다.".format(button, your_bank_name))

    elif button == "3":
        your_bank_name = "shinhan"
        print("\n{}번 [{}] 은행을 선택하셨습니다.".format(button, your_bank_name))

    elif button == "4":
        your_bank_name = "hana"
        print("\n{}번 [{}] 은행을 선택하셨습니다.".format(button, your_bank_name))

    elif button == "5":
        your_bank_name = "nonghyup"
        print("\n{}번 [{}] 은행을 선택하셨습니다.".format(button, your_bank_name))

    else:
        print("\n올바른 값을 입력해주세요!")
        return

    if your_bank_name != my_bank_name:
        print("\n==========================")
        print("타 은행 송금입니다.\n수수료 500원이 부과됩니다.")
        print("==========================\n")

    else:
        print("\n==========================")
        print("동일 은행 송금입니다.\n수수료가 부과되지 않습니다.")
        print("==========================\n")

    commission = 500

    # 계좌 잔액이 부족할 경우
    if(balance1 - (money + commission) < 0):
        print("\n[계좌 잔액이 부족합니다.]")
        print("===========================")
        print("처리결과 : 잔액부족")
        print("요청금액(수수료 포함) : {}원".format((money + commission)))
        print("현재 잔액 : {}원".format(balance1))
        print("===========================")
        print("\n잠시 후 초기화면으로 돌아갑니다.\n")
        time.sleep(2.5)
        return

    temp3 = input("송금 받으실 분의 계좌번호를 입력해주세요 : ")
    try:
       acc_your_number = int(temp3)

    except:
        print("clear")
        print("올바른 값을 입력해주세요!\n")
        return

    # 송금 받는 사람의 계좌 정보 일치 확인(불일치시 1 반환)
    check = Except.acc_discord(acc_your_number, your_bank_name)
    if check == 1: return

    print("\n==========송금인==========\t==========수취인==========")
    print("은행 : {}\t\t\t\t\t은행 : {}\n계좌번호 : {}\t\t\t계좌번호 : {}\n송금액 : {}원".format(my_bank_name, your_bank_name, acc_number, acc_your_number, money))
    print("========================\t========================\n")
    print("위 정보가 맞으면 1번(이체), 틀리면 2번(취소)을 입력해주세요.")
    temp4 = input("1번[이체], 2번[취소] : ")

    try:
        okay = int(temp4)
    except:
        print("clear")
        print("올바른 값을 입력해주세요!\n")
        return

    if okay == 1:
        conn = pymysql.connect(host="localhost", user="root", password="cjch5391", db="atm_python", charset="utf8mb4")
        curs = conn.cursor()

        # 동일 은행 송금시
        if my_bank_name == your_bank_name:
            commission = 0
            # 송금하는 계좌 mysql 쿼라문
            sql1 = "update bank set balance = balance - %s where bank_name = %s and account_num = %s and acc_password = %s"
            # 송금받는 계좌 mysql 쿼리문
            sql2 = "update bank set balance = balance + %s where bank_name = %s and account_num = %s"

            curs.execute(sql1, (money, my_bank_name, acc_number, acc_password))
            conn.commit()
            curs.execute(sql2, (money, your_bank_name, acc_your_number))
            conn.commit()


        else:
            print("\n타 은행 송금으로 [수수료 {}원]이 부과됩니다.".format(commission))

            # 송금하는 계좌 mysql 쿼라문
            sql1 = "update bank set balance = balance - %s - 500 where bank_name = %s and account_num = %s and acc_password = %s"
            # 송금받는 계좌 mysql 쿼리문
            sql2 = "update bank set balance = balance + %s where bank_name = %s and account_num = %s"

            curs.execute(sql1, (money, my_bank_name, acc_number, acc_password))
            conn.commit()
            curs.execute(sql2, (money, your_bank_name, acc_your_number))
            conn.commit()

        print("\n이체가 완료되었습니다!\n잠시 후 거래명세표가 출력됩니다...\n")
        time.sleep(1.5) # 1.5초간 지연

        # 현재 잔액 반환 함수 사용
        balance2 = Except.lack_of_balance(acc_number)

        # 송금 거래명세표 출력
        Except.soa(my_bank_name, "계좌이체", money, balance2 ,commission, acc_number, your_bank_name, acc_your_number)
        print("거래가 완료되었습니다.\n잠시 후 초기화면으로 돌아갑니다.\n")
        time.sleep(2) # 2초간 지연
        return

     # 2번[아니오]를 눌렀을 경우
    elif okay == 2:
        print("\n2번[취소]을 누르셨습니다. 잠시 후 초기화면으로 돌아갑니다...")
        time.sleep(1.7)
        return

    # 잘못 입력했을 경우
    else:
        print("잘못 입력하셨습니다.\n정확한 값을 입력해주세요.\n")
        return