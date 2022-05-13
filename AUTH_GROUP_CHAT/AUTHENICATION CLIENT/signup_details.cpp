#include<iostream>
#include<fstream>
using namespace std;
int main()
{
    ofstream file;
    string username, password;
    file.open("user_check.txt");
    int i = 0;
    while (i < 5)
    {
        cout << "enter username:" << endl;

        cin >> username;
        cout << "enter password:" << endl;
        cin >> password;
        file << username + "_";
        file << password + ' ';

        cout << "success" << endl;
        i++;
    }
}