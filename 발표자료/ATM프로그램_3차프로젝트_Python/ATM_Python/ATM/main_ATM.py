import PrintScreen
import Deposit
import Withdraw
import Remit
import CheckTheBalance
import CreateAccount
import ChangePassword
import Administrator

while True:

    PrintScreen.start_screen()
    button = input("원하시는 업무를 선택해주세요 : ")

    if button == "1":
        print("\n1번[입금]을 선택하셨습니다.")
        Deposit.deposit()

    elif button == "2":
        print("\n2번[출금]을 선택하셨습니다.")
        Withdraw.withdraw()

    elif button == "3":
        print("\n3번[송금]을 선택하셨습니다.")
        Remit.remit()

    elif button == "4":
        print("\n4번[잔액조회]을 선택하셨습니다.")
        CheckTheBalance.check_the_balance()

    elif button == "5":
        print("\n5번[계좌 생성]을 선택하셨습니다.")
        CreateAccount.create_acc()

    elif button == "6":
        print("\n6번[비밀번호 변경]을 선택하셨습니다.")
        ChangePassword.change_password()

    elif button == "7":
        print("\n7번[관리자]을 선택하셨습니다.")
        Administrator.administrator()

    elif button == "8":
        print("프로그램을 종료합니다.")
        break

    elif button == "cls":
        print("clear")

    else:
        print("잘못 입력하셨습니다.")