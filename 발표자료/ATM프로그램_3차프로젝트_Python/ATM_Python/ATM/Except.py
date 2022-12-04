import pymysql
import datetime

# 계좌 정보가 불일치 할 경우 1을 반환
def acc_discord(acc_number, bank_name):
    conn = pymysql.connect(host="localhost", user="root", password="cjch5391", db="atm_python", charset="utf8mb4")

    try:
        sql = "select * from bank where account_num = %s and bank_name = %s"
        curs = conn.cursor()
        curs.execute(sql, (acc_number, bank_name))

        with conn:
            with conn.cursor() as cur:
                cur.execute(sql, (acc_number, bank_name))
                result = cur.fetchall()
                a = result[0][2] # mysql에서 계좌번호 컬럼

    except:
        print("\n입력하신 계좌 정보가 없습니다.\n")
        return 1

# 계좌 정보가 불일치 할 경우 1을 반환(비밀번호 변경, 관리자 part)
def acc_discord2(acc_number):
    conn = pymysql.connect(host="localhost", user="root", password="cjch5391", db="atm_python", charset="utf8mb4")

    try:
        sql = "select * from bank where account_num = %s"
        curs = conn.cursor()
        curs.execute(sql, (acc_number))

        with conn:
            with conn.cursor() as cur:
                cur.execute(sql, (acc_number))
                result = cur.fetchall()
                a = result[0][2]  # mysql에서 계좌번호 컬럼

    except:
        print("\n입력하신 계좌 정보가 없습니다.\n")
        return 1

# 비밀번호 검사 메소드(비밀번호 반환 or (오류 or 비밀번호 3회 틀렸을 시 0반환))
def check_password(acc_number):
    acc_password1 = input("비밀번호를 입력해주세요 : ")

    conn = pymysql.connect(host="localhost", user="root", password="cjch5391", db="atm_python", charset="utf8mb4")

    try:
        sql1 = "select * from bank where account_num = %s"
        curs = conn.cursor()
        curs.execute(sql1, (acc_number))

        conn.cursor()
        curs.execute(sql1, (acc_number))
        result = curs.fetchall()

        a = result[0][3] # mysql에서 비밀번호 컬럼, a에 mysql에 저장된 비밀번호를 저장
        now_error = result[0][5] # 현재 mysql에 저장된 error 횟수를 가져옴

        try:
            if a == int(acc_password1):
                now_error = 0
                sql5 = "update bank set error = %s where account_num = %s"
                curs.execute(sql5, (now_error, acc_number))
                conn.commit()
                return int(acc_password1)

            else:
                print("\n비밀번호가 틀렸습니다.")
                now_error += 1
                sql2 = "update bank set error = %s where account_num = %s"
                curs.execute(sql2, (now_error, acc_number))
                conn.commit()

                print("[오류 횟수] : {}회".format(now_error))
                if now_error >= 3:
                    print("계좌가 정지되었습니다.\n")
                    return 0;


                while now_error < 3:
                    acc_password2 = input("\n비밀번호를 입력해주세요 : ")

                    if a != int(acc_password2):
                        print("\n비밀번호가 틀렸습니다.")
                        now_error += 1
                        sql3 = "update bank set error = %s where account_num = %s"
                        curs.execute(sql3, (now_error, acc_number))
                        conn.commit()
                        print("[오류 횟수] : {}회".format(now_error))

                    if now_error >= 3:
                        print("계좌가 정지되었습니다.\n")
                        return 0;

                    if a == int(acc_password2):
                        now_error = 0
                        sql4 = "update bank set error = %s where account_num = %s"
                        curs.execute(sql4, (now_error, acc_number))
                        conn.commit()

                        return int(acc_password2)
        except:
            print("\n비밀번호가 틀렸습니다.")
            now_error += 1
            sql2 = "update bank set error = %s where account_num = %s"
            curs.execute(sql2, (now_error, acc_number))
            conn.commit()

            print("[오류 횟수] : {}회".format(now_error))
            if now_error >= 3:
                print("계좌가 정지되었습니다.\n")
                return 0;

            while now_error < 3:
                acc_password2 = input("\n비밀번호를 입력해주세요 : ")

                try:
                    if a != int(acc_password2):
                        print("\n비밀번호가 틀렸습니다.")
                        now_error += 1
                        sql3 = "update bank set error = %s where account_num = %s"
                        curs.execute(sql3, (now_error, acc_number))
                        conn.commit()
                        print("[오류 횟수] : {}회".format(now_error))

                    if now_error >= 3:
                        print("계좌가 정지되었습니다.\n")
                        return 0;

                    if a == int(acc_password2):
                        now_error = 0
                        sql4 = "update bank set error = %s where account_num = %s"
                        curs.execute(sql4, (now_error, acc_number))
                        conn.commit()

                        return int(acc_password2)
                except:
                    print("\n비밀번호가 틀렸습니다.")
                    now_error += 1
                    sql2 = "update bank set error = %s where account_num = %s"
                    curs.execute(sql2, (now_error, acc_number))
                    conn.commit()

                    print("[오류 횟수] : {}회".format(now_error))
                    if now_error >= 3:
                        print("계좌가 정지되었습니다.\n")
                        return 0;

                    while now_error < 3:
                        acc_password2 = input("\n비밀번호를 입력해주세요 : ")
                        try:
                            if a != int(acc_password2):
                                print("\n비밀번호가 틀렸습니다.")
                                now_error += 1
                                sql3 = "update bank set error = %s where account_num = %s"
                                curs.execute(sql3, (now_error, acc_number))
                                conn.commit()
                                print("[오류 횟수] : {}회".format(now_error))

                            if now_error >= 3:
                                print("계좌가 정지되었습니다.\n")
                                return 0;

                            if a == int(acc_password2):
                                now_error = 0
                                sql4 = "update bank set error = %s where account_num = %s"
                                curs.execute(sql4, (now_error, acc_number))
                                conn.commit()

                                return int(acc_password2)

                        except:
                            print("\n비밀번호가 틀렸습니다.")
                            now_error += 1
                            sql2 = "update bank set error = %s where account_num = %s"
                            curs.execute(sql2, (now_error, acc_number))
                            conn.commit()

                            print("[오류 횟수] : {}회".format(now_error))

                            if now_error >= 3:
                                print("계좌가 정지되었습니다.\n")
                                return 0;

        conn.close()

    except:
        print("\n[error!].\n")
        conn.close()
        return 1

