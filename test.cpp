#include <stdint.h>

#include <unordered_map>
#include <string.h>
#include <cstdlib>
//#include <gtest/gtest.h>

#include "yfast.h"

#include <iostream>

/*
namespace yfast {


typedef uint8_t Key;
typedef std::string Value;
typedef std::unordered_map<Key, void*> Hash;
typedef xfast<Key, Value, Hash> XFast;

TEST(XFast, InsertZero) {
    XFast xfast;
    const Key key = 0;
    const Value value = "zero";
    xfast.insert(key, value);
    XFast::Leaf* leaf = xfast.find(key);
    ASSERT_TRUE(leaf);
    EXPECT_EQ(value, leaf->value);
}

TEST(XFast, InsertZeroAndOne) {
    XFast xfast;
    const Key key0 = 0;
    const Value value0 = "zero";
    xfast.insert(key0, value0);
    const Key key1 = 1;
    const Value value1 = "one";
    xfast.insert(key1, value1);
    XFast::Leaf* leaf0 = xfast.find(key0);
    ASSERT_TRUE(leaf0);
    EXPECT_EQ(value0, leaf0->value);
    XFast::Leaf* leaf1 = xfast.find(key1);
    ASSERT_TRUE(leaf1);
    EXPECT_EQ(value1, leaf1->value);
}

TEST(XFast, PredSuccOne) {
    XFast xfast;
    const Key key0 = 0;
    const Value value0 = "zero";
    xfast.insert(key0, value0);
    const Key key2 = 2;
    const Value value2 = "two";
    xfast.insert(key2, value2);
    const Key key1 = 1;
    XFast::Leaf* leaf0 = xfast.pred(key1);
    ASSERT_TRUE(leaf0);
    EXPECT_EQ(value0, leaf0->value);
    XFast::Leaf* leaf2 = xfast.succ(key1);
    ASSERT_TRUE(leaf2);
    EXPECT_EQ(value2, leaf2->value);
}

*/


/*
typedef uint8_t Key;
typedef std::string Value;
typedef std::unordered_map<Key, void*> Hash;
typedef xfast<Key, Value, Hash> XFast;
*/
int main(){
    typedef int Key;
    typedef std::unordered_map<Key, void*> Hash;
    typedef x_fast<Key, splay<Key>, Hash> XFast;
    typedef yfast<Key, Hash> YFast;
    YFast* yf = new YFast();
    Key i = 0, j = 0;
    int p = 0, q = 0;

    while(p < 1000){
        if(i < 500){
            const Key key = i;
            std::cout << i << std::endl;
            i = 1000 - i;
            yf->insert(key);
            printf("wgz\n");
        }
        else{
            const Key key = i;
            std::cout << i << std::endl;
            i = 1000 - i + 1;
            yf->insert(key);
            printf("wgz\n");
        }
        p++;
    }


    while(q < 1000){
        if(q != 500){
            YFast::SN* leaf = yf->find(j);
            if(leaf != NULL){
                std::cout << leaf->key << std::endl;
                yf->dele(j);
            }
            else{
                std::cout << "Not Found" << std::endl;
            }
        }
        q++;
        j++;
    }

    delete yf;

}
    /*









    const Key key0 = 0;
    const Value value0 = "zero";
    xfast.insert(key0, value0);
    const Key key1 = 1;
    const Value value1 = "one";
    xfast.insert(key1, value1);
    const Key key2 = 2;
    const Value value2 = "two";
    xfast.insert(key2, value2);
    const Key key3 = 3;
    const Value value3 = "three";
    xfast.insert(key3, value3);
    const Key key4 = 4;
    const Value value4 = "four";
    xfast.insert(key4, value4);
    XFast::Leaf* leaf0 = xfast.find(key0);
    std::cout << leaf0->value << std::endl;

    XFast::Leaf* leaf1 = xfast.find(key1);
    std::cout << leaf1->value << std::endl;
    XFast::Leaf* leaf2 = xfast.find(key2);
    std::cout << leaf2->value << std::endl;
    XFast::Leaf* leaf3 = xfast.find(key3);
    std::cout << leaf3->value << std::endl;

    XFast::Leaf* leaf4 = xfast.find(key4);
    std::cout << leaf4->value << std::endl;
}*/
