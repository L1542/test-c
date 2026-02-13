#include <iostream>
#include <string>
using namespace std;

class BankAccount {
    private:
        string holder;
        string accountNum;
        double balance;
    public:
        BankAccount() { holder = "Unknow"; accountNum = "Unknow"; balance = 0.0; }
        BankAccount(string accNum, double b){
            holder = "Undefined"; accountNum = accNum; balance = b; 
            cout <<"Call account number: " << accountNum <<endl;
        }
        BankAccount(string h, string accNum, double b){
            holder = h; accountNum = accNum; balance = b;
            cout << "Call account holder name: " <<holder << endl; }
            ~BankAccount() { cout << "Destructor class Bank Account" <<endl; }
            void deposit(double amount) {
                if (amount > 0) {
                    balance += amount;
                    cout << "Deposit: " << amount << endl;
                }
            }
            void withdraw(double amount) {
                if (amount > 0 && amount <= balance) {
                    balance -= amount;
                    cout << "Withdraw: " << amount << endl;
                }
            }
            void showData() {
                cout << "Holder name: " << holder << endl;
                cout << "Account Number: " << accountNum << endl;
                cout << "Balance: " << balance << endl;
            }
};

int main() {
    BankAccount acc1;
    acc1.deposit(500);
    acc1.withdraw(180);
    acc1.showData();
    cout << "************\n" << endl;
    
    BankAccount acc2("Somsri","000-100-999",2500);
    acc2.deposit(1000);
    acc2.withdraw(250);
    acc2.showData();
    cout << "************\n" << endl;
    
    BankAccount acc3 = acc2;
    acc3.deposit(100);
    acc3.withdraw(500);
    acc3.showData();

    return 0;
};