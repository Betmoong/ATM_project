#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <mysql.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

MYSQL* conn; //mysql�� Ŀ�ؼ��� ��µ� ���������� ���Ǵ� ����
MYSQL_RES* res;  //�������� ���� result���� �޴� ��ġ���� 
MYSQL_ROW row;   //�������� ���� ���� �����Ͱ��� ����ִ� ����

int create_acc();
int check_password();
static int radmin_password = 1234;


int main(void)
{
    char* server = "localhost";  //������ ���
    char* user = "root"; //mysql�α��� ���̵�
    char* password = "cjch5391"; // �н����� �Է�
    char* database = "mydb";  //Database �̸�   

    conn = mysql_init(NULL); //connection ���� �ʱ�ȭ

    //DB���� MYSQL�� ����
    //mysql_real_connect()�Լ��� ������ �����ִ� �Լ�
    if (!mysql_real_connect(conn, server, user, password, NULL, 0, NULL, 0))
    {
        fprintf(stderr, "%s\n", mysql_error(conn));
        printf("mysql ���ῡ �����߽��ϴ�.\n");
        exit(1);
    }
    //������ ������� res�� �ִ°ǵ���. ���� ������ ���ص� ��������.


    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    while (1)
    {
        char bank_name[40] = "";
        char bank_name2[40] = "";
        char query[255];
        char input[10];
        int button;
        int access = 0; // ������ ���� ����
        int acc_number = 0; //��,��� ���¹�ȣ ����
        int acc_password = 0; // ���� ��й�ȣ ����
        int my_number = 0; // �۱���Ʈ ���� ���¹�ȣ ����
        int your_number = 0; // �۱� ���� ���¹�ȣ ����
        int money = 0; // ����ݾ� ����
        int okay = 0; // 1��(��), 2��(�ƴϿ�) ����

        printf("======ATM ���α׷�======\n");
        printf("1.�Ա�\n2.���\n3.�۱�\n4.�ܾ���ȸ\n5.���� ����\n6.��й�ȣ����\n7.������\n");
        printf("========================\n");
        printf("���Ͻô� ������ �������ּ��� : _\b");
        scanf("%s", &input);
        printf("\n");

        button = atoi(input); // ���ڸ� ���������� ��ȯ

        if (button == 1 || button == 2 || button == 3 || button == 4 || button == 5 || button == 6 || button == 7)
        {
            switch (button)
            {
                // ====================�Ա�========================
            case 1:
                printf("1�� [�Ա�]�� �����ϼ̽��ϴ�.\n\n");
                printf("===========================================\n");
                printf("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup\n");
                printf("\n�Ա��Ͻ� ������ ������ �������ּ��� : _\b");
                scanf("%d", &button);

                switch (button)
                {
                case 1:
                    strcpy(bank_name, "kakao");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;

                case 2:
                    strcpy(bank_name, "woori");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;

                case 3:
                    strcpy(bank_name, "shinhan");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;

                case 4:
                    strcpy(bank_name, "hana");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;

                case 5:
                    strcpy(bank_name, "nonghyup");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;
                }

                printf("���¹�ȣ�� �Է����ּ��� : ________\b\b\b\b\b\b\b\b");
                scanf("%d", &acc_number);
                printf("��й�ȣ�� �Է����ּ��� : ");
                scanf("%d", &acc_password);
                printf("�Աݾ��� �Է����ּ��� :  ");
                scanf("%d", &money);

                system("cls"); // �ܼ�â clear

                printf("\n===========================\n\n");
                printf("�����Ī : %s \n���¹�ȣ : %d\n�Աݾ� : %d��\n", bank_name, acc_number, money);
                printf("\n===========================\n\n");
                printf("�� ������ ������ 1��(�Ա�), Ʋ���� 2��(���)�� �Է����ּ���.\n");
                printf("1��[�Ա�], 2��[���] : ");
                scanf("%d", &okay);

                if (okay == 1)
                {
                    MYSQL_RES* result = mysql_store_result(conn);

                    sprintf(query, "SELECT * FROM `atm`.`bank` WHERE account_num = '%d' AND bank_name = '%s' AND acc_password = '%d'", acc_number, bank_name, acc_password);
                    mysql_query(conn, query);
                    result = mysql_store_result(conn);
                    if ((int)(result->row_count) == 0) {
                        fprintf(stderr, "%s\n", mysql_error(conn));
                        printf("�Է��Ͻ� ���� ������ �����ϴ�. \n����, ���¹�ȣ�� �´��� Ȯ�����ּ���.\n\n");
                        printf("��� �� �ʱ� ȭ������ ���ư��ϴ�...");
                        Sleep(3000);
                        system("cls");
                        break;
                    }

                    sprintf(query, "UPDATE `atm`.`bank` SET `balance` = `balance` + '%d' WHERE account_num = '%d' AND bank_name = '%s' AND acc_password = '%d'", money, acc_number, bank_name, acc_password);
                    mysql_query(conn, query);

                    printf("\n�Ա��� �Ϸ�Ǿ����ϴ�!\n");
                    printf("����� ��ǥ�� ��µ˴ϴ�...");
                    Sleep(1500);
                    system("cls");

                    printf("%s �ŷ���ǥ\n[�����Ա�]\n------------------------------------------\n", bank_name);
                    printf("��û�ݾ� :\t\t%d��\n", money);
                    printf("ó����� : ����ó��\n");

                    sprintf(query, "SELECT * FROM atm.bank WHERE account_num = '%d'", acc_number);
                    mysql_query(conn, query);

                    MYSQL_RES* result1 = mysql_store_result(conn);      // mysql_store_result �Լ��� result set�� �������� result set�� MYSQL_RES ����ü�� ����.
                    if (result1 == NULL)
                    {
                        fprintf(stderr, "%s\n", mysql_error(conn));
                        exit(1);
                    }

                    int num_fields = mysql_num_fields(result1);        //mysql_num_fields �Լ��� ���̺��� Column ���� �˾Ƴ���.

                    MYSQL_ROW row; // row�� ���� ������ �ݺ��ؼ� row�� �������鼭 ������ ����ŭ row�� ������ �ֿܼ� ����Ѵ�.
                    while (row = mysql_fetch_row(result1))
                    {
                        printf("\t\t�ŷ��� �ܾ� : [%s]��\t\n", row[4]);
                    }
                    mysql_free_result(result1); // result set�� ����(free)���ش�.
                    mysql_close(conn);

                    printf("\t\t\t������ : 0��\n");
                    printf("------------------------------------------\n");
                    printf("�ŷ��Ͻ� : %d.%d.%d\t\t%d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                    printf("���¹�ȣ : %s--%d\n\n\n", bank_name, acc_number);

                    printf("1��[�ŷ�����], �ƹ�Ű[�ʱ�ȭ��] : ");

                    scanf("%s", &input);
                    okay = atoi(input);

                    if (okay == 1)
                    {
                        printf("\n�ŷ��� ����Ǿ����ϴ�.\n");
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
                    printf("\n��ҹ�ư�� �����̽��ϴ�. ��� �� �ʱ� ȭ������ ���ư��ϴ�...");
                    Sleep(1500); // 1.5�� delay �� �ʱ�ȭ������ ���ư�.
                    system("cls"); // �ܼ�â clear
                }

                break;

                // ====================���========================
            case 2:
                printf("2�� [���]�� �����ϼ̽��ϴ�.\n\n");
                printf("===========================================\n");
                printf("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup\n");
                printf("\n����Ͻ� ������ ������ �������ּ��� : _\b");
                scanf("%d", &button);

                switch (button)
                {
                case 1:
                    strcpy(bank_name, "kakao");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;

                case 2:
                    strcpy(bank_name, "woori");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;

                case 3:
                    strcpy(bank_name, "shinhan");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;

                case 4:
                    strcpy(bank_name, "hana");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;

                case 5:
                    strcpy(bank_name, "nonghyup");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;
                }

                printf("���¹�ȣ�� �Է����ּ��� : ________\b\b\b\b\b\b\b\b");
                scanf("%d", &acc_number);
                
                
                // ���� ������ ����ġ�� ���
                MYSQL_RES* result = mysql_store_result(conn);
                sprintf(query, "SELECT * FROM `atm`.`bank` WHERE account_num = '%d' AND bank_name = '%s'", acc_number, bank_name);
                mysql_query(conn, query);
                result = mysql_store_result(conn);

                if ((int)(result->row_count) == 0)
                {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                    printf("�Է��Ͻ� ���� ������ �����ϴ�. \n����, ���¹�ȣ�� �´��� Ȯ�����ּ���.\n\n");
                    printf("��� �� �ʱ� ȭ������ ���ư��ϴ�...");
                    Sleep(3000);
                    system("cls");
                    break;
                }


                //���� ���� ���� Ȯ��
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
                    printf("\n������ �����Դϴ�.\n�����ڿ��� �������ּ���!\n\n");
                    printf("��� �� �ʱ�ȭ������ ���ư��ϴ�...");
                    Sleep(2500);
                    system("cls");
                    break;
                }

                int count2 = 0;

                MYSQL_RES* result72 = mysql_store_result(conn);
                MYSQL_ROW rrrow2; // ���� ��й�ȣ�� �˱� ����
                char now_pass2[40];
                int inow_pass2 = 0;

                while (count2 < 3)
                {
                    printf("��й�ȣ�� �Է����ּ��� : ");
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
                        printf("\n��й�ȣ�� Ʋ�Ƚ��ϴ�.\n");
                        count2++;
                        sprintf(query, "UPDATE `atm`.`bank` SET `error` = '%d' WHERE account_num = '%d'", count2, acc_number);
                        mysql_query(conn, query);

                        printf("[���� Ƚ��] : %d\n\n", count2);
                    }

                    if (count2 >= 3)
                    {
                        printf("���°� �����Ǿ����ϴ�.\n");
                        return 0;
                    }

                    //��й�ȣ�� �¾��� ���
                    if (inow_pass2 == acc_password)
                    {
                        count2 = 0;
                        sprintf(query, "UPDATE `atm`.`bank` SET `error` = '%d' WHERE account_num = '%d'", count2, acc_number);
                        mysql_query(conn, query);

                        printf("\n��ݾ��� �Է����ּ��� :  ");
                scanf("%d", &money);

                system("cls"); // �ܼ�â clear

                printf("\n===========================\n\n");
                printf("�����Ī : %s \n���¹�ȣ : %d\n��ݾ� : %d��\n", bank_name, acc_number, money);
                printf("\n===========================\n\n");
                printf("�� ������ ������ 1��(���), Ʋ���� 2��(���)�� �Է����ּ���.\n");
                printf("1��[���], 2��[���] : ");
                scanf("%d", &okay);

                if (okay == 1)
                {
                    
                    MYSQL_ROW rrow; // ���� ���� �ܾ��� �˱����� -> �ܾ� ������ ��� ���x
                    char now_balance[255];
                    int inow_balance;

                    // ���� �ܾ��� ������ ���

                    rrow = mysql_fetch_row(result);
                    strcpy(now_balance, rrow[4]);

                    inow_balance = atoi(now_balance);

                    if (inow_balance - money < 0)
                    {
                        system("cls");
                        printf("\n���� �ܾ��� �����մϴ�.\n\n");
                        printf("===========================\n");
                        printf("ó����� : �ܾ׺���\n");
                        printf("��û�ݾ� : %d��\n", money);
                        printf("���� �ܾ� : %d��\n", inow_balance);
                        printf("===========================\n\n\n");

                        printf("1��[�ŷ�����], �ƹ�Ű[�ʱ�ȭ��] : ");

                        scanf("%s", &input);
                        okay = atoi(input);

                        if (okay == 1)
                        {
                            printf("\n�ŷ��� ����Ǿ����ϴ�.\n");
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

                        printf("\n����� �Ϸ�Ǿ����ϴ�!\n");
                        printf("����� ��ǥ�� ��µ˴ϴ�...");
                        Sleep(1500);
                        system("cls");

                        printf("%s �ŷ���ǥ\n[�������]\n------------------------------------------\n", bank_name);
                        printf("��û�ݾ� :\t\t%d��\n", money);
                        printf("ó����� : ����ó��\n");

                        sprintf(query, "SELECT * FROM atm.bank WHERE account_num = '%d'", acc_number);
                        mysql_query(conn, query);

                        MYSQL_RES* result1 = mysql_store_result(conn);      // mysql_store_result �Լ��� result set�� �������� result set�� MYSQL_RES ����ü�� ����.
                        if (result1 == NULL)
                        {
                            fprintf(stderr, "%s\n", mysql_error(conn));
                            exit(1);
                        }

                        int num_fields = mysql_num_fields(result1);        //mysql_num_fields �Լ��� ���̺��� Column ���� �˾Ƴ���.

                        MYSQL_ROW row; // row�� ���� ������ �ݺ��ؼ� row�� �������鼭 ������ ����ŭ row�� ������ �ֿܼ� ����Ѵ�.
                        while (row = mysql_fetch_row(result1))
                        {
                            printf("\t\t�ŷ��� �ܾ� : [%s]��\t\n", row[4]);
                        }
                        mysql_free_result(result1); // result set�� ����(free)���ش�.
                        mysql_close(conn);

                        printf("\t\t\t������ : 0��\n");
                        printf("------------------------------------------\n");
                        printf("�ŷ��Ͻ� : %d.%d.%d\t\t%d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                        printf("���¹�ȣ : %s--%d\n\n\n", bank_name, acc_number);

                        printf("1��[�ŷ�����], �ƹ�Ű[�ʱ�ȭ��] : ");

                        scanf("%s", &input);
                        okay = atoi(input);

                        if (okay == 1)
                        {
                            printf("\n�ŷ��� ����Ǿ����ϴ�.\n");
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
                    printf("\n��ҹ�ư�� �����̽��ϴ�. ��� �� �ʱ� ȭ������ ���ư��ϴ�...");
                    Sleep(1500); // 1.5�� delay �� �ʱ�ȭ������ ���ư�.
                    system("cls"); // �ܼ�â clear
                }

                break;
                    }

                }


                // ====================�۱�========================
            case 3:
         

                printf("3�� [�۱�]�� �����ϼ̽��ϴ�.\n\n");
                printf("==============================================\n");
                printf("Ÿ �������� �۱ݽ� [������ 500��]�� �ΰ��˴ϴ�.\n\n");
                printf("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup\n");
                printf("\n���� ������ ������ �������ּ��� : _\b");
                scanf("%d", &button);

                switch (button)
                {
                case 1:
                    strcpy(bank_name, "kakao");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;

                case 2:
                    strcpy(bank_name, "woori");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;

                case 3:
                    strcpy(bank_name, "shinhan");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;

                case 4:
                    strcpy(bank_name, "hana");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;

                case 5:
                    strcpy(bank_name, "nonghyup");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;
                }

                printf("���� ���¹�ȣ�� �Է����ּ��� : ");
                scanf("%d", &my_number);

                MYSQL_RES* result4 = mysql_store_result(conn);
                sprintf(query, "SELECT * FROM `atm`.`bank` WHERE account_num = '%d' AND bank_name = '%s'", my_number, bank_name);
                mysql_query(conn, query);
                result4 = mysql_store_result(conn);

                // ���� ���� ������ ����ġ�� ���
                if ((int)(result4->row_count) == 0)
                {
                    //fprintf(stderr, "%s\n", mysql_error(conn));
                    printf("\n�Է��Ͻ� ���� ���� ������ �����ϴ�. \n����, ���¹�ȣ�� �´��� Ȯ�����ּ���.\n\n");
                    printf("��� �� �ʱ� ȭ������ ���ư��ϴ�...");
                    Sleep(3000);
                    system("cls");
                    break;
                }

                //���� ���� ���� Ȯ��
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
                    printf("\n������ �����Դϴ�.\n�����ڿ��� �������ּ���!\n\n");
                    printf("��� �� �ʱ�ȭ������ ���ư��ϴ�...");
                    Sleep(2500);
                    system("cls");
                    break;
                }


                int count = 0;

                MYSQL_RES* result71 = mysql_store_result(conn);
                MYSQL_ROW rrrow1; // ���� ��й�ȣ�� �˱� ����
                char now_pass1[40];
                int inow_pass1 = 0;

                while (count < 3)
                {
                    printf("��й�ȣ�� �Է����ּ��� : ");
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
                        printf("\n��й�ȣ�� Ʋ�Ƚ��ϴ�.\n");
                        count++;
                        sprintf(query, "UPDATE `atm`.`bank` SET `error` = '%d' WHERE account_num = '%d'", count, my_number);
                        mysql_query(conn, query);

                        printf("[���� Ƚ��] : %d\n", count);
                    }

                    if (count >= 3)
                    {
                        printf("���°� �����Ǿ����ϴ�.\n");
                        break;
                    }

                    //��й�ȣ�� �¾��� ���
                    if (inow_pass1 == acc_password)
                    {
                        count = 0;
                        sprintf(query, "UPDATE `atm`.`bank` SET `error` = '%d' WHERE account_num = '%d'", count, my_number);
                        mysql_query(conn, query);

                        printf("��ü���� �Է����ּ��� :  ");
                        scanf("%d", &money);



                        MYSQL_ROW rrow; // ���� ���� �ܾ��� �˱����� -> �ܾ� ������ ��� ���x
                        char now_balance[255];
                        int inow_balance;

                        // ���� �ܾ��� ������ ���
                        rrow = mysql_fetch_row(result4);
                        strcpy(now_balance, rrow[4]);

                        inow_balance = atoi(now_balance);

                        if (inow_balance - money < 0)
                        {
                            system("cls");
                            printf("\n���� �ܾ��� �����մϴ�.\n\n");
                            printf("===========================\n");
                            printf("ó����� : �ܾ׺���\n");
                            printf("��û�ݾ� : %d��\n", money);
                            printf("���� �ܾ� : %d��\n", inow_balance);
                            printf("===========================\n\n\n");

                            printf("1��[�ŷ�����], �ƹ�Ű[�ʱ�ȭ��] : ");

                            scanf("%s", &input);
                            okay = atoi(input);

                            if (okay == 1)
                            {
                                printf("\n�ŷ��� ����Ǿ����ϴ�.\n");
                                return 0;
                            }

                            else
                            {
                                system("cls");
                                break;
                            }
                        }

                        system("cls"); // �ܼ�â clear

                        printf("\n==========����==========\n");
                        printf("���� : %s\n���¹�ȣ : %d\n�۱ݾ� : %d��\n", bank_name, my_number, money);
                        printf("========================\n\n");
                        printf("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup\n");
                        printf("\n�۱� ������ ���� ������ ������ �������ּ��� : _\b");
                        scanf("%d", &button);

                        switch (button)
                        {
                        case 1:
                            strcpy(bank_name2, "kakao");
                            printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name2);
                            break;

                        case 2:
                            strcpy(bank_name2, "woori");
                            printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name2);
                            break;

                        case 3:
                            strcpy(bank_name2, "shinhan");
                            printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name2);
                            break;

                        case 4:
                            strcpy(bank_name2, "hana");
                            printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name2);
                            break;

                        case 5:
                            strcpy(bank_name2, "nonghyup");
                            printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name2);
                            break;
                        }

                        int pandan = strcmp(bank_name, bank_name2); // ���ڿ� ���Լ� strcmp ���. ���� ������ ���� 0 ��ȯ
                        if (!pandan == 0)
                        {
                            printf("\n==========================\n");
                            printf("\nŸ ���� �۱��Դϴ�.\n������ 500���� �ΰ��˴ϴ�.\n\n");
                            printf("==========================\n");
                        }

                        else
                        {
                            printf("\n===========================\n");
                            printf("\n���� ���� �۱��Դϴ�.\n�����ᰡ �ΰ����� �ʽ��ϴ�.\n\n");
                            printf("===========================\n");
                        }

                        printf("\n�۱� ������ ���� ���¹�ȣ�� �Է����ּ��� : ________\b\b\b\b\b\b\b\b");
                        scanf("%d", &your_number);

                        MYSQL_RES* result5 = mysql_store_result(conn);
                        sprintf(query, "SELECT * FROM `atm`.`bank` WHERE account_num = '%d' AND bank_name = '%s'", your_number, bank_name2);
                        mysql_query(conn, query);
                        result5 = mysql_store_result(conn);

                        // �۱ݹ޴� ������ ������ ����ġ�� ���
                        if ((int)(result5->row_count) == 0)
                        {
                            fprintf(stderr, "%s\n", mysql_error(conn));
                            printf("========================================\n");
                            printf("�۱� ���� ������ �����ϴ�. \n����, ���¹�ȣ�� �´��� Ȯ�����ּ���.\n\n");
                            printf("��� �� �ʱ� ȭ������ ���ư��ϴ�...\n");
                            printf("========================================\n");
                            Sleep(3000);
                            system("cls");
                            break;
                        }

                        system("cls"); // �ܼ�â clear

                        printf("\n==========����==========\t==========Ÿ��==========\n");
                        printf("���� : %s\t\t\t���� : %s\n���¹�ȣ : %d\t\t���¹�ȣ : %d\n�۱ݾ� : %d��\n", bank_name, bank_name2, my_number, your_number, money);
                        printf("========================\t========================\n\n");
                        printf("�� ������ ������ 1��(��ü), Ʋ���� 2��(���)�� �Է����ּ���.\n");
                        printf("1��[��ü], 2��[���] : ");
                        scanf("%d", &okay);

                        if (okay == 1)
                        {
                            if (pandan == 0)
                            {
                                //�۱��ϴ� ���� mysql ������Ʈ
                                sprintf(query, "UPDATE `atm`.`bank` SET `balance` = `balance` - '%d' WHERE account_num = '%d' AND bank_name = '%s' AND acc_password = '%d'", money, my_number, bank_name, acc_password);
                                mysql_query(conn, query);

                                //�۱ݹ޴� ���� mysql ������Ʈ
                                sprintf(query, "UPDATE `atm`.`bank` SET `balance` = `balance` + '%d' WHERE account_num = '%d' AND bank_name = '%s'", money, your_number, bank_name2);
                                mysql_query(conn, query);
                            }

                            else
                            {
                                printf("\nŸ ���� �۱����� [������ 500��]�� �ΰ��˴ϴ�.\n");

                                //�۱��ϴ� ���� mysql ������Ʈ
                                sprintf(query, "UPDATE `atm`.`bank` SET `balance` = `balance` - '%d' - 500 WHERE account_num = '%d' AND bank_name = '%s' AND acc_password = '%d'", money, my_number, bank_name, acc_password);
                                mysql_query(conn, query);

                                //�۱ݹ޴� ���� mysql ������Ʈ
                                sprintf(query, "UPDATE `atm`.`bank` SET `balance` = `balance` + '%d' WHERE account_num = '%d' AND bank_name = '%s'", money, your_number, bank_name2);
                                mysql_query(conn, query);
                            }

                            printf("\n��ü �Ϸ�Ǿ����ϴ�!\n");
                            printf("����� ��ǥ�� ��µ˴ϴ�...");
                            Sleep(1500);
                            system("cls");

                            printf("%s �ŷ���ǥ\n[������ü]\n------------------------------------------\n", bank_name);
                            printf("��û�ݾ� :\t\t%d��\n", money);
                            printf("ó����� : ����ó��\n");

                            sprintf(query, "SELECT * FROM atm.bank WHERE account_num = '%d'", my_number);
                            mysql_query(conn, query);

                            MYSQL_RES* result1 = mysql_store_result(conn);      // mysql_store_result �Լ��� result set�� �������� result set�� MYSQL_RES ����ü�� ����.
                            if (result1 == NULL)
                            {
                                fprintf(stderr, "%s\n", mysql_error(conn));
                                exit(1);
                            }

                            int num_fields = mysql_num_fields(result1);        //mysql_num_fields �Լ��� ���̺��� Column ���� �˾Ƴ���.

                            MYSQL_ROW row; // row�� ���� ������ �ݺ��ؼ� row�� �������鼭 ������ ����ŭ row�� ������ �ֿܼ� ����Ѵ�.
                            while (row = mysql_fetch_row(result1))
                            {
                                printf("\t\t�ŷ��� �ܾ� : [%s]��\t\n", row[4]);
                            }
                            mysql_free_result(result1); // result set�� ����(free)���ش�.
                            mysql_close(conn);

                            if (pandan == 0) printf("\t\t\t������ : 0��\n");
                            else printf("\t\t\t������ : 500��\n");

                            printf("------------------------------------------\n");
                            printf("�ŷ��Ͻ� : %d.%d.%d\t\t%d:%d:%d\n", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);
                            printf("��� ���¹�ȣ : %s--%d\n", bank_name, my_number);
                            printf("�Ա� ���¹�ȣ : %s--%d\n\n\n", bank_name2, your_number);


                            printf("1��[�ŷ�����], �ƹ�Ű[�ʱ�ȭ��] : ");

                            scanf("%s", &input);
                            okay = atoi(input);

                            if (okay == 1)
                            {
                                printf("\n�ŷ��� ����Ǿ����ϴ�.\n");
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
                            printf("\n��ҹ�ư�� �����̽��ϴ�. ��� �� �ʱ� ȭ������ ���ư��ϴ�...");
                            Sleep(1500); // 1.5�� delay �� �ʱ�ȭ������ ���ư�.
                            system("cls"); // �ܼ�â clear
                        }

                        break;

                        Sleep(50000);


                       
                    }

                }


                // ====================�ܾ���ȸ========================
            case 4:
                printf("4�� [�ܾ���ȸ]�� �����ϼ̽��ϴ�.\n\n");

                printf("�����, �̸�, ���¹�ȣ, [�ܾ�]\n");

                mysql_query(conn, "SELECT * FROM atm.bank");       // mydb���� atm.bank ���̺��� ��� ������

                MYSQL_RES* rresult = mysql_store_result(conn);      // mysql_store_result �Լ��� result set�� �������� result set�� MYSQL_RES ����ü�� �����Ѵ�.
                if (rresult == NULL)
                {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                    exit(1);
                }
                int num_fields = mysql_num_fields(rresult);        //mysql_num_fields �Լ��� ���̺��� Column ���� �˾Ƴ���.

                MYSQL_ROW row; // row�� ���� ������ �ݺ��ؼ� row�� �������鼭 ������ ����ŭ row�� ������ �ֿܼ� ����Ѵ�.
                while (row = mysql_fetch_row(rresult))
                {
                    printf("---------------------------------------\n");

                    for (int i = 0; i < 1; i++)
                    {
                        printf("%s / %s / %s / [%s]��", row[0], row[1], row[2], row[4]);
                    }
                    printf("\n");
                    printf("---------------------------------------\n");

                }
                mysql_free_result(rresult); // result set�� ����(free)���ش�.
                mysql_close(conn);
                printf("\n1��[�ŷ�����], �ƹ�Ű[�ʱ�ȭ��] : ");

                scanf("%s", &input);

                okay = atoi(input);

                if (okay == 1)
                {
                    printf("\n�ŷ��� ����Ǿ����ϴ�.\n");
                    return 0;
                }

                else
                {
                    system("cls");
                    break;
                }


                // ====================���� ����========================
            case 5:
                printf("5�� [���� ����]�� �����ϼ̽��ϴ�.\n");
                printf("=================================\n");
                printf("1.kakao 2.woori 3.shinhan 4.hana 5.nonghyup\n");
                printf("\n����� ������ �������ּ��� : _\b");
                scanf("%s", &input);

                button = atoi(input); // ���ڸ� ���������� ��ȯ
                int end = 0;

                switch (button)
                {
                case 1:
                    strcpy(bank_name, "kakao");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;

                case 2:
                    strcpy(bank_name, "woori");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;

                case 3:
                    strcpy(bank_name, "shinhan");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;

                case 4:
                    strcpy(bank_name, "hana");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;

                case 5:
                    strcpy(bank_name, "nonghyup");
                    printf("\n%d�� [%s] ������ �����ϼ̽��ϴ�.\n", button, bank_name);
                    break;

                default :
                    printf("\n�ùٸ� ���� �Է����ּ���!\n\n");
                    end = 1;
                    break;
                }

                if (end == 1)
                {
                    break;
                }

                int balance = 0;
                int acc_num = create_acc();
                printf("�ű� ���¹�ȣ: %d\n", acc_num);

                char name[40]; // �̸� ����

                printf("�̸��� �Է����ּ��� : ");
                scanf("%s", name);
                printf("���� ��й�ȣ�� �Է����ּ��� : ");
                scanf("%d", &acc_password);

                system("cls"); // �ܼ�â clear

                printf("\n===========================\n\n");
                printf("�����Ī : %s \n���¹�ȣ : %d\n�̸� : %s\n��й�ȣ : %d\n", bank_name, acc_num, name, acc_password);
                printf("\n===========================\n\n");
                printf("�� ������ ������ 1��(���»���), Ʋ���� 2��(���)�� �Է����ּ���.\n");
                printf("1��[���»���], 2��[���] : _\b");
                scanf("%d", &okay);

                if (okay == 1)
                {
                    sprintf(query, "INSERT INTO `atm`.`bank` (`bank_name`, `name`, `account_num`, `acc_password`, `balance`) VALUES ('%s', '%s', '%d', '%d', '%d')", bank_name, name, acc_num, acc_password, balance);
                    mysql_query(conn, query);
                    printf("\n���� ������ �Ϸ�Ǿ����ϴ�!\n");
                    return 0;
                }

                else if (okay == 2)
                {
                    printf("\n��ҹ�ư�� �����̽��ϴ�. ��� �� �ʱ�ȭ������ ���ư��ϴ�...");
                    Sleep(1500); // 1.5�� delay �� �ʱ�ȭ������ ���ư�.
                    system("cls"); // �ܼ�â clear
                    break;
                }

                // ====================��й�ȣ ����========================
            case 6:

                printf("��й�ȣ�� ������ ���¹�ȣ�� �Է����ּ��� : ________\b\b\b\b\b\b\b\b");
                scanf("%d", &acc_number);

                MYSQL_RES* result3 = mysql_store_result(conn);

                sprintf(query, "SELECT * FROM `atm`.`bank` WHERE account_num = '%d'", acc_number);
                mysql_query(conn, query);
                result3 = mysql_store_result(conn);
                if ((int)(result3->row_count) == 0)
                {
                    fprintf(stderr, "%s\n", mysql_error(conn));
                    printf("�Է��Ͻ� ���� ������ �����ϴ�. \n���¹�ȣ�� �´��� Ȯ�����ּ���.\n\n");
                    printf("��� �� �ʱ� ȭ������ ���ư��ϴ�...");
                    Sleep(3000);
                    system("cls");
                    break;
                }

                //���� ���� ���� Ȯ��
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
                    printf("\n������ �����Դϴ�.\n�����ڿ��� �������ּ���!\n\n");
                    printf("��� �� �ʱ�ȭ������ ���ư��ϴ�...");
                    Sleep(2500);
                    system("cls");
                    break;
                }

                int count1 = 0;

                MYSQL_RES* result7 = mysql_store_result(conn);
                MYSQL_ROW rrrow; // ���� ��й�ȣ�� �˱� ����
                char now_pass[40];
                int inow_pass = 0;

                while (count1 < 3)
                {
                    printf("\n���� ��й�ȣ�� �Է����ּ��� : ");
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
                        printf("\n��й�ȣ�� Ʋ�Ƚ��ϴ�.\n");
                        count1++;
                        sprintf(query, "UPDATE `atm`.`bank` SET `error` = '%d' WHERE account_num = '%d'", count1, acc_number);
                        mysql_query(conn, query);

                        printf("[���� Ƚ��] : %d\n", count1);
                    }

                    if (count1 >= 3)
                    {
                        printf("\n���°� �����Ǿ����ϴ�.\n");
                        printf("�����ڿ��� �����Ͻñ� �ٶ��ϴ�.\n");
                        Sleep(50000);
                        break;
                    }

                    //��й�ȣ�� �¾��� ���
                    if (inow_pass == acc_password)
                    {
                        count1 = 0;
                        sprintf(query, "UPDATE `atm`.`bank` SET `error` = '%d' WHERE account_num = '%d'", count1, acc_number);
                        mysql_query(conn, query);

                        while (rrrow = mysql_fetch_row(result7))
                        {
                            printf("\n\t--���� ����--\n���� : %s\n�̸� : %s \t\n", rrrow[0], rrrow[1]);
                        }
                        int acc_newpassword = 0; // ���ο� ��й�ȣ ����

                        printf("\n�����Ͻ� ��й�ȣ�� �Է����ּ��� : ");
                        scanf("%d", &acc_newpassword);

                        sprintf(query, "UPDATE `atm`.`bank` SET `acc_password` = '%d' WHERE account_num = '%d' AND acc_password = '%d'", acc_newpassword, acc_number, acc_password);
                        mysql_query(conn, query);

                        printf("��й�ȣ�� ���������� ����Ǿ����ϴ�.\n");

                        printf("1��[�ŷ�����], �ƹ�Ű[�ʱ�ȭ��] : ");
                        scanf("%d", &okay);
                    }

                    
                }
                
                if (okay == 1)
                {
                    printf("\n�ŷ��� ����Ǿ����ϴ�.\n");
                    return 0;
                }

                else
                {
                    system("cls");
                    break;
                }
                // ====================������========================
            case 7:

                printf("������ ��й�ȣ �Է� : ");
                int admin_password = radmin_password;
                int input_password = 0;
                scanf("%d", &input_password);

                if (admin_password == input_password)
                {
                    printf("\n==============������ �޴�==============\n");
                    printf("\n1.������������\n2.������ ��й�ȣ����\n");
                    printf("\n=======================================\n");
                    printf("�Է� : ");
                    scanf("%s", &input);
                    printf("\n");
                    button = atoi(input); // ���ڸ� ���������� ��ȯ

                    if (button == 1)
                    {
                        printf("������ ������ ���¹�ȣ �Է� : ");
                        scanf("%d", &acc_number);
                        int error = 0;

                        // ���� ������ ����ġ�� ���
                        MYSQL_RES* result99 = mysql_store_result(conn);
                        sprintf(query, "SELECT * FROM `atm`.`bank` WHERE account_num = '%d'", acc_number);
                        mysql_query(conn, query);
                        result99 = mysql_store_result(conn);

                        if ((int)(result99->row_count) == 0)
                        {
                            fprintf(stderr, "%s\n", mysql_error(conn));
                            printf("�Է��Ͻ� ���� ������ �����ϴ�. \n���¹�ȣ�� �´��� Ȯ�����ּ���.\n\n");
                            printf("��� �� �ʱ� ȭ������ ���ư��ϴ�...");
                            Sleep(3000);
                            system("cls");
                            break;
                        }

                        //���� ���� ���� Ȯ��
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
                            printf("\n������ �����Դϴ�.\n\n���� ������ �����Ͻðڽ��ϱ�?\n");
                            printf("1��[��], 2��[�ƴϿ�] : ");
                            scanf("%d", &okay);

                            if (okay == 1)
                            {
                                sprintf(query, "UPDATE `atm`.`bank` SET `error` = '%d' WHERE account_num = '%d'", error, acc_number);
                                mysql_query(conn, query);
                                printf("\n���� ������ ���� �����Ǿ����ϴ�.\n");
                                return 0;
                            }

                            else if (okay == 2)
                            {
                                printf("\n2��[�ƴϿ�]�� �����̽��ϴ�. ��� �� �ʱ�ȭ������ ���ư��ϴ�...");
                                Sleep(1500); // 1.5�� delay �� �ʱ�ȭ������ ���ư�.
                                system("cls"); // �ܼ�â clear
                            }

                            break;
                        }

                        else
                        {
                            printf("\n�������� ���� �����Դϴ�.\nerror count�� �ʱ�ȭ �Ͻðڽ��ϱ�?\n");
                            printf("1��[��], 2��[�ƴϿ�] : ");
                            scanf("%d", &okay);

                            if (okay == 1)
                            {
                                sprintf(query, "UPDATE `atm`.`bank` SET `error` = '%d' WHERE account_num = '%d'", error, acc_number);
                                mysql_query(conn, query);
                                printf("\nerror count�� ���� �ʱ�ȭ �Ǿ����ϴ�.\n");
                                return 0;
                            }

                            else if (okay == 2)
                            {
                                printf("\n2��[�ƴϿ�]�� �����̽��ϴ�. ��� �� �ʱ�ȭ������ ���ư��ϴ�...");
                                Sleep(1500);
                                system("cls");
                            }
                            break;
                        }
                    }

                    else if (button == 2)
                    {
                        int a = 0;
                        printf("������ ��й�ȣ�� �����Ͻðڽ��ϱ�?\n");
                        printf("1��[��], 2��[�ƴϿ�]\n");
                        scanf("%d", &a);
                        
                        if (a == 1)
                        {
                            printf("������ ��й�ȣ �Է� : ");
                            scanf("%d", &radmin_password);
                            printf("\n\n������ ��й�ȣ�� ���������� ����Ǿ����ϴ�.\n\n");
                            break;
                        }

                        else if (a == 2)
                        {
                            printf("�����մϴ�.\n\n");
                            break;
                        }


                        
                        break;
                    }



                }


                else
                {
                    printf("\n��й�ȣ�� �ٸ��ϴ�.\n\n");
                    break;
                }


                return 0;
            }
        }

        else
        {
            system("cls");
            printf("\n\n�߸� �Է��ϼ̽��ϴ�. ��Ȯ�� ���� �Է����ּ���!\n\n");
        }


    }

}

//-----------------------------------�Լ�--------------------------------------

// ���� ���»���(8�ڸ�) �Լ�
int create_acc()
{
    char str1[9];
    char str2[9];

    srand(time(NULL));
    int random1 = rand() % 9000 + 1000;
    int random2 = rand() % 9000 + 1000;

    sprintf(str1, "%d", random1);
    sprintf(str2, "%d", random2);

    strcat(str1, str2); // str1�� str2 ���ڿ� �̾����

    int num1 = atoi(str1); //str1 + str2 ���� ���������� ��ȯ ... atoi() : �������� ���������� ��ȯ

    return num1;
}