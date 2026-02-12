#include <iostream>
#include <string>
using namespace std;

class dormitory{
    private:
        string name;
        string ID;
        int electric, water;
    public:
        dormitory(){name = "unknow"; ID = "unknow"; electric=0; water=0;}
        dormitory(string n,string id, int ele, int wa) {name=n; ID=id; electric=ele; water=wa;}
        dormitory(int e){electric=e;water=0;}
        dormitory(int e,int w){electric=e;water=w;}
        ~dormitory(){cout << "Destructor Called" <<endl;}
        void showData() {
            cout << "Name: " << name << endl;
            cout << "roomID: " << ID << endl;
            cout << "electric: " << electric << endl;
            cout << "water: " << water << endl;
        }
};
int main() {
    dormitory room1;
    room1.showData();
    cout << "****************" << endl;
    dormitory room2("Natchanon","1542",999,99);
    room2.showData();
    cout << "******************" << endl;
    dormitory room3 = room2;
    room3.showData();
};