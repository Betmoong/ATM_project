package ATM;

import java.sql.Connection;

//추상클래스
public abstract class Bank {
    //필드
    public String button;
    public String my_bank_name; //계좌번호 변수
    public String your_bank_name; //송금 받을 계좌번호 변수
    public int acc_number; //입,출금 계좌번호 변수

    public String number;

    public int acc_password; //계좌 비밀번호 변수
    public int money; //입,출금액 변수
    public String okay; //1번(예), 2번(아니오) 변수
    public String name; //이름 변수
    public int balance; //거래 후 잔액
    public int check; //계좌 정보 일치 확인
    public int now_error; //비밀번호 오류 횟수

    String driverName = "com.mysql.jdbc.Driver"; //드라이버 명칭은 고정
    String dbURL = "jdbc:mysql://localhost:3306/ATM_JAVA"; //연결문자열
    String dbID = "root"; //데이터베이스 ID
    String dnPW = "cjch5391"; //데이터베이스 PW

    Connection conn = null; //데이터 베이스와 연결을 위한 객체

    //생성자

    //메소드
}
