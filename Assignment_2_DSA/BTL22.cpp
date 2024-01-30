
#include <iostream>
#include<string>
#include <fstream>
#include <iostream>
#include<sstream>
#include <string>
#include <math.h>
#include<map>
#include<queue>
#include<algorithm>
#include<vector>
#include<utility>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
using namespace std;


#include <iostream>
#include<string>
#include <fstream>
#include <iostream>
#include<sstream>
#include <string>
#include <math.h>
#include<map>
#include<queue>
#include<algorithm>
#include<vector>
#include<utility>
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#include <stdlib.h>
using namespace std;

 

static int MAXSIZE;
static int time_global;
class BTL2
{
public:

    /* #region  XỬ LÝ HUFFMAN */
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////            XỬ LÝ HUFFMAN
    struct NodeHuffman
    {
    public:
        char ch;
        int freq;
        NodeHuffman* left;
        NodeHuffman* right;
        int nth;

        NodeHuffman(char ch, int freq, int nth)
        {
            this->ch = ch;
            this->freq = freq;
            this->left = NULL;
            this->right = NULL;
            this->nth = nth;
        }
        NodeHuffman(NodeHuffman*& left, NodeHuffman*& right, int nth)
        {
            this->ch = '$';
            this->freq = left->freq + right->freq;
            this->left = left;
            this->right = right;
            this->nth = nth;
        }

        ~NodeHuffman()
        {
        
        }
    };

    static void remove_all_huffmannode(NodeHuffman*& root)
    {
        vector<NodeHuffman*> vt;
        allnode_Huffman(root, vt);
        while (!vt.empty())
        {
            int size = vt.size() - 1;
            NodeHuffman* temp = vt[size];
            delete temp;
            vt.pop_back();
            
        }
    }
    static  void allnode_Huffman(NodeHuffman*& root, vector<NodeHuffman*>& vt)
    {
        if (root != NULL)
        {
            allnode_Huffman(root->left, vt);
            allnode_Huffman(root->right, vt);
            vt.push_back(root);
        }
    }


