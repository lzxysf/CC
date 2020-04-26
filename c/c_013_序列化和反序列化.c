/*
C语言中要对一个结构体的数据进行存储或传输，可以使用json，但是这样就需要手工生成和解析json字符串，带来很大的工作量。
更简便的办法是对结构体进行序列化，但是这个结构体内存应该是连续的，或者说这个结构体中没有指针，因为指针会指向一块新的内存，破坏结构体内存的连续性，
这样就会使结构体的序列化变得很困难，因为要手动拼接成连续的内存才能序列化，这样也就没有意义，还不如用json的方式了。
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct _User
{
    int id;
    char name[64];
    char address[128];
};
typedef struct _User User;

int main()
{
    User user;
    user.id = 1;
    strcpy(user.name, "lishifu");
    strcpy(user.address, "shijiazhaung");

    //序列化
    char *_user = malloc(sizeof(User));
    memcpy(_user, &user, sizeof(User));

    //反序列化
    User user2;
    memcpy(&user2, _user, sizeof(User));
    printf("id:%d, name:%s, address:%s\n", user2.id, user2.name, user2.address);

    return 0;
}
