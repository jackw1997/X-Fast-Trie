#ifndef _XFAST_H
#define _XFAST_H

#include <map>

#include <iostream>


//#include "splay.h"

//Proudly Presented by Wu Guangzheng

//Y Fast Trie has an X Fast Trie as a base and balanced binary tree
//We Construct a Class of X Fast Trie here

template <typename Key, typename Value>
struct x_fast_node;

template <typename Key, typename Value>
struct x_fast_leaf{
    Key             key;
    Value           value;

    //The X Fast Trie Leaves are constructed by a linked list. We use a double linked list here.
    x_fast_leaf*     prev;
    x_fast_leaf*     next;

    x_fast_node<Key, Value>* parent;

    //In Y Fast Trie, each leaf connects a BST, we provide the BST here.
    //Key              this_smallest;
    //Key              next_smallest;
    //map<Key, void*>  BST;
    //int              size;
    //Construction
    x_fast_leaf(Key key, const Value& value);
};

template <typename Key, typename Value>
x_fast_leaf<Key, Value>::x_fast_leaf(Key key_con, const Value& value_con){
    key = key_con;
    value = value_con;
    prev = NULL;
    next = NULL;
    parent = NULL;
}

template <typename Key, typename Value>
struct x_fast_node{
    x_fast_node*    left;
    x_fast_node*    right;
    x_fast_node*    parent;

    //The leaf is pointed to the last node which is on the left side of this tree.
    //Include in the tree itself.
    //Not considering the threads.
    //The "leaf->prev" is donated as the last node outside the tree, which is useful as well.
    x_fast_leaf<Key, Value>*    leaf;

    //Construction
    x_fast_node();
};

template <typename Key, typename Value>
x_fast_node<Key, Value>::x_fast_node(){
    left = NULL;
    right = NULL;
    parent = NULL;
    leaf = NULL;
}

template <typename Key, typename Value, typename Hash>
class x_fast{
    private:
        typedef x_fast_node<Key, Value> XFN;
        typedef x_fast_leaf<Key, Value> XFL;

        //Root of the X Fast Trie, and the Hash Table needed for Hash
        //You can find the hash table defined in the main function
        XFN*        root;

        //A trie tree has some limitations on the key stored.
        //The size of the maximum size is based on the first input of the key.
        static const int H = 8 * sizeof(Key);
        Hash        hash[H];

        //approximation to find
        //the return is the leaf with largest key which is smaller than the input.
        XFL*        approximation(Key key) const;

    public:
        typedef XFL Leaf;
        x_fast();
        void insert(Key key, const Value& value);
        XFL* find(Key key) const;
        XFL* succ(Key key) const;
        XFL* pred(Key key) const;
        void dele(Key key);
        bool empty() const;
        void destory();
        XFN* get_root();
};

template <typename Key, typename Value, typename Hash>
typename x_fast<Key, Value, Hash>::XFN*
x_fast<Key, Value, Hash>::get_root(){
    return this->root;
}
template <typename Key, typename Value, typename Hash>
x_fast<Key, Value, Hash>::x_fast(){
    this->root = NULL;
}

template <typename Key, typename Value, typename Hash>
bool x_fast<Key, Value, Hash>::empty() const{
    if(this->root == NULL){
        return true;
    }
    else{
        return false;
    }
}

template <typename Key, typename Value, typename Hash>
typename x_fast<Key, Value, Hash>::XFL*
x_fast<Key, Value, Hash>::approximation(Key key) const {
    if(root == NULL){
        return NULL;
    }
    //We use a hash table here.
    //In this step, we find at which height we need to call the hash table.
    //It is a binary search on the height, so it is O(log log n)
    int m, l = 0, r = H;
    while(r - l > 1){
        m = (r + l)/2;
        if(hash[m].count(key >> m) > 0){
            r = m;
        }
        else{
            l = m;
        }
    }
    //Detailing the level.
    //When the loop ends, we have at most two possible level remains.
    //O(1)
    if (hash[l].count(key >> l) > 0) {
        m = l;
    }
    else {
        m = r;
    }
    XFL* leaf;
    //Approximate the key based on the hash table. O(1)
    if (H == m) {
        leaf = this->root->leaf;
    }
    else if(0 == m){
        //When the level detection reaches towards the lowest level.
        //We need to hash on the leaves.
        //Which means the key exists.
        typename Hash::const_iterator i = hash[0].find(key);
        void* p = i->second;
        leaf = static_cast<XFL*>(p);
    }
    else{
        //Hash on the nodes.
        typename Hash::const_iterator i = hash[m].find(key >> m);
        void* p = i->second;
        XFN* node = static_cast<XFN*>(p);
        leaf = node->leaf;
    }
    return leaf;
}

