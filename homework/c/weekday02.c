#include<stdio.h>
//2.输入一个字串，判断它是否是对称串。如”abcdcba”是对称串，”123456789”不是。
size_t mystrlen(const char*s)
{
	size_t i = 0;
	for(i=0;s[i]!='\0';i++);
	return i;
}
/*int mystrcmp(const char *s1,const char *s2)
{
	int i = 0;
	for(i = 0;s1[i] == s2[i] && s1[i] !='\0'; i++);//空循环
	
	return s1[i] - s2[i];

}
*/
int main(void)
{
	char arr[64] = { 0 };
	int leng = 0, left = 0 ,right = 0;
	printf("请输入你的字符串：\n");
	scanf("%s",arr);
	leng = mystrlen(arr);
	printf("%d\n",leng);//字符串长度
	right = leng - 1;
	//	用while循环遍历座到右，右到早。
	while(right>left)
	{
		if(arr[left]!=arr[right]){
				printf("no\n");
				break;}
			left++;
			right--;
			}
		if(arr[left] == arr[right])
		printf("yes\n");
		
	return 0;
}
