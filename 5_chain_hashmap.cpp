#include <iostream>
#include <vector>
#include <list>
#include <optional>
#include <memory>
#include <string>

// 这是一个简化实现，hash计算取模，然后key和value只能是int
class simpleChainHashMap
{
    struct Node
    {
        int key;
        int value;
        Node(int k, int v) : key(k), value(v) {}
    };
    std::vector<std::list<Node>> table;
    int hash(int key)
    {
        return key % table.size();
    }

public:
    simpleChainHashMap(int size) : table(size) {}

    int get(int key)
    {
        int index = hash(key);
        std::list<Node> &chain = table[index];
        if (chain.empty())
            return -1; // 没有找到
        for (const Node &node : chain)
        {
            if (node.key == key)
                return node.value;
        }
        return -1; // 没有找到
    }

    void put(int key, int value)
    {
        int index = hash(key);
        std::list<Node> &chain = table[index];
        for (Node &node : chain)
        {
            if (node.key == key)
            {
                node.value = value;
                return;
            }
        }
        chain.push_back(Node(key, value));
    }

    void remove(int key)
    {
        int index = hash(key);
        std::list<Node> &chain = table[index];
        if (chain.empty())
        {
            return;
        }
        for (std::list<Node>::iterator it = chain.begin(); it != chain.end(); ++it)
        {
            if (it->key == key)
            {
                chain.erase(it);
                return;
            }
        }

        // 也可以用remove_if和lambda表达式来实现
        // chain.remove_if([key](const Node &node){return node.key ==key;})
    }
};

template <typename K, typename V>
class ChainHashMap
{
    struct Node
    {
        K key;
        V value;
        Node(K k, V v) : key(k), value(v) {}
    };
    std::vector<std::list<Node>> table;
    int total_count; // 存入键值对个数
    static constexpr int INIT_SIZE = 5;

    int hash(const K &key)
    {
        return std::hash<K>()(key) % table.size();
    }

    void resize(int new_size)
    {
        ChainHashMap<K, V> new_map(new_size);
        for (const auto &chain : table)
        {
            for (const auto &node : chain)
            {
                new_map.put(node.key, node.value);
            }
        }
        this->table = new_map.table;
    }

public:
    ChainHashMap() : ChainHashMap(INIT_SIZE)
    {
    }
    explicit ChainHashMap(int size)
    {
        total_count = 0;
        size = std::max(size, INIT_SIZE);
        table.resize(size);
    }
    // 查
    // 返回 std::optional<V>，如果没有找到返回 std::nullopt
    // 返回的值可以通过 .value() 来获取实际的值，也可以用 .has_value() 来检查是否有值
    std::optional<V> get(const K &key)
    {
        int index = hash(key);
        const auto &chain = table[index];
        if (chain.empty())
        {
            return std::nullopt; // 没有找到
        }
        for (const auto &node : chain)
        {
            if (node.key == key)
            {
                return node.value;
            }
        }
        return std::nullopt;
    }

    // 增
    void put(K k, V v)
    {
        int index = hash(k);
        for (auto &node : table[index])
        {
            if (node.key == k)
            {
                node.value = v;
                return;
            }
        }
        table[index].push_back(Node(k, v));
        total_count++;

        if (total_count >= table.size() * 0.75)
        {
            resize(table.size() * 2);
        }
    }

    void remove(K key)
    {
        int index = hash(key);
        auto &chain = table[index];
        for (auto it = chain.begin(); it != chain.end(); it++)
        {
            if (it->key == key)
            {
                chain.erase(it);
                total_count--;
                if (total_count <= table.size() / 8)
                {
                    resize(table.size() / 4);
                }
                return;
            }
        }
        std::cout << "没有找到要删除的key" << std::endl;
    }

    // 返回所有 key
    std::list<K> keys()
    {
        std::list<K> keys;
        for (const auto &list : table)
        {
            for (const auto &node : list)
            {
                keys.push_back(node.key);
            }
        }
        return keys;
    }

    // **** 其他工具函数 ****

    int count() const
    {
        return total_count;
    }
};

int main()
{
    //
    ChainHashMap<int, std::shared_ptr<int>> map;
    map.put(1, std::make_shared<int>(1));
    map.put(2, std::make_shared<int>(2));
    map.put(3, std::make_shared<int>(3));
    std::cout << *map.get(1).value() << std::endl; // 1
    std::cout << *map.get(2).value() << std::endl; // 2

    map.put(1, std::make_shared<int>(100));
    std::cout << *map.get(1).value() << std::endl; // 100

    map.remove(2);
    std::cout << (map.get(2) == std::nullopt ? "null" : std::to_string(*map.get(2).value())) << std::endl; // null

    for (const auto key : map.keys())
    {
        std::cout << key << " ";
    }
    // [1, 3]（顺序可能不同）
    std::cout << std::endl;

    map.remove(1);
    map.remove(2);
    map.remove(3);
    std::cout << (map.get(1) == std::nullopt ? "null" : std::to_string(*map.get(1).value())) << std::endl; // null

    auto ret = map.get(42);
    if (ret.has_value())
    {
        std::cout << "找到: " << ret.value() << "\n";
    }
    else
    {
        std::cout << "没找到\n";
    }
}