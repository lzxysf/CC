/*
c语言与mysql数据库连接使用了之前安装的libmysqlclient-dev库
具体的so库和头文件的路径在makefile脚本中有描述
*/

#include <stdio.h>
#include "mysql.h"

int main()
{
    MYSQL *mysql = NULL;
    int ret = 0;

    //1.初始化
    mysql = mysql_init(NULL);
    if(mysql == NULL)
    {
        ret = mysql_errno(mysql);
        printf("init error:%d\r\n", ret);
        return ret;
    }
    printf("mysql init ok...\r\n");

    //2.连接数据库
    mysql = mysql_real_connect(mysql, "localhost", "root", "216857", "school", 0, NULL, 0);
    if(mysql == NULL)
    {
        ret = mysql_errno(mysql);
        printf("mysql_real_connect error:%d\r\n", ret);
        return ret;
    }
    printf("connect ok...\r\n");

    //3.执行sql语句
    char* psql = "select * from students";
    ret = mysql_query(mysql, psql);
    if(ret != 0)
    {
        printf("mysql_query err:%d\r\n", ret);
        return ret;
    }

    //4.获取结果集
    MYSQL_RES *result = NULL;
    result = mysql_store_result(mysql);
    if(result == NULL)
    {
        ret = mysql_errno(mysql);
        printf("mysql_store_result error:%d\r\n", ret);
        return ret;
    }

    //5.解析结果集，提取数据
    MYSQL_ROW row;
    while((row = mysql_fetch_row(result)))
    {
        printf("%s\t%s\r\n", row[0], row[1]);
    }

    //6.释放结果集和关闭mysql连接
    mysql_free_result(result);
    mysql_close(mysql);

    printf("done\r\n");
}
