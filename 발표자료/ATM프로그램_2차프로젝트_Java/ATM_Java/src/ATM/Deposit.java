/* 입금 */
package ATM;

import javax.swing.plaf.nimbus.State;
import javax.xml.transform.Result;
import java.sql.*;
import java.util.Scanner;

public class Deposit extends Bank {
    Scanner scanner = new Scanner(System.in);
    DBClass dbc = new DBClass();
    Except except = new Except();
    StatementOfAccount statementOfAccount = new StatementOfAccount();

    //필드

    //생성자

    //메소드
    void deposit()
    {
        System.out.println("\n1번[입금]을 선택하셨습니다.");
        System.out.println("===========================================");
        System.out.println("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup");
        System.out.print("입금하실 계좌의 은행을 선택해주세요 : ");
        button = scanner.next();

        switch(button)
        {
            case "1":
                my_bank_name = "kakao";
                System.out.println("\n" + button + "번 [" + my_bank_name + "] 은행을 선택하셨습니다.");
                break;
            case "2":
                my_bank_name = "woori";
                System.out.println("\n" + button + "번 [" + my_bank_name + "] 은행을 선택하셨습니다.");
                break;
            case "3":
                my_bank_name = "shinhan";
                System.out.println("\n" + button + "번 [" + my_bank_name + "] 은행을 선택하셨습니다.");
                break;
            case "4":
                my_bank_name = "hana";
                System.out.println("\n" + button + "번 [" + my_bank_name + "] 은행을 선택하셨습니다.");
                break;
            case "5":
                my_bank_name = "nonghyup";
                System.out.println("\n" + button + "번 [" + my_bank_name + "] 은행을 선택하셨습니다.");
                break;
            default:
                System.out.println("\n올바른 값을 입력해주세요!\n");
                return;
        }

        System.out.print("계좌번호를 입력해주세요 : ");
        try {
            acc_number = scanner.nextInt();
        } catch (Exception e) {
            System.out.println("\n잘못 입력하셨습니다. 정확한 값을 입력해주세요!\n");
            scanner.next(); // 쓰레기값 삭제
            return;
        }

        //계좌 정보 일치 확인(불일치 or 오류시 1 반환)
        check = except.acc_discord(acc_number, my_bank_name);
        if(check == 1) {return;}

        //계좌 정지 여부 확인
        if(except.acc_stop(acc_number) == 3) {
            System.out.println("\n정지된 계좌입니다.\n관리자에게 문의해주세요.\n");
            System.out.print("잠시 후 초기화면으로 돌아갑니다...");
            try {
                Thread.sleep(2000); // 2초간 지연
                System.out.println("cls");
                return;
            } catch(Exception e) {
                System.out.println(e);
            }
        }

        //비밀번호 검사 메소드 호출 (비밀번호 반환 or (오류 or 비밀번호 3회 틀렸을 시 0반환))
        acc_password = except.check_password(acc_number);
        if(acc_password == 0) {
            System.out.print("\n잠시 후 초기화면으로 돌아갑니다...");
            try {
                Thread.sleep(2000); // 2초간 지연
                System.out.println("cls");
                return;
            } catch(Exception e) {
                System.out.println(e);
            }
            return;
        }

        System.out.print("입금액을 입력해주세요 : ");
        try {
            money = scanner.nextInt();
        } catch (Exception e) {
            System.out.println("\n잘못 입력하셨습니다. 정확한 값을 입력해주세요!\n");
            scanner.next(); // 쓰레기값 삭제
            return;
        }


        System.out.println("cls"); //콘솔창 초기화


        System.out.println("\n===========================");
        System.out.println("은행명칭 : " + my_bank_name + "\n계좌번호 : " + acc_number + "\n입금액 : " + money + "원");
        System.out.println("===========================");
        System.out.println("위 정보가 맞으면 1번(입금), 틀리면 2번(취소)을 입력해주세요.");
        System.out.print("1번[입금], 2번[취소] : ");
        okay = scanner.next();

       if(okay.equals("1"))
        {
            Connection conn = null;
            PreparedStatement pstmt = null;

            String SQL = "update bank set balance=balance+? where bank_name=? and account_num=? and acc_password=?";

            try {
                conn = DriverManager.getConnection(dbURL, dbID, dnPW);
                pstmt = conn.prepareStatement(SQL);

                pstmt.setInt(1, money);
                pstmt.setString(2, my_bank_name);
                pstmt.setInt(3, acc_number);
                pstmt.setInt(4, acc_password);

                pstmt.executeUpdate();
                System.out.println("\n입금이 완료되었습니다!\n잠시 후 거래명세표가 출력됩니다.\n");
                try {
                    Thread.sleep(1500); // 1.5초간 지연
                } catch(Exception e) {
                    System.out.println(e);
                }

                //mysql DB에서 (거래 후 잔액)을 가져옴
                ResultSet rs;

                pstmt = conn.prepareStatement("select * from bank where account_num=?");
                pstmt.setInt(1, acc_number);

                rs = pstmt.executeQuery();

                while(rs.next()) {
                    balance = rs.getInt(5);
                }

                //거래명세표 출력
                statementOfAccount.soa(my_bank_name, "예금입금", money, balance ,0, acc_number);
                System.out.println("거래가 완료되었습니다.\n잠시 후 초기화면으로 돌아갑니다.\n");
                try {
                    Thread.sleep(2000); // 2초간 지연
                    return;
                } catch(Exception e) {
                    System.out.println(e);
                }

            } catch (SQLException e) {
                System.out.println("[JDBC Connector Driver 오류 : " + e.getMessage() + "]");
            } finally {
                //사용순서와 반대로 close 함
                if (pstmt != null) {
                    try {
                        pstmt.close();
                    } catch (SQLException e) {
                        e.printStackTrace();
                    }
                }
                if (conn != null) {
                    try {
                        conn.close();
                    } catch (SQLException e) {
                        e.printStackTrace();
                    }
                }
            }
        }

        else {
           System.out.println("\n잘못 입력하셨습니다.\n정확한 값을 입력해주세요.\n");
           return;
       }

    }
}