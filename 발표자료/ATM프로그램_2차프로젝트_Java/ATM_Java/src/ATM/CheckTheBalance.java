/* 잔액조회 */
package ATM;

import java.sql.*;
import java.util.Scanner;

public class CheckTheBalance extends Bank {
    Scanner scanner = new Scanner(System.in);
    DBClass dbc = new DBClass();
    Except except = new Except();

    //메소드
    public void check_the_balance() {
        System.out.println("\n4번[잔액조회]을 선택하셨습니다.");
        System.out.println("===========================================");
        System.out.println("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup");
        System.out.print("조회하실 계좌의 은행을 선택해주세요 : ");
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

        //비밀번호 검사 메소드 호출 (비밀번호 반환 or (오류 or 비밀번호 3회 틀렸을 시 0반환))
        acc_password = except.check_password(acc_number);
        if(acc_password == 0) {
            System.out.println("\n잠시 후 초기화면으로 돌아갑니다.\n");
            try {
                Thread.sleep(2000); // 2초간 지연
                return;
            } catch(Exception e) {
                System.out.println(e);
            }
            return;
        }


        Connection conn = null;
        PreparedStatement pstmt = null;
        ResultSet rs;

        String SQL = "select * from bank where account_num = ? and acc_password = ?";

        try {
            conn = DriverManager.getConnection(dbURL, dbID, dnPW);
            pstmt = conn.prepareStatement(SQL);

            pstmt.setInt(1, acc_number);
            pstmt.setInt(2, acc_password);

           rs = pstmt.executeQuery();

            System.out.println();
            while(rs.next()) {
                System.out.println("은행명\t이름\t\t계좌번호\t[잔액]");
                System.out.println(rs.getString(1) + "\t" + rs.getString(2) + "\t" +rs.getInt(3) + "\t" + rs.getInt(5) + "원\n");
            }

            System.out.print("1번[거래종료], 아무키[초기화면] : ");
            okay = scanner.next();
            System.out.println();

            if(okay.equals("1")) {System.exit(0);}

            else {return;}

        } catch(SQLException e) {
            e.printStackTrace();
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
}
