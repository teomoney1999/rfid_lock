#include <iostream>
#include <string>
#include <typeinfo>

using namespace std;

void tokenize(string str, string del = " ") {
    int start = 0; 
    int end = str.find(del);

    string data[16]; 
    int index = 0;
    int size = 16;

    while (end != -1) {
        if (index == size) {
            break;
        }
        string temp = str.substr(start, end - start); 
        data[++index] = temp; 
    
        start = end + del.size(); 
        end = str.find(del, start);
    } 
    data[index] = str.substr(start);

    for (int i=0; i<size; i++) {
        cout << data[i] << endl;
    }
}

int main(void) {
    unsigned char dataBlock[]    = {
        0x01, 0x02, 0x03, 0x04, //  1,  2,   3,  4,
        0x05, 0x06, 0x07, 0x08, //  5,  6,   7,  8,
        0x09, 0x0a, 0xff, 0x0b, //  9, 10, 255, 11,
        0x0c, 0x0d, 0x0e, 0x0f  // 12, 13, 14, 15
    }; 

    unsigned char data[16];
    string dataStr = "01 02 03 04 05 06 07 08 09 0a ff 0b 0c 0d 0e 0f";
    int size = (int) dataStr.size();

    tokenize(dataStr, " ");

    int hex = 0x11;

    cout << hex << endl;



}