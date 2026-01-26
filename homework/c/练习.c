#include<stdio.h>
char *mystrcpy(char *dest, const char *src){
	int i = 0;
	for(i = 0;dest[i] != '\0'; i++)
		dest[i] = src[i];//拷贝有效的字符
	dest[i] = src[i];//			\0
	return dest;
}
char *mystrcat(char *s1, const char *s2){
	int i = 0, j = 0;
	for(i = 0;s1[i] != '\0'; i++);//遍历第一个数组到最后
		for(j=0;s2[j] != '\0';j++)
			s1[i+j] = s2[j];//连接所有有效字符
		s1[i+j] = s2[j];
	return s1;
}
int mystrcmp(const char *s1, const char *s2){
	int i = 0;
	for(i=0;s1[i]==s2[i]&&s1[i]!='\0';i++);
	return s1[i] - s2[i];
}
size_t mystrlen(const char *s){
	size_t i = 0;
	for(i=0;s[i]!='\0';i++);
	return i;
}

int main(void)
{
	char dest[10] = "Hello";
    char *src = "World";

    printf("strcpy = %s\n", mystrcpy(dest, src));
    printf("strcat = %s\n", mystrcat(dest, src));
    printf("strcmp = %d\n", mystrcmp(dest, src));
    printf("strlen = %ld\n", mystrlen(dest));


	return 0;
}

