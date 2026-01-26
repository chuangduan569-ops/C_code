#include <stdio.h>
#include "llist.h"

int main(void)
{
	LLIST *handler = NULL;
	LLIST *find = NULL;
	int find_id = 102;
	char *find_name = "stu3";
	
	LLIST data;
	LLIST save;
	int i = 0;
	int ret = 0;
	
	handler = llist_create();
	if(handler == NULL)
	{
		fprintf(stderr,"llist_create() Is Failed\n");
		return -1;
	}
	
	for(i = 0; i < 5; i++)
	{
		data.id = 100 + i;
		sprintf(data.name,"stu%d",i);
		data.math = 50 + i;
		data.next = NULL;
		llist_insert(handler,&data,HEADINSERT);
	}

	llist_display(handler);
	
	printf("-----------------------\n");
/*
	find = llist_find(handler,&find_id);
	if(find == NULL)
		printf("Not Find It\n");
	else
		printf("Find It!%d	%s	%d\n",find->id,find->name,find->math);
*/

#if 0
	ret = llist_delete(handler,&find_id);
	if(ret != 0)
		printf("Delete It Failed\n");
	else
		printf("Delete It\n");
#else
	ret = llist_fetch(handler, &find_id, &save);
	if(ret != 0)
		printf("Fetch Failed\n");
	else
		printf("Fetch It! %d	%s	 %d\n",save.id, save.name, save.math);
	
#endif
	printf("------------------------\n");
	
	llist_display(handler);

	llist_destroy(handler);


	return 0;
}

