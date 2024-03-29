#ifndef fib_h
#define fib_h

#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>

using namespace std;

typedef struct FibNode* node;
struct FibNode {
    struct node left, right;
    struct node parent, kid;
    int marked;
    int dist; // 距离
    int vertex; // vertex id
    int degree;//度数
} FibNode;

typedef struct FibHeap* heap;
struct FibHeap {
    node min;//堆中的最小值
    int size;//堆的大小
    node *cons;//最大度的内存区域
    int maxdegree;//堆中的最大度数
};

node initialnode(int vertex, int dist);//初始化节点
heap initialheap();//初始化堆

void addnode(node x, node y);//将x插到y的前面
void insert(heap H, node x);//将新节点插入到堆中

heap merge(heap H1, heap H2);//合并两个堆
node findmin(heap H);//找到最小值
void deletemin(heap H);//删除最小值
void remove(node x);//删除节点
void adjust(heap H);
void link(node x, node y);//连接两个节点
void updateroot(heap H, node x);//更新根节点
void decrease(heap H, node x, int k);//减小节点的值


#endif // fib_h
