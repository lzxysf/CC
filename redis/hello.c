#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <hiredis/hiredis.h>

int main()
{
    //1.连接
    redisContext *context = redisConnect("127.0.0.1", 6379);
    if(context == NULL)
    {
        printf("connect error\r\n");
        return -1;
    }
    
    //2.授权登陆(如果设置了需要密码)
    redisReply *reply = (redisReply*)redisCommand(context, "auth 216857");
    if(reply==NULL)
    {
        printf("command error\r\n");
        return -2;
    }
    //登陆成功判断
    if(reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str, "OK") == 0 && reply->len == 2)
    {
        printf("login success\r\n");
    }
    freeReplyObject(reply);

    //3.执行set命令添加数据
    reply = (redisReply*)redisCommand(context, "set name lishifu");
    if(reply==NULL)
    {
        printf("command error\r\n");
        return -2;
    }
    if(reply->type == REDIS_REPLY_STATUS && strcasecmp(reply->str, "OK") == 0 && reply->len == 2)
    {
        printf("set success\r\n");
    }
    freeReplyObject(reply);

    //4.执行get命令获取数据
    reply = (redisReply*)redisCommand(context, "get name");
    if(reply==NULL)
    {
        printf("command error\r\n");
        return -2;
    }
    if(reply->type == REDIS_REPLY_STRING)
    {
        printf("get success, value:%s\r\n", reply->str);
    }
    printf("%ld\r\n", (long)reply->integer);
    freeReplyObject(reply);

    redisFree(context);
}
