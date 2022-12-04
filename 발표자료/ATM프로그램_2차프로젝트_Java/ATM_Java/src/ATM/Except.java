package ATM;

import java.sql.*;
import java.util.Scanner;

public class Except extends Bank {
    DBClass dbc = new DBClass();
    Scanner scanner = new Scanner(System.in);


    //필드

    //생성자

    //메소드

    //계좌 정보가 불일치할 경우 1을 반환
    public int acc_discord(int acc_number, String bank_name) {
        PreparedStatement pstmt = null;
        int n = 0;
        int a = 0;
        try {
            conn = DriverManager.getConnection(dbURL, dbID, dnPW);
            String sql = "select count(*) from bank where account_num = ? and bank_name = ?";
            pstmt = conn.prepareStatement(sql);
            pstmt.setInt(1, acc_number);
            pstmt.setString(2, bank_name);

            ResultSet rs = pstmt.executeQuery(); //테이블 조회
            rs.next();
            n = rs.getInt(1);

            //계좌정보 불일치시
            if(n == 0)
            {
                System.out.println("\n입력하신 계좌정보가 없습니다.\n은행, 계좌번호가 맞는지 확인해주세요.\n");
                a = 1;
                return a;
            }

            return 0;

        } catch (SQLException e) {
            e.printStackTrace();
            return a = 1;
        } finally {
            try {
                if (pstmt != null && !pstmt.isClosed())
                    pstmt.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }

    //계좌 정보가 불일치할 경우 1을 반환(비밀번호 변경, 관리자 part 사용) 메소드 오버로딩
    public int acc_discord(int acc_numer) {
        PreparedStatement pstmt = null;
        int n = 0;
        int a = 0;
        try {
            conn = DriverManager.getConnection(dbURL, dbID, dnPW);
            String sql = "select count(*) from bank where account_num = ?";
            pstmt = conn.prepareStatement(sql);
            pstmt.setInt(1, acc_numer);

            ResultSet rs = pstmt.executeQuery(); //테이블 조회
            rs.next();
            n = rs.getInt(1);

            //계좌정보 불일치시
            if(n == 0)
            {
                System.out.println("\n입력하신 계좌정보가 없습니다.\n은행, 계좌번호가 맞는지 확인해주세요.\n");
                a = 1;
                return a;
            }

            return 0;

        } catch (SQLException e) {
            e.printStackTrace();
            return a = 1;
        } finally {
            try {
                if (pstmt != null && !pstmt.isClosed())
                    pstmt.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }

    //현재 잔액 반환
    public int lack_of_balance(int acc_number)
    {
        int a = 0;
        PreparedStatement pstmt = null;
        try {
            conn = DriverManager.getConnection(dbURL, dbID, dnPW);
            String sql = "select * from bank where account_num = ?";
            pstmt = conn.prepareStatement(sql);
            pstmt.setInt(1, acc_number);

            ResultSet rs = pstmt.executeQuery();
            rs.next();

            a = rs.getInt(5);

            return a;

        } catch (SQLException e) {
            e.printStackTrace();
            return 0;
        } finally {
            try {
                if (pstmt != null && !pstmt.isClosed())
                    pstmt.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }

    //계좌 정지 상태일 경우 3 반환
    public int acc_stop(int acc_number) {
        int a = 0;
        PreparedStatement pstmt = null;
        try {
            conn = DriverManager.getConnection(dbURL, dbID, dnPW);
            String sql = "select * from bank where account_num = ?";
            pstmt = conn.prepareStatement(sql);
            pstmt.setInt(1, acc_number);

            ResultSet rs = pstmt.executeQuery();
            rs.next();

            a = rs.getInt(6);

            return a;

        } catch (SQLException e) {
            e.printStackTrace();
            return 0;
        } finally {
            try {
                if (pstmt != null && !pstmt.isClosed())
                    pstmt.close();
            } catch (SQLException e) {
                e.printStackTrace();
            }
        }
    }

    //비밀번호 검사 메소드(비밀번호 반환 or (오류 or 비밀번호 3회 틀렸을 시 0반환))
    public int check_password(int acc_number) {
        System.out.print("비밀번호를 입력해주세요 : ");
        try {
            acc_password = scanner.nextInt();
        } catch (Exception e) {
            scanner.next(); // 쓰레기값 삭제
        }

        Connection conn = null;
        PreparedStatement pstmt = null;
        PreparedStatement pstmt2 = null;

        try {
            conn = DriverManager.getConnection(dbURL, dbID, dnPW);
            String sql = "select * from bank where account_num = ?";
            pstmt = conn.prepareStatement(sql);
            pstmt.setInt(1, acc_number);

            ResultSet rs = pstmt.executeQuery();
            rs.next();
            now_error = rs.getInt(6);

            int a = rs.getInt(4); //mysql에 저장된 비밀번호를 가져옴

            if (a == acc_password)
            {
                return acc_password;
            }
            else
            {
                System.out.println("\n비밀번호가 틀렸습니다.");
                now_error++;
                pstmt2 = conn.prepareStatement("update bank set error = ? where account_num = ?");
                pstmt2.setInt(1, now_error);
                pstmt2.setInt(2, acc_number);
                pstmt2.executeUpdate();

                System.out.println("[오류 횟수] : " + now_error + "회");

                while(now_error < 3)
                {
                    System.out.print("비밀번호를 입력해주세요 : ");
                    try {
                        acc_password = scanner.nextInt();
                    } catch (Exception e) {
                        scanner.next(); // 쓰레기값 삭제
                    }

                    if(a != acc_password)
                    {
                        now_error++;
                        pstmt2 = conn.prepareStatement("update bank set error = ? where account_num = ?");
                        pstmt2.setInt(1, now_error);
                        pstmt2.setInt(2, acc_number);
                        pstmt2.executeUpdate();
                        System.out.println("\n비밀번호가 틀렸습니다.");
                        System.out.println("[오류 횟수] : " + now_error + "회");
                    }

                    if(now_error >= 3)
                    {
                        System.out.println("계좌가 정지되었습니다.");
                        return 0;
                    }

                    if(a == acc_password)
                    {
                        now_error = 0;
                        pstmt2 = conn.prepareStatement("update bank set error = ? where account_num=?");
                        pstmt2.setInt(1, now_error);
                        pstmt2.setInt(2, acc_number);
                        pstmt2.executeUpdate();

                        return acc_password;
                    }
                }

                if(now_error >= 3)
                {
                    System.out.println("계좌가 정지되었습니다.");
                    return 0;
                }
            }

        } catch (SQLException e) {
            e.printStackTrace();
            return 5;
        }

        return 999;
    }



}



/*

[생각하지 못했던 예외]

1. 계좌에 10,000원이 남았을 경우 10,000원을 출금 또는 송금할 때 수수료 500원이 포함되지 않아 -500원이 찍히는 경우(해결)
    - 수수료 500원을 추가로 뺌

2. 출금액, 송금액을 음수로 할 경우 출금, 송금되는 문제(미해결)

*/
