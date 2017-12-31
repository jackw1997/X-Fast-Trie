#ifndef YFAST_H
#define YFAST_H

#include "xfast.h"
#include "splay.h"

#include <cmath>

template <typename Key, typename Hash>
class yfast{
    public:
        typedef splay_node<Key> SN;
        typedef x_fast_leaf<Key, splay<Key> > XFL;
        typedef x_fast_node<Key, splay<Key> > XFN;
        typedef splay<Key> sp;


    private:
        typedef x_fast<Key, sp, Hash> XF;
        XF* xfast;

        static const int H = 8 * sizeof(Key);
        int size;
        //Y node here is just the splay tree defined in splay.h
        //It is included in this folder.

        void des_xfl(XFL* xfl);//
        void des_xfn(XFN* xfn);//


    public:
        yfast();//
        void init();//
        SN* find(Key key);//
        void insert(Key key);//
        SN* succ(Key key);
        SN* pred(Key key);
        void dele(Key key);//
        void destory();//

};

template <typename Key, typename Hash>
yfast<Key, Hash>::yfast(){
    this->xfast = new XF();
    this->size = 0;
}

template <typename Key, typename Hash>
void yfast<Key, Hash>::init(){
    this->yfast();
}

template <typename Key, typename Hash>
typename yfast<Key, Hash>::SN*
yfast<Key, Hash>::find(Key key){
    XFL* index = this->xfast->succ(key);
    if(index == NULL){
        return NULL;
    }
    sp splay_x = index->value;
    return splay_x.find(key);
}

template <typename Key, typename Hash>
typename yfast<Key, Hash>::SN*
yfast<Key, Hash>::succ(Key key){
    XFL* index = this->xfast->succ(key);
    if(index == NULL){
        return NULL;
    }
    if(key == index->key){
        if(index->next == NULL){
            return NULL;
        }
        sp* x = index->next->value;
        SN* node = x->splay_root();
        while(node->left != NULL){
            node = node->left;
        }
        return node;
    }
    else{
        sp* x = index->value;
        return x->succ(key);
    }
}

template <typename Key, typename Hash>
typename yfast<Key, Hash>::SN*
yfast<Key, Hash>::pred(Key key){
    XFL* index = this->xfast->succ(key);
    if(index == NULL){
        index = this->xfast->pred(key);
        sp* x = index->value;
        SN* node = x->splay_root();
        while(node->right != NULL){
            node = node->right;
        }
        return node;
    }
    if(key <= index->value->minimum()){
        if(index->prev == NULL){
            return NULL;
        }
        XFL* index_prev = index->prev;
        sp* x = index_prev->value;
        SN* node = x->splay_root();
        while(node->right != NULL){
            node = node->right;
        }
        return node;
    }
    else{
        sp* x = index->value;
        return x->pred(key);
    }
}

template <typename Key, typename Hash>
void yfast<Key, Hash>::insert(Key key){
    if(this->find(key) != NULL){
        return;
    }
    this->size ++;
    int upH = 2 * (int)(log(double(this->size)) / log(2.0));
    int lowH = (int)(log(double(this->size)) / log(2.0)) / 2;
    if(this->xfast->empty()){
        sp* new_splay = new sp();
        new_splay->insert(key);
        this->xfast->insert(key, *new_splay);
    }
    else{
        XFL* succ = this->xfast->succ(key);
        if(succ != NULL){
            succ->value.insert(key);
            if(succ->value.size_of_splay() > upH){
                sp* new_splay = new sp();
                succ->value.split(new_splay);
                this->xfast->insert(new_splay->maximum(), *new_splay);
            }
        }

        else{
            sp* new_splay = new sp();
            new_splay->insert(key);
            this->xfast->insert(key, *new_splay);
            /*
            XFL* pred = this->xfast->pred(key);
            printf("%d\n", pred->key);
            sp tmp = pred->value;
            tmp.insert(key);
            this->xfast->dele(pred->key);
            this->xfast->insert(key, tmp);
            *//*
            if(tmp.size_of_splay() > upH){
                sp* new_splay = new sp();
                tmp.split(new_splay);
                this->xfast->insert(new_splay->maximum(), *new_splay);
            }
            */
            printf("gly\n");
        }

    }
}

template <typename Key, typename Hash>
void yfast<Key, Hash>::dele(Key key){
    SN* dele_node = this->find(key);
    if(dele_node == NULL){
        printf("No such node. Cannot delete!\n");
        return;
    }

    XFL* index = this->xfast->succ(key);
    sp splay_x = index->value;
    Key maxier = splay_x.maximum();
    if(maxier == key){
        splay_x.dele(key);
        this->xfast->dele(key);
        Key new_maximer = splay_x.maximum();
        this->xfast->insert(new_maximer, splay_x);
    }
    else{
        splay_x.dele(key);
    }

    this->size --;
    int upH = 2 * (int)(log(double(this->size)) / log(2.0));
    int lowH = (int)(log(double(this->size)) / log(2.0)) / 2;

    int x_size = splay_x.size_of_splay();

    if(x_size < lowH){
        index = this->xfast->succ(key);
        XFL* index_prev = index->prev;
        sp splay_y = index_prev->value;
        this->xfast->dele(index_prev->key);
        splay_y.join(splay_x);
        index->value = splay_y;
        if(splay_y.size_of_splay() > upH){
            sp* new_splay = new sp();
            splay_y.split(new_splay);
            this->xfast->insert(new_splay->maximum(), *new_splay);
        }

    }

}

template <typename Key, typename Hash>
void yfast<Key, Hash>::destory(){
    if(this->xfast->empty()){
        return;
    }
    XFN* root_xf = this->xfast->get_root();
    this->des_node(root_xf);
}

template <typename Key, typename Hash>
void yfast<Key, Hash>::des_xfl(XFL* xfl){
    xfl->value->destory();
    delete xfl;
}

template <typename Key, typename Hash>
void yfast<Key, Hash>::des_xfn(XFN* xfn){
    if(xfn->left == NULL && xfn->right == NULL){
        XFL* xfl = xfn->leaf;
        des_xfl(xfl);
        delete xfn;
    }
    else{
        if(xfn->left != NULL){
            this->des_node(xfn->left);
        }
        if(xfn->right != NULL){
            this->des_node(xfn->right);
        }
        delete xfn;
    }
}

#endif
