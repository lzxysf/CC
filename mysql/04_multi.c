/*
mysql多语句执行功能
1.在mysql_real_connect中使用CLIENT_MULTI_STATEMENTS标志位，使能多语句查询功能
2.对结果进行轮询，取出所有的查询结果
*/

#include <stdio.h>
#include "mysql.h"

int main()
{
    MYSQL *mysql = NULL;
    int ret = 0;
    int status;

    //初始化
    mysql = mysql_init(NULL);
    if(mysql == NULL)
    {
        ret = mysql_errno(mysql);
        printf("init error:%d\r\n", ret);
        return ret;
    }
    printf("mysql init ok...\r\n");

    //连接数据库，使用CLIENT_MULTI_STATEMENTS标志位，使能多语句执行功能
    mysql = mysql_real_connect(mysql, "localhost", "root", "216857", "school", 0, NULL, CLIENT_MULTI_STATEMENTS);
    if(mysql == NULL)
    {
        ret = mysql_errno(mysql);
        printf("mysql_real_connect error:%d\r\n", ret);
        return ret;
    }
    printf("connect ok...\r\n");

    //执行多条sql语句
    char* psql = "select * from students;select name from students group by name;";
    ret = mysql_query(mysql, psql);
    if(ret != 0)
    {
        printf("mysql_query err:%d\r\n", ret);
        return ret;
    }

    do{
        int num = mysql_field_count(mysql);
        MYSQL_RES *result = mysql_store_result(mysql);
        if(result == NULL)
        {
            ret = mysql_errno(mysql);
            printf("mysql_store_result error:%d\r\n", ret);
            return ret;
        }

        //打印表头
        MYSQL_FIELD *fields = mysql_fetch_fields(result);
        for(int i = 0; i < num; i++)
        {
            printf("%s\t", fields[i].name);
        }
        printf("\r\n");
        //解析结果集，提取数据
        MYSQL_ROW row;
        while((row = mysql_fetch_row(result)))
        {
            for(int i = 0; i < num; i++)
            {
                printf("%s\t", row[i]);
            }
            printf("\r\n");
        }
        mysql_free_result(result);
        
        status = mysql_next_result(mysql);
    }while(status==0);
    
    //6.释放结果集和关闭mysql连接
    mysql_close(mysql);

    printf("done\r\n");
}
