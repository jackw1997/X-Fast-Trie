#ifndef SPLAY_H
#define SPLAY_H

#include <iostream>

template <typename Key>
struct splay_node{
    Key         key;
    Key         max;
    Key         min;
    int         size;
    int         num;

    splay_node* left;
    splay_node* right;
    splay_node* parent;

    splay_node(Key key);
};

template <typename Key>
splay_node<Key>::splay_node(Key key_in){
    this->key = key_in;
    this->left = NULL;
    this->right = NULL;
    this->parent = NULL;
}

template <typename Key>
class splay{
    public:
        typedef splay_node<Key> Node;
        typedef splay<Key> sp;

    private:

        int size;

        void preOrder(Node* x) const;//
        void inOrder(Node* x) const;//
        void postOrder(Node* x) const;//


        Node* root;
        Node* zigzag(Node* x, Key key);//
        void  destory(Node* &x);//
        Node* find_pri(Node* x, Key key) const;//
        Node* find_par(Node* x, Key key) const;
        Node* dele(Node* &x, Key key);//
        Node* mini(Node* x);//
        Node* maxi(Node* x);//

    public:

        void preOrder() const;//
        void inOrder() const;//
        void postOrder() const;//

        splay();//
        ~splay();//
        Node* splay_root();//
        void Add_root(Node* node);
        void Add_size(int size);
        void zigzag(Key key);//
        void insert(Key key);//
        Node* find(Key key) const;//
        void destory();//
        void split(sp* &splay2);
        //In function split, we require that splay2 as an empty splay.
        void join(sp splay2);//
        void dele(Key key);//
        Node* succ(Key key) const;//
        Node* pred(Key key) const;//
        Key minimum();//
        Key maximum();//
        int size_of_splay();//
        Node* find_parent(Key key) const;//
};


template <typename Key>
splay<Key>::~splay(){
    this->destory();
}

template <typename Key>
splay<Key>::splay():root(NULL), size(0){
}

template <typename Key>
typename splay<Key>::Node*
splay<Key>::dele(Node* &x, Key key){
    Node* tmp;
    if(x == NULL){
        return x;
    }
    if(this->find_pri(x, key) == NULL){
        std::cout << "no such key, cannot delete" << std::endl;
        return x;
    }
    x = this->zigzag(this->root, key);
    Node* left = x->left;
    Node* right = x->right;
    delete x;
    this->size --;
    if(left != NULL){
        tmp = zigzag(left, maxi(left)->key);
        tmp->right = right;
    }
    else{
        tmp = right;
    }
    return tmp;
}

template <typename Key>
void splay<Key>::dele(Key key){
    this->root = this->dele(this->root, key);
}

template <typename Key>
typename splay<Key>::Node*
splay<Key>::mini(Node* x){
    if(x->left == NULL){
        return x;
    }
    else{
        return this->mini(x->left);
    }
}

template <typename Key>
Key splay<Key>::minimum(){
    return this->mini(this->root)->key;
}

template <typename Key>
typename splay<Key>::Node*
splay<Key>::maxi(Node* x){
    if(x->right == NULL){
        return x;
    }
    else{
        return this->maxi(x->right);
    }
}

template <typename Key>
Key splay<Key>::maximum(){
    return this->maxi(this->root)->key;
}

template <typename Key>
typename splay<Key>::Node*
splay<Key>::zigzag(Node* x, Key key){
    if(key < x->key){
        Node *tmp = zigzag(x->left, key);
        x->left = tmp;
    }
    else if(key > x->key){
        Node *tmp = zigzag(x->right, key);
        x->right = tmp;
    }
    Node *c;
    if(x->key == key){
        return x;
    }
    if(x->left != NULL){
        if(x->left->key == key){
            c = x->left;
            x->left = c->right;
            c->right = x;
            return c;
        }
    }
    if(x->right != NULL){
        if(x->right->key == key){
            c = x->right;
            x->right = c->left;
            c->left = x;
            return c;
        }
    }

}

template <typename Key>
void splay<Key>::zigzag(Key key){
    root = zigzag(root, key);
}

template <typename Key>
void splay<Key>::insert(Key key){
    Node  *insert_node = new Node(key);
    Node  *tmp = root;
    Node  *tmp2 = NULL;
    this->size ++;

    //std::cout << ":" << std::endl;
    //printf("%d", (tmp == NULL));
    //printf("%d", insert_node->key);

    if(tmp == NULL){
        this->root = insert_node;
        return;
    }
    int s = 0;
    printf("%d\n", tmp->key);
    while(tmp != NULL){
        tmp2 = tmp;
        printf("%d %d\n", key, tmp->key);
        if(insert_node->key < tmp->key){
            if(tmp->left == NULL){
                break;
            }
            else{
                tmp = tmp->left;
            }
        }
        else if(insert_node->key > tmp->key){
            if(tmp->right == NULL){
                break;
            }
            else{
                tmp = tmp->right;
            }
        }
        else{
            printf("gly2\n");
            delete insert_node;
            this->root = zigzag(this->root, key);\
            this->size--;
            return;
        }
    }
    if(tmp2 == NULL){
        this->root = insert_node;
    }
    else if(insert_node->key < tmp2->key){
        tmp2->left = insert_node;
    }
    else{
        tmp2->right = insert_node;
    }
    this->root = zigzag(this->root, key);
}

template <typename Key>
void splay<Key>::destory(){
    this->destory(this->root);
}

