package ATM;

import java.sql.*;
import java.util.Scanner;

public class main_Atm extends Bank {

    public static void main(String[] args) throws SQLException {

        String button; //초기화면 선택 버튼(switch)

        //객체 생성
        DBClass dbc = new DBClass();
        Scanner scanner = new Scanner(System.in);
        PrintfScreen myScreen = new PrintfScreen();
        Deposit myDeposit = new Deposit();
        Withdraw myWithdraw = new Withdraw();
        Remit myRemit = new Remit();
        CreateAccount myCreateAccount = new CreateAccount();
        CheckTheBalance mycheckTheBalance = new CheckTheBalance();
        ChangePassword myChangePassword = new ChangePassword();
        Administrator myadministrator = new Administrator();


        while(true)
        {
            myScreen.startScreen(); //초기화면 출력
            button = scanner.next();

            if(button.equals("1") || button.equals("2") || button.equals("3") || button.equals("4") || button.equals("5") || button.equals("6") || button.equals("7") || button.equals("8") || button.equals("cls"))
            {
                switch(button)
                {
                    case "1":
                        myDeposit.deposit();
                        break;

                    case "2":
                        myWithdraw.withdraw();
                        break;

                    case "3":
                        myRemit.remit();
                        break;

                    case "4":
                        mycheckTheBalance.check_the_balance();
                        break;

                    case "5":
                        myCreateAccount.create_acc();
                        break;

                    case "6":
                        myChangePassword.change_password();
                        break;

                    case "7":
                        myadministrator.administrator();
                        break;

                    case "8":
                        System.out.println("프로그램을 종료합니다.");
                        return;

                    //히든키(콘솔창 초기화)
                    case "cls":
                        System.out.println("cls");
                        break;
                }
            }
            else
            {
                System.out.println("\n\nError code1: You have entered an invalid value.");
                System.out.println("잘못 입력하셨습니다. 정확한 값을 입력해주세요!\n\n");
            }
        }

    }
}