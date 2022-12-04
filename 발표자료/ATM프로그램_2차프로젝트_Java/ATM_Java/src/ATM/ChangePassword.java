package ATM;

import java.sql.*;
import java.util.Scanner;

public class ChangePassword extends Bank {
    Scanner scanner = new Scanner(System.in);
    DBClass dbc = new DBClass();
    Except except = new Except();

    //필드

    //생성자

    //메소드

    public void change_password() {
        System.out.print("비밀번호를 변경할 계좌번호를 입력해주세요 : ");
        try {
            acc_number = scanner.nextInt();
        } catch (Exception e) {
            System.out.println("\n잘못 입력하셨습니다. 정확한 값을 입력해주세요!\n");
            scanner.next(); // 쓰레기값 삭제
            return;
        }

        //계좌 정보 일치 확인(불일치 or 오류시 1 반환)
        check = except.acc_discord(acc_number);
        if (check == 1) {return;}

        //계좌 정지 여부 확인
        if (except.acc_stop(acc_number) == 3) {
            System.out.println("\n정지된 계좌입니다.\n관리자에게 문의해주세요.\n");
            System.out.println("잠시 후 초기화면으로 돌아갑니다...\n");
            try {
                Thread.sleep(2000); // 2초간 지연
                return;
            } catch (Exception e) {
                System.out.println(e);
            }
        }

        //비밀번호 검사 메소드 호출 (비밀번호 반환 or (오류 or 비밀번호 3회 틀렸을 시 0반환))
        acc_password = except.check_password(acc_number);
        if (acc_password == 0) {
            System.out.print("\n잠시 후 초기화면으로 돌아갑니다...");
            try {
                Thread.sleep(2000); // 2초간 지연
                return;
            } catch (Exception e) {
                System.out.println(e);
            }
            return;
        }

        System.out.print("변경할 비밀번호를 입력해주세요 : ");
        int new_password = 0;
        try {
            new_password = scanner.nextInt();
        } catch (Exception e) {
            System.out.println("\n잘못 입력하셨습니다. 정확한 값을 입력해주세요!\n");
            scanner.next(); // 쓰레기값 삭제
            return;
        }

        Connection conn = null;
        PreparedStatement pstmt = null;

        String SQL = "update bank set acc_password = ? where account_num=? and acc_password=?";

        try {
            conn = DriverManager.getConnection(dbURL, dbID, dnPW);
            pstmt = conn.prepareStatement(SQL);

            pstmt.setInt(1, new_password);
            pstmt.setInt(2, acc_number);
            pstmt.setInt(3, acc_password);

            pstmt.executeUpdate();
            System.out.print("\n비밀번호가 정상적으로 변경되었습니다.\n잠시 후 초기 화면으로 돌아갑니다...");
            try {
                Thread.sleep(1500); // 1.5초간 지연
                System.out.println("cls");
            } catch (Exception e) {
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
}