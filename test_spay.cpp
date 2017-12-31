/**
 * C++ 语言: 伸展树
 *
 * @author skywang
 * @date 2014/02/03
 */

#include <iostream>
#include "splay.h"
using namespace std;

int main()
{


    typedef int Key;
    splay<Key>* tree = new splay<Key>();

    Key i = 0, j = 0;
    int p = 0, q = 0;

    while(p < 1000){
        if(i < 500){
            const Key key = i;
            std::cout << "inserting" << i << std::endl;
            i = 1000 - i;
            tree->insert(key);
        }
        else{
            const Key key = i;
            std::cout << "insering" << i << std::endl;
            i = 1000 - i + 1;
            tree->insert(key);

        }
        p++;
    }

    printf("after join, the size is %d\n",tree->size_of_splay());

    p = 1000;
    i = 1000;
    splay<Key>* tree2 = new splay<Key>();

    while(p < 2000){
        if(i < 1500){
            const Key key = i;
            std::cout << "inserting" << i << std::endl;
            i = 3000 - i;
            tree2->insert(key);
        }
        else{
            const Key key = i;
            std::cout << "insering" << i << std::endl;
            i = 3000 - i + 1;
            tree2->insert(key);
        }
        p++;
    }

    printf("after join, the size is %d\n",tree2->size_of_splay());

    tree->join(tree2);

    printf("after join, the size is %d\n",tree->size_of_splay());

    tree->preOrder();

    splay<Key>* tree3 = new splay<Key>();
    printf("wgz\n");
    tree->split(tree3);
    printf("after join, the size is %d\n",tree->size_of_splay());
    printf("after join, the size is %d\n",tree3->size_of_splay());

    tree->preOrder();
    printf("\n");
    tree3->preOrder();

    /*
    i = 0;
    while(q < 1000){
        if(i < 500){
            const Key key = i;
            std::cout << "deleting" << i << std::endl;
            i = 1000 - i;
            tree->dele(key);
        }
        else{
            const Key key = i;
            std::cout << "deleting" << i << std::endl;
            i = 1000 - i + 1;
            tree->dele(key);

        }
        q++;
    }
    */
    /*

    for(i=0; i<=1000; i++)
    {
        if(i != 500){
            tree->dele(i);
        }
    }
    */
    tree->destory();
    tree2->destory();
    tree3->destory();
    delete tree;
    delete tree2;
    delete tree3;
    return 0;
}
