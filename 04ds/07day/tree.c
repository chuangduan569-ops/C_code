
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMESIZE 20

struct stu//数据域结构
{
	int id;
	char name[NAMESIZE];
	int math;
	
};

struct tree_node//树节点结构
{
	struct stu data;//数据域成员
	struct tree_node *l;//左子树指针
	struct tree_node *r;//右子数指针
};

struct tree_node *tree = NULL;//tree全局指针，指向整个数的树根

int tree_insert(struct tree_node **root, struct stu data)
{	
	if(*root == NULL)//判断当前指针是否没有指向
	{
		*root = malloc(sizeof(struct tree_node));//开辟数据节点的空间
		if(*root == NULL)
			return -1;
		(*root)->data = data;
		(*root)->l = (*root)->r = NULL;
		return 0;
	}
	if((*root)->data.id < data.id)//如果当前节点数据小于要插入的数据
		tree_insert(&(*root)->r,data);//往当前节点的右边插入
	else//递归操作，拿当前节点的数据与参数的数据做对比，
		//每次递归的时候都会从新开辟新的数据节点空间
		tree_insert(&(*root)->l,data);//往当前节点的左边插入
}

void print(struct stu data)
{
	printf("%d	%s	%d\n",data.id, data.name, data.math);
}

void tree_display(struct tree_node *root)
{
	if(root == NULL)//判断当前以root指向的节点为根节点,是否为空树
		return ;//由于是空树,结束函数
	tree_display(root->r);//递归遍历右子树
	tree_display(root->l);//递归遍历左子树
	print(root->data);//打印根节点
//	tree_display(root->r);//递归遍历右子树
	//前序遍历
}

void tree_destroy(struct tree_node *root)
{
	if(root == NULL)
		return ;
	tree_destroy(root->l);//递归遍历左子树
	tree_destroy(root->r);//递归遍历右子树
	printf("destroying...\n");
	free(root);//释放节点
	//销毁节点只能用后序
}

struct stu *tree_find(struct tree_node *root, int find_id)
{
	if(root == NULL)//判断当前以root指向的节点为根节点,是否为空树
		return NULL;//由于是空树,结束函数,并且返回NULL
	if(root->data.id == find_id)//判断当前root指向的节点是否为要查找的节点
		return &root->data;//返回查找到的数据节点的数据域的地址
	else if(root->data.id > find_id)//判断当前root指向的节点是否大于要查找的数据
		return tree_find(root->l, find_id);//往树的左子树查找
	else
		return tree_find(root->r, find_id);//往树的右子树查找
}

struct tree_node *find_max(struct tree_node *root)
{
	if(root == NULL)
		return NULL;
	if(root->r == NULL)
		return root;
	return find_max(root->r);
}

struct tree_node *find_min(struct tree_node *root)
{
	if(root == NULL)
		return NULL;
	if(root->l == NULL)
		return root;
	return find_min(root->l);
}

void _draw(struct tree_node *root, int level)
{
	int i = 0;
	if(root == NULL)//判断当前以root指向得头节点为根节点是否为空树
	return ;//由于当前以root指向的节点为根节点是空树,结束函数

	_draw(root->r, level +1);
	for(i = 0; i < level; i++)
		printf(" ");
	printf("%d\n",root->data.id);
	_draw(root->l, level +1);//
}

void tree_draw(struct tree_node *root)
{
	system("clear");
	_draw(root,0);
	getchar();
}

int getnum(struct tree_node *root)
{
	if(root == NULL)
	return -1;
	return 1 + getnum(root->l) + getnum(root->r);
}

void turnleft(struct tree_node **root)
{
	struct tree_node *node = *root;//node代替root指针做操作
	*root = node->r;//选择当前节点得右孩子作为根节点
	node->r = NULL;//断掉之前的连接
	find_min(*root)->l = node;//把断掉的节点挂在新的根节点的最小节点的左边
	tree_draw(tree);//画整个树
}

void turnright(struct tree_node **root)
{
	struct tree_node *node = *root;//选择当前节点得右孩子作为根节点
	
	*root = node->l;//选择当前节点的左孩子作为根节点
	node->l = NULL;//断掉之前的连接
	find_max(*root)->r = node;//把断掉的节点挂在新的根节点的最大节点的右边
	tree_draw(tree);//画整个树

}



void tree_balance(struct tree_node **root)
{
	int num = 0;//用来存储左子树的节点个数和右子树节点个数的差值
	if(*root == NULL)///判断当前以*root指向的节点为根节点,是否为空树
		return ;//由于当前以*root指向的节点为根节点是空树,结束函数
	while(1)
	{
		num = getnum((*root)->l) - getnum((*root)->r);//计算差值
		if(num>=-1 && num<=1)
			break;//判断是否在平衡范围内
		if(num<-1)//右边沉
		turnleft(root);//向左转
		else //左边沉
		turnright(root);//向右转
	}
}



int tree_delete(struct tree_node **root, int find_id)
{
	struct tree_node **node = root;//node指针代替root指针做操作
	struct tree_node *cur = NULL;//cur指针指向要删除的节点

	while(*node != NULL && (*node)->data.id != find_id)
	//如果当前节点不是叶子节点并且不是要删除的节点,进入循环
	{
		if((*node)->data.id > find_id)
			node = &(*node)->l;//node指针往左走
		else
			node = &(*node)->r;//node指针往右走
	}
	if(node == NULL)//判断是否没有找到要删除的节点
	return -1;//由于没有找到要删除的节点,结束程序,并且返回-1
	cur = *node;//判断是否没有找到要删除的节点

	if(cur->l == NULL)//判断要删除的节点是否没有左子树
		*node = cur->r;//把要删除的节点的右子树往上顶
	else
	{
		*node = cur->l;//把要删除节点的左子树往上顶

		find_max(cur->l)->r = cur->r;		
		//把删除节点的右子树挂到删除节点的左子树的最右边
		
	}

}






int main(void)
{
	int find_id = 7;
	struct stu *datap = NULL;//datap指针指向找到的节点的数据域
	int arr[] = {1,2,3,7,6,5,4,9,8};
	struct stu data;

	int i = 0;
	int ret = 0;
	for(i = 0; i < sizeof(arr) / sizeof(*arr); i++)
	{
		data.id = arr[i];
		sprintf(data.name,"stu%d",arr[i]);
		data.math = 100 - arr[i];
		tree_insert(&tree, data);
	}

	tree_display(tree);//遍历

	printf("---------------------------\n");

	datap = tree_find(tree,find_id);	
	if(datap == NULL)
		printf("Not Find It!\n");
	else
		{
			printf("Find It!\n");
			print(*datap);
		}
	ret = tree_delete(&tree,find_id);
	if(ret != 0)
	printf("Delete Failed!\n");
	else
	printf("Delete It!\n");
	
	printf("---------------------------\n");

	tree_display(tree);//遍历
	
	tree_draw(tree);

	tree_balance(&tree);
	
	tree_destroy(tree);//销毁
	printf("destroied!\n");
	return 0;
}






