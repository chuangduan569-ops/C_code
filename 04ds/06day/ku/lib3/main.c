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

int main(void)
{
	LLIST *handler = NULL;

	struct stu data;
	struct stu save;
	struct stu *find = NULL;
	int find_id = 102;
	int find_tel = 10013;
	char *find_name = "stu4";
	int ret = 0;
	int i = 0;
	
	handler = malloc(sizeof(struct stu));











	return 0;
}

