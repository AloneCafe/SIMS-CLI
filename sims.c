/******************************************************************
本软件是在MIT许可证下发布的自由开源软件，源代码完全公开，用户可自由获取，使用，修改，分享，重新发布本软件，
若要查阅许可证请查看与源代码一同发布的LICENSE文件。
******************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <conio.h>

//字段大小
#define STUDENT_SIZE 250
#define NUM_SIZE 20
#define NAME_SIZE 20
#define DATE_SIZE 20
#define ADDRESS_SIZE 100
#define TEL_SIZE 15
#define EMAIL_SIZE 30

//数据文件名（同目录下的students.dat数据文件）
#define DATAFILE "students.dat"

//帮助信息
#define HELPINFO	"本系统是一个基于CLI（命令行界面），\
超轻量级的、自由且开源的学生信息管理系统，\n\
可以使管理者仅通过键盘直接管理学生信息。所有信息全部以ASCII文本文件的形式保存下来，\n\
源代码格式规范，条理清晰，程序执行效率高\n\
整个系统的构造十分简单，无需操作数据库。\n\n\
录入模块：录入新学生的记录，可录入学号，姓名，年龄，性别，出生年月，地址，电话，Email等字段。\n\
浏览模块：可以查看表中所有的学生信息。\n\
查询模块：可以依据学号和姓名来查询学生信息（支持前序的模糊查询）。\n\
排序模块：用于对学生表中的记录进行升序/降序排序（支持依据学号、姓名、年龄进行排序）。\n\
修改模块：可以修改学生表中某条记录的字段值，如学号，姓名，年龄，性别，出生年月，地址，电话，Email等字段。\n\
删除模块：可以删除学生表中某条记录。\
系统帮助：显示本帮助信息。\n"


//性别字段编码/解码宏
#define _ENCODE_SEX_(s) (s == "男" ? 'M' : 'F')
#define _DECODE_SEX_(s) (s == 'M'? "男" : "女")


//学生表数据结构
typedef struct
{
    //学号，姓名，年龄，性别，出生年月，地址，电话，Email等字段
	char num[NUM_SIZE];
	char name[NAME_SIZE];
	int age;
	char sex;
	char date[DATE_SIZE];
	char address[ADDRESS_SIZE];
	char tel[TEL_SIZE];
	char email[EMAIL_SIZE];
}Student;

//字段类型
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
	MALE = 'M',            //男性
	FEMALE = 'F'           //女性
}Sex;


//******************* 全局变量定义部分 *******************

//学生表指针
Student *T;
//记录的总数
int g_record_num;
//操作字
char g_opt;


//******************* 函数声明部分 *******************

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



//******************* 函数定义部分 *******************

//学生表内存释放函数
void free_student_table()
{
	free(T);
}

//学生表创建函数，size是表大小（记录的最大个数）
Student *create_student_table(int size)
{
	Student *p = NULL;
	p = (Student *)malloc(sizeof(Student) * size);
	return p;
}

//学生表清空函数
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
	
	//记录总数归零
	g_record_num = 0;
	
}

//此函数从数据文件读学生表，path字符串表示数据文件路径，record_max表示最大读取的记录数
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

//此函数写学生表到数据文件，path字符串表示数据文件路径
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

//很傻瓜的前序字符串匹配函数
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

//清屏函数
void clear()
{
	system("cls");
}

//录入模块界面函数
void input_mod()
{
	Student s;
	
	printf("##################  学生信息管理系统——信息录入模块  ##################\n");
	printf("快捷键：\n[N]:\t录入新学生记录\n[Q]:\t返回\n");
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
	
	printf("依次输入学号，姓名，年龄，性别，出生年月，地址，电话，Email：\n");
	printf("学号：");
	scanf("%s", s.num);
	printf("姓名：");
	scanf("%s", s.name);
	printf("年龄：");
	scanf("%d", &s.age);
	do
	{
		printf("性别 ([M]男 / [F]女)：");
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
	
	printf("出生年月：");
	scanf("%s", s.date);
	printf("地址：");
	scanf("%s", s.address);
	printf("电话：");
	scanf("%s", s.tel);
	printf("Email：");
	scanf("%s", s.email);
	
	
	
	strcpy(T[g_record_num].num, s.num);
	strcpy(T[g_record_num].name, s.name);
	T[g_record_num].age = s.age;
	T[g_record_num].sex = s.sex;
	strcpy(T[g_record_num].date, s.date);
	strcpy(T[g_record_num].address, s.address);
	strcpy(T[g_record_num].tel, s.tel);
	strcpy(T[g_record_num].email, s.email);

    //记录总数增加
	g_record_num++;
	
	printf("新信息录入成功\n");
	
	//写回数据文件
	write_student_table(DATAFILE);
	getch();
	clear();
	input_mod();
}

//浏览模块界面函数
void view_mod()
{
	int i, s = 0;
	printf("##################  学生信息管理系统——信息浏览模块  ##################\n");
	printf("快捷键：\n[V]:\t查看全部信息\n[Q]:\t返回\n");
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
	printf(" 学号\t姓名\t年龄\t性别\t出生年月\t地址\t电话\tEmail\n");
	for(i = 0; i < g_record_num; i++)
	{
		printf("%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\n", T[i].num, T[i].name, T[i].age, _DECODE_SEX_(T[i].sex), T[i].date, T[i].address, T[i].tel, T[i].email);
		s++;
	}
	printf("--------------------------------\n");
	printf("##### 共 %d 条记录 #####\n",s);
	
	//写回数据文件
	write_student_table(DATAFILE);
	getch();
	clear();
	view_mod();
	
}

//查询模块界面函数
void query_mod()
{
	printf("##################  学生信息管理系统——信息查询模块  ##################\n");
	printf("快捷键：\n[I]:\t按学号查询\n[N]:\t按姓名查询\n[Q]:\t返回\n注：模块支持模糊片段查询！\n");
	g_opt = getch();
	switch(g_opt)
	{
		case 'I':
		case 'i':{
			char num[NUM_SIZE];
			int i, s = 0;
			printf("请输入要查询的学号：");
			scanf("%s", num);
			printf("--------------------------------\n");
			printf("学号\t姓名\t年龄\t性别\t出生年月\t地址\t电话\tEmail\n");
			for(i = 0; i < g_record_num; i++)
			{
				if(match_str(num, T[i].num))
				{
				    printf("%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\n", T[i].num, T[i].name, T[i].age, _DECODE_SEX_(T[i].sex), T[i].date, T[i].address, T[i].tel, T[i].email);
					s++;
				}
			}
			printf("--------------------------------\n");
			printf("##### 共 %d 条记录 #####\n", s);
			break;
		}
		case 'N':
		case 'n':{
			char name[NAME_SIZE];
			int i, s = 0;
			printf("请输入要查询的姓名：");
			scanf("%s", name);
			printf("--------------------------------\n");
			printf(" 学号 \t 姓名 \t 年龄 \t 性别 \t 出生年月 \t 地址 \t 电话 \t Email \n");
   			for(i = 0; i < g_record_num; i++)
			{
				if(match_str(name, T[i].name))
				{
				    printf("%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\n", T[i].num, T[i].name, T[i].age, _DECODE_SEX_(T[i].sex), T[i].date, T[i].address, T[i].tel, T[i].email);
					s++;
				}
			}
			printf("--------------------------------\n");
			printf("##### 共 %d 条记录 #####\n", s);
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
	
	//写回数据文件
	write_student_table(DATAFILE);
	getch();
	clear();
	query_mod();
}

//排序模块界面函数
void sort_mod()
{
	int i, j, s = 0;
	char sort;
	Student temp;
	printf("##################  学生信息管理系统——信息排序模块  ##################\n");
	printf("快捷键：\n[I]:\t按学号排序\n[N]:\t按姓名排序\n[A]:\t按年龄排序\n[Q]:\t返回\n");
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
		printf("请选择排序方式：[A]:升序 / [D]:降序\n");
		sort = getch();
		putchar('\n');
	}while(sort != 'A' && sort != 'a' && sort != 'D' && sort != 'd');
	
	printf("--------------------------------\n");
	printf("学号\t姓名\t年龄\t性别\t出生年月\t地址\t电话\tEmail\n");
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
	printf("##### 共 %d 条记录 #####\n", s);
	printf("排序完毕！\n");
	
	//写回数据文件
	write_student_table(DATAFILE);
	getch();
	clear();
	sort_mod();
}

//修改模块界面函数
void modify_mod()
{
	int i, record, field, s = 0;
	printf("##################  学生信息管理系统——信息修改模块  ##################\n");
	printf("快捷键：\n[I]:\t根据记录号修改学生信息\n[Q]:\t返回\n");
	g_opt = getch();
	switch(g_opt)
	{
		case 'I':
		case 'i':{
			printf("--------------------------------\n");
			printf("记录号\t学号\t姓名\t年龄\t性别\t出生年月\t地址\t电话\tEmail\n");
			for(i = 0; i < g_record_num; i++)
			{
				printf("[%d]：\t%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\n", s, T[i].num, T[i].name, T[i].age, _DECODE_SEX_(T[i].sex), T[i].date, T[i].address, T[i].tel, T[i].email);
				s++;
			}
			printf("--------------------------------\n");
			printf("##### 共 %d 条记录 #####\n", s);
			
			printf("请输入记录号：");
			scanf("%d", &record);
			
			_FIELD_MODIFY_:
			printf("请选择要修改的字段，[0]学号\t[1]姓名\t[2]年龄\t[3]性别\t[4]出生年月\t[5]地址\t[6]电话\t[7]Email：\n");
			field = getch();
			
			switch(field)
			{
				case _NUM_:{
					printf("请输入学号：");
					scanf("%s", T[record].num);
					break;
				}
				case _NAME_:{
					printf("请输入姓名：");
					scanf("%s", T[record].name);
					break;
				}
				case _AGE_:{
					printf("请输入年龄：");
					scanf("%d", &T[record].age);
					break;
				}
				case _SEX_:{
					do
					{
						printf("请输入性别 ([M]男 / [F]女)：");
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
					printf("请输入出生日期：");
					scanf("%s", T[record].date);
					break;
				}
				case _ADDRESS_:{
					printf("请输入地址：");
					scanf("%s", T[record].address);
					break;
				}
				case _TEL_:{
					printf("请输入电话号码：");
					scanf("%s", T[record].tel);
					break;
				}
				case _EMAIL_:{
					printf("请输入Email：");
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
	printf("信息成功修改！结果如下：\n");
	printf("学号\t姓名\t年龄\t性别\t出生年月\t地址\t电话\tEmail\n");
	printf("%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\n", T[record].num, T[record].name, T[record].age, _DECODE_SEX_(T[record].sex), T[record].date, T[record].address, T[record].tel, T[record].email);
	printf("修改完毕！\n");
	
	//写回数据文件
	write_student_table(DATAFILE);
	getch();
	clear();
	modify_mod();
}

//删除模块界面函数
void del_mod()
{
	int i, record, s = 0;
	printf("##################  学生信息管理系统——信息删除模块  ##################\n");
	printf("快捷键：\n[I]:\t根据记录号删除学生信息\n[Q]:\t返回\n");
	g_opt = getch();
	switch(g_opt)
	{
		case 'I':
		case 'i':{
			printf("--------------------------------\n");
			printf("记录号\t学号\t姓名\t年龄\t性别\t出生年月\t地址\t电话\tEmail\n");
			for(i = 0; i < g_record_num; i++)
			{
				printf("[%d]：\t%s\t%s\t%d\t%s\t%s\t%s\t%s\t%s\n", s, T[i].num, T[i].name, T[i].age, _DECODE_SEX_(T[i].sex), T[i].date, T[i].address, T[i].tel, T[i].email);
				s++;
			}
			printf("--------------------------------\n");
			printf("##### 共 %d 条记录 #####\n", s);

			printf("请输入记录号：");
			scanf("%d", &record);
			
			
			for(i = record + 1; i < g_record_num; i++)
			{
				memcpy(&T[i - 1], &T[i], sizeof(Student));
			}
			
			//记录总数减小
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
	
	printf("该条记录已删除！\n");

	//写回数据文件
	write_student_table(DATAFILE);
	getch();
	clear();
	del_mod();
	
	
}

//系统帮助界面函数
void help_mod()
{
	printf("##################  学生信息管理系统——系统帮助  ##################\n");
	printf(HELPINFO);
	getch();
	
	return;
}

//主菜单界面函数
void main_menu()
{
	printf("##################  学生信息管理系统  ##################\n");
	printf("快捷键：\n[I]:\t信息录入\n[V]:\t信息浏览\n[Q]:\t信息查询\n[S]:\t信息排序\n[M]:\t信息修改\n[D]:\t信息删除\n[H]:\t显示帮助\n[E]:\t退出系统\n");
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
	//写回数据文件
	write_student_table(DATAFILE);
	clear();
	main_menu();
}

//主函数
int main(int args, char *argv[])
{
	//CMD设定
	system("mode con cols=160 lines=50");
	system("color 0b");
	
	//初始化全局变量——学生表
	T = create_student_table(STUDENT_SIZE);
	
	//读文件到学生表
	read_student_table(DATAFILE, STUDENT_SIZE);
	
	//调用主菜单
	main_menu();
	
	return EXIT_SUCCESS;
}
