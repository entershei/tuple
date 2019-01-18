#include "tuple.h"
#include <string>
#include <iostream>
#include <memory>
#include "assert.h"

using namespace std;

struct A{
    int b;
    A()=default;
    A(const A& a) : b(a.b){}
    A(A &&a) = delete;
};

int main() {
    my::Tuple<int, long, string> tp(5, 6, "text");
    assert(my::get<0>(tp) == 5);
    assert(my::get<1>(tp) == 6);
    assert(my::get<2>(tp) == "text");
    assert(my::get<int>(tp) == 5);
    assert(my::get<long>(tp) == 6);
    assert(my::get<string>(tp) == "text");

    //cerr << my::get<3>(tp);

    my::Tuple<string, int, bool> t("abc", 5, true);
    assert(my::get<0>(t) == "abc");
    assert(my::get<1>(t) == 5);
    assert(my::get<2>(t) == true);
    assert(my::get<int>(t)== 5);
    assert(my::get<bool>(t)== true);
    assert(my::get<string>(t)== "abc");

    const my::Tuple<string, int, bool> tc("abc", 5, true);
    const string res = "abc";
    assert(my::get<0>(tc) == res);
    assert(my::get<string>(tc) == res);
    //assert(my::get<long>(t)== 5);
    //assert(my::get<-1>(t) == 0);

    unique_ptr<int> up(new int(10));
    my::Tuple<unique_ptr<int>> t1(std::move(up));
    assert(*(my::get<0>(t1)) == 10);

    A a;
    my::Tuple<A> tr(a);

    my::Tuple<int> tiii(10);
    my::Tuple<int> t2iii(tiii);

    return 0;
}