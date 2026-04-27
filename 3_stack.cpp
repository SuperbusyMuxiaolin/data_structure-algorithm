#include <iostream>
#include <list>

// 用list实现栈还是很简单的
template <typename T>
class list_Stack
{
private:
    std::list<T> ls;

public:
    // 向栈顶加入元素
    void push(const T &t)
    {
        ls.push_back(t);
    }

    // 从栈顶弹出元素
    T pop()
    {
        T temp_t = ls.back();
        ls.pop_back();
        return temp_t;
    }

    // 获取栈顶元素
    T get_top()
    {
        return ls.back();
    }

    // 或者栈中元素个数
    size_t get_size()
    {
        return ls.size();
    }
};

// 用数组实现就省略了  差不多的

int main()
{
    list_Stack<int> s1;
    s1.push(20);
    s1.push(30);
    s1.pop();
    std::cout << s1.pop() << std::endl;
    std::cout << s1.get_size() << std::endl;
}