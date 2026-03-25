#include <stdio.h>
#include <string.h>
#include "llist.h"

#define NAMESIZE 20

struct stu
{
	int id;
	char name[NAMESIZE];
	int math;
	int tel;
};

void print(const void *data)
{
	const struct stu *p = data;//转换指针
	printf("%d	%s	%d	%d\n", p->id, p->name, p->math,p->tel);
}

int id_cmp (const void *data, const void *key)
{
	const struct stu *p = data;
	const int *k = key;
	
	return !(p->id - *k);
}

int tel_cmp (const void *data, const void *key)
{
	const struct stu *p = data;//转换数据指针
	const int *k = key;
	
	return !(p->tel - *k);
}

int name_cmp (const void *data, const void *key)
{
	const struct stu *p = data;
	const char *k = key;
	
	return !strcmp(p->name,k);
}
int main(void)
{
	LLIST *handler = NULL;
	struct stu data;
	struct stu save;
	struct stu *find = NULL;
	int find_id = 100;
	char *find_name = "stu3";
	int find_tel = 10012;
	int i = 0;
	int ret = 0;
	
	handler = llist_create(sizeof(struct stu));
	if(handler == NULL)
	{
		fprintf(stderr,"llist_create() Failed\n");
		return -1;
	}
	for(i = 0; i < 5; i++)
	{
		data.id = 100 + i;
		sprintf(data.name,"stu%d",i);
		data.math = 50 + i;
		data.tel = 10010 + i;
		llist_insert(handler, &data, HEADINSERT);
		llist_save(handler,"111.txt");
	}

		llist_display(handler,print);

	printf("\n---------------------------\n");
/*
	find = _find(handler, &find_id, id_cmp);
	if(find == NULL)
		printf("Find Failed\n");
	else
		{
			printf("Find It!\n");
			print(find);
		}	
*/
	
#if 1
	ret = llist_delete(handler, &find_tel, tel_cmp);
	if(ret != 0)
		printf("Delete Failed\n");
	else
		printf("Delete It!\n");
	llist_save(handler,"111.txt");
#else
	ret = llist_fetch(handler, find_name, name_cmp, &save);
	if(ret != 0)
		printf("Fetch Failed\n");
	else
		{
			printf("Fetch It!\n");
			print(&save);
		}
		llist_save(handler,"111.txt");
#endif	

	printf("\n---------------------------\n");

	llist_display(handler,print);
	
	llist_destory(handler);

	


	return 0;
}








