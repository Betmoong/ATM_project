/* 계좌생성 */
package ATM;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;
import java.util.Random;
import java.util.Scanner;

public class CreateAccount extends Bank {
    Scanner scanner = new Scanner(System.in);
    DBClass dbc = new DBClass();

    //메소드
    void create_acc() {
        System.out.println("\n5번 [계좌 생성]을 선택하셨습니다.");
        System.out.println("===========================================");
        System.out.println("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup");
        System.out.print("만드실 은행을 선택해주세요 : ");
        button = scanner.next();

        switch (button) {
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
                System.out.println("\n올바른 값을 입력해주세요!");
                return;
        }

        //신규 계좌번호(난수) 생성
        Random random = new Random();
        acc_number = random.nextInt(88888888) + 11111111; //(11111111 ~ 99999999) 범위

        System.out.println("신규 계좌번호 : " + acc_number);
        System.out.print("이름을 입력해주세요 : ");
        scanner.nextLine(); //개행문자(엔터)를 제거하기 위해 사용
        name = scanner.nextLine();
        System.out.print("계좌 비밀번호를 입력해주세요 : ");
        try {
            acc_password = scanner.nextInt();
        } catch (Exception e) {
            System.out.println("\n잘못 입력하셨습니다. 정확한 값을 입력해주세요!\n");
            scanner.next(); // 쓰레기값 삭제
            return;
        }

        System.out.println("\n===========================");
        System.out.println("은행명칭 : " + my_bank_name + "\n계좌번호 : " + acc_number + "\n이름 : " + name + "\n비밀번호 : " + acc_password);
        System.out.println("===========================");
        System.out.println("위 정보가 맞으면 1번(계좌생성), 틀리면 2번(취소)을 입력해주세요.");
        System.out.print("1번[계좌 생성], 2번[취소] : ");
        okay = scanner.next();

        if (okay.equals("1")) {
            Connection conn = null;
            PreparedStatement pstmt = null;

            String SQL = "insert into bank(bank_name, name, account_num, acc_password, balance) values(?, ?, ?, ?, ?)";

            try {
                conn = DriverManager.getConnection(dbURL, dbID, dnPW);
                pstmt = conn.prepareStatement(SQL);

                pstmt.setString(1, my_bank_name);
                pstmt.setString(2, name);
                pstmt.setInt(3, acc_number);
                pstmt.setInt(4, acc_password);
                pstmt.setInt(5, 0);

                pstmt.executeUpdate();
                System.out.print("\n계좌생성이 완료되었습니다!\n잠시 후 초기화면으로 돌아갑니다...");
                try {
                    Thread.sleep(2000); // 2초간 지연
                    System.out.println("cls");
                    return;
                } catch(Exception e) {
                    System.out.println(e);
                }
                return;

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
        else if (okay.equals("2")) {
            System.out.print("\n취소버튼을 누르셨습니다.\n잠시 후 초기 화면으로 돌아갑니다...");
            try {
                Thread.sleep(1500); // 1.5초간 지연
                System.out.println("cls");
                return;
            } catch(Exception e) {
                System.out.println(e);
            }
        }

        else {
            System.out.print("\n잘못 입력하셨습니다.\n잠시 후 초기 화면으로 돌아갑니다...");
            try {
                Thread.sleep(1500); // 1.5초간 지연
                System.out.println("cls");
                return;
            } catch(Exception e) {
                System.out.println(e);
            }
        }

    }
}
