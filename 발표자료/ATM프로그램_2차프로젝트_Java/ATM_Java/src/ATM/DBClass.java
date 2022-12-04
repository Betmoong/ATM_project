package ATM;

import javax.swing.plaf.nimbus.State;
import java.sql.*;

public class DBClass extends Bank {
    //MySQL 연동


    public DBClass() {
    } //DBClass ed

    //DB접속 메소드(드라이버 로드 및 DB접속까지 처리)
    public void dbconnect() {
        try {
            Class.forName(this.driverName); //드라이버 로딩
            System.out.println("드라이버 로드 성공");

            this.conn = DriverManager.getConnection(this.dbURL, this.dbID, this.dnPW); //드라이버들을 관리해주는 클래스명 getConnection 변수를 선언 시켜줘야함
            System.out.println("DB접속 성공");
            }   catch (SQLException e) {
                    System.out.println("ERR connect:" + e.getMessage());
            }   catch (ClassNotFoundException e) {
                    System.out.println("ERR mySQL Driver Load:" + e.getMessage());
            }
    }

    // 전체 데이터 조회 메소드(select*from bank)단 조회 결과를 Main으로 전달
    public ResultSet select() {

        String selectQuery = "SELECT * FROM bank";
        ResultSet rs = null;
        try {
            Statement stmt = conn.createStatement(); //Statement 쿼리문을 실행할 수 있는 메소드를 가지고 있음//Connection 객체를 통해서 연결
            rs = stmt.executeQuery(selectQuery); //쿼리실행코드 (executeUpdate() <- delete/update/insert)
        }   catch (SQLException e) {
            System.out.println("ERR:SQL오류" + e.getMessage());
            }
            return rs;
    }
    //전체 레코드 갯수 메소드(select count(*)from bank)
    //count(*)은 필드명이 아닌 mysql의 함수명이다.
    public int recordCount() {
        int n = 0; //레코드의 총 갯수 저장
        try {
            Statement stmt = conn.createStatement();
            ResultSet rs = stmt.executeQuery("SELECT COUNT(*) FROM bank");
            rs.next();
            n = rs.getInt(1);
            System.out.println(n);
            } catch (SQLException e) {
                System.out.println("ERR count(*)오류" + e.getMessage());
            }
            return 0;
    }

} //class DBClass ed
