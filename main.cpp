#include <iostream>
#include <string>
#include <memory>
#include <exception>
#include <thread>

using namespace std;

// Box cannot hold nullptr
template<class T>
class Box
{
public:  
    template <class... Args>
    static Box<T> create(Args&&... args)
    {
        return Box<T>(make_shared<T>(std::forward<Args>(args)...));
    }
    
    T * operator-> () const
    {
        return ptr.operator -> ();
    }
private:
    Box(const shared_ptr<T> & ptr)
        : ptr(ptr)
    {}
    
    shared_ptr<T> ptr;
};

struct Test
{
    Test(int i, double d, const string & s)
        : i(i), d(d), s(s)
    {
        cout << "Test ctor" << endl;
    }
    
    ~Test()
    {
        cout << "Test dtor" << endl;
    }
    
    Test(const Test & test) = delete;
    Test(Test && test) = delete;
    Test & operator = (const Test & test) = delete;
    Test & operator = (Test && test) = delete;
    
    int i;
    double d;
    string s;
};

void print(const Box<Test> & test)
{
    // there is no need to check for nullptr
    cout << test->i << " " << test->d << " " << test->s << endl;
}

void twice(Box<Test> test)
{
    // there is no need to check for nullptr
    test->i *= 2;
    test->d *= 2;
    test->s += test->s;
}

/*
stdout:
Test ctor
42 3.14 hello
84 6.28 hellohello
Test dtor
*/
int main()
{
    Box<Test> test = Box<Test>::create(42, 3.14, "hello");
    print(test);
    
    thread t(twice, test);
    t.join();
    
    print(test);
}
