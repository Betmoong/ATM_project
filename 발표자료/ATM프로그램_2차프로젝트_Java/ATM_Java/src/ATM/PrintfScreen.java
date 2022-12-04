package ATM;

public class PrintfScreen {

    //필드

    //생성자

    //메소드
    public void startScreen()
    {
        System.out.println("=====ATM 프로그램=====");
        System.out.println("1.입금\n2.출금\n3.송금\n4.잔액조회\n5.계좌 생성\n6.비밀번호변경\n7.관리자\n8.종료");
        System.out.println("=====================");
        System.out.print("원하시는 업무를 선택해주세요 : ");
    }
}