    int height(NodeHuffman*& root)
    {
        if (root == NULL)
            return 0;
        if ((root->left == NULL) && (root->right == NULL))
            return 1;
        return 1 + max(height(root->left), height(root->right));
    }
    // LL
    NodeHuffman* rightRotate(NodeHuffman*& node)
    {
        NodeHuffman* x = node->left;
        node->left = x->right;
        x->right = node;
        return x;
    }
    // RR
    NodeHuffman* leftRotate(NodeHuffman*& node)
    {
        NodeHuffman* x = node->right;
        node->right = x->left;
        x->left = node;
        return x;
    }
    // RL
    NodeHuffman* R_L_Rotate(NodeHuffman*& node)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    } // LR
    // LR
    NodeHuffman* L_R_Rotate(NodeHuffman*& node)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    bool check_balance(NodeHuffman*& root, int& count)
    {
        if (root == NULL)
            return false;
        if ((root->left == NULL) && (root->right == NULL))
            return false;
        int l = height(root->left);
        int r = height(root->right);
        if ((l - r >= -1) && (l - r <= 1))
            return false; // ko xoay
        // xu ly xoay, thay doi count
        count--;
        if (l - r > 1) // left cao hon
        {
            if (height(root->left->left) >= height(root->left->right)) // LL
            {                                                          // xu ly LL
                root = rightRotate(root);
            }
            else
            { // xu ly LR

                root = L_R_Rotate(root);
            }
        }
        else // right cao hon
        {
            if (height(root->right->right) >= height(root->right->left)) // RR
            {                                                            // xu ly RR
                root = leftRotate(root);
            }
            else
            { // xu ly RL
                root = R_L_Rotate(root);
            }
        }
        return true;
    }

    ///////////////////////////////                 MOST IMPORTANCE FUNCTION!!!!!!!!!!!!!!!!!         MOST IMPORTANCE FUNCTION         !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    //////////////////////////////
    void balance(NodeHuffman*& root, NodeHuffman*& node, int& count)
    {
        if ((node != NULL) && (count > 0))
        {
            if (check_balance(node, count))
            {
                balance(root, root, count);
            } // neu true => da xoay => goi lai balance  || neu false: chua xoay => kiem tra tiep ben trai, ben phai
            else
            {
                if (node->left != NULL)
                    balance(root, node->left, count);
                if (node->right != NULL)
                    balance(root, node->right, count);
            }
        }
    }

    struct comp_huffman
    {
        bool operator()(const NodeHuffman* lhs, const NodeHuffman* rhs) const
        {
            // So sánh theo tần suất tăng dần
            if (lhs->freq != rhs->freq)
            {
                return lhs->freq > rhs->freq;
            }
            // Nếu tần suất bằng nhau, so sánh theo chữ cái tăng dần
            else if (lhs->ch == rhs->ch)
                return lhs->nth > rhs->nth;
            else if (lhs->ch == '$')
                return true;
            else if (rhs->ch == '$')
                return false;
            else
            {
                if (((lhs->ch >= 'a') && (rhs->ch >= 'a')) || ((lhs->ch <= 'Z') && (rhs->ch <= 'Z')))
                {
                    return lhs->ch > rhs->ch;
                }
                else if (lhs->ch < 'a')
                    return true;
                else
                    return false;
            }
        }
    };

    void printTreeDiagram(NodeHuffman*& root, int space = 0, int vSpacing = 5)
    {
        if (root == nullptr)
        {
            return;
        }

        // Tăng khoảng trắng giữa các dòng khi đi xuống cây
        space += vSpacing;

        // In nút bên phải trước
        printTreeDiagram(root->right, space);

        // In thông tin của nút hiện tại
        std::cout << std::endl;
        for (int i = vSpacing; i < space; i++)
        {
            std::cout << " ";
        }
        std::cout << root->ch << " - " << root->freq << std::endl;

        // In nút bên trái
        printTreeDiagram(root->left, space);
    }

    NodeHuffman* build_HUFFMAN(string& str)
    {
        map<char, int> freq;
        for (char ch : str)
            freq[ch]++;
        // kiem tra 3 ky tu khac nhau
        if (int(freq.size()) < 3)
            return NULL;

        map<char, int> freq2;
        map<char, char> ceasar;
        for (auto it = freq.begin(); it != freq.end(); it++)
        {
            if (it->first >= 'a' && it->first <= 'z')
            {
                char chr = (it->first - 'a' + it->second) % 26 + 'a';
                freq2[chr] += it->second;
                ceasar[it->first] = chr;
            }
            else
            {
                char chr = (it->first + it->second - 'A') % 26 + 'A';
                freq2[chr] += it->second;
                ceasar[it->first] = chr;
            }
        }

        // make new name by ceasar:
        string newname = "";
        for (char i : str)
        {
            newname += ceasar[i];
        }
        str = newname;

        priority_queue<NodeHuffman*, vector<NodeHuffman*>, comp_huffman> pq;
        for (auto i : freq2)
        {
            NodeHuffman* newnode = new NodeHuffman(i.first, i.second, 0);
            pq.push(newnode);
        }
        int nth = 1;
        // bool flag = false;
        while (pq.size() > 1)
        {
            NodeHuffman* left = pq.top();
            pq.pop();
            NodeHuffman* right = pq.top();
            pq.pop();
            NodeHuffman* newnode = new NodeHuffman(left, right, nth);
            int count = 3;
            balance(newnode, newnode, count);
            newnode->nth = nth;
            nth++;
            pq.push(newnode);
        }

        NodeHuffman* root = pq.top();
        pq.pop();
        return root;
    }

    ///////////////////////////////////////////////////// the resst off HUFFMAN

    void buildHuffmanCode(NodeHuffman*& root, string code, map<char, string>& huffmanCode)
    {
        if (root == nullptr)
            return;
        if (root->ch != '$')
            huffmanCode[root->ch] = code;
        buildHuffmanCode(root->left, code + "0", huffmanCode);
        buildHuffmanCode(root->right, code + "1", huffmanCode);
    }

    string encodeHuffman(string name, map<char, string>& huffmanCode)
    {
        string binary = "";
        int size = (int)huffmanCode.size();
        if (size == 1)
            return "0"; // cay HUFFMAN CHI CO 1 NODE => ((chr, freg),"")
        for (char ch : name)
        {
            binary += huffmanCode[ch];
        }
        return binary;
    }

    string trim_10bits(string binary)
    {
        if (binary.size() > 10)
        {
            return binary.substr(binary.size() - 10);
        }
        return binary;
    }

    int binaryToDecimal(string binary)
    {
        int decimal = 0;
        int size = (int)binary.size();
        // reverse(binary.begin(), binary.end());
        for (int i = 0; i < size; i++)
        {
            if (binary[i] == '1')
            {
                decimal = decimal + pow(2, i);
            }
        }
        return decimal;
    }

    int calculate(NodeHuffman*& name_huff, string name_ceasar)
    {
        map<char, string> huffmanCode;
        string code = "";
        buildHuffmanCode(name_huff, code, huffmanCode);
        string name_encode = encodeHuffman(name_ceasar, huffmanCode);
        string binary = trim_10bits(name_encode);
        return binaryToDecimal(binary);
    }

    /* #endregion */ ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /* #region CLASS HEAP, BST, CUSTOMER */

    class customer
    {
    public:
        string name;
        NodeHuffman* name_huff;
        int time;
        int result;
        int id;
        customer() : name(""), name_huff(NULL), time(0) {};
        customer(string name, NodeHuffman*& name_huff, int result)
        {
            this->time = time_global;
            this->name = name;
            this->name_huff = name_huff;
            this->result = result;
            this->id = result % MAXSIZE + 1;
        }
        ~customer()
        {
            this->name = "";
            remove_all_huffmannode(name_huff);
            this->result = -1;
            this->time = -1;
        }
    };

    struct BSTNode
    {
        customer* cus;
        BSTNode* left;
        BSTNode* right;
        BSTNode()
        {
            this->cus = NULL;
            this->left = NULL;
            this->right = NULL;
        }
        BSTNode(customer*& cus)
        {
            this->cus = cus;
            this->left = NULL;
            this->right = NULL;
        }
        ~BSTNode()
        {
                delete cus;
                cus = NULL;

        }
    };

    class BSTree
    {
    public:
        BSTNode* root;
        int soluong;

        BSTree()
        {
            this->soluong = 0;
        }
        ~BSTree()
        {
            vector<BSTNode*> vt;
            post_order_push(this->root, vt);
            while ((int)vt.size() > 0)
            {
                BSTNode* temp = vt[int(vt.size() - 1)];
                vt.pop_back();
                delete temp;
            }


        }

        void post_order_push(BSTNode*& root, vector<BSTNode*>& vt)
        {
            if (root != NULL)
            {
                post_order_push(root->left, vt);
                post_order_push(root->right, vt);
                vt.push_back(root);
            }
        }
        // Function to make Fact arr
        void insert(customer*& cus)
        {
            BSTNode* newnode = new BSTNode(cus);
            re_insert(this->root, newnode);
            this->soluong++;
        }
        void re_insert(BSTNode*& root, BSTNode*& newnode)
        {
            if (root == NULL)
                root = newnode;
            else
            {
                if (root->cus->result > newnode->cus->result)
                    re_insert(root->left, newnode);
                else
                    re_insert(root->right, newnode);
            }
        }
        void remove(BSTNode*& node)
        {
            if (node != NULL)
            {
                re_delete(this->root, node);
                this->soluong--;
            }
        }
        void re_delete(BSTNode*& root, BSTNode*& node)
        {
            if (root == NULL)
                return;
            if ((root->cus->result == node->cus->result) && (root->cus->time == node->cus->time)) // phai dung result va dung time
            {
                if ((root->left == NULL) && (root->right == NULL))
                {
                    delete root;
                    root = NULL;
                }
                else if (root->right == NULL)
                {
                    BSTNode* temp = root;
                    root = root->left;
                    delete temp;
                }
                else // lay min ben phai
                {                                           
                    BSTNode* temp = root->right;
                    while (temp->left != NULL)
                    {
                        temp = temp->left;
                    }
                    // SWAP 
                    customer* target_cus = temp->cus;
                    temp->cus = root->cus;
                    root->cus = target_cus;
                    re_delete(root->right, temp);
                }
            }
            else if (root->cus->result < node->cus->result)
                re_delete(root->right, node);
            else
                re_delete(root->left, node);
        }
    };

    class HeapNode
    {
    public:
        friend class Heap;

        struct comp_NodeS_unFIFO {
            bool operator()(const customer* a, const customer* b) const {
                return a->time > b->time;
            }
        };

        struct comp_NodeS_FIFO {
            bool operator()(const customer* a, const customer* b) const {
                return a->time < b->time;
            }
        };

        struct comp_NodeS_LIFO {
            bool operator()(const customer* a, const customer* b) const {
                return a->time > b->time;
            }
        };
        vector<customer*> khuvuc_S;

        int id;
        int soluong;
        int time_newchange;
        HeapNode()
        {
            this->soluong = 0;
            this->id = -1;
            this->time_newchange = -1;
        }
        HeapNode(customer*& newcus)
        {
            this->soluong = 1;
            this->id = newcus->id;
            this->time_newchange = time_global;
            this->khuvuc_S.push_back(newcus);
        }
        ~HeapNode()
        {
            while (this->soluong > 0)
            {
                customer* temp = this->khuvuc_S[this->soluong - 1];
                if (temp != NULL)    delete temp;
                temp = NULL;
                this->khuvuc_S.pop_back();
                this->soluong--;
            }

        }
        // them khach vao HeapNode có id = id
        void insert(customer*& newcustomer)
        {
            this->khuvuc_S.push_back(newcustomer);
            this->soluong++;
            this->time_newchange = time_global;
        }
        // lenh KEITEIKEN
        void remove(int number)
        {
            if (number > this->soluong)     number = this->soluong;
            sort(khuvuc_S.begin(), khuvuc_S.end(), comp_NodeS_unFIFO()); // sap xep giam dan => pop_back
            while (number > 0)
            {
                int id = (int)khuvuc_S.size() - 1;
                customer* temp = khuvuc_S[id];
                khuvuc_S.pop_back();
                // in khach theo lenh KEITEIKEN
                cout << temp->result << temp->id << '\n';
                delete temp;
                number--;
            }
            this->time_newchange = time_global;
        }
        void remove_KEITEIKEN(int number)
        {
            if (number > this->soluong)     number = this->soluong;
            sort(khuvuc_S.begin(), khuvuc_S.end(), comp_NodeS_unFIFO()); // sap xep giam dan => pop_back
            while (number > 0)
            {
                int id = khuvuc_S.size() - 1;
                customer* temp = khuvuc_S[id];
                khuvuc_S.pop_back();
                cout << temp->result << "-" << temp->id << '\n';
                delete temp;
                this->soluong--;
                number--;
            }
            this->time_newchange = time_global;

        }
        void print_cleave(int NUM)
        {
            if (NUM > this->soluong) NUM = this->soluong;
            sort(khuvuc_S.begin(), khuvuc_S.end(), comp_NodeS_LIFO()); // sap xep giảm dần time => LIFO
            for (int i = 0; i < NUM; i++)
            {
                cout << this->id << '-' << khuvuc_S[i]->result << '\n';
            }
        }
    };

    class Heap
    {
    public:
        vector<HeapNode*> elements;
        int count;

        struct comp_HeapS
        {
            bool operator()(HeapNode*& a, HeapNode*& b)
            {
                if (a->soluong == b->soluong)
                    return a->time_newchange < b->time_newchange;
                else
                    return (a->soluong < b->soluong);
            }
        };

        struct comp_NodeS
        {
            bool operator()(customer* a, customer* b)
            {
                return a->time < b->time;
            }
        };

        Heap()
        {
            vector<HeapNode*> elements;
            this->count = 0;
        }

        ~Heap() //////////////////////// can thay doi khong, kiem tra ~HeapNode
        {
            while (this->count > 0)
            {
                HeapNode* temp = elements[(int)elements.size() - 1];
                //elements.pop_back();
                //delete temp;
                remove_HeapNode(temp);
                /*count--;*/
            }

        }

        bool isEmpty()
        {
            return (this->count == 0);
        }

        HeapNode* get_HeapNode(int id)
        {
            if (this->isEmpty())
                return NULL;
            for (int i = 0; i < this->count; ++i)
            {
                if (this->elements[i]->id == id)
                    return this->elements[i];
            }
            return NULL;
        }

        HeapNode* peek()
        {
            if (this->count > 0)
                return this->elements[0];
            else return NULL;
        }

        void pop()
        {
            if (this->isEmpty())
                return;
            this->elements[0] = this->elements[this->count - 1];
            --this->count;
            this->reheapDown(0);
            delete this->elements[count - 1];
            return;
        }

        void remove_HeapNode(HeapNode*& node)
        {
            int id = this->contain(node->id);
            if (id < 0) return;
            HeapNode* temp = elements[id];
            elements[id] = elements[this->count - 1];
            this->elements.pop_back();
            this->count--;
            this->reheapDown(id);
            delete temp;
            temp = NULL;
        }

        void reheapUp(int position)
        {
            while (position >= 0)
            {
                int parent = (position - 1) / 2;
                if ((elements[position]->soluong < elements[parent]->soluong) ||
                    ((elements[position]->soluong == elements[parent]->soluong) && (elements[position]->time_newchange < elements[parent]->time_newchange)))
                {
                    HeapNode* temp = elements[parent];
                    elements[parent] = elements[position];
                    elements[position] = temp;
                    position = parent;
                }
                else
                    position = -1;
            }
        }
        void reheapDown(int index)
        {
            int last_index = this->count - 1;
            int left = index * 2 + 1;
            int right = index * 2 + 2;

            while (left <= last_index)
            {
                int min_er = left;
                if (right <= last_index)
                {
                    if ((elements[right]->soluong < elements[left]->soluong) ||
                        ((elements[right]->soluong == elements[left]->soluong) && (elements[right]->time_newchange < elements[left]->time_newchange)))
                        min_er = right;
                }
                if ((elements[min_er]->soluong < elements[index]->soluong) ||
                    ((elements[min_er]->soluong == elements[index]->soluong) && (elements[min_er]->time_newchange < elements[index]->time_newchange)))
                {
                    HeapNode* temp = elements[min_er];
                    elements[min_er] = elements[index];
                    elements[index] = temp;
                }
                index = min_er;
                left = index * 2 + 1;
                right = index * 2 + 2;
            }
        }
        int contain(int id) // co HeapNode cho khu vuc ID chua
        {
            for (int i = 0; i < count; i++)
            {
                if (elements[i]->id == id)
                    return i;
            }
            return -1;
        }
        void push_newcus(customer*& newcus) // them khach vao nha_S
        {
            int id = newcus->id;
            // tim kiem id co ton tai trong day heap hay khong
            int place = this->contain(id);
            if (place >= 0)
            {
                elements[place]->insert(newcus);
                this->reheapDown(place);
            }
            else
            {
                HeapNode* newnode = new HeapNode(newcus);
                this->push_Heapnode(newnode);
            }
        }
        void push_Heapnode(HeapNode*& newnode)
        {
            elements.push_back(newnode);
            reheapUp(count);
            this->count++;
            /////////////// kiem tra lai co can reheap down khong
            reheapDown(0);
        }
    };

    /* #endregion */ ////////////////////////////////////////////////////////////////// NHÀ S VÀ NHÀ G

