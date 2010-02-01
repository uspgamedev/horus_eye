// teste da classe Frame
#include <iostream>
#include "../framework/frame.h"
using std::cout;
using std::endl;
using framework::Frame;

// mostra dados de um frame
void ShowFrame(const Frame& frame)
{
    cout << "left,top,right,bottom: ";
    cout << frame.left() << ",";
    cout << frame.top() << ",";
    cout << frame.right() << ",";
    cout << frame.bottom() << endl;

    cout << "width,height: ";
    cout << frame.width() << ",";
    cout << frame.height() << endl;
}

// imprime um caso de testes
void Test(const Frame& f, const Frame& g)
{
    cout << "Frame f" << endl;
    ShowFrame(f);
    cout << "Frame g" << endl;
    ShowFrame(g);
    cout << "f colide com g? " << (f.Collides(g) ? "SIM" : "NAO") << endl;
    cout << endl;
}

// main
int main()
{
    Frame f(10,0,200,10);
    Frame g(0,0,20,20);

    Test(f, g);

    f.set_left(100);
    Test(f, g);

    g.set_bottom(500);
    g.set_top(400);
    Test(f, g);

    return 0;
}
