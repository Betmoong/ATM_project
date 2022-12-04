# 비밀번호 변경

import pymysql
import Except
import time

def change_password():
    temp1 = input("비밀번호를 변경할 계좌번호를 입력해주세요 : ")
    try:
        acc_number = int(temp1)
    except:
        print("clear")
        print("올바른 값을 입력해주세요!\n")
        return

    # 계좌 정보 일치 확인(불일치시 1 반환)
    check = Except.acc_discord2(acc_number)
    if check == 1: return

    # 계좌 정지 여부 확인
    if Except.acc_stop(acc_number) == 3:
        print("\n정지된 계좌입니다.\n관리자에게 문의해주세요.\n")
        print("잠시 후 초기화면으로 돌아갑니다...")
        time.sleep(2)
        print("clear")
        return

    # 비밀번호 검사 메소드 호출
    acc_password = Except.check_password(acc_number)
    if acc_password == 0:
        print("\n잠시 후 초기화면으로 돌아갑니다...")
        time.sleep(2) # 2초간 지연
        return

    temp2 = input("변경할 비밀번호를 입력해주세요 : ")
    try:
        new_password = int(temp2)
    except:
        print("clear")
        print("올바른 값을 입력해주세요!\n")
        return

    if new_password < 0:
        print("\n올바른 값을 입력해주세요!\n")
        return

    conn = pymysql.connect(host="localhost", user="root", password="cjch5391", db="atm_python", charset="utf8mb4")
    curs = conn.cursor()

    sql = "update bank set acc_password = %s where account_num = %s and acc_password = %s"
    curs.execute(sql, (new_password, acc_number, acc_password))
    conn.commit()

    print("\n비밀번호가 정상적으로 변경되었습니다.\n잠시 후 초기화면으로 돌아갑니다...")
    time.sleep(1.5) # 1.5초간 지연
    print("clear")
    return