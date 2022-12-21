​		

# <center>密码学编程选做1：利用Shank's算法计算32bit的离散对数</center>

### <center>作者：张浩旸 日期：2022/12/12 编程语言：C++</center>



### 1.运行截图

![image-20221216013453720](C:\Users\ASUS\AppData\Roaming\Typora\typora-user-images\image-20221216013453720.png)

经过benchmark，32bit以内的离散对数问题可以有效求解

### 2.算法原理

BSGS（Baby-Step-Giant-Step），用于求解离散对数问题，是一种时间-空间权衡算法。求解曲线上的离散对数问题：
$$
Ax=B \ \ where \ A \ and \ B \ are \ EC \ points
$$
首先对，等式左边拆分
$$
x=a\lceil\sqrt{p}\rceil-b,where \ 0\le\ a,b\le\lceil\sqrt{p}\rceil
$$
带入原问题：
$$
A(a\lceil\sqrt{p}\rceil-b)=B
$$
然后移位：
$$
A*a\lceil\sqrt{p}\rceil=B+Ab
$$
如此一来我们可以遍历左边的`A`，然后建立一张Hash表，然后遍历右边的`B`，寻找碰撞，当碰撞产生时，说明问题得到了解决

### 3.文件说明

- `BSGS.h`：算法的主要接口的声明以及实现
- `CurveParameters.h`：椭圆曲线点参数，在这里可以灵活切换曲线，曲线的表示方式为16进制的字节表示
- `miracl.h&&mirdef.h`：miracl库接口声明

### 4.接口说明

- `point`：椭圆曲线点

- `RightJustify`：大数类型`big`转换为字节类型的向右对齐标志

- ```c++
  typedef struct MyNode
  {
      MyNode* LeftChild = NULL;
      MyNode* RightChild = NULL;
      int Value;
  }Node;
  ```

  二叉线索树的结点声明，每一个节点带有一个`Value`，类型是4字节整型，两个左右孩子节点

- `miracl* Init()`：椭圆曲线运算初始化函数，返回一个miracl instance

- `void ChangeCurve(big a,big b,big p)`：**三个参数分别表示有限域上的椭圆曲线表示的三个参数**，调用此函数可以改变当前的active curve，达到灵活切换曲线的目的

- `void PrintPoint(point a)`：打印一个椭圆曲线点到stdout

- `void LoadCurveParam(big a, big b, big p,big order)`：**从`CurveParameters.h`中读取曲线参数**，通常在`ChangeCurve`函数之前调用，前三个参数读取有限域上椭圆曲线的参数，第四个参数读取该曲线的阶

- `void SetGenerator(point G)`：从`CurveParameters.h`中读取该曲线生成元，储存到`G`中

- `void InitSystem(big outA,big outB,big outP,big outOrder,point outG)`：从读取到的参数中初始化椭圆曲线运算系统，是前几个读取-初始化函数的更高一级的抽象

- `void GetXY(point A, big outX, big outY)`：从第一个参数代表的一个椭圆曲线点中取出横纵坐标，放在第二个和第三个参数中

- `[[deprecated]] void HashMapInsert(std::unordered_map<unsigned long long, unsigned int>MyMap, point MyPoint,big Value)`：**已经弃用**的哈希表构建方案，标准库中的`unordered_map`的性能一直受人诟病，因此弃用之

- **`void InsertDataIntoSearchTree(char* Data, unsigned int Value, Node* root,unsigned int ByteNumber)`**：将一个数据插入二叉搜索树中。**第一个参数是`Key`键**，用于**索引第二个参数`Value`**，**第三个参数是二叉搜索树的树根**，**最后一个参数用于决定从第一个参数中读取多长的数据作为索引**，**单位是字节**。我实现的树的索引规则是：**如果比特为0，向左；反之向右**，除了叶节点之外的节点中的`Value`成员都没有意义，**叶节点的`Value`成员是被索引的值**。

- **`bool SearchData(char* Data, Node* root,unsigned int ByteNumber,unsigned int* outValue)`**：**第一个参数是Key键**，**第二个参数是搜索树的树根**，**第三个参数是从Key键中读取的字节长度**，**最后一个参数是读取到的Value**，返回值：如果索引的值存在，返回`true`，否则返回`false`，注意：**第三个参数返回的值只有返回值为`true`才有意义，否则不会对该参数做任何处理**

- `void InitRootNode(Node* root)`：初始化输入的节点

- `void BSGS(point G, point kG, big outK)`包含BSGS算法的接口，第一个参数是生成元，第二个参数是公钥，最后一个参数是由BSGS算法解出的私钥