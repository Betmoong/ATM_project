# 관리자
import time
import Except
import pymysql

def administrator():
    temp1 = input("관리자 비밀번호 입력 : ")
    try:
        input_admin_password = int(temp1)
    except:
        print("clear")
        print("올바른 값을 입력해주세요!\n")
        return

    # mysql에서 관리자 비밀번호를 갖고 오는 함수 호출
    now_admin_password = Except.bring_PW()

    # 입력된 관리자 비밀번호와 현재 mysql에 저장된 관리자 비밀번호가 일치할 경우 관리자 메뉴 진입
    if input_admin_password == now_admin_password:
        print("\n===============관리자 메뉴===============")
        print("1.계좌정지해제\n2.관리자 비밀번호변경")
        print("=======================================")
        okay = input("입력 : ")

        # 관리자 메뉴에서 1번 계좌정지해제를 선택했을 경우
        if okay == "1":
           temp2 = input("정지를 해제할 계좌번호 입력 : ")
           try:
               acc_number = int(temp2)
           except:
               print("clear")
               print("올바른 값을 입력해주세요!\n")
               return

           # 계좌 정보 일치 확인(불일치시 1 반환)
           check = Except.acc_discord2(acc_number)
           if check == 1: return

           # 계좌 정지 여부 확인
           init_error = 0

           if Except.acc_stop(acc_number) == 3:
               print("\n정지된 계좌입니다.\n\n계좌 정지를 해제하시겠습니까?")
               okay = input("1번[예], 2번[아니오] : ")

               if okay == "1":
                   conn = pymysql.connect(host="localhost", user="root", password="cjch5391", db="atm_python", charset="utf8mb4")

                   sql = "update bank set error = %s where account_num = %s"
                   curs = conn.cursor()
                   curs.execute(sql, (init_error, acc_number))
                   conn.commit()

                   print("\n계좌 정지가 정상 해제되었습니다.\n잠시 후 초기화면으로 돌아갑니다...")
                   time.sleep(1.5) # 1.5초간 지연
                   print("clear")
                   return

               elif okay == "2":
                   print("\n2번[아니오]를 누르셨습니다.\n잠시 후 초기화면으로 돌아갑니다...")
                   time.sleep(1.2)  # 1.2초간 지연
                   print("clear")
                   return

               else:
                   print("\n잘못 입력하셨습니다.\n정확한 값을 입력해주세요.\n")
                   return

           # 정지되지 않은 계좌일 경우
           else:
               print("\n정지되지 않은 계좌입니다.\nerror count를 초기화 하시겠습니까?")
               okay = input("1번[예], 2번[아니오] : ")

               # 정지되지 않은 계좌에서 1번[예]를 선택했을 경우
               if okay == "1":
                   conn = pymysql.connect(host="localhost", user="root", password="cjch5391", db="atm_python", charset="utf8mb4")

                   sql = "update bank set error = %s where account_num = %s"
                   curs = conn.cursor()
                   curs.execute(sql, (init_error, acc_number))
                   conn.commit()

                   print("\nerror count가 정상 초기화 되었습니다.\n")
                   time.sleep(1.5)  # 1.5초간 지연
                   print("clear")
                   return

               # 정지되지 않은 계좌에서 2번[아니오]를 선택했을 경우
               elif okay == "2":
                   print("\n2번[아니오]를 누르셨습니다.\n잠시 후 초기화면으로 돌아갑니다...")
                   time.sleep(1.2)  # 1.2초간 지연
                   print("clear")
                   return

               else:
                   print("\n잘못 입력하셨습니다.\n정확한 값을 입력해주세요.\n")
                   return

        # 관리자 메뉴에서 2번 관리자 비밀번호변경을 선택했을 경우
        elif okay == "2":
            print("\n관리자 비밀번호를 변경하시겠습니까?")
            okay = input("1번[예], 2번[아니오] : ")

            if okay == "1":
                temp2 = input("\n변경할 관리자 비밀번호 입력(숫자) : ")
                try:
                    new_admin_pw = int(temp2)
                except:
                    print("clear")
                    print("올바른 값을 입력해주세요!\n")
                    return

                conn = pymysql.connect(host="localhost", user="root", password="cjch5391", db="atm_python", charset="utf8mb4")
                curs = conn.cursor()

                sql2 = "update administrator set admin_pw = %s"
                curs.execute(sql2, (new_admin_pw))
                conn.commit()
                print("\n관리자 비밀번호가 정상적으로 변경되었습니다.\n")
                print("초기화면으로 돌아갑니다...")
                time.sleep(2.3) # 2.3초간 지연

            elif okay == "2":
                print("\n2번[아니오]를 누르셨습니다.\n잠시 후 초기화면으로 돌아갑니다...")
                time.sleep(1.2)  # 1.2초간 지연
                print("clear")
                return

            else:
                print("\n잘못 입력하셨습니다.\n정확한 값을 입력해주세요.\n")
                return

        # 잘못 된 값을 입력했을 경우
        else:
            print("\n잘못 입력하셨습니다.\n정확한 값을 입력해주세요.\n")
            return

    # 관리자 비밀번호가 틀렸을 경우
    else:
        print("\n비밀번호가 다릅니다.")
        print("초기화면으로 돌아갑니다...")
        time.sleep(1.2) # 1.2초간 지연
        print("")
        print("clear")
        return