#include <stdio.h>
#include <string.h>
#include "mysql.h"

int main()
{
    MYSQL *mysql = NULL;
    int ret = 0;
    char buffer[1024];

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

    while(1)
    {
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);
        if(buffer == NULL)
        {
            continue;
        }
        if(strncmp(buffer, "exit", 4) == 0 || strncmp(buffer, "EXIT", 4) == 0 ||
            strncmp(buffer, "quit", 4) == 0 || strncmp(buffer, "QUIT", 4) == 0)
        {
            break;
        }

        //查询操作
        if(strncmp(buffer, "select", 6) == 0)
        {
            ret = mysql_query(mysql, buffer);
            if(ret != 0)
            {
                printf("mysql_query err:%d\r\n", ret);
                return ret;
            }

            int num = mysql_field_count(mysql);
            MYSQL_RES *result = NULL;
            result = mysql_store_result(mysql);
            if(result == NULL)
            {
                ret = mysql_errno(mysql);
                printf("mysql_store_result error:%d\r\n", ret);
                return ret;
            }
            //打印表头
            MYSQL_FIELD *fields;
            fields = mysql_fetch_fields(result);
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
            //释放结果集
            mysql_free_result(result);
        }
        //非查询操作
        else
        {
            ret = mysql_query(mysql, buffer);
            if(ret != 0)
            {
                printf("mysql_query err:%d\r\n", ret);
                return ret;
            }
            else
            {
                printf("mysql execute success\r\n");
            }
        }
    }

    mysql_close(mysql);
    printf("done\r\n");
}
