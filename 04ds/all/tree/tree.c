#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NAMESIZE 20

struct stu
{
	int id;
	char name[NAMESIZE];
	int math;
};

struct tree_node
{
	struct stu data;//数据成员地址
	struct tree_node *l;//指向左子树的指针
	struct tree_node *r;//指向右子树的指针
};

struct tree_node *tree = NULL;//tree全局指针，指向整个树的根

int tree_insert(struct tree_node **root, struct stu data)
{
	if(*root == NULL)//判断当前指针是否没有指向
	{
		*root = malloc(sizeof(struct tree_node));//开辟数据节点空间
		if(*root == NULL)
			return -1;//开辟空间失败
		(*root)->data = data;//把客户的数据存储到数据节点的数据域中
		(*root)->l = NULL;//让当前左子树指针指向NULL
		(*root)->r = NULL;
		return 0;
	}
	if((*root)->data.id < data.id)//如果当前节点的值小于要插入数据的值，递归调用插入函数(往右节点)
		tree_insert(&(*root)->r, data);
	else
		tree_insert(&(*root)->l, data);
}

void print(struct stu data)
{
	printf("%d	%s	%d\n",data.id, data.name, data.math);
}

void tree_display(struct tree_node *root)
{
	if(root == NULL)//判断当前是否为空树
		return ;
	tree_display(root->l);//递归遍历左子树
	//tree_display(root->r);//递归遍历右子树
	print(root->data);//打印根节点的数据
	tree_display(root->r);//递归遍历右子树
}

void tree_destroy(struct tree_node *root)
{
	if(root == NULL)
		return ;
	//销毁只能用后序
	tree_destroy(root->l);//递归遍历左子树
	tree_destroy(root->r);//递归遍历右子树
}


struct stu *tree_find(struct tree_node *root, int find_id)
{
	if(root == NULL)//判断空树
		return NULL;
	if(root->data.id == find_id)//判断当前root指向的节点是否为要查找的节点
		return &root->data;//返回查找的数据节点的数据域地址
	else if(root->data.id > find_id)//判断当前root指向的节点是否大于要查找的数据
		tree_find(root->l, find_id);//往树的左子树查找
	else
		tree_find(root->r, find_id);//往树的右子树查找
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

int tree_delete(struct tree_node **root, int find_id)
{
	struct tree_node **node = root;//代替root指针做操作
	struct tree_node *cur = NULL;//cur指向要删除的节点

	while(*node != NULL && (*node)->data.id != find_id)
	//如果当前节点不是叶子节点并且不是要删除的节点进入循环
	{
		if((*node)->data.id > find_id)
			node = &(*node)->l;//node指针往左走
		else
			node = &(*node)->r;//node指针往右走
	}
		if(*node == NULL)
		return -1;
	cur = *node;//cur指向要删除的节点
	if(cur->l == NULL)//判断删除的节点是否没有左子树
		*node = cur->r;//把要删除的节点的右子树往上顶
	else	
	{
		*node = cur->l;//把要删除的左子树往上顶
		find_max(cur->l)->r = cur->r;
		//把删除节点的右子树挂在   删除节点左子树(找到删除节点左子树的最大)的最右边 
	}
	free(cur);//释放要删除的节点
	return 0;

}

void _draw(struct tree_node *root, int level)
{
	if(root == NULL)//判断空树
		return ;

	_draw(root->r,level + 1);//因为倒过来是右边，所以先遍历右子树
	for(int i = 0;i < level; i++)
		printf(" ");//打印层级空格
	printf("%d\n",root->data.id);//打印id
	_draw(root->l, level + 1);//遍历左子树
}

void tree_draw(struct tree_node *root)
{
	system("clear");
	_draw(root, 0);
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
	struct tree_node *node = *root;//node代替*root指针做操作
	
	*root = node->r;//选择当前节点的右孩子作为根节点
	node->r = NULL;//断掉之前的连接
	find_min(*root)->l = node;//把断掉的节点挂在新节点的最小节点的左边
	tree_draw(tree);//画整个树

}

void turnright(struct tree_node **root)
{
	struct tree_node *node = *root;

	*root = node->l;
	node->l = NULL;
	find_max(*root)->r = node;
	tree_draw(tree);
}

void tree_balance(struct tree_node **root)
{
	int num = 0;//存储左子树节点个数和右子树节点个数
	
	if(*root == NULL)
		return ;

	while(1)
	{
		num = getnum((*root)->l) - getnum((*root)->r);//计算左子树与右子树的差值

		if(num > -1 && num < 1)//判断是否在平衡范围内
			break;
		else if(num < -1)//右边沉
		turnleft(root);//向左传
		else//左边沉
		turnright(root);//向右转
	}
	tree_balance(&(*root)->l);//递归遍历左子树
	tree_balance(&(*root)->r);//递归遍历右子树
}


int main(void)
{

	int arr[] = {1,2,3,7,6,5,4,9,8};//数据原
	struct stu data;//data变量存储客户录入的数据
	struct stu *datap = NULL;//datap指针指向找到的节点的数据域
	int find_id = 7;//存储要查找的id号
	int i = 0;
	int ret = 0;//ret变量存储函数的返回值

	for(i = 0; i < sizeof(arr) / sizeof(*arr); i++)
	{
		data.id = arr[i];
		sprintf(data.name,"stu%d",arr[i]);
		data.math = 50 + arr[i];
		tree_insert(&tree, data);//插入数据节点
	}
	
	tree_display(tree);//遍历树
	
	printf("-----------------------------\n");

	datap = tree_find(tree,find_id);//查找数据节点
	if(datap == NULL)
	printf("Not Find!\n");
	else
	{
		printf("Find It!");
		print(*datap);
	}
	ret = tree_delete(&tree,find_id);
	if(ret != 0)
	printf("Delete Failed!\n");
	else
	printf("Delete!\n");
	
	printf("-----------------------------\n");
	
	tree_display(tree);

	tree_draw(tree);//画树

	tree_balance(&tree);//画平衡二叉树



	
	tree_display(tree);
	tree_destroy(tree);//销毁树
	return 0;
}

