
#include "Interface.h"

int main() {
    try
    {
        setlocale(LC_ALL, "Rus");
        Interface* _interface = new Interface;
        _interface->OpenApp();
        _interface->IsReady();
        delete _interface;
    }
    catch (string err)
    {
        cout << err << endl;
    }
    return 0;
}