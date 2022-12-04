/* 거래명세표 */
package ATM;

import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;

public class StatementOfAccount {
    //현재 날짜/시간
    LocalDateTime now = LocalDateTime.now();
    String formatedNow = now.format(DateTimeFormatter.ofPattern("yyyy.MM.dd\t\t\tHH:mm:ss"));

    //필드

    //생성자

    //메소드

    //입출금 거래명세표
    public void soa(String bank_name, String title, int money, int balance ,int commission, int acc_number)
    {
        System.out.println(bank_name + " 거래명세표");
        System.out.println("[" + title + "]");
        System.out.println("------------------------------------------");
        System.out.println("요청금액 :\t\t" + money + "원");
        System.out.println("처리결과 : 정상처리");
        System.out.println("\t\t\t거래 후 잔액 : [" + balance + "]원");
        System.out.println("\t\t\t\t\t수수료 : " + commission + "원");
        System.out.println("------------------------------------------");
        System.out.println("거래일시 : " + formatedNow);
        System.out.println("계좌번호 : " + bank_name + "--" + acc_number + "\n\n");

    }

    //송금 거래명세표(메소드 오버로딩)
    public void soa(String bank_name, String title, int money, int balance ,int commission, int acc_number, String your_bank_name, int acc_your_number)
    {
        System.out.println(bank_name + " 거래명세표");
        System.out.println("[" + title + "]");
        System.out.println("------------------------------------------");
        System.out.println("요청금액 :\t\t" + money + "원");
        System.out.println("처리결과 : 정상처리");
        System.out.println("\t\t\t거래 후 잔액 : [" + balance + "]원");
        System.out.println("\t\t\t\t\t수수료 : " + commission + "원");
        System.out.println("------------------------------------------");
        System.out.println("거래일시 : " + formatedNow);
        System.out.println("송금인 계좌번호 : " + bank_name + "--" + acc_number);
        System.out.println("수취인 계좌번호 : " + your_bank_name + "--" + acc_your_number + "\n\n");
    }
}