////////////////////////////////////////////////////////////////////////////////////           CÁC BIẾN TOÀN CỤC VÀ CLASS MAIN

    vector<BSTree*> NHA_G;
    Heap* NHA_S;
    stringstream the_last;

    BTL2()
    {
        this->the_last.str("");
        this->NHA_S = new Heap();
        for (int i = 0; i < MAXSIZE; i++)
        {
            BSTree* newnode = new BSTree();
            this->NHA_G.push_back(newnode);
        }
    }
    void getMAXSIZE(int maxsize)
    {
        MAXSIZE = maxsize;
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////////            XỬ LÝ CÁC LỆNH CỦA CÂU

    //////////////////////////////////////////////////////////////////// LAPSE
    void LAPSE(string name)
    {
        NodeHuffman* name_huff = build_HUFFMAN(name);
        if (name_huff != NULL)
        {
            int result = calculate(name_huff, name);
            int id = result % MAXSIZE + 1;
            customer* new_customer = new customer(name, name_huff, result);
            this->the_last.str("");
            this->action_HAND(name_huff, the_last);
            //// BỐ TRÍ CUSTOMER
            if (result % 2 == 1) // nhà G
            {
                // insert khác hàng mới vào AVL TREE
                NHA_G[id - 1]->insert(new_customer);
            }
            else // nhà S
            {
                if (NHA_S->contain(id) >= 0) // nếu đã có nhà S khu vực id => push_customer để thêm KH vào danh sách và number cho HeapNode
                {
                    NHA_S->push_newcus(new_customer);
                }
                else // chưa tồn tại khu vực id => tạo mới eHeapNode và và push vào HEAP
                {
                    HeapNode* place_Heap = new HeapNode(new_customer);
                    NHA_S->push_Heapnode(place_Heap);
                    //
                }
            }
            time_global++;
        }
    }

    //////////////////////////////////////////////////////////////////// KOKUSEN
    static void pre_order_push(BSTNode*& root, vector<BSTNode*>& vt, vector<int>& vt2)
    {
        if (root != NULL)
        {
            vt.push_back(root);
            vt2.push_back(root->cus->result);
            pre_order_push(root->left, vt, vt2);
            pre_order_push(root->right, vt, vt2);
        }
    }

    void calculateFact(int*& fact, int N)
    {
        fact[0] = 1;
        for (long long int i = 1; i < N; i++) fact[i] = fact[i - 1] * i;
    }

    int nCr(int fact[], int N, int R)
    {
        if (R > N) return 0;
        int res = fact[N] / fact[R];
        res /= fact[N - R];
        return res;
    }

    int countWays(vector<int>& arr, int*& fact)
    {
        int N = arr.size();
        if (N <= 2)   return 1;
        vector<int> leftSubTree;
        vector<int> rightSubTree;
        int root = arr[0];

        for (int i = 1; i < N; i++) {
            if (arr[i] < root)  leftSubTree.push_back(arr[i]);
            else  rightSubTree.push_back(arr[i]);
        }
        int N1 = leftSubTree.size();
        //int N2 = rightSubTree.size();
        int countLeft = countWays(leftSubTree, fact) % MAXSIZE;
        int countRight = countWays(rightSubTree, fact) % MAXSIZE;
        return nCr(fact, N - 1, N1) * countLeft * countRight % MAXSIZE;
    }


    struct compare_KOKUSEN_FIFO {    // FIFO
        bool operator()(BSTNode*& a, BSTNode*& b) const {
            return a->cus->time < b->cus->time;
        }
    };
    struct compare_KOKUSEN_unFIFO {    // unFIFO
        bool operator()(BSTNode*& a, BSTNode*& b) const {
            return a->cus->time > b->cus->time;
        }
    };

    void KOKUSEN() // NHA_G đuổi khách
    {
        vector<vector<BSTNode*>> vt;
        for (int i = 0; i < MAXSIZE; i++)
        {
            if (NHA_G[i]->soluong>0)
            {
                int NUM = NHA_G[i]->soluong;
                // if no item => next
                if (NUM == 0)   continue;
                // step1: convert BST to arr: post_order
                vector<BSTNode*> vt;
                vector<int>vt2;
                pre_order_push(NHA_G[i]->root, vt, vt2);
                int* fact = new int[NUM];
                int Y = 0;
                if (NUM <= 2) Y = 1;
                else Y = countWays(vt2, fact) % MAXSIZE;
                // xóa Y giá trị
                sort(vt.begin(), vt.end(), compare_KOKUSEN_FIFO());
                if (Y > NUM) Y = NUM;
                for (int j=0; j< Y; j++)
                {
                    BSTNode* remove_node = vt[j];
                    NHA_G[i]->remove(remove_node);
                    Y--;
                }
                //if (Y = NUM) {
                //    NHA_G[i]->root = NULL; NHA_G[i]->soluong = 0;
                //};
                delete[] fact;
            }
        }
    }
    //////////////////////////////////////////////////////////////////// KEITEIKEN
    struct compare_keiteiken {
        bool operator()(HeapNode* a, HeapNode* b) const {
            if (a->soluong == b->soluong) return a->time_newchange < b->time_newchange;
            else
            {
                return a->soluong < b->soluong;
            }
        }
    };


    void KEITEIKEN(int NUM) // NHA_S đuổi khách
    {
        if (NHA_S->count == 0) return;
        vector<HeapNode*> vt = NHA_S->elements;
        sort(vt.begin(), vt.end(), compare_keiteiken());
        int sl = min(NUM, NHA_S->count);
        for (int i = 0; i < sl; i++)
        {
            int id = NHA_S->contain(vt[i]->id);
            HeapNode* temp = NHA_S->elements[id];
            temp->remove_KEITEIKEN(NUM);
            if (temp->soluong == 0)
            {
                NHA_S->remove_HeapNode(temp);
            }
        }

    }

    //////////////////////////////////////////////////////////////////// HAND
    void action_HAND(NodeHuffman*& root, stringstream& text) // print in order
    {
        if (root != NULL)
        {

            action_HAND(root->left, text);
            if (root->ch == '$') // in num
            {
                text << root->freq << '\n';
            }
            else
            {
                text << root->ch << '\n';
            }
            action_HAND(root->right, text);

        }
    }
    void HAND() // print huffman of the_last
    {
        if (the_last.str().length() == 0) return;
        cout << the_last.str();
    }

    void action_LIMIT(BSTNode*& root)
    {
        if (root != NULL)
        {
            action_LIMIT(root->left);
            cout << root->cus->result << '\n';
            action_LIMIT(root->right);
        }
    }
    void LIMITLESS(int NUM)
    {
        if (NUM > MAXSIZE) return;
        if (NHA_G[NUM - 1]->soluong > 0)
        {
            action_LIMIT(NHA_G[NUM - 1]->root);
        }
    }


    void action_CLEAVE(int index, int size, int NUM)
    {
        if (index < size)
        {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            NHA_S->elements[index]->print_cleave(NUM);
            action_CLEAVE(left, size, NUM);
            action_CLEAVE(right, size, NUM);
        }
    }

    void CLEAVE(int NUM)
    {
        if (NHA_S->count > 0)
        {
            action_CLEAVE(0, NHA_S->count, NUM);
        }
    }


    ~BTL2()
    {
        delete NHA_S;
        while ((int)NHA_G.size() > 0)
        {
            delete NHA_G[(int)NHA_G.size() - 1];
            NHA_G.pop_back();
        }
    }


};

