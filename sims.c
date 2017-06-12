/******************************************************************
���������GNU GPL V2���֤�·��������������Դ������ȫ�������û������ɻ�ȡ��ʹ�ã��޸ģ��������·����������
���Ҹ���GPL���֤�����ݣ��û�������޸İ��Լ�����������һͬ������GPL���֤�·����������������ν������ɻ���
��Ҫ�������֤��μ��������ӣ�
GPL V2 ���֤ԭ�ģ�http://www.gnu.org/licenses/old-licenses/gpl-2.0.html
GPL V2 ���֤���ķ��룺 http://mprc.pku.edu.cn/~ruankunliang/gpl.html
******************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

//�ֶδ�С
#define STUDENT_SIZE 250
#define NUM_SIZE 20
#define NAME_SIZE 20
#define DATE_SIZE 20
#define ADDRESS_SIZE 100
#define TEL_SIZE 15
#define EMAIL_SIZE 30

//�����ļ�����ͬĿ¼�µ�students.dat�����ļ���
#define DATAFILE "students.dat"

//������Ϣ
#define HELPINFO	"��ϵͳ��һ������CLI�������н��棩��\
���������ġ������ҿ�Դ��ѧ����Ϣ����ϵͳ��\n\
����ʹ�����߽�ͨ������ֱ�ӹ���ѧ����Ϣ��������Ϣȫ����ASCII�ı��ļ�����ʽ����������\n\
Դ�����ʽ�淶����������������ִ��Ч�ʸ�\n\
����ϵͳ�Ĺ���ʮ�ּ򵥣�����������ݿ⡣\n\n\
¼��ģ�飺¼����ѧ���ļ�¼����¼��ѧ�ţ����������䣬�Ա𣬳������£���ַ���绰��Email���ֶΡ�\n\
���ģ�飺���Բ鿴�������е�ѧ����Ϣ��\n\
��ѯģ�飺��������ѧ�ź���������ѯѧ����Ϣ��֧��ǰ���ģ����ѯ����\n\
����ģ�飺���ڶ�ѧ�����еļ�¼��������/��������֧������ѧ�š�����������������򣩡�\n\
�޸�ģ�飺�����޸�ѧ������ĳ����¼���ֶ�ֵ����ѧ�ţ����������䣬�Ա𣬳������£���ַ���绰��Email���ֶΡ�\n\
ɾ��ģ�飺����ɾ��ѧ������ĳ����¼��\
ϵͳ��������ʾ��������Ϣ��\n"


//�Ա��ֶα���/�����
#define _ENCODE_SEX_(s) (s == "��" ? 'M' : 'F')
#define _DECODE_SEX_(s) (s == 'M'? "��" : "Ů")


//ѧ�������ݽṹ
typedef struct
{
    //ѧ�ţ����������䣬�Ա𣬳������£���ַ���绰��Email���ֶ�
	char num[NUM_SIZE];
	char name[NAME_SIZE];
	int age;
	char sex;
	char date[DATE_SIZE];
	char address[ADDRESS_SIZE];
	char tel[TEL_SIZE];
	char email[EMAIL_SIZE];
}Student;

//�ֶ�����
typedef enum
{
	_NUM_ = '0',
	_NAME_,
	_AGE_,
	_SEX_,
	_DATE_,
	_ADDRESS_,
	_TEL_,
	_EMAIL_
}Field;

typedef enum
{
	MALE = 'M',            //����
	FEMALE = 'F'           //Ů��
}Sex;


//******************* ȫ�ֱ������岿�� *******************

//ѧ����ָ��
Student *T;
//��¼������
int g_record_num;
//������
char g_opt;


//******************* ������������ *******************

Student *create_student_table(int size);
void clear_student_table();
int read_student_table(char path[], int record_max);
int write_student_table(char path[]);
int match_str(char *opt, char *str);
void input_mod();
void view_mod();
void query_mod();
void sort_mod();
void modify_mod();
void del_mod();
void help_mod();
void main_menu();
void clear();



//******************* �������岿�� *******************

//ѧ�����ڴ��ͷź���
void free_student_table()
{
	free(T);
}

//ѧ������������size�Ǳ��С����¼����������
Student *create_student_table(int size)
{
	Student *p = NULL;
	p = (Student *)malloc(sizeof(Student) * size);
	return p;
}

//ѧ������պ���
void clear_student_table()
{
	int i;
	for(i = 0; i < g_record_num; i++)
	{
		strcpy(T[i].num, "");
		strcpy(T[i].name, "");
		T[i].age = 0;
		T[i].sex = 0;
		strcpy(T[i].date, "");
		strcpy(T[i].address, "");
		strcpy(T[i].tel, "");
		strcpy(T[i].email, "");
	}
	
	//��¼��������
	g_record_num = 0;
	
}

//�˺����������ļ���ѧ����path�ַ�����ʾ�����ļ�·����record_max��ʾ����ȡ�ļ�¼��
int read_student_table(char path[], int record_max)
{
	FILE *fd;
	int i = 0;
	
	if((fd = fopen(path, "r")) == NULL)
	{
		perror(path);
		fclose(fd);
		return EXIT_FAILURE;
	}

	while(fscanf(fd, "%s %s %d %c %s %s %s %s", T[i].num, T[i].name, &T[i].age, &T[i].sex, T[i].date, T[i].address, T[i].tel, T[i].email) != EOF)
	{
		if(++i == record_max)
		{
			break;
		}
	}
	
	g_record_num = i;
	fclose(fd);
	
	return EXIT_SUCCESS;
}

//�˺���дѧ���������ļ���path�ַ�����ʾ�����ļ�·��
int write_student_table(char path[])
{
	FILE *fd;
	int i;

	if((fd = fopen(path, "w")) == NULL)
	{
		perror(path);
		fclose(fd);
		return EXIT_FAILURE;
	}
	for(i = 0; i < g_record_num; i++)
	{
		fprintf(fd, "%s %s %d %c %s %s %s %s\n", T[i].num, T[i].name, T[i].age, T[i].sex, T[i].date, T[i].address, T[i].tel, T[i].email);
	}
	
	fclose(fd);
	return EXIT_SUCCESS;
	
}

//��ɵ�ϵ�ǰ���ַ���ƥ�亯��
int match_str(char *opt, char *str)
{
	int i = 0;
	int len = strlen(opt);
	while(*opt++ == *str++ && i < len)
	{
		i++;
	}
	
	return i == len ? 1 : 0;
}

//��������
void clear()
{
	system("cls");
}

//¼��ģ����溯��
void input_mod()
{
	Student s;
	
	printf("##################  ѧ����Ϣ����ϵͳ������Ϣ¼��ģ��  ##################\n");
	printf("��ݼ���\n[N]:\t¼����ѧ����¼\n[Q]:\t����\n");
	g_opt = getch();
	switch(g_opt)
	{
		case 'N':
		case 'n':{
			break;
		}
		case 'Q':
		case 'q':{
			clear();
			return;
			break;
		}
		default:{
			clear();
			input_mod();
			break;
		}
	}
	
	printf("��������ѧ�ţ����������䣬�Ա𣬳������£���ַ���绰��Email��\n");
	printf("ѧ�ţ�");
	scanf("%s", s.num);
	printf("������");
	scanf("%s", s.name);
	printf("���䣺");
	scanf("%d", &s.age);
	do
	{
		printf("�Ա� ([M]�� / [F]Ů)��");
		s.sex = getch();
		
		if(s.sex == 'm')
		{
			s.sex = 'M';
		}
		else if(s.sex == 'f')
		{
			s.sex = 'F';
		}
		printf("%s",_DECODE_SEX_(s.sex));
		putchar('\n');
	}while(s.sex != 'M' && s.sex != 'F');
	
	printf("�������£�");
	scanf("%s", s.date);
	printf("��ַ��");
	scanf("%s", s.address);
	printf("�绰��");
	scanf("%s", s.tel);
	printf("Email��");
	scanf("%s", s.email);
	
	
	
	strcpy(T[g_record_num].num, s.num);
	strcpy(T[g_record_num].name, s.name);
	T[g_record_num].age = s.age;
	T[g_record_num].sex = s.sex;
	strcpy(T[g_record_num].date, s.date);
	strcpy(T[g_record_num].address, s.address);
	strcpy(T[g_record_num].tel, s.tel);
	strcpy(T[g_record_num].email, s.email);

    //��¼��������
	g_record_num++;
	
	printf("����Ϣ¼��ɹ�\n");
	
	//д�������ļ�
	write_student_table(DATAFILE);
	getch();
	clear();
	input_mod();
}

//���ģ����溯��
void view_mod()
{
	int i, s = 0;
	printf("##################  ѧ����Ϣ����ϵͳ������Ϣ���ģ��  ##################\n");
	printf("��ݼ���\n[V]:\t�鿴ȫ����Ϣ\n[Q]:\t����\n");
	g_opt = getch();
	switch(g_opt)
	{
		case 'V':
		case 'v':{
			break;
		}
		case 'Q':
		case 'q':{
			clear();
			return;
			break;
		}
		default:{
			clear();
			view_mod();
			break;
		}
	}
	printf("--------------------------------\n");
	printf(" ѧ��\t����\t����\t�Ա�\t��������\t��ַ\t�绰\tEmail\n");
	for(i = 0; i < g_record_num; i++)
	{
		printf("%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\n", T[i].num, T[i].name, T[i].age, _DECODE_SEX_(T[i].sex), T[i].date, T[i].address, T[i].tel, T[i].email);
		s++;
	}
	printf("--------------------------------\n");
	printf("##### �� %d ����¼ #####\n",s);
	
	//д�������ļ�
	write_student_table(DATAFILE);
	getch();
	clear();
	view_mod();
	
}

//��ѯģ����溯��
void query_mod()
{
	printf("##################  ѧ����Ϣ����ϵͳ������Ϣ��ѯģ��  ##################\n");
	printf("��ݼ���\n[I]:\t��ѧ�Ų�ѯ\n[N]:\t��������ѯ\n[Q]:\t����\nע��ģ��֧��ģ��Ƭ�β�ѯ��\n");
	g_opt = getch();
	switch(g_opt)
	{
		case 'I':
		case 'i':{
			char num[NUM_SIZE];
			int i, s = 0;
			printf("������Ҫ��ѯ��ѧ�ţ�");
			scanf("%s", num);
			printf("--------------------------------\n");
			printf("ѧ��\t����\t����\t�Ա�\t��������\t��ַ\t�绰\tEmail\n");
			for(i = 0; i < g_record_num; i++)
			{
				if(match_str(num, T[i].num))
				{
				    printf("%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\n", T[i].num, T[i].name, T[i].age, _DECODE_SEX_(T[i].sex), T[i].date, T[i].address, T[i].tel, T[i].email);
					s++;
				}
			}
			printf("--------------------------------\n");
			printf("##### �� %d ����¼ #####\n", s);
			break;
		}
		case 'N':
		case 'n':{
			char name[NAME_SIZE];
			int i, s = 0;
			printf("������Ҫ��ѯ��������");
			scanf("%s", name);
			printf("--------------------------------\n");
			printf(" ѧ�� \t ���� \t ���� \t �Ա� \t �������� \t ��ַ \t �绰 \t Email \n");
   			for(i = 0; i < g_record_num; i++)
			{
				if(match_str(name, T[i].name))
				{
				    printf("%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\n", T[i].num, T[i].name, T[i].age, _DECODE_SEX_(T[i].sex), T[i].date, T[i].address, T[i].tel, T[i].email);
					s++;
				}
			}
			printf("--------------------------------\n");
			printf("##### �� %d ����¼ #####\n", s);
			break;
		}
		case 'Q':
		case 'q':{
			clear();
			return;
			break;
		}
		default:{
			clear();
			view_mod();
			break;
		}
	}
	
	//д�������ļ�
	write_student_table(DATAFILE);
	getch();
	clear();
	query_mod();
}

//����ģ����溯��
void sort_mod()
{
	int i, j, s = 0;
	char sort;
	Student temp;
	printf("##################  ѧ����Ϣ����ϵͳ������Ϣ����ģ��  ##################\n");
	printf("��ݼ���\n[I]:\t��ѧ������\n[N]:\t����������\n[A]:\t����������\n[Q]:\t����\n");
	g_opt = getch();
	switch(g_opt)
	{
		case 'I':
		case 'i':{
			
			for(i = 0; i < g_record_num; i++)
			{
				for(j = i + 1; j < g_record_num; j++)
				{
					if(strcmp(T[i].num, T[j].num) > 0)
					{
						memcpy(&temp, &T[j], sizeof(Student));
						memcpy(&T[j], &T[i], sizeof(Student));
						memcpy(&T[i], &temp, sizeof(Student));
						
					}
				}
			}
			
			break;
		}
		case 'N':
		case 'n':{
			for(i = 0; i < g_record_num; i++)
			{
				for(j = i + 1; j < g_record_num; j++)
				{
					if(strcmp(T[i].name, T[j].name) > 0)
					{
						memcpy(&temp, &T[j], sizeof(Student));
						memcpy(&T[j], &T[i], sizeof(Student));
						memcpy(&T[i], &temp, sizeof(Student));

					}
				}
			}
			
			break;
		}
		case 'A':
		case 'a':{
			for(i = 0; i < g_record_num; i++)
			{
				for(j = i + 1; j < g_record_num; j++)
				{
					if(T[i].age > T[j].age)
					{
						memcpy(&temp, &T[j], sizeof(Student));
						memcpy(&T[j], &T[i], sizeof(Student));
						memcpy(&T[i], &temp, sizeof(Student));

					}
				}
			}
			
			break;
		}
		case 'Q':
		case 'q':{
			clear();
			return;
			break;
		}
		default:{
			clear();
			sort_mod();
			break;
		}
	}

	do{
		printf("��ѡ������ʽ��[A]:���� / [D]:����\n");
		sort = getch();
		putchar('\n');
	}while(sort != 'A' && sort != 'a' && sort != 'D' && sort != 'd');
	
	printf("--------------------------------\n");
	printf("ѧ��\t����\t����\t�Ա�\t��������\t��ַ\t�绰\tEmail\n");
	switch(sort)
	{
		case 'a':
		case 'A':{

			for(i = 0; i < g_record_num; i++)
			{
				printf("%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\n", T[i].num, T[i].name, T[i].age, _DECODE_SEX_(T[i].sex), T[i].date, T[i].address, T[i].tel, T[i].email);
				s++;
			}
			break;
		}
		case 'd':
		case 'D':{
			for(i = 0; i < g_record_num; i++)
			{
				for(j = i + 1; j < g_record_num; j++)
				{
					memcpy(&temp, &T[i], sizeof(Student));
					memcpy(&T[i], &T[j], sizeof(Student));
					memcpy(&T[j], &temp, sizeof(Student));
				}
			}
			
			for(i = 0; i < g_record_num; i++)
			{
				printf("%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\n", T[i].num, T[i].name, T[i].age, _DECODE_SEX_(T[i].sex), T[i].date, T[i].address, T[i].tel, T[i].email);
				s++;
			}
			
			break;
		}
	}
	printf("--------------------------------\n");
	printf("##### �� %d ����¼ #####\n", s);
	printf("������ϣ�\n");
	
	//д�������ļ�
	write_student_table(DATAFILE);
	getch();
	clear();
	sort_mod();
}

//�޸�ģ����溯��
void modify_mod()
{
	int i, record, field, s = 0;
	printf("##################  ѧ����Ϣ����ϵͳ������Ϣ�޸�ģ��  ##################\n");
	printf("��ݼ���\n[I]:\t���ݼ�¼���޸�ѧ����Ϣ\n[Q]:\t����\n");
	g_opt = getch();
	switch(g_opt)
	{
		case 'I':
		case 'i':{
			printf("--------------------------------\n");
			printf("��¼��\tѧ��\t����\t����\t�Ա�\t��������\t��ַ\t�绰\tEmail\n");
			for(i = 0; i < g_record_num; i++)
			{
				printf("[%d]��\t%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\n", s, T[i].num, T[i].name, T[i].age, _DECODE_SEX_(T[i].sex), T[i].date, T[i].address, T[i].tel, T[i].email);
				s++;
			}
			printf("--------------------------------\n");
			printf("##### �� %d ����¼ #####\n", s);
			
			printf("�������¼�ţ�");
			scanf("%d", &record);
			
			_FIELD_MODIFY_:
			printf("��ѡ��Ҫ�޸ĵ��ֶΣ�[0]ѧ��\t[1]����\t[2]����\t[3]�Ա�\t[4]��������\t[5]��ַ\t[6]�绰\t[7]Email��\n");
			field = getch();
			
			switch(field)
			{
				case _NUM_:{
					printf("������ѧ�ţ�");
					scanf("%s", T[record].num);
					break;
				}
				case _NAME_:{
					printf("������������");
					scanf("%s", T[record].name);
					break;
				}
				case _AGE_:{
					printf("���������䣺");
					scanf("%d", &T[record].age);
					break;
				}
				case _SEX_:{
					do
					{
						printf("�������Ա� ([M]�� / [F]Ů)��");
						T[record].sex = getch();

						if(T[record].sex == 'm')
						{
							T[record].sex = 'M';
						}else if(T[record].sex == 'f')
						{
							T[record].sex = 'F';
						}
						printf("%s",_DECODE_SEX_(T[record].sex));
						putchar('\n');
					}while(T[record].sex != 'M' && T[record].sex != 'F');
					break;
				}
				case _DATE_:{
					printf("������������ڣ�");
					scanf("%s", T[record].date);
					break;
				}
				case _ADDRESS_:{
					printf("�������ַ��");
					scanf("%s", T[record].address);
					break;
				}
				case _TEL_:{
					printf("������绰���룺");
					scanf("%s", T[record].tel);
					break;
				}
				case _EMAIL_:{
					printf("������Email��");
					scanf("%s", T[record].email);
					break;
				}
				default:{
					goto _FIELD_MODIFY_;
					break;
				}
			}
			
			break;
		}
		case 'Q':
		case 'q':{
			clear();
			return;
			break;
		}
		default:{
			clear();
			modify_mod();
			break;
		}
	}
	printf("��Ϣ�ɹ��޸ģ�������£�\n");
	printf("ѧ��\t����\t����\t�Ա�\t��������\t��ַ\t�绰\tEmail\n");
	printf("%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\n", T[record].num, T[record].name, T[record].age, _DECODE_SEX_(T[record].sex), T[record].date, T[record].address, T[record].tel, T[record].email);
	printf("�޸���ϣ�\n");
	
	//д�������ļ�
	write_student_table(DATAFILE);
	getch();
	clear();
	modify_mod();
}

//ɾ��ģ����溯��
void del_mod()
{
	int i, record, s = 0;
	printf("##################  ѧ����Ϣ����ϵͳ������Ϣɾ��ģ��  ##################\n");
	printf("��ݼ���\n[I]:\t���ݼ�¼��ɾ��ѧ����Ϣ\n[Q]:\t����\n");
	g_opt = getch();
	switch(g_opt)
	{
		case 'I':
		case 'i':{
			printf("--------------------------------\n");
			printf("��¼��\tѧ��\t����\t����\t�Ա�\t��������\t��ַ\t�绰\tEmail\n");
			for(i = 0; i < g_record_num; i++)
			{
				printf("[%d]��\t%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\n", s, T[i].num, T[i].name, T[i].age, _DECODE_SEX_(T[i].sex), T[i].date, T[i].address, T[i].tel, T[i].email);
				s++;
			}
			printf("--------------------------------\n");
			printf("##### �� %d ����¼ #####\n", s);

			printf("�������¼�ţ�");
			scanf("%d", &record);
			
			
			for(i = record + 1; i < g_record_num; i++)
			{
				memcpy(&T[i - 1], &T[i], sizeof(Student));
			}
			
			//��¼������С
			g_record_num--;
			break;
		}
		case 'Q':
		case 'q':{
			clear();
			return;
			break;
		}
		default:{
			clear();
			del_mod();
			break;
		}
	}
	
	printf("������¼��ɾ����\n");

	//д�������ļ�
	write_student_table(DATAFILE);
	getch();
	clear();
	del_mod();
	
	
}

//ϵͳ�������溯��
void help_mod()
{
	printf("##################  ѧ����Ϣ����ϵͳ����ϵͳ����  ##################\n");
	printf(HELPINFO);
	getch();
	
	return;
}

//���˵����溯��
void main_menu()
{
	printf("##################  ѧ����Ϣ����ϵͳ  ##################\n");
	printf("��ݼ���\n[I]:\t��Ϣ¼��\n[V]:\t��Ϣ���\n[Q]:\t��Ϣ��ѯ\n[S]:\t��Ϣ����\n[M]:\t��Ϣ�޸�\n[D]:\t��Ϣɾ��\n[H]:\t��ʾ����\n[E]:\t�˳�ϵͳ\n");
	g_opt = getch();
	switch(g_opt)
	{
		case 'I':
		case 'i':{
			clear();
			input_mod();
			break;
		}
		case 'V':
		case 'v':{
			clear();
			view_mod();
			break;
		}
		case 'Q':
		case 'q':{
			clear();
			query_mod();
			break;
		}
		case 'S':
		case 's':{
			clear();
			sort_mod();
			break;
		}
		case 'M':
		case 'm':{
			clear();
			modify_mod();
			break;
		}
		case 'D':
		case 'd':{
			clear();
			del_mod();
			break;
		}
		case 'H':
		case 'h':{
			clear();
			help_mod();
			break;
		}
		case 'E':
		case 'e':{
			free_student_table();
			exit(0);
		}
		default:{
			break;
		}
			
	}
	//д�������ļ�
	write_student_table(DATAFILE);
	clear();
	main_menu();
}

//������
int main(int args, char *argv[])
{
	//CMD�趨
	system("mode con cols=160 lines=50");
	system("color 0b");
	
	//��ʼ��ȫ�ֱ�������ѧ����
	T = create_student_table(STUDENT_SIZE);
	
	//���ļ���ѧ����
	read_student_table(DATAFILE, STUDENT_SIZE);
	
	//�������˵�
	main_menu();
	
	return EXIT_SUCCESS;
}