# 입출금 거래명세표
def soai(bank_name, title, money, balance, commission, acc_number):
    now = datetime.datetime.now()
    print(bank_name, "거래명세표")
    print("[{}]".format(title))
    print("------------------------------------------")
    print("요청금액 : \t\t{}원".format(money))
    print("처리결과 : 정상처리")
    print("\t\t\t거래 후 잔액 : [{}]원".format(balance))
    print("\t\t\t\t\t수수료 : {}원".format(commission))
    print("------------------------------------------")
    print("거래일시 :", now.strftime("%Y.%m.%d\t\t\t%H:%M:%S"))
    print("계좌번호 : {}--{}\n\n".format(bank_name, acc_number))

# 송금 거래명세표
def soa(bank_name, title, money, balance, commission, acc_number, your_bank_name, acc_your_number):
    now = datetime.datetime.now()
    print(bank_name, "거래명세표")
    print("[{}]".format(title))
    print("------------------------------------------")
    print("요청금액 : \t\t{}원".format(money))
    print("처리결과 : 정상처리")
    print("\t\t\t거래 후 잔액 : [{}]원".format(balance))
    print("\t\t\t\t\t수수료 : {}원".format(commission))
    print("------------------------------------------")
    print("거래일시 :", now.strftime("%Y.%m.%d\t\t\t%H:%M:%S"))
    print("송금인 계좌번호 : {}--{}c".format(bank_name, acc_number))
    print("수취인 계좌번호 : {}--{}\n\n".format(your_bank_name, acc_your_number))

# 현재 잔액 반환
def lack_of_balance(acc_number):
    conn = pymysql.connect(host="localhost", user="root", password="cjch5391", db="atm_python", charset="utf8mb4")

    sql = "select * from bank where account_num = %s"
    curs = conn.cursor()
    curs.execute(sql, (acc_number))

    result = curs.fetchall()
    balance = result[0][4] # mysql에서 잔액 컬럼, balance에 mysql에 저장된 잔액을 저장
    return balance

#mysql에서 관리자 비밀번호를 갖고오는 메소드
def bring_PW():
    conn = pymysql.connect(host="localhost", user="root", password="cjch5391", db="atm_python", charset="utf8mb4")
    curs = conn.cursor()
    sql = "SELECT * FROM administrator"
    curs.execute(sql)

    result = curs.fetchall()
    return result[0][0] # 현재 관리자 비밀번호 반환

# (mysql에 저장된 현재 error 횟수 반환)_계좌 정지 상태일 경우 3 반환
def acc_stop(acc_number):
    conn = pymysql.connect(host="localhost", user="root", password="cjch5391", db="atm_python", charset="utf8mb4")
    curs = conn.cursor()
    sql = "select * from bank where account_num = %s"
    curs.execute(sql, (acc_number))

    result = curs.fetchall()
    a = result[0][5]
    return a












