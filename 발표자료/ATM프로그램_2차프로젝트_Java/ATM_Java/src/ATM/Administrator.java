package ATM;

import java.sql.*;
import java.util.Scanner;

public class Administrator extends Bank {
    Scanner scanner = new Scanner(System.in);
    DBClass dbc = new DBClass();
    Except except = new Except();

    //필드

    //생성자

    //메소드

    public void administrator() {
        int now_admin_password;
        System.out.print("관리자 비밀번호 입력 : ");
        int input_admin_password = 0;
        try {
            input_admin_password = scanner.nextInt();
        } catch (Exception e) {
            System.out.println("\n잘못 입력하셨습니다. 정확한 값을 입력해주세요!\n");
            scanner.next(); // 쓰레기값 삭제
            return;
        }

        now_admin_password = bringPW(); // mysql에서 관리자 비밀번호를 갖고 오는 메소드 호출

        //입력된 관리자 비밀번호와 현재 mysql에 저장된 관리자 비밀번호가 일치할 경우 관리자 메뉴 진입
        if(input_admin_password == now_admin_password) {
            int init_error = 0;
            System.out.println("\n==============관리자 메뉴==============");
            System.out.println("1.계좌정지해제\n2.관리자 비밀번호변경");
            System.out.println("=======================================");
            System.out.print("입력 : ");
            okay = scanner.next();

            //관리자 메뉴에서 1번 계좌정지해제를 선택했을 경우
            if(okay.equals("1")) {
                System.out.print("정지를 해제할 계좌번호 입력 : ");
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

                //계좌 정지 여부 확인(정지일 경우 3반환)
                if(except.acc_stop(acc_number) == 3) {
                    System.out.println("\n정지된 계좌입니다.\n\n계좌 정지를 해제하시겠습니까?");
                    System.out.print("1번[예], 2번[아니오] : ");
                    okay = scanner.next();

                    if(okay.equals("1")) {
                        Connection conn = null;
                        PreparedStatement pstmt = null;

                        String SQL = "update bank set error = ? where account_num=?";

                        try {
                            conn = DriverManager.getConnection(dbURL, dbID, dnPW);
                            pstmt = conn.prepareStatement(SQL);

                            pstmt.setInt(1, init_error);
                            pstmt.setInt(2, acc_number);

                            pstmt.executeUpdate();
                            System.out.println("\n계좌 정지가 정상 해제되었습니다.\n잠시 후 초기 화면으로 돌아갑니다...");
                            try {
                                Thread.sleep(1500); // 1.5초간 지연
                                System.out.println("cls");
                                return;
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

                    if(okay.equals("2")) {
                        System.out.print("\n2번[아니오]을 누르셨습니다.\n잠시 후 초기화면으로 돌아갑니다...");
                        try {
                            Thread.sleep(1200); // 1.2초간 지연
                            System.out.println("cls");
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

                //정지되지 않은 계좌일 경우
                else {
                    System.out.println("\n정지되지 않은 계좌입니다.\nerror count를 초기화 하시겠습니까?");
                    System.out.print("1번[예], 2번[아니오] : ");
                    okay = scanner.next();

                    //정지되지 않은 계좌에서 1번[예]를 선택했을 경우
                    if(okay.equals("1"))
                    {
                        Connection conn = null;
                        PreparedStatement pstmt = null;

                        String SQL = "update bank set error = ? where account_num=?";

                        try {
                            conn = DriverManager.getConnection(dbURL, dbID, dnPW);
                            pstmt = conn.prepareStatement(SQL);

                            pstmt.setInt(1, init_error);
                            pstmt.setInt(2, acc_number);

                            pstmt.executeUpdate();
                            System.out.println("\nerror count가 정상 초기화 되었습니다.\n");
                            try {
                                Thread.sleep(1500); // 1.5초간 지연
                                System.out.println("cls");
                                return;
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

                    //정지되지 않은 계좌에서 2번[아니오]를 선택했을 경우
                    else if (okay.equals("2")) {
                        System.out.println("\n2번[아니오]을 누르셨습니다. 잠시 후 초기화면으로 돌아갑니다...\n");
                        try {
                            Thread.sleep(1500); // 1.5초간 지연
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

            //관리자 메뉴에서 2번 관리자 비밀번호변경을 선택했을 경우
            else if(okay.equals("2")) {
                System.out.println("\n관리자 비밀번호를 변경하시겠습니까?");
                System.out.print("1번[예], 2번[아니오] : ");
                okay = scanner.next();

                if(okay.equals("1"))
                {
                    System.out.print("\n변경할 관리자 비밀번호 입력 : ");
                    int new_admin_pw = scanner.nextInt();

                    Connection conn = null;
                    PreparedStatement pstmt = null;

                    String SQL = "update administrator set admin_pw = ?";

                    try {
                        conn = DriverManager.getConnection(dbURL, dbID, dnPW);
                        pstmt = conn.prepareStatement(SQL);

                        pstmt.setInt(1, new_admin_pw);

                        pstmt.executeUpdate();
                        System.out.println("\n관리자 비밀번호가 정상적으로 변경되었습니다.\n");
                        System.out.println("초기화면으로 돌아갑니다...");
                        try {
                            Thread.sleep(2300); // 2.3초간 지연
                        } catch (Exception e) {
                            System.out.println(e);
                        }
                        System.out.println("cls");
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

            }

            //관리자 메뉴 선택화면에서 잘못된 입력을 했을 경우
            else{
                System.out.println("\n잘못 입력하셨습니다.\n초기 화면으로 돌아갑니다.\n");
                try {
                    Thread.sleep(1500); // 1.5초간 지연
                } catch (Exception e) {
                    System.out.println(e);
                }
            }
        }

        //관리자 비밀번호가 틀렸을 경우
        else {
            System.out.println("\n비밀번호가 다릅니다.");
            System.out.print("초기화면으로 돌아갑니다...");
            try {
                Thread.sleep(1300); // 1.3초간 지연
            } catch (Exception e) {
                System.out.println(e);
            }
            System.out.println("cls"); //콘솔창 지우기
            return;
        }
    }


    //mysql에서 관리자 비밀번호를 가져오는 메소드
    public int bringPW() {
        int n = 0; //mysql에 저장된 관리자 비밀번호를 저장시키는 변수

        try {
            conn = DriverManager.getConnection(dbURL, dbID, dnPW);
            Statement stmt = conn.createStatement();
            ResultSet rs = stmt.executeQuery("SELECT * FROM administrator");
            rs.next();

            n = rs.getInt(1);

        } catch (SQLException e) {
            System.out.println("ERR count(*)오류" + e.getMessage());
        }
        return n; //현재 관리자 비밀번호 반환
    }


}
