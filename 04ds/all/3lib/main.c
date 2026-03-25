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
	const struct stu *p = data;
	printf("%d	%s	%d	%d\n", p->id, p->name, p->math, p->tel);
}

int id_cmp(const void *data, const void *key)
{
	const struct stu *p = data;
	const int *k = key;
	return !(p->id - *k);
}

int tel_cmp(const void *data, const void *key)
{
	const struct stu *p = data;
	const int *k = key;
	return !(p->tel - *k);
}

int name_cmp(const void *data, const void *key)
{//传过来的两个参数做比较
	const struct stu *p = data;
	const char *k = key;
	return !strcmp(p->name,k);
}

int main(void)
{
	LLIST *handler = NULL;
	struct stu data;
	struct stu save;
	struct llist_node *find = NULL;
	int find_id = 102;
	char *find_name = "stu2";
	int find_tel = 10011;
	int i = 0;
	int ret = 0;

	handler = llist_create(sizeof(struct stu));
	if(handler == 	NULL)
	{
		fprintf(stderr,"llist_create Failed!\n");
		return -1;
	}

	for(i = 0; i < 5; i++)
	{
		data.id = 100 + i;
		sprintf(data.name,"stu%d",i);
		data.math = 50 + i;
		data.tel = 10010 + i;
		llist_insert(handler, &data, HEADINSERT);
	}

	llist_display(handler, print);

	printf("---------------------------\n");
	
	find = llist_find(handler, &find_id, id_cmp);
	if(find == NULL)
	printf("Not Find!\n");
	else
	{
		printf("Find It!\n");
		print(find);
	}
	
	
	
	printf("---------------------------\n");
#if 0
	ret = llist_delete(handler, &find_tel, tel_cmp);
	if(ret != 0)
	printf("Delete Failed!\n");
	else
	printf("Delete!\n");
#else
	ret = llist_fetch(handler,find_name,name_cmp,&save);
	if(ret != 0)
	printf("Fetch Failed!\n");
	else
	{
		printf("Fetch It!\n");
		print(&save);
	}
#endif
	printf("---------------------------\n");


	llist_display(handler, print);
	llist_destroy(handler);


	

	return 0;

}