template <typename Key, typename Value, typename Hash>
void x_fast<Key, Value, Hash>::insert(Key key, const Value& value){
    XFL* prev = NULL;
    XFL* next = NULL;
    XFL* guess = approximation(key);
    XFN* x_root = this->root;

    //Do exist, no need to insert.
    if((guess != NULL) && (guess->key == key)){
        return;
    }
    XFL* insert_leaf = new XFL(key, value);


    if(guess == NULL){
        //Insert the first element. Should insert the whole height, O(log n)
        XFN* node_par = new XFN();
        XFN* x_root = node_par;
        this->root = x_root;
        x_root->leaf = insert_leaf;
        int s = 0;
        for (int h = 1; h < H; h++){
            s = s+1;
            XFN* node = new XFN();
            node->parent = node_par;
            if(key & (1 << (H-1-h))){
                node_par->right = node;
            }
            else{
                node_par->left = node;
            }
            node->leaf = insert_leaf;
            void* p = node;
            hash[H-h].insert(std::make_pair(key >> (H-h), p));
            node_par = node;
        }
        insert_leaf->parent = node_par;
    }
    else{
        insert_leaf->prev = guess;
        if(guess->next != NULL){
            insert_leaf->next = guess->next;
            guess->next->prev = insert_leaf;
        }
        guess->next = insert_leaf;
        XFN* node_par = x_root;
        bool flag = true;
        for(int h = 1; h < H; h++){
            if(key & (1 << (H-1-h))){
                if(node_par->right == NULL){
                    XFN* node = new XFN();
                    node_par->right = node;
                    node->parent = node_par;
                    node->leaf = insert_leaf;
                    void* p = node;
                    hash[H-h].insert(std::make_pair(key >> (H-h),p));
                }
                node_par = node_par->right;
            }
            else{
                if(node_par->left == NULL){
                    XFN* node = new XFN();
                    node_par->left = node;
                    node->parent = node_par;
                    node->leaf = insert_leaf;
                    void* p = node;
                    hash[H-h].insert(std::make_pair(key >> (H-h),p));
                }
                else{
                     flag = false;
                }
                node_par = node_par->left;
            }
        }
        insert_leaf->parent = node_par;
        if(flag == false){
            node_par = node_par->parent;
            while(((node_par->parent->left != node_par)
                    && (node_par->parent->left != NULL))){
                        if(node_par->left != NULL){
                            node_par->leaf = node_par->left->leaf;
                        }
                        else{
                            node_par->leaf = node_par->right->leaf;
                        }
                        if(node_par->parent = x_root){
                            if(node_par->left != NULL){
                                node_par->leaf = node_par->left->leaf;
                            }
                            else{
                                node_par->leaf = node_par->right->leaf;
                            }
                            break;
                        }
                        node_par = node_par->parent;
                    }
        }
    }
    void* p = insert_leaf;
    hash[0].insert(std::make_pair(key, p));
}

template <typename Key, typename Value, typename Hash>
typename x_fast<Key, Value, Hash>::XFL*
x_fast<Key, Value, Hash>::pred(Key key) const{
    XFL* guess = approximation(key);
    if(guess == NULL){
        return NULL;
    }
    if(guess->key < key){
        return guess;
    }
    return guess->prev;
}

template <typename Key, typename Value, typename Hash>
typename x_fast<Key, Value, Hash>::XFL*
x_fast<Key, Value, Hash>::succ(Key key) const{
    XFL* guess = approximation(key);
    if(guess == NULL){
        return NULL;
    }
    else{
        return guess->next;
    }
}

template <typename Key, typename Value, typename Hash>
typename x_fast<Key, Value, Hash>::XFL*
x_fast<Key, Value, Hash>::find(Key key) const {
    typename Hash::const_iterator i = hash[0].find(key);
    if (i != hash[0].end()) {
        void* p = i->second;
        XFL* leaf = static_cast<XFL*>(p);
        return leaf;
    }
    else {
        return NULL;
    }
}


template <typename Key, typename Value, typename Hash>
void x_fast<Key, Value, Hash>::dele(Key key){
    XFL* deleting = this->find(key);
    if(deleting == NULL){
        std::cout << "no such key, cannot delete" << std::endl;
        return;
    }

    XFL* pre = deleting->prev;
    XFL* nex = deleting->next;
    if(pre == NULL && nex == NULL){
        XFN* node_pre = deleting->parent;
        std::cout << "deleting with key " << deleting->key << std::endl;
        delete deleting;
        int h = 0;
        typename Hash::const_iterator i = hash[0].find(key);
        hash[h].erase(i);
        while(node_pre != this->root){
            h++;
            XFN* tmp = node_pre;
            i = hash[h].find(key << h);
            hash[h].erase(i);
            node_pre = node_pre->parent;
            delete tmp;
        }
        delete this->root;
        return;
    }
    else if(pre == NULL){
        deleting->next->prev = NULL;
    }
    else if(nex == NULL){
        deleting->prev->next = NULL;
    }
    else{
        deleting->next->prev = pre;
        deleting->prev->next = nex;
    }

    XFN* node_pre = deleting->parent;
    int h = 0;
    typename Hash::const_iterator i = hash[0].find(key);
    hash[h].erase(i);
    bool flag = true;
    while(node_pre->leaf == deleting){
        h++;
        if(flag != true){
            if(node_pre->left == NULL){
                node_pre->leaf = node_pre->right->leaf;
            }
            else{
                node_pre->leaf = node_pre->left->leaf;
            }
            if(node_pre == this->root){
                break;
            }
            node_pre = node_pre->parent;
        }
        else{
            if(node_pre->right == NULL){
                XFN* tmp = node_pre;
                node_pre = node_pre->parent;
                i = hash[h].find(key << h);
                if(i != hash[h].end()){
                    hash[h].erase(i);
                }
                delete tmp;
            }
            else{
                node_pre->leaf = node_pre->right->leaf;
                flag = false;
                node_pre = node_pre->parent;
            }
        }
    }
    std::cout << "deleting with key " << deleting->key << std::endl;
    delete deleting;
    return;
}









#endif
