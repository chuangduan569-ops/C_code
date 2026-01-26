#脚本文件，需要使用#进行注释
#注意 ：makefile文件非常注重格式,空格就是空格,tab就是tab,不能混为一谈
#makefile文件的编译规则是倒着写的(从可执行文件- >  .c源文件)
#生成的文件 : 依赖的文件
main : main.o add.o
	gcc main.o add.o -o main
#把依赖的文件编译成可执行文件所使用的工具
#生成的文件的语句和工具前后差一个tab
main.o : main.s
	gcc -c main.s -o main.o
main.s : main.i
	gcc -S main.i -o main.s
main.i : main.c
	gcc -E main.c -o main.i
add.o : add.s
	gcc -c add.s -o add.o
add.s : add.i
	gcc -S add.i -o add.s
add.i : add.c
	gcc -E add.c -o add.i
clean :
	rm -rf *.o*.s*.i main


