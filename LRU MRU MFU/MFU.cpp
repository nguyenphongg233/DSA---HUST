#include <bits/stdc++.h>
using namespace std;

struct Node {
    int key;
    int cnt;
    Node *prev, *next;
    string value;

    Node(int k, string val) : key(k), cnt(0), prev(nullptr), next(nullptr), value(val) {}
};

struct RowNode {
    int freq;
    RowNode* prev;
    RowNode* next;

    struct ListNode {
        Node* data;
        ListNode* prev;
        ListNode* next;
        ListNode(Node* node) : data(node), prev(nullptr), next(nullptr) {}
    };

    ListNode* head;
    ListNode* tail;

    RowNode(int f) : freq(f), prev(nullptr), next(nullptr), head(nullptr), tail(nullptr) {}

    void push_front(Node* node) {
        ListNode* newNode = new ListNode(node);
        newNode->next = head;
        if (head) head->prev = newNode;
        head = newNode;
        if (!tail) tail = newNode;
    }

    void pop_back() {
        if (!tail) return;
        ListNode* tmp = tail;
        if (tail->prev) tail->prev->next = nullptr;
        tail = tail->prev;
        if (!tail) head = nullptr;
        delete tmp;
    }

    Node* back() {
        return tail ? tail->data : nullptr;
    }

    bool empty() {
        return head == nullptr;
    }
};

struct MFU_Cache {
    RowNode* saveHead = nullptr;
    unordered_map<int, Node*> cacheMap;
    int Max = 0;
    int sz = 0;
    int lim;
    Node *head = nullptr, *tail = nullptr;

    MFU_Cache(int LIM) : lim(LIM) {}

    // Tìm hoặc tạo hàng với tần suất f
    RowNode* getRow(int f) {
        RowNode* cur = saveHead;
        RowNode* last = nullptr;
        while (cur) {
            if (cur->freq == f) return cur;
            last = cur;
            cur = cur->next;
        }
        RowNode* newRow = new RowNode(f);
        if (!saveHead) {
            saveHead = newRow;
        } else {
            last->next = newRow;
            newRow->prev = last;
        }
        return newRow;
    }

    void removeNode(Node* node) {
        if (node->prev) node->prev->next = node->next;
        else head = node->next;

        if (node->next) node->next->prev = node->prev;
        else tail = node->prev;

        sz--;
    }

    void addNodeToHead(Node* node) {
        node->next = head;
        node->prev = nullptr;
        if (head) head->prev = node;
        head = node;
        if (!tail) tail = node;
        sz++;
    }

    void moveNodeToHead(Node* node) {
        removeNode(node);
        addNodeToHead(node);
    }

    void del_Max() {
        RowNode* cur = saveHead;
        while (cur && cur->freq != Max) cur = cur->next;
        while (cur && cur->empty()) {
            cur = cur->prev;
            Max--;
        }

        if (!cur) return;

        Node* need_del = cur->back();
        cur->pop_back();
        cacheMap.erase(need_del->key);
        removeNode(need_del);
    }

    void putCache(int key, string value) {
        auto it = cacheMap.find(key);
        if (it != cacheMap.end()) {
            Node* node = it->second;
            node->cnt++;
            Max = max(Max, node->cnt);
            RowNode* row = getRow(node->cnt);
            row->push_front(node);
            moveNodeToHead(node);
        } else {
            if (sz == lim) del_Max();
            Node* node = new Node(key, value);
            node->cnt = 1;
            Max = max(Max, 1);
            RowNode* row = getRow(1);
            row->push_front(node);
            addNodeToHead(node);
            cacheMap[key] = node;
        }
    }

    void getCache(int key) {
        auto it = cacheMap.find(key);
        if (it != cacheMap.end()) {
            Node* node = it->second;
            node->cnt++;
            Max = max(Max, node->cnt);
            RowNode* row = getRow(node->cnt);
            row->push_front(node);
            moveNodeToHead(node);
        }
    }

    void printCache() {
        Node* cur = head;
        cout << "Cache: ";
        while (cur) {
            cout << "(" << cur->key << "," << cur->value << ") ";
            cur = cur->next;
        }
        cout << '\n';
    }
};

// Giao diện tương tự như API
MFU_Cache* initCache(int lim) {
    return new MFU_Cache(lim);
}

void put(MFU_Cache* cache, int key, const string value) {
    cache->putCache(key, value);
}

void get(MFU_Cache* cache, int key) {
    cache->getCache(key);
}

void printCache(MFU_Cache* cache) {
    cache->printCache();
}

// ===================== MAIN ========================
int main() {
    int CACHE_SIZE = 3;
    MFU_Cache* cache = initCache(CACHE_SIZE);

    put(cache, 1, "Value 1");
    put(cache, 2, "Value 2");
    put(cache, 3, "Value 3");
    printCache(cache); // Cache: (3,Value 3) (2,Value 2) (1,Value 1)

    get(cache, 2); 
    printCache(cache); // Cache: (2,Value 2) (3,Value 3) (1,Value 1)

    put(cache, 4, "Value 4"); // Xoá node có freq cao nhất
    printCache(cache); // Cache: (4,Value 4) (2,Value 2) (3 hoặc 1)

    get(cache, 1); // nếu còn
    return 0;
}