template <typename Key>
void splay<Key>::destory(Node* &x){
    if(x == NULL){
        return;
    }

    if(x->left != NULL){
        destory(x->left);
    }
    if(x->right != NULL){
        destory(x->right);
    }
    delete x;
}

template <typename Key>
typename splay<Key>::Node*
splay<Key>::find_pri(Node* x, Key key) const{
    if(x == NULL || x->key == key){
        return x;
    }
    if(key < x->key){
        return this->find_pri(x->left, key);
    }
    else{
        return this->find_pri(x->right, key);
    }
}

template <typename Key>
typename splay<Key>::Node*
splay<Key>::find_parent(Key key) const{
    return this->find_par(this->root, key);
}

template <typename Key>
typename splay<Key>::Node*
splay<Key>::find_par(Node* x, Key key) const{
    if(this->root != NULL){
        if(this->root->left != NULL){
            if(this->root->left->key == key){
                return this->root;
            }
        }
        if(this->root->right != NULL){
            if(this->root->right->key == key){
                return this->root;
            }
        }
    }
    while(true){
        if(x == NULL){
            return NULL;
        }
        if(x->key > key){
            if(x->left == NULL){
                return NULL;
            }
            if(x->left->key == key){
                return x;
            }
            else{
                x = x->left;
            }
        }
        else if(x->key < key){
            if(x->right == NULL){
                return NULL;
            }
            if(x->right->key == key){
                return x;
            }
            else{
                x = x->right;
            }
        }
    }
}

template <typename Key>
typename splay<Key>::Node*
splay<Key>::find(Key key) const{
    return this->find_pri(this->root, key);
}

template <typename Key>
int splay<Key>::size_of_splay(){
    return this->size;
}

template <typename Key>
void splay<Key>::preOrder(Node* x) const
{
    if(x != NULL)
    {
        printf("%d \0", x->key);
        preOrder(x->left);
        preOrder(x->right);
    }
}

template <typename Key>
typename splay<Key>::Node*
splay<Key>::succ(Key key) const{
    Node* next = find(key);
    if(next == root){
        if(root->right == NULL){
            return NULL;
        }
        else{
            next = next->right;
            while(next->left != NULL){
                next = next->left;
            }
            return next;
        }
    }
    Node* parent = find_parent(key);
    if(next->right == NULL){
        if(parent->right == next){
            Node* maxier = root;
            while(maxier->right != NULL){
                maxier = maxier->right;
            }
            if(next == maxier){
                return NULL;
            }
            while(parent != find_parent(parent->key)->left){
                parent = find_parent(parent->key);
            }
            return find_parent(parent->key);
        }
        else{
            return parent;
        }
    }
    else{
        next = next->right;
        while(next->left != NULL){
            next = next->left;
        }
        return next;
    }
}

template <typename Key>
typename splay<Key>::Node*
splay<Key>::pred(Key key) const{
    Node* next = find(key);
    Node* parent = find_parent(key);
    if(next->left == NULL){
        if(parent->left == next){
            Node* minier = root;
            while(minier->left != NULL){
                minier = minier->left;
            }
            if(next == minier){
                return NULL;
            }
            while(parent != find_parent(parent->key)->right){
                parent = find_parent(parent->key);
            }
            return find_parent(parent->key);
        }
        else{
            return parent;
        }
    }
    else{
        next = next->left;
        while(next->right != NULL){
            next = next->right;
        }
        return next;
    }
}

template <typename Key>
void splay<Key>::preOrder() const
{
    this->preOrder(root);
}

template <typename Key>
void splay<Key>::inOrder(Node* x) const
{
    if(x != NULL)
    {
        inOrder(x->left);
        printf("%d \0", x->key);
        inOrder(x->right);
    }
}

template <typename Key>
void splay<Key>::inOrder() const
{
    this->inOrder(root);
}

template <typename Key>
void splay<Key>::postOrder(Node* x) const
{
    if(x != NULL)
    {
        postOrder(x->left);
        postOrder(x->right);
        printf("%d \0", x->key);
    }
}

template <typename Key>
void splay<Key>::postOrder() const
{
    this->postOrder(root);
}

template <typename Key>
typename splay<Key>::Node*
splay<Key>::splay_root()
{
    return this->root;
}

template <typename Key>
void splay<Key>::join(sp splay2){
    //Require all member in splay2 larger than splay1
    Node* this_max = this->maxi(this->root);
    this->root = this->zigzag(this->root, this_max->key);
    this_max->right = splay2.splay_root();
    this->size = this->size + splay2.size_of_splay();
}

template <typename Key>
int count(splay_node<Key>* node){
    if(node == NULL){
        return 0;
    }
    return count(node->left) + count(node->right) + 1;
}

template <typename Key>
void splay<Key>::split(sp* &splay2){
    printf("gly\n");
    int sizer = this->size_of_splay() / 2;
    printf("gly %d %d\n", sizer, this->size);
    Node* x = mini(this->root);
    for(int i = 0; i < sizer; i++){
        printf("%d\n", x->key);
        x = succ(x->key);
        printf("I love GLY\n");
    }
    printf("the key is %d\n", succ(x->key)->key);
    this->root = zigzag(this->root, succ(x->key)->key);
    splay2->Add_root(this->root->left);
    splay2->Add_size(sizer);
    this->root->left = NULL;
    this->size = this->size - sizer;
}

template <typename Key>
void splay<Key>::Add_root(Node* node){
    this->root = node;
}

template <typename Key>
void splay<Key>::Add_size(int si){
    this->size = si;
}

#endif
