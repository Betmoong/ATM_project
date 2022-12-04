#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

MYSQL* conn; //mysql과 커넥션을 잡는데 지속적으로 사용되는 변수
MYSQL_RES* res;  //쿼리문에 대한 result값을 받는 위치변수 
MYSQL_ROW row;   //쿼리문에 대한 실제 데이터값이 들어있는 변수

int create_acc();
int check_password();
static int radmin_password = 1234;


int main(void)
{
    char* server = "localhost";  //서버의 경로
    char* user = "root"; //mysql로그인 아이디
    char* password = "cjch5391"; // 패스워드 입력
    char* database = "mydb";  //Database 이름   

    conn = mysql_init(NULL); //connection 변수 초기화

    //DB없이 MYSQL만 접속
    //mysql_real_connect()함수가 연결을 시켜주는 함수
    if (!mysql_real_connect(conn, server, user, password, NULL, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        printf("mysql 연결에 실패했습니다.\n");
        exit(1);
    }
    //접속한 결과값은 res에 넣는건데요. 딱히 지금은 안해도 상관없어요.


    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    while (1)
    {
        char bank_name[40] = "";
        char bank_name2[40] = "";
        char query[255];
        char input[10];
        int button;
        int access = 0; // 관리자 접속 변수
        int acc_number = 0; //입,출금 계좌번호 변수
        int acc_password = 0; // 계좌 비밀번호 변수
        int my_number = 0; // 송금파트 본인 계좌번호 변수
        int your_number = 0; // 송금 받을 계좌번호 변수
        int money = 0; // 입출금액 변수
        int okay = 0; // 1번(예), 2번(아니오) 변수

        printf("======ATM 프로그램======\n");
        printf("1.입금\n2.출금\n3.송금\n4.잔액조회\n5.계좌 생성\n6.비밀번호변경\n7.관리자\n");
        printf("========================\n");
        printf("원하시는 업무를 선택해주세요 : _\b");
        scanf("%s", &input);
        printf("\n");

        button = atoi(input); // 문자를 정수형으로 변환

        if (button == 1 || button == 2 || button == 3 || button == 4 || button == 5 || button == 6 || button == 7)
        {
            switch (button)
            {
                // ====================입금========================
            case 1:
                printf("1번 [입금]을 선택하셨습니다.\n\n");
                printf("===========================================\n");
                printf("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup\n");
                printf("\n입금하실 계좌의 은행을 선택해주세요 : _\b");
                scanf("%d", &button);

                switch (button)
                {
                case 1:
                    strcpy(bank_name, "kakao");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;

                case 2:
                    strcpy(bank_name, "woori");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;

                case 3:
                    strcpy(bank_name, "shinhan");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;

                case 4:
                    strcpy(bank_name, "hana");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;

                case 5:
                    strcpy(bank_name, "nonghyup");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;
                }

                printf("계좌번호를 입력해주세요 : ________\b\b\b\b\b\b\b\b");
                scanf("%d", &acc_number);
                printf("비밀번호를 입력해주세요 : ");
                scanf("%d", &acc_password);
                printf("입금액을 입력해주세요 :  ");
                scanf("%d", &money);

                system("cls"); // 콘솔창 clear

                printf("\n===========================\n\n");
                printf("은행명칭 : %s \n계좌번호 : %d\n입금액 : %d원\n", bank_name, acc_number, money);
                printf("\n===========================\n\n");
                printf("위 정보가 맞으면 1번(입금), 틀리면 2번(취소)을 입력해주세요.\n");
                printf("1번[입금], 2번[취소] : ");
                scanf("%d", &okay);

                if (okay == 1)
                {
                    MYSQL_RES* result = mysql_store_result(conn);

                    sprintf(query, "SELECT * FROM `atm`.`bank` WHERE account_num = '%d' AND bank_name = '%s' AND acc_password = '%d'", acc_number, bank_name, acc_password);
                    mysql_query(conn, query);
                    result = mysql_store_result(conn);
                    if ((int)(result->row_count) == 0) {
                        fprintf(stderr, "%s\n", mysql_error(conn));
                        printf("입력하신 계좌 정보가 없습니다. \n은행, 계좌번호가 맞는지 확인해주세요.\n\n");
                        printf("잠시 후 초기 화면으로 돌아갑니다...");
                        Sleep(3000);
                        system("cls");
                        break;
                    }

                    sprintf(query, "UPDATE `atm`.`bank` SET `balance` = `balance` + '%d' WHERE account_num = '%d' AND bank_name = '%s' AND acc_password = '%d'", money, acc_number, bank_name, acc_password);
                    mysql_query(conn, query);

                    printf("\n입금이 완료되었습니다!\n");
                    printf("잠시후 명세표가 출력됩니다...");
                    Sleep(1500);
                    system("cls");

                    printf("%s 거래명세표\n[예금입금]\n------------------------------------------\n", bank_name);
                    printf("요청금액 :\t\t%d원\n", money);
                    printf("처리결과 : 정상처리\n");

                    sprintf(query, "SELECT * FROM atm.bank WHERE account_num = '%d'", acc_number);
                    mysql_query(conn, query);

                    MYSQL_RES* result1 = mysql_store_result(conn);      // mysql_store_result 함수로 result set을 가져오고 result set을 MYSQL_RES 구조체에 저장.
                    if (result1 == NULL)
                    {
                        fprintf(stderr, "%s\n", mysql_error(conn));
                        exit(1);
                    }

                    int num_fields = mysql_num_fields(result1);        //mysql_num_fields 함수로 테이블의 Column 수를 알아낸다.

                    MYSQL_ROW row; // row가 없을 때까지 반복해서 row를 가져오면서 영역의 수만큼 row의 내용을 콘솔에 출력한다.
                    while (row = mysql_fetch_row(result1))
                    {
                        printf("\t\t거래후 잔액 : [%s]원\t\n", row[4]);
                    }
                    mysql_free_result(result1); // result set을 해제(free)해준다.
                    mysql_close(conn);

                    printf("\t\t\t수수료 : 0원\n");
                    printf("------------------------------------------\n");
                    printf("거래일시 : %d.%d.%d\t\t%d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                    printf("계좌번호 : %s--%d\n\n\n", bank_name, acc_number);

                    printf("1번[거래종료], 아무키[초기화면] : ");

                    scanf("%s", &input);
                    okay = atoi(input);

                    if (okay == 1)
                    {
                        printf("\n거래가 종료되었습니다.\n");
                        return 0;
                    }

                    else
                    {
                        system("cls");
                        break;
                    }

                }

                else if (okay == 2)
                {
                    printf("\n취소버튼을 누르셨습니다. 잠시 후 초기 화면으로 돌아갑니다...");
                    Sleep(1500); // 1.5초 delay 후 초기화면으로 돌아감.
                    system("cls"); // 콘솔창 clear
                }

                break;

                // ====================출금========================
            case 2:
                printf("2번 [출금]을 선택하셨습니다.\n\n");
                printf("===========================================\n");
                printf("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup\n");
                printf("\n출금하실 계좌의 은행을 선택해주세요 : _\b");
                scanf("%d", &button);

                switch (button)
                {
                case 1:
                    strcpy(bank_name, "kakao");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;

                case 2:
                    strcpy(bank_name, "woori");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;

                case 3:
                    strcpy(bank_name, "shinhan");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;

                case 4:
                    strcpy(bank_name, "hana");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;

                case 5:
                    strcpy(bank_name, "nonghyup");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;
                }

                printf("계좌번호를 입력해주세요 : ________\b\b\b\b\b\b\b\b");
                scanf("%d", &acc_number);
                
                
                // 계좌 정보가 불일치할 경우
                MYSQL_RES* result = mysql_store_result(conn);
                sprintf(query, "SELECT * FROM `atm`.`bank` WHERE account_num = '%d' AND bank_name = '%s'", acc_number, bank_name);
                mysql_query(conn, query);
                result = mysql_store_result(conn);

                if ((int)(result->row_count) == 0)
                {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                    printf("입력하신 계좌 정보가 없습니다. \n은행, 계좌번호가 맞는지 확인해주세요.\n\n");
                    printf("잠시 후 초기 화면으로 돌아갑니다...");
                    Sleep(3000);
                    system("cls");
                    break;
                }


                //계좌 정지 여부 확인
                MYSQL_RES* result9 = mysql_store_result(conn);
                MYSQL_ROW row9;
                char now_error2[40];
                int inow_error2 = 0;
                sprintf(query, "SELECT * FROM `atm`.`bank` WHERE account_num = '%d'", acc_number);
                mysql_query(conn, query);
                result9 = mysql_store_result(conn);
                row9 = mysql_fetch_row(result9);
                strcpy(now_error2, row9[5]);
                inow_error2 = atoi(now_error2);

                if (inow_error2 == 3)
                {
                    printf("\n정지된 계좌입니다.\n관리자에게 문의해주세요!\n\n");
                    printf("잠시 후 초기화면으로 돌아갑니다...");
                    Sleep(2500);
                    system("cls");
                    break;
                }

                int count2 = 0;

                MYSQL_RES* result72 = mysql_store_result(conn);
                MYSQL_ROW rrrow2; // 현재 비밀번호를 알기 위함
                char now_pass2[40];
                int inow_pass2 = 0;

                while (count2 < 3)
                {
                    printf("비밀번호를 입력해주세요 : ");
                    scanf("%d", &acc_password);

                    sprintf(query, "SELECT * FROM `atm`.`bank` WHERE account_num = '%d' AND acc_password = '%d'", acc_number, acc_password);
                    mysql_query(conn, query);
                    result72 = mysql_store_result(conn);

                    if ((int)(result72->row_count) != 0)
                    {
                        rrrow2 = mysql_fetch_row(result72);
                        strcpy(now_pass2, rrrow2[3]);
                        inow_pass2 = atoi(now_pass2);
                    }

                    if ((int)(result72->row_count) == 0)
                    {
                        printf("\n비밀번호가 틀렸습니다.\n");
                        count2++;
                        sprintf(query, "UPDATE `atm`.`bank` SET `error` = '%d' WHERE account_num = '%d'", count2, acc_number);
                        mysql_query(conn, query);

                        printf("[오류 횟수] : %d\n\n", count2);
                    }

                    if (count2 >= 3)
                    {
                        printf("계좌가 정지되었습니다.\n");
                        return 0;
                    }

                    //비밀번호가 맞았을 경우
                    if (inow_pass2 == acc_password)
                    {
                        count2 = 0;
                        sprintf(query, "UPDATE `atm`.`bank` SET `error` = '%d' WHERE account_num = '%d'", count2, acc_number);
                        mysql_query(conn, query);

                        printf("\n출금액을 입력해주세요 :  ");
                scanf("%d", &money);

                system("cls"); // 콘솔창 clear

                printf("\n===========================\n\n");
                printf("은행명칭 : %s \n계좌번호 : %d\n출금액 : %d원\n", bank_name, acc_number, money);
                printf("\n===========================\n\n");
                printf("위 정보가 맞으면 1번(출금), 틀리면 2번(취소)을 입력해주세요.\n");
                printf("1번[출금], 2번[취소] : ");
                scanf("%d", &okay);

                if (okay == 1)
                {
                    
                    MYSQL_ROW rrow; // 현재 계좌 잔액을 알기위함 -> 잔액 부족일 경우 출금x
                    char now_balance[255];
                    int inow_balance;

                    // 계좌 잔액이 부족할 경우

                    rrow = mysql_fetch_row(result);
                    strcpy(now_balance, rrow[4]);

                    inow_balance = atoi(now_balance);

                    if (inow_balance - money < 0)
                    {
                        system("cls");
                        printf("\n계좌 잔액이 부족합니다.\n\n");
                        printf("===========================\n");
                        printf("처리결과 : 잔액부족\n");
                        printf("요청금액 : %d원\n", money);
                        printf("현재 잔액 : %d원\n", inow_balance);
                        printf("===========================\n\n\n");

                        printf("1번[거래종료], 아무키[초기화면] : ");

                        scanf("%s", &input);
                        okay = atoi(input);

                        if (okay == 1)
                        {
                            printf("\n거래가 종료되었습니다.\n");
                            return 0;
                        }

                        else
                        {
                            system("cls");
                            break;
                        }
                    }

                    else
                    {
                        sprintf(query, "UPDATE `atm`.`bank` SET `balance` = `balance` - '%d' WHERE account_num = '%d' AND bank_name = '%s' AND acc_password = '%d'", money, acc_number, bank_name, acc_password);
                        mysql_query(conn, query);

                        printf("\n출금이 완료되었습니다!\n");
                        printf("잠시후 명세표가 출력됩니다...");
                        Sleep(1500);
                        system("cls");

                        printf("%s 거래명세표\n[예금출금]\n------------------------------------------\n", bank_name);
                        printf("요청금액 :\t\t%d원\n", money);
                        printf("처리결과 : 정상처리\n");

                        sprintf(query, "SELECT * FROM atm.bank WHERE account_num = '%d'", acc_number);
                        mysql_query(conn, query);

                        MYSQL_RES* result1 = mysql_store_result(conn);      // mysql_store_result 함수로 result set을 가져오고 result set을 MYSQL_RES 구조체에 저장.
                        if (result1 == NULL)
                        {
                            fprintf(stderr, "%s\n", mysql_error(conn));
                            exit(1);
                        }

                        int num_fields = mysql_num_fields(result1);        //mysql_num_fields 함수로 테이블의 Column 수를 알아낸다.

                        MYSQL_ROW row; // row가 없을 때까지 반복해서 row를 가져오면서 영역의 수만큼 row의 내용을 콘솔에 출력한다.
                        while (row = mysql_fetch_row(result1))
                        {
                            printf("\t\t거래후 잔액 : [%s]원\t\n", row[4]);
                        }
                        mysql_free_result(result1); // result set을 해제(free)해준다.
                        mysql_close(conn);

                        printf("\t\t\t수수료 : 0원\n");
                        printf("------------------------------------------\n");
                        printf("거래일시 : %d.%d.%d\t\t%d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                        printf("계좌번호 : %s--%d\n\n\n", bank_name, acc_number);

                        printf("1번[거래종료], 아무키[초기화면] : ");

                        scanf("%s", &input);
                        okay = atoi(input);

                        if (okay == 1)
                        {
                            printf("\n거래가 종료되었습니다.\n");
                            return 0;
                        }

                        else
                        {
                            system("cls");
                            break;
                        }
                    }
                }

                else if (okay == 2)
                {
                    printf("\n취소버튼을 누르셨습니다. 잠시 후 초기 화면으로 돌아갑니다...");
                    Sleep(1500); // 1.5초 delay 후 초기화면으로 돌아감.
                    system("cls"); // 콘솔창 clear
                }

                break;
                    }

                }


                // ====================송금========================
            case 3:
         

                printf("3번 [송금]을 선택하셨습니다.\n\n");
                printf("==============================================\n");
                printf("타 은행으로 송금시 [수수료 500원]이 부과됩니다.\n\n");
                printf("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup\n");
                printf("\n본인 계좌의 은행을 선택해주세요 : _\b");
                scanf("%d", &button);

                switch (button)
                {
                case 1:
                    strcpy(bank_name, "kakao");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;

                case 2:
                    strcpy(bank_name, "woori");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;

                case 3:
                    strcpy(bank_name, "shinhan");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;

                case 4:
                    strcpy(bank_name, "hana");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;

                case 5:
                    strcpy(bank_name, "nonghyup");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;
                }

                printf("본인 계좌번호를 입력해주세요 : ");
                scanf("%d", &my_number);

                MYSQL_RES* result4 = mysql_store_result(conn);
                sprintf(query, "SELECT * FROM `atm`.`bank` WHERE account_num = '%d' AND bank_name = '%s'", my_number, bank_name);
                mysql_query(conn, query);
                result4 = mysql_store_result(conn);

                // 본인 계좌 정보가 불일치할 경우
                if ((int)(result4->row_count) == 0)
                {
                    //fprintf(stderr, "%s\n", mysql_error(conn));
                    printf("\n입력하신 본인 계좌 정보가 없습니다. \n은행, 계좌번호가 맞는지 확인해주세요.\n\n");
                    printf("잠시 후 초기 화면으로 돌아갑니다...");
                    Sleep(3000);
                    system("cls");
                    break;
                }

                //계좌 정지 여부 확인
                MYSQL_RES* result10 = mysql_store_result(conn);
                MYSQL_ROW row10;
                char now_error3[40];
                int inow_error3 = 0;
                sprintf(query, "SELECT * FROM `atm`.`bank` WHERE account_num = '%d'", my_number);
                mysql_query(conn, query);
                result10 = mysql_store_result(conn);
                row10 = mysql_fetch_row(result10);
                strcpy(now_error3, row10[5]);
                inow_error3 = atoi(now_error3);

                if (inow_error3 == 3)
                {
                    printf("\n정지된 계좌입니다.\n관리자에게 문의해주세요!\n\n");
                    printf("잠시 후 초기화면으로 돌아갑니다...");
                    Sleep(2500);
                    system("cls");
                    break;
                }


                int count = 0;

                MYSQL_RES* result71 = mysql_store_result(conn);
                MYSQL_ROW rrrow1; // 현재 비밀번호를 알기 위함
                char now_pass1[40];
                int inow_pass1 = 0;

                while (count < 3)
                {
                    printf("비밀번호를 입력해주세요 : ");
                    scanf("%d", &acc_password);

                    sprintf(query, "SELECT * FROM `atm`.`bank` WHERE account_num = '%d' AND acc_password = '%d'", my_number, acc_password);
                    mysql_query(conn, query);
                    result71 = mysql_store_result(conn);

                    if ((int)(result71->row_count) != 0)
                    {
                        rrrow1 = mysql_fetch_row(result71);
                        strcpy(now_pass1, rrrow1[3]);
                        inow_pass1 = atoi(now_pass1);
                    }

                    if ((int)(result71->row_count) == 0)
                    {
                        printf("\n비밀번호가 틀렸습니다.\n");
                        count++;
                        sprintf(query, "UPDATE `atm`.`bank` SET `error` = '%d' WHERE account_num = '%d'", count, my_number);
                        mysql_query(conn, query);

                        printf("[오류 횟수] : %d\n", count);
                    }

                    if (count >= 3)
                    {
                        printf("계좌가 정지되었습니다.\n");
                        break;
                    }

                    //비밀번호가 맞았을 경우
                    if (inow_pass1 == acc_password)
                    {
                        count = 0;
                        sprintf(query, "UPDATE `atm`.`bank` SET `error` = '%d' WHERE account_num = '%d'", count, my_number);
                        mysql_query(conn, query);

                        printf("이체액을 입력해주세요 :  ");
                        scanf("%d", &money);



                        MYSQL_ROW rrow; // 현재 계좌 잔액을 알기위함 -> 잔액 부족일 경우 출금x
                        char now_balance[255];
                        int inow_balance;

                        // 계좌 잔액이 부족할 경우
                        rrow = mysql_fetch_row(result4);
                        strcpy(now_balance, rrow[4]);

                        inow_balance = atoi(now_balance);

                        if (inow_balance - money < 0)
                        {
                            system("cls");
                            printf("\n계좌 잔액이 부족합니다.\n\n");
                            printf("===========================\n");
                            printf("처리결과 : 잔액부족\n");
                            printf("요청금액 : %d원\n", money);
                            printf("현재 잔액 : %d원\n", inow_balance);
                            printf("===========================\n\n\n");

                            printf("1번[거래종료], 아무키[초기화면] : ");

                            scanf("%s", &input);
                            okay = atoi(input);

                            if (okay == 1)
                            {
                                printf("\n거래가 종료되었습니다.\n");
                                return 0;
                            }

                            else
                            {
                                system("cls");
                                break;
                            }
                        }

                        system("cls"); // 콘솔창 clear

                        printf("\n==========본인==========\n");
                        printf("은행 : %s\n계좌번호 : %d\n송금액 : %d원\n", bank_name, my_number, money);
                        printf("========================\n\n");
                        printf("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup\n");
                        printf("\n송금 받으실 분의 계좌의 은행을 선택해주세요 : _\b");
                        scanf("%d", &button);

                        switch (button)
                        {
                        case 1:
                            strcpy(bank_name2, "kakao");
                            printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name2);
                            break;

                        case 2:
                            strcpy(bank_name2, "woori");
                            printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name2);
                            break;

                        case 3:
                            strcpy(bank_name2, "shinhan");
                            printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name2);
                            break;

                        case 4:
                            strcpy(bank_name2, "hana");
                            printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name2);
                            break;

                        case 5:
                            strcpy(bank_name2, "nonghyup");
                            printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name2);
                            break;
                        }

                        int pandan = strcmp(bank_name, bank_name2); // 문자열 비교함수 strcmp 사용. 값이 같으면 정수 0 반환
                        if (!pandan == 0)
                        {
                            printf("\n==========================\n");
                            printf("\n타 은행 송금입니다.\n수수료 500원이 부과됩니다.\n\n");
                            printf("==========================\n");
                        }

                        else
                        {
                            printf("\n===========================\n");
                            printf("\n동일 은행 송금입니다.\n수수료가 부과되지 않습니다.\n\n");
                            printf("===========================\n");
                        }

                        printf("\n송금 받으실 분의 계좌번호를 입력해주세요 : ________\b\b\b\b\b\b\b\b");
                        scanf("%d", &your_number);

                        MYSQL_RES* result5 = mysql_store_result(conn);
                        sprintf(query, "SELECT * FROM `atm`.`bank` WHERE account_num = '%d' AND bank_name = '%s'", your_number, bank_name2);
                        mysql_query(conn, query);
                        result5 = mysql_store_result(conn);

                        // 송금받는 계좌의 정보가 불일치할 경우
                        if ((int)(result5->row_count) == 0)
                        {
                            fprintf(stderr, "%s\n", mysql_error(conn));
                            printf("========================================\n");
                            printf("송금 계좌 정보가 없습니다. \n은행, 계좌번호가 맞는지 확인해주세요.\n\n");
                            printf("잠시 후 초기 화면으로 돌아갑니다...\n");
                            printf("========================================\n");
                            Sleep(3000);
                            system("cls");
                            break;
                        }

                        system("cls"); // 콘솔창 clear

                        printf("\n==========본인==========\t==========타인==========\n");
                        printf("은행 : %s\t\t\t은행 : %s\n계좌번호 : %d\t\t계좌번호 : %d\n송금액 : %d원\n", bank_name, bank_name2, my_number, your_number, money);
                        printf("========================\t========================\n\n");
                        printf("위 정보가 맞으면 1번(이체), 틀리면 2번(취소)을 입력해주세요.\n");
                        printf("1번[이체], 2번[취소] : ");
                        scanf("%d", &okay);

                        if (okay == 1)
                        {
                            if (pandan == 0)
                            {
                                //송금하는 계좌 mysql 업데이트
                                sprintf(query, "UPDATE `atm`.`bank` SET `balance` = `balance` - '%d' WHERE account_num = '%d' AND bank_name = '%s' AND acc_password = '%d'", money, my_number, bank_name, acc_password);
                                mysql_query(conn, query);

                                //송금받는 계좌 mysql 업데이트
                                sprintf(query, "UPDATE `atm`.`bank` SET `balance` = `balance` + '%d' WHERE account_num = '%d' AND bank_name = '%s'", money, your_number, bank_name2);
                                mysql_query(conn, query);
                            }

                            else
                            {
                                printf("\n타 은행 송금으로 [수수료 500원]이 부과됩니다.\n");

                                //송금하는 계좌 mysql 업데이트
                                sprintf(query, "UPDATE `atm`.`bank` SET `balance` = `balance` - '%d' - 500 WHERE account_num = '%d' AND bank_name = '%s' AND acc_password = '%d'", money, my_number, bank_name, acc_password);
                                mysql_query(conn, query);

                                //송금받는 계좌 mysql 업데이트
                                sprintf(query, "UPDATE `atm`.`bank` SET `balance` = `balance` + '%d' WHERE account_num = '%d' AND bank_name = '%s'", money, your_number, bank_name2);
                                mysql_query(conn, query);
                            }

                            printf("\n이체 완료되었습니다!\n");
                            printf("잠시후 명세표가 출력됩니다...");
                            Sleep(1500);
                            system("cls");

                            printf("%s 거래명세표\n[계좌이체]\n------------------------------------------\n", bank_name);
                            printf("요청금액 :\t\t%d원\n", money);
                            printf("처리결과 : 정상처리\n");

                            sprintf(query, "SELECT * FROM atm.bank WHERE account_num = '%d'", my_number);
                            mysql_query(conn, query);

                            MYSQL_RES* result1 = mysql_store_result(conn);      // mysql_store_result 함수로 result set을 가져오고 result set을 MYSQL_RES 구조체에 저장.
                            if (result1 == NULL)
                            {
                                fprintf(stderr, "%s\n", mysql_error(conn));
                                exit(1);
                            }

                            int num_fields = mysql_num_fields(result1);        //mysql_num_fields 함수로 테이블의 Column 수를 알아낸다.

                            MYSQL_ROW row; // row가 없을 때까지 반복해서 row를 가져오면서 영역의 수만큼 row의 내용을 콘솔에 출력한다.
                            while (row = mysql_fetch_row(result1))
                            {
                                printf("\t\t거래후 잔액 : [%s]원\t\n", row[4]);
                            }
                            mysql_free_result(result1); // result set을 해제(free)해준다.
                            mysql_close(conn);

                            if (pandan == 0) printf("\t\t\t수수료 : 0원\n");
                            else printf("\t\t\t수수료 : 500원\n");

                            printf("------------------------------------------\n");
                            printf("거래일시 : %d.%d.%d\t\t%d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                            printf("출금 계좌번호 : %s--%d\n", bank_name, my_number);
                            printf("입금 계좌번호 : %s--%d\n\n\n", bank_name2, your_number);


                            printf("1번[거래종료], 아무키[초기화면] : ");

                            scanf("%s", &input);
                            okay = atoi(input);

                            if (okay == 1)
                            {
                                printf("\n거래가 종료되었습니다.\n");
                                return 0;
                            }

                            else
                            {
                                system("cls");
                                break;
                            }
                        }

                        else if (okay == 2)
                        {
                            printf("\n취소버튼을 누르셨습니다. 잠시 후 초기 화면으로 돌아갑니다...");
                            Sleep(1500); // 1.5초 delay 후 초기화면으로 돌아감.
                            system("cls"); // 콘솔창 clear
                        }

                        break;

                        Sleep(50000);


                       
                    }

                }


                // ====================잔액조회========================
            case 4:
                printf("4번 [잔액조회]를 선택하셨습니다.\n\n");

                printf("은행명, 이름, 계좌번호, [잔액]\n");

                mysql_query(conn, "SELECT * FROM atm.bank");       // mydb에서 atm.bank 테이블을 모두 가져옴

                MYSQL_RES* rresult = mysql_store_result(conn);      // mysql_store_result 함수로 result set을 가져오고 result set을 MYSQL_RES 구조체에 저장한다.
                if (rresult == NULL)
                {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                    exit(1);
                }
                int num_fields = mysql_num_fields(rresult);        //mysql_num_fields 함수로 테이블의 Column 수를 알아낸다.

                MYSQL_ROW row; // row가 없을 때까지 반복해서 row를 가져오면서 영역의 수만큼 row의 내용을 콘솔에 출력한다.
                while (row = mysql_fetch_row(rresult))
                {
                    printf("---------------------------------------\n");

                    for (int i = 0; i < 1; i++)
                    {
                        printf("%s / %s / %s / [%s]원", row[0], row[1], row[2], row[4]);
                    }
                    printf("\n");
                    printf("---------------------------------------\n");

                }
                mysql_free_result(rresult); // result set을 해제(free)해준다.
                mysql_close(conn);
                printf("\n1번[거래종료], 아무키[초기화면] : ");

                scanf("%s", &input);

                okay = atoi(input);

                if (okay == 1)
                {
                    printf("\n거래가 종료되었습니다.\n");
                    return 0;
                }

                else
                {
                    system("cls");
                    break;
                }


                // ====================계좌 생성========================
            case 5:
                printf("5번 [계좌 생성]을 선택하셨습니다.\n");
                printf("=================================\n");
                printf("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup\n");
                printf("\n만드실 은행을 선택해주세요 : _\b");
                scanf("%s", &input);

                button = atoi(input); // 문자를 정수형으로 변환
                int end = 0;

                switch (button)
                {
                case 1:
                    strcpy(bank_name, "kakao");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;

                case 2:
                    strcpy(bank_name, "woori");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;

                case 3:
                    strcpy(bank_name, "shinhan");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;

                case 4:
                    strcpy(bank_name, "hana");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;

                case 5:
                    strcpy(bank_name, "nonghyup");
                    printf("\n%d번 [%s] 은행을 선택하셨습니다.\n", button, bank_name);
                    break;

                default :
                    printf("\n올바른 값을 입력해주세요!\n\n");
                    end = 1;
                    break;
                }

                if (end == 1)
                {
                    break;
                }

                int balance = 0;
                int acc_num = create_acc();
                printf("신규 계좌번호: %d\n", acc_num);

                char name[40]; // 이름 변수

                printf("이름을 입력해주세요 : ");
                scanf("%s", name);
                printf("계좌 비밀번호를 입력해주세요 : ");
                scanf("%d", &acc_password);

                system("cls"); // 콘솔창 clear

                printf("\n===========================\n\n");
                printf("은행명칭 : %s \n계좌번호 : %d\n이름 : %s\n비밀번호 : %d\n", bank_name, acc_num, name, acc_password);
                printf("\n===========================\n\n");
                printf("위 정보가 맞으면 1번(계좌생성), 틀리면 2번(취소)을 입력해주세요.\n");
                printf("1번[계좌생성], 2번[취소] : _\b");
                scanf("%d", &okay);

                if (okay == 1)
                {
                    sprintf(query, "INSERT INTO `atm`.`bank` (`bank_name`, `name`, `account_num`, `acc_password`, `balance`) VALUES ('%s', '%s', '%d', '%d', '%d')", bank_name, name, acc_num, acc_password, balance);
                    mysql_query(conn, query);
                    printf("\n계좌 생성이 완료되었습니다!\n");
                    return 0;
                }

                else if (okay == 2)
                {
                    printf("\n취소버튼을 누르셨습니다. 잠시 후 초기화면으로 돌아갑니다...");
                    Sleep(1500); // 1.5초 delay 후 초기화면으로 돌아감.
                    system("cls"); // 콘솔창 clear
                    break;
                }

                // ====================비밀번호 변경========================
            case 6:

                printf("비밀번호를 변경할 계좌번호를 입력해주세요 : ________\b\b\b\b\b\b\b\b");
                scanf("%d", &acc_number);

                MYSQL_RES* result3 = mysql_store_result(conn);

                sprintf(query, "SELECT * FROM `atm`.`bank` WHERE account_num = '%d'", acc_number);
                mysql_query(conn, query);
                result3 = mysql_store_result(conn);
                if ((int)(result3->row_count) == 0)
                {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                    printf("입력하신 계좌 정보가 없습니다. \n계좌번호가 맞는지 확인해주세요.\n\n");
                    printf("잠시 후 초기 화면으로 돌아갑니다...");
                    Sleep(3000);
                    system("cls");
                    break;
                }

                //계좌 정지 여부 확인
                MYSQL_RES* result8 = mysql_store_result(conn);
                MYSQL_ROW row8;
                char now_error1[40];
                int inow_error1 = 0;
                sprintf(query, "SELECT * FROM `atm`.`bank` WHERE account_num = '%d'", acc_number);
                mysql_query(conn, query);
                result8 = mysql_store_result(conn);
                row8 = mysql_fetch_row(result8);
                strcpy(now_error1, row8[5]);
                inow_error1 = atoi(now_error1);

                if (inow_error1 == 3)
                {
                    printf("\n정지된 계좌입니다.\n관리자에게 문의해주세요!\n\n");
                    printf("잠시 후 초기화면으로 돌아갑니다...");
                    Sleep(2500);
                    system("cls");
                    break;
                }

                int count1 = 0;

                MYSQL_RES* result7 = mysql_store_result(conn);
                MYSQL_ROW rrrow; // 현재 비밀번호를 알기 위함
                char now_pass[40];
                int inow_pass = 0;

                while (count1 < 3)
                {
                    printf("\n현재 비밀번호를 입력해주세요 : ");
                    scanf("%d", &acc_password);

                    sprintf(query, "SELECT * FROM `atm`.`bank` WHERE account_num = '%d' AND acc_password = '%d'", acc_number, acc_password);
                    mysql_query(conn, query);
                    result7 = mysql_store_result(conn);

                    if ((int)(result7->row_count) != 0)
                    {
                        rrrow = mysql_fetch_row(result7);
                        strcpy(now_pass, rrrow[3]);
                        inow_pass = atoi(now_pass);
                    }

                    if ((int)(result7->row_count) == 0)
                    {
                        printf("\n비밀번호가 틀렸습니다.\n");
                        count1++;
                        sprintf(query, "UPDATE `atm`.`bank` SET `error` = '%d' WHERE account_num = '%d'", count1, acc_number);
                        mysql_query(conn, query);

                        printf("[오류 횟수] : %d\n", count1);
                    }

                    if (count1 >= 3)
                    {
                        printf("\n계좌가 정지되었습니다.\n");
                        printf("관리자에게 문의하시기 바랍니다.\n");
                        Sleep(50000);
                        break;
                    }

                    //비밀번호가 맞았을 경우
                    if (inow_pass == acc_password)
                    {
                        count1 = 0;
                        sprintf(query, "UPDATE `atm`.`bank` SET `error` = '%d' WHERE account_num = '%d'", count1, acc_number);
                        mysql_query(conn, query);

                        while (rrrow = mysql_fetch_row(result7))
                        {
                            printf("\n\t--계좌 정보--\n은행 : %s\n이름 : %s \t\n", rrrow[0], rrrow[1]);
                        }
                        int acc_newpassword = 0; // 새로운 비밀번호 변수

                        printf("\n변경하실 비밀번호를 입력해주세요 : ");
                        scanf("%d", &acc_newpassword);

                        sprintf(query, "UPDATE `atm`.`bank` SET `acc_password` = '%d' WHERE account_num = '%d' AND acc_password = '%d'", acc_newpassword, acc_number, acc_password);
                        mysql_query(conn, query);

                        printf("비밀번호가 정상적으로 변경되었습니다.\n");

                        printf("1번[거래종료], 아무키[초기화면] : ");
                        scanf("%d", &okay);
                    }

                    
                }
                
                if (okay == 1)
                {
                    printf("\n거래가 종료되었습니다.\n");
                    return 0;
                }

                else
                {
                    system("cls");
                    break;
                }
                // ====================관리자========================
            case 7:

                printf("관리자 비밀번호 입력 : ");
                int admin_password = radmin_password;
                int input_password = 0;
                scanf("%d", &input_password);

                if (admin_password == input_password)
                {
                    printf("\n==============관리자 메뉴==============\n");
                    printf("\n1.계좌정지해제\n2.관리자 비밀번호변경\n");
                    printf("\n=======================================\n");
                    printf("입력 : ");
                    scanf("%s", &input);
                    printf("\n");
                    button = atoi(input); // 문자를 정수형으로 변환

                    if (button == 1)
                    {
                        printf("정지를 해제할 계좌번호 입력 : ");
                        scanf("%d", &acc_number);
                        int error = 0;

                        // 계좌 정보가 불일치할 경우
                        MYSQL_RES* result99 = mysql_store_result(conn);
                        sprintf(query, "SELECT * FROM `atm`.`bank` WHERE account_num = '%d'", acc_number);
                        mysql_query(conn, query);
                        result99 = mysql_store_result(conn);

                        if ((int)(result99->row_count) == 0)
                        {
                            fprintf(stderr, "%s\n", mysql_error(conn));
                            printf("입력하신 계좌 정보가 없습니다. \n계좌번호가 맞는지 확인해주세요.\n\n");
                            printf("잠시 후 초기 화면으로 돌아갑니다...");
                            Sleep(3000);
                            system("cls");
                            break;
                        }

                        //계좌 정지 여부 확인
                        MYSQL_RES* result11 = mysql_store_result(conn);
                        MYSQL_ROW row11;
                        char now_error4[40];
                        int inow_error4 = 0;
                        sprintf(query, "SELECT * FROM `atm`.`bank` WHERE account_num = '%d'", acc_number);
                        mysql_query(conn, query);
                        result11 = mysql_store_result(conn);
                        row11 = mysql_fetch_row(result11);
                        strcpy(now_error4, row11[5]);
                        inow_error4 = atoi(now_error4);

                        if (inow_error4 == 3)
                        {
                            printf("\n정지된 계좌입니다.\n\n계좌 정지를 해제하시겠습니까?\n");
                            printf("1번[예], 2번[아니오] : ");
                            scanf("%d", &okay);

                            if (okay == 1)
                            {
                                sprintf(query, "UPDATE `atm`.`bank` SET `error` = '%d' WHERE account_num = '%d'", error, acc_number);
                                mysql_query(conn, query);
                                printf("\n계좌 정지가 정상 해제되었습니다.\n");
                                return 0;
                            }

                            else if (okay == 2)
                            {
                                printf("\n2번[아니오]을 누르셨습니다. 잠시 후 초기화면으로 돌아갑니다...");
                                Sleep(1500); // 1.5초 delay 후 초기화면으로 돌아감.
                                system("cls"); // 콘솔창 clear
                            }

                            break;
                        }

                        else
                        {
                            printf("\n정지되지 않은 계좌입니다.\nerror count를 초기화 하시겠습니까?\n");
                            printf("1번[예], 2번[아니오] : ");
                            scanf("%d", &okay);

                            if (okay == 1)
                            {
                                sprintf(query, "UPDATE `atm`.`bank` SET `error` = '%d' WHERE account_num = '%d'", error, acc_number);
                                mysql_query(conn, query);
                                printf("\nerror count가 정상 초기화 되었습니다.\n");
                                return 0;
                            }

                            else if (okay == 2)
                            {
                                printf("\n2번[아니오]을 누르셨습니다. 잠시 후 초기화면으로 돌아갑니다...");
                                Sleep(1500);
                                system("cls");
                            }
                            break;
                        }
                    }

                    else if (button == 2)
                    {
                        int a = 0;
                        printf("관리자 비밀번호를 변경하시겠습니까?\n");
                        printf("1번[예], 2번[아니오]\n");
                        scanf("%d", &a);
                        
                        if (a == 1)
                        {
                            printf("변경할 비밀번호 입력 : ");
                            scanf("%d", &radmin_password);
                            printf("\n\n관리자 비밀번호가 정상적으로 변경되었습니다.\n\n");
                            break;
                        }

                        else if (a == 2)
                        {
                            printf("종료합니다.\n\n");
                            break;
                        }


                        
                        break;
                    }



                }


                else
                {
                    printf("\n비밀번호가 다릅니다.\n\n");
                    break;
                }


                return 0;
            }
        }

        else
        {
            system("cls");
            printf("\n\n잘못 입력하셨습니다. 정확한 값을 입력해주세요!\n\n");
        }


    }

}

//-----------------------------------함수--------------------------------------

// 랜덤 계좌생성(8자리) 함수
int create_acc()
{
    char str1[9];
    char str2[9];

    srand(time(NULL));
    int random1 = rand() % 9000 + 1000;
    int random2 = rand() % 9000 + 1000;

    sprintf(str1, "%d", random1);
    sprintf(str2, "%d", random2);

    strcat(str1, str2); // str1에 str2 문자열 이어붙임

    int num1 = atoi(str1); //str1 + str2 값을 정수형으로 변환 ... atoi() : 문자형을 숫자형으로 반환

    return num1;
}