all:
	gcc 01_hello.c -o hello -I/usr/include/mysql/ -L/usr/lib64/mysql/ -lmysqlclient -lstdc++ -ldl -lpthread -lrt
	gcc 02_hello2.c -o hello2 -I/usr/include/mysql/ -L/usr/lib64/mysql/ -lmysqlclient -lstdc++ -ldl -lpthread -lrt
	gcc 03_tool.c -o tool -I/usr/include/mysql/ -L/usr/lib64/mysql/ -lmysqlclient -lstdc++ -ldl -lpthread -lrt
	gcc 04_multi.c -o multi -I/usr/include/mysql/ -L/usr/lib64/mysql/ -lmysqlclient -lstdc++ -ldl -lpthread -lrt
	gcc 05_stmt.c -o stmt -I/usr/include/mysql/ -L/usr/lib64/mysql/ -lmysqlclient -lstdc++ -ldl -lpthread -lrt
.PHONY:clean

clean:
	rm hello hello2 tool multi stmt