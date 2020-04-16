/*
mysql_stmt_init         初始化预处理环境句柄，返回一个结构体指针MYSQL_STMT *stmt
mysql_stmt_prepare      向上面句柄中添加SQL语句，带有(?,?,?)占位符
mysql_stmt_param_count  求绑定的变量个数（辅助函数）,有多少个'?'就返回几
mysql_stmt_bind_param   将?对应的实参，设置到预处理环境句柄中
mysql_stmt_execute      执行预处理SQL语句

mysql_stmt_affected_rows 返回上一次mysql操作所影响的行数
*/

#include <stdio.h>
#include <string.h>
#include "mysql.h"

#define INSERT_SAMPLE "insert into students values(?,?,?)"

int main()
{
    MYSQL *mysql = NULL;
    MYSQL_STMT *stmt;
    MYSQL_BIND bind[3];
    int ret = 0;
    my_ulonglong affected_rows;

    int int_data = 0;
    char str_data_name[10];
    unsigned long str_length_name;
    char str_data_address[100];
    unsigned long str_length_address;

    mysql = mysql_init(NULL);
    if(mysql == NULL)
    {
        ret = mysql_errno(mysql);
        printf("init error:%d\r\n", ret);
        return ret;
    }
    printf("mysql init ok...\r\n");

    mysql = mysql_real_connect(mysql, "localhost", "root", "216857", "school", 0, NULL, 0);
    if(mysql == NULL)
    {
        ret = mysql_errno(mysql);
        printf("mysql_real_connect error:%d\r\n", ret);
        return ret;
    }
    printf("connect ok...\r\n");

    //初始化预处理环境句柄
    stmt = mysql_stmt_init(mysql);
    if(!stmt)
    {
        fprintf(stderr, "mysql_stmt_init error\r\n");
        return -1;
    }

    //向预处理环境句柄中添加带(?,?,?)的sql语句
    if(mysql_stmt_prepare(stmt, INSERT_SAMPLE, strlen(INSERT_SAMPLE)))
    {
        fprintf(stderr, "mysql_stmt_prepare error\r\n");
        fprintf(stderr, "%s\r\n", mysql_stmt_error(stmt));
        return -2;
    }

    int param_count = mysql_stmt_param_count(stmt);
    if(param_count != 3)
    {
        fprintf(stderr, "invalid parameter count returned by mysql\r\n");
        return -3;
    }

    memset(bind, 0, sizeof(bind));

    //构造?对应的实参
    bind[0].buffer_type = MYSQL_TYPE_LONG;
    bind[0].buffer = (char*)&int_data;
    bind[0].is_null = 0;
    bind[0].length = 0;

    bind[1].buffer_type = MYSQL_TYPE_STRING;
    bind[1].buffer = (char*)str_data_name;
    bind[1].buffer_length = 10;
    bind[1].is_null = 0;
    bind[1].length = &str_length_name;
    
    bind[2].buffer_type = MYSQL_TYPE_STRING;
    bind[2].buffer = (char*)str_data_address;
    bind[2].buffer_length = 100;
    bind[2].is_null = 0;
    bind[2].length = &str_length_address;

    //将实参填充到预处理句柄中
    if(mysql_stmt_bind_param(stmt, bind))
    {
        fprintf(stderr, "mydql_stmt_bind_param failed\r\n");
        fprintf(stderr, "%s\r\n", mysql_stmt_error(stmt));
        return -4;
    }

    //指定实参的数据值
    int_data = 0;
    strcpy(str_data_name, "liangze");
    str_length_name = strlen(str_data_name);
    strcpy(str_data_address, "shijiazhuang");
    str_length_address = strlen(str_data_address);

    //执行预处理sql语句
    if(mysql_stmt_execute(stmt))
    {
        fprintf(stderr, "mydql_stmt_execute failed\r\n");
        fprintf(stderr, "%s\r\n", mysql_stmt_error(stmt));
        return -4;
    }
    //获得受影响的行数
    affected_rows = mysql_stmt_affected_rows(stmt);
    printf("total affected rows %lu\r\n", (unsigned long)affected_rows);

    //再插入另一条数据
    //指定实参的数据值
    int_data = 0;
    strcpy(str_data_name, "dongwei");
    str_length_name = strlen(str_data_name);
    strcpy(str_data_address, "shijiazhuang");
    str_length_address = strlen(str_data_address);
    //执行预处理sql语句
    if(mysql_stmt_execute(stmt))
    {
        fprintf(stderr, "mydql_stmt_execute failed\r\n");
        fprintf(stderr, "%s\r\n", mysql_stmt_error(stmt));
        return -4;
    }
    //获得受影响的行数
    affected_rows = mysql_stmt_affected_rows(stmt);
    printf("total affected rows %lu\r\n", (unsigned long)affected_rows);

    mysql_stmt_close(stmt);
    mysql_close(mysql);
    printf("done\r\n");
}
