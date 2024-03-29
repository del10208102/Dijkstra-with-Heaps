#include "fib.h"
#include <iostream>

node initialnode(int vertex, int dist)//初始化节点
{
    node x = (node)malloc(sizeof(struct FibNode));
    x->vertex = vertex;
    x->dist = dist;
    x->degree = 0;
    x->left = x->right = x;
    x->parent = x->child = NULL;
    x->marked = 0;
    return x;
}

heap initialheap()//初始化堆
{
    heap H = (heap)malloc(sizeof(struct FibHeap));
    H->min = NULL;
    H->size = 0;
    H->maxdegree = 0;
    H->cons = NULL;
    return H;

}

void addnode(node x, node y);//将x插到y的前面
{
    x->left = y->left;
    y->left->right = x;
    x->right = y;
    y->left = x;
}

void insert(heap H, node x);//将新节点插入到堆中
{
    if(x == NULL)
        return;
    if(H == NULL)
        H->min = x;
    else{
        addnode(x, H->min);//将x插到H的最小值左边
        if(x->dist < H->min->dist)
            H->min = x;
    }
    H->size++;//更新堆的大小
}

heap merge(heap H1, heap H2);//合并两个堆
{
    if(H1 == NULL)
        return H2;
    if(H2 == NULL)
        return H1;

    heap temp;
    if(H1->maxdegree < H2->maxdegree){
        temp = H1;
        H1 = H2;
        H2 = temp;
    }//确保H1的最大度数大于H2

    if(H1->min == NULL){
        H1->min = H2->min;
        H1->size = H2->size;
    }//如果H1的最小值为空，则直接将H2的最小值赋给H1
    
    else if(H2->min != NULL){
        addnode(H2->min, H1->min);
        if(H2->min->dist < H1->min->dist)
            H1->min = H2->min;
        H1->size += H2->size;
    }
    free(H2);
    free(H2->cons);
    return H1;
    
}

node findmin(heap H)//找到最小值
{
    return H->min;
}

void remove(node x)//删除节点
{
    x->left->right = x->right;
    x->right->left = x->left;
}

void deletemin(heap H)//删除最小值
{
    if(H == NULL || H->min == NULL)
        return;
    node MIN = H->min;
    while(MIN->child != NULL){
        node temp = MIN->child;
        remove(temp);
        if(temp->right == temp)
            MIN->child = NULL;
        else
            MIN->child = temp->right;
        addnode(temp, H->min);
        temp->parent = NULL;
    }
    remove(MIN);
    if(MIN->right == MIN)
        H->min = NULL;
    else
        H->min = MIN->right;
    H->size--;
    adjust(H);
}

void adjust(heap H)//维护删除节点后的堆
{
    if(H == NULL || H->min == NULL)
        return;
    H->maxdegree = (int)(log2((double)(H->size))) + 1;
    H->cons =  (node*)malloc(sizeof(node) * H->maxdegree + 1);
    for(int i = 0; i <= H->maxdegree; i++)
        H->cons[i] = NULL;//初始化堆的cons数组
    while(H->min!=NULL){
        node x = H->min;
        if(x->right == x)
            H->min = NULL;
        else{
            H->min = x->right;
            remove(x);//取出x
        }
        x->left = x->right = x;//初始化

        int d = x->degree;
        while(H->cons[d] != NULL){
            node y = H->cons[d];//取出度数相同的节点
            if(x->dist > y->dist){
                node temp = x;
                x = y;
                y = temp;
            }//找出根节点中较小的那个点
            remove(y);
            if(x->child == NULL)
                x->child = y;
            else
                addnode(y, x->child);
            y ->parent = x;
            y ->marked = 0;
            x->degree++;
            H->cons[d] = NULL;
            d++;
        }
        H->cons[d] = x;//将x插入到cons数组中
    }
    H-<min = NULL;//重新构建堆
    for(int i = 0; i <= H->maxdegree; i++){
        if(H->cons[i] != NULL){
            if(H->min == NULL)
                H->min = H->cons[i];
            else{
                addnode(H->cons[i], H->min);
                if(H->cons[i]->dist < H->min->dist)
                    H->min = H->cons[i];
            }
        }
    }
}

void updateroot(heap H, node x)//更新根节点
{
    if(H == NULL || x == NULL)
        return;
    node par = x->parent;
    remove(x);//把x给提取出来
    if(x->right == x)//如果x是唯一的节点
        par->child = NULL;
    else
        par->child = x->right;
    //par->degree--;//度数减一
    addnode(x, H->min);//将x插入到堆中
    x->parent = NULL;
    x->marked = 0;
    if(par->marked == 0)//如果父节点没有被标记
        par->marked = 1;
    else
        updateroot(H, par);//递归更新
}

void decrease(heap H, node x, int k)//减小节点的值
{
    if(x == NULL || H == NULL || H->min == NULL)
        return;
    x->dist = k;//更新节点的值
    node par = x->parent;
    if(par!=NULL && x->dist < par->dist){
        updateroot(H, x);//更新根节点
    }
    if(x->dist < H->min->dist)
        H->min = x;//更新最小值

}