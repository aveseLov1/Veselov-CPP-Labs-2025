// #include <iostream>

// int main(int, char**) {
//     std::cout << "Hello, World!" << std::endl;

//     return 0;
// }

#include <iostream>

struct S {
    int a = 0;
    double b = 0.;
};

void foo(int a, int* b, S& c) {
    std::cout << a << std::endl;

    int d = 10;
    std::cout << d << std::endl;

    if (b) {
        *b = 5;
    }

    c.a = 11;
    c.b = 15.25;
}

int main(int, char**) {
    int a = 17;
    S s{};

    foo(15, &a, s);

    std::cout <<"*  a="<< a << std::endl;
    std::cout << s.a << ' ' << s.b << std::endl;

    return 0;
}
