#include <iostream>
#include <vector>


class AVLNode
{
public:
    int key;
    AVLNode *left;
    AVLNode *right;
    int depth;
};

//получить максимум два целых числа
int max(int a, int b){
    return (a > b)? a : b;
}

//функция для получения высоты дерева
int depth(AVLNode *n)
{
    if (n == nullptr)
        return 0;
    return n->depth;
}
// выделите новый узел с переданным ключом
AVLNode* newNode(int key)
{
    AVLNode* node = new AVLNode();
    node->key = key;
    node->left = nullptr;
    node->right = nullptr;
    node->depth = 1; // новый узел добавлен как лист
    return(node);
}

void preOrder(AVLNode *node)
{
    if (node != nullptr) {
        std::cout << node->key << " "; // Посетить корень
        preOrder(node->left);      // Перейти налево
        preOrder(node->right);     // Перейти вправо
    }
}

// Инфиксный обход (In-order)
void inOrder(AVLNode *node)
{
    if (node != nullptr) {
        inOrder(node->left);       // Перейти налево
        std::cout << node->key << " ";  // Посетить корень
        inOrder(node->right);      // Перейти вправо
    }
}

// Постфиксный обход (Post-order)
void postOrder(AVLNode *node)
{
    if (node != nullptr) {
        postOrder(node->left);     // Перейти налево
        postOrder(node->right);    // Перейти вправо
        std::cout << node->key << " ";   // Посетить корень
    }
}


AVLNode* search(AVLNode* root, int key) {
    AVLNode* current = root;

    while (current != nullptr) {
        if (key == current->key) {
            return current; // Ключ найден
        } else if (key < current->key) {
            current = current->left; // Продолжаем поиск в левом поддереве
        } else {
            current = current->right; // Продолжаем поиск в правом поддереве
        }
    }

    return nullptr; // Ключ не найден
}

void searchRange(AVLNode* root, int low, int high, std::vector<AVLNode*>& result) {
    AVLNode* current = root;

    while (current != nullptr) {
        if (current->key < low) {
            current = current->right; // Ищем только в правом поддереве
        } else if (current->key > high) {
            current = current->left; // Ищем только в левом поддереве
        } else {
            // Ключ находится в диапазоне, добавляем узел в результат
            result.push_back(current);
            // Двигаемся в оба поддерева
            searchRange(current->left, low, high, result); // Ищем в левом поддереве
            searchRange(current->right, low, high, result); // Ищем в правом поддереве
            return; // Возвращаемся, чтобы избежать дальнейших ненужных проверок
        }
    }
}



// поверните вправо поддерево с корнем y
AVLNode *rightRotate(AVLNode *y)
{
    AVLNode *x = y->left;
    AVLNode *T2 = x->right;

    // Выполнить вращение
    x->right = y;
    y->left = T2;

    // Высоты обновления
    y->depth = max(depth(y->left),
                   depth(y->right)) + 1;
    x->depth = max(depth(x->left),
                   depth(x->right)) + 1;

    // Вернуть новый корень
    return x;
}

// поворот влево дочернего дерева с корнем x
AVLNode *leftRotate(AVLNode *x)
{
    AVLNode *y = x->right;
    AVLNode *T2 = y->left;

    // Выполнить вращение
    y->left = x;
    x->right = T2;
    // Высоты обновления
    x->depth = max(depth(x->left),
                   depth(x->right)) + 1;
    y->depth = max(depth(y->left),
                   depth(y->right)) + 1;

    // Вернуть новый корень
    return y;
}

// Получить коэффициент баланса узла N
int getBalance(AVLNode *N)
{
    if (N == nullptr)
        return 0;
    return depth(N->left) -
           depth(N->right);
}
//операция вставки для узла в дереве AVL
AVLNode* insert(AVLNode* node, int key)  {
    //вращение по BST
    if (node == nullptr)
        return(newNode(key));

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else // Одинаковые ключи не допускаются
        return node;

    //обновить высоту узла-предка
    node->depth = 1 + max(depth(node->left),  depth(node->right));

    int balance = getBalance(node);        //получить коэффициент баланса

    // вращать, если несбалансирован

    // Лево лево
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Право право
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);
    // Лево право
    if (balance > 1 && key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Право лево
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

// найти узел с минимальным значением
AVLNode * minValueNode(AVLNode* node)
{
    AVLNode* current = node;

    // найти крайний левый лист */
    while (current->left != nullptr)
        current = current->left;

    return current;
}
// удалить узел из дерева AVL с заданным ключом
AVLNode* deleteNode(AVLNode* root, int key)
{
    if (root == nullptr)
        return root;

    //удалить по BST
    if ( key < root->key )
        root->left = deleteNode(root->left, key);

    else if( key > root->key )
        root->right = deleteNode(root->right, key);

    else
    {
        // узел только с одним дочерним элементом или без дочернего элемента
        if( (root->left == nullptr) ||
            (root->right == nullptr) )
        {
            AVLNode *temp = root->left ?
                            root->left :
                            root->right;

            if (temp == nullptr)
            {
                temp = root;
                root = nullptr;
            }
            else
                *root = *temp;
            free(temp);
        }
        else
        {
            AVLNode* temp = minValueNode(root->right);

            root->key = temp->key;

            // Удалить преемника по порядку
            root->right = deleteNode(root->right,
                                     temp->key);
        }
    }

    if (root == nullptr)
        return root;

    // глубина обновления
    root->depth = 1 + max(depth(root->left),
                          depth(root->right));

    // получить коэффициент баланса
    int balance = getBalance(root);

    //повернуть дерево, если оно несбалансировано

    // Лево лево
    if (balance > 1 &&
        getBalance(root->left) >= 0)
        return rightRotate(root);

    // Лево право
    if (balance > 1 &&  getBalance(root->left) < 0)  {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    // Право право
    if (balance < -1 &&  getBalance(root->right) <= 0)
        return leftRotate(root);

    // Право лево
    if (balance < -1 && getBalance(root->right) > 0)   {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}



Node* splay(Node* root, int key){
    if(root == nullptr || root->key = key) return root;
    if(key < root->key){
        if(root->left == nullptr) return root;
        if(key < root->left->key){
            root->left = splay(root->left, key);
            root = rightRotate(root);
        }
        if(key > root->left->key){
            root->left->right = splay(root->left->right);
            if(root->left->right != nullptr){
                root->left = leftRotate(root->left);
            }
        }
        return (root->left == nullptr) ? root : rightRotate(root);
    }
    else{
        if(root->right == nullptr) return root;
        if(key > root->right->key){
            root->right = splay(root->right);
            root = leftRotate(root);
        }
        if(key < root->right->key){
            root->right->left = splay(root->right->left);
            root->right = rightRotate(root->right);
        }
    }
    return (root->right == nullptr) ? root : leftRotate(root);
}