void simulate(string filename)
{
    BTL2* r = NULL;
    ifstream ss(filename);
    string str, num, maxsize, name;
    while (ss >> str)
    {
        if (str == "MAXSIZE")
        {
            ss >> maxsize;
            int size = stoi(maxsize);
            MAXSIZE = size;
            r = new BTL2();
        }
        else if (str == "LAPSE") // LAPSE - huffman, customer
        {
            ss >> name;
            r->LAPSE(name);
        }
        else if (str == "KEITEIKEN") // KEITEIKEN -- NHÀ S ĐUỔI KHÁCH THEO KHU VỰC VÀ FIFO
        {
            ss >> num;
            r->KEITEIKEN(stoi(num));
        }
        else if (str == "KOKUSEN") // KOKUSEN -- NHÀ G ĐUỔI KHÁCH (BIẾN ĐỔI BST VÀ XÓA KHÁCH)
        {
            r->KOKUSEN();
        }
        else if (str == "HAND") // HAND -- IN KHACH GAN NHAT
        {
            r->HAND();
        }
        else if (str == "LIMITLESS") // LIMITLESS -- IN CÂY BST TẠI KHU VỰC NUM CỦA NHÀ G THEO IN-ORDER
        {
            ss >> num;
            r->LIMITLESS(stoi(num));
        }
        else if (str == "CLEAVE") // CLEAVE -- IN NUM KHÁCH HÀNG THEO FIFO THEO PRE-ORDER TRONG MINHEAP
        {
            ss >> num;
            r->CLEAVE(stoi(num));
        }
    }
    delete r;

}



int main()
{
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);


    //std::stringstream coutBuffer;
    //// Lưu lại đối tượng cout hiện tại
    //std::streambuf* originalCoutBuffer = std::cout.rdbuf();
    //// Chuyển hướng đầu ra của cout sang coutBuffer
    //std::cout.rdbuf(coutBuffer.rdbuf());

    string fileName = "test.txt";
    simulate(fileName);

    //// Đưa cout về lại trạng thái ban đầu
    //std::cout.rdbuf(originalCoutBuffer);

    //// Lấy nội dung của cout từ coutBuffer
    //std::string coutOutput = coutBuffer.str();

    //// In nội dung của cout hoặc lưu vào tệp tin
    //std::cout << "Output from simulate function:\n" << coutOutput;

    //// Lưu vào tệp tin
    //std::ofstream outFile("output.txt");
    //outFile << coutOutput;
    //outFile.close();


    return 0;
    _CrtDumpMemoryLeaks();
}




