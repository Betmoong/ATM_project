/* 송금 */
package ATM;

import java.sql.*;
import java.util.Scanner;

public class Remit extends Bank {
    String your_bank_name;
    int acc_your_number;

    Scanner scanner = new Scanner(System.in);
    DBClass dbc = new DBClass();
    Except except = new Except();
    StatementOfAccount statementOfAccount = new StatementOfAccount();


    //필드

    //생성자

    //메소드
    void remit()
    {
        int commission = 0;

        System.out.println("\n3번[송금]을 선택하셨습니다.");
        System.out.println("===========================================");
        System.out.println("타 은행으로 송금시 [수수료 500원]이 부과됩니다.\n");
        System.out.println("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup");
        System.out.print("본인 계좌의 은행을 선택해주세요 : ");
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

        System.out.print("본인 계좌번호를 입력해주세요 : ");
        try {
            acc_number = scanner.nextInt();
        } catch (Exception e) {
            System.out.println("\n잘못 입력하셨습니다. 정확한 값을 입력해주세요!\n");
            scanner.next(); // 쓰레기값 삭제
            return;
        }

        //본인 계좌 정보 일치 확인(불일치 or 오류시 1 반환)
        check = except.acc_discord(acc_number, my_bank_name);
        if(check == 1) {return;}

        //계좌 정지 여부 확인
        if(except.acc_stop(acc_number) == 3) {
            System.out.println("\n정지된 계좌입니다.\n관리자에게 문의해주세요.\n");
            System.out.println("잠시 후 초기화면으로 돌아갑니다...\n");
            try {
                Thread.sleep(2000); // 2초간 지연
                return;
            } catch(Exception e) {
                System.out.println(e);
            }
        }

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

        balance = except.lack_of_balance(acc_number); //현재 잔액 반환 메소드 사용
        System.out.println("*** 현재 잔액 : [" + balance + "]원 ***");
        System.out.print("송금액을 입력해주세요 : ");
        try {
            money = scanner.nextInt();
        } catch (Exception e) {
            System.out.println("\n잘못 입력하셨습니다. 정확한 값을 입력해주세요!\n");
            scanner.next(); // 쓰레기값 삭제
            return;
        }
        System.out.println("cls");


        System.out.println("\n==========본인==========");
        System.out.println("은행 : " + my_bank_name + "\n계좌번호 : " + acc_number + "\n송금액 : " + money + "원");
        System.out.println("========================\n");
        System.out.println("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup");
        System.out.print("송금 받으실 분의 계좌의 은행을 선택해주세요 : ");
        button = scanner.next();

        switch(button)
        {
            case "1":
                your_bank_name = "kakao";
                System.out.println("\n" + button + "번 [" + your_bank_name + "] 은행을 선택하셨습니다.");
                break;
            case "2":
                your_bank_name = "woori";
                System.out.println("\n" + button + "번 [" + your_bank_name + "] 은행을 선택하셨습니다.");
                break;
            case "3":
                your_bank_name = "shinhan";
                System.out.println("\n" + button + "번 [" + your_bank_name + "] 은행을 선택하셨습니다.");
                break;
            case "4":
                your_bank_name = "hana";
                System.out.println("\n" + button + "번 [" + your_bank_name + "] 은행을 선택하셨습니다.");
                break;
            case "5":
                your_bank_name = "nonghyup";
                System.out.println("\n" + button + "번 [" + your_bank_name + "] 은행을 선택하셨습니다.");
                break;
            default:
                System.out.println("\n올바른 값을 입력해주세요!\n");
                return;
        }

        if(my_bank_name.equals(your_bank_name) == false) {
            commission = 500;
            System.out.println("\n==========================");
            System.out.println("타 은행 송금입니다.\n수수료 500원이 부과됩니다.");
            System.out.println("==========================\n");
        }

        else {
            System.out.println("\n==========================");
            System.out.println("동일 은행 송금입니다.\n수수료가 부과되지 않습니다.");
            System.out.println("==========================\n");
        }

        //계좌 잔액이 부족할 경우
        if(balance - (money + commission) < 0) {
            System.out.println("\n[계좌 잔액이 부족합니다.]\n");
            System.out.println("===========================");
            System.out.println("처리결과 : 잔액부족");
            System.out.println("요청금액(수수료 포함) : " + (money + commission) + "원");
            System.out.println("현재 잔액 : " + balance + "원");
            System.out.println("===========================");
            System.out.println("\n잠시 후 초기화면으로 돌아갑니다.\n");
            try {
                Thread.sleep(2500); // 2.5초간 지연
                return;
            } catch(Exception e) {
                System.out.println(e);
            }

        }

        System.out.print("송금 받으실 분의 계좌번호를 입력해주세요 : ");
        try {
            acc_your_number = scanner.nextInt();
        } catch (Exception e) {
            System.out.println("\n잘못 입력하셨습니다. 정확한 값을 입력해주세요!\n");
            scanner.next(); // 쓰레기값 삭제
            return;
        }

        // 송금 받는 사람의 계좌 정보 일치 확인(불일치 or 오류시 1 반환)
        check = except.acc_discord(acc_your_number, your_bank_name);
        if(check == 1) {return;}

        System.out.println("\n==========송금인==========\t==========수취인==========");
        System.out.println("은행 : " + my_bank_name + "\t\t\t\t\t은행 : " + your_bank_name + "\n계좌번호 : " + acc_number + "\t\t\t\t계좌번호 : " + acc_your_number + "\n송금액 : " + money + "원");
        System.out.println("========================\t========================\n");
        System.out.println("위 정보가 맞으면 1번(이체), 틀리면 2번(취소)을 입력해주세요.");
        System.out.print("1번[이체], 2번[취소] : ");
        okay = scanner.next();

        if(okay.equals("1"))
        {
            Connection conn = null;
            PreparedStatement pstmt1 = null;
            PreparedStatement pstmt2 = null;

            //동일 은행 송금시
            if(my_bank_name.equals(your_bank_name) == true)
            {
                //송금하는 계좌 mysql 쿼리문
                String SQL1 = "update bank set balance = balance - ? where bank_name = ? and account_num=? and acc_password=?";
                //송금받는 계좌 mysql 쿼리문
                String SQL2 = "update bank set balance = balance + ? where bank_name = ? and account_num=?";

                try {
                    conn = DriverManager.getConnection(dbURL, dbID, dnPW);
                    pstmt1 = conn.prepareStatement(SQL1);
                    pstmt2 = conn.prepareStatement(SQL2);

                    pstmt1.setInt(1, money);
                    pstmt1.setString(2, my_bank_name);
                    pstmt1.setInt(3, acc_number);
                    pstmt1.setInt(4, acc_password);
                    pstmt1.executeUpdate();

                    pstmt2.setInt(1, money);
                    pstmt2.setString(2, your_bank_name);
                    pstmt2.setInt(3, acc_your_number);
                    pstmt2.executeUpdate();

                } catch(SQLException e) {
                    System.out.println("[JDBC Connector Driver 오류 : " + e.getMessage() + "]");
                } finally {
                    //사용순서와 반대로 close 함
                    if (pstmt1 != null || pstmt2 != null) {
                        try {
                            pstmt1.close();
                            //pstmt2.close();
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

            //타 은행 송금시
            else
            {
                System.out.println("\n타 은행 송금으로 [수수료 500원]이 부과됩니다.");

                //송금하는 계좌 mysql 쿼리문
                String SQL1 = "update bank set balance = balance - ? - 500 where bank_name = ? and account_num=? and acc_password=?";
                //송금받는 계좌 mysql 쿼리문
                String SQL2 = "update bank set balance = balance + ? where bank_name = ? and account_num=?";

                try {
                    conn = DriverManager.getConnection(dbURL, dbID, dnPW);
                    pstmt1 = conn.prepareStatement(SQL1);
                    pstmt2 = conn.prepareStatement(SQL2);

                    pstmt1.setInt(1, money);
                    pstmt1.setString(2, my_bank_name);
                    pstmt1.setInt(3, acc_number);
                    pstmt1.setInt(4, acc_password);
                    pstmt1.executeUpdate();

                    pstmt2.setInt(1, money);
                    pstmt2.setString(2, your_bank_name);
                    pstmt2.setInt(3, acc_your_number);
                    pstmt2.executeUpdate();

                } catch(SQLException e) {
                    System.out.println("[JDBC Connector Driver 오류 : " + e.getMessage() + "]");
                } finally {
                    //사용순서와 반대로 close 함
                    if (pstmt1 != null || pstmt2 != null) {
                        try {
                            pstmt1.close();
                            //pstmt2.close();
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

            System.out.println("\n이체가 완료되었습니다!\n잠시 후 거래명세표가 출력됩니다...\n");
            try {
                Thread.sleep(1500); // 1.5초간 지연
            } catch(Exception e) {
                System.out.println(e);
            }

            balance = except.lack_of_balance(acc_number); //현재 잔액 반환 메소드 사용

            //송금 거래명세표 출력
            statementOfAccount.soa(my_bank_name, "계좌이체", money, balance ,commission, acc_number, your_bank_name, acc_your_number);
            System.out.println("거래가 완료되었습니다.\n잠시 후 초기화면으로 돌아갑니다.\n");
            try {
                Thread.sleep(2000); // 2초간 지연
                return;
            } catch(Exception e) {
                System.out.println(e);
            }

        }

        //2번[아니오]를 눌렀을 경우
        if(okay.equals("2")) {
            System.out.print("\n2번[아니오]을 누르셨습니다. 잠시 후 초기화면으로 돌아갑니다...");
            try {
                Thread.sleep(1700); // 1.7초간 지연
                System.out.println("cls");
                return;
            } catch (Exception e) {
                System.out.println(e);
            }
        }

        //잘못 입력했을 경우
        else {
            System.out.println("\n잘못 입력하셨습니다.\n정확한 값을 입력해주세요.\n");
            return;
        }

    }
}
