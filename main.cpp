//
//  main.cpp
//  pttask
//

#include <iostream>
#include <iomanip>
#include <array>

#include "IDataHandler.h"
#include "StreamReader.h"

using namespace std;

class MyDataHandler: public IDataHandler {
public:
    MyDataHandler(){}
    ~MyDataHandler(){}

    void handle_binary_data(const char* data, size_t size){
        cout<<">>bin data ("<<size<<" bytes):";
        auto flags = cout.flags();
        for(int i=0; i<size; i++){
            if(!i%16)
                cout<<"\n\t";
            cout<<hex<<setfill('0')<<setw(2)<<int((unsigned char)data[i])<<" ";// не нравится, но работает :/
        }
        cout<<endl;
        cout.flags(flags);
    }

    void handle_text_data(const char* data, size_t size){
        cout<<">>str data ("<<size<<" bytes):"<<endl<<"\t\t\t";
        for(size_t i=0; i<size; i++){
            if (data[i] == '\r')
                cout<<"\\r";
            else if (data[i] == '\n')
                cout<<"\\n";
            else if (data[i] == '\0')
                cout<<"\\0";
            else
                cout<<data[i];
        }
        cout<<endl;

    }
};

void cout_rn(const string &str){
    for(auto&i:str){
        if (i == '\r')
            cout<<"\\r";
        else if (i == '\n')
            cout<<"\\n";
        else
            cout<<i;
    }
}

int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Hello, World!\n";
    shared_ptr<IDataHandler> handler(new MyDataHandler());
    StreamReader receiver(handler);

    array<string, 5> strs = {{"str_test1\r\n\r\n", "str_test2\r\n\r\r\n\r\r\n\r\n", "str_test3\n\r\n\r-\n\r\n\r\n",
                             "str\r\n\r\n", "str\n\r\n\r\n"}};
    cout<<"Parse data:\n";
    int npack = 0;
    vector<char> vec;

    //строки
    cout<<"STRINGS DATA"<<endl;
    for(auto& i:strs){
        vec.erase(vec.begin(), vec.end());
        vec.insert(vec.begin(), i.begin(), i.end());
        cout<<"original:\t";cout_rn(i);cout<<endl;
        npack+=receiver.receive(vec.data(), vec.size());
    }
    //<--5 pack
    cout<<"<<"<<npack<<" pack\n";
    vec.erase(vec.begin(), vec.end());
    vec.insert(vec.end(), strs[1].begin(), strs[1].end());
    vec.insert(vec.end(), strs[1].begin(), strs[1].end()-strs[1].size()/2);
    npack+=receiver.receive(vec.data(), vec.size());
    //<--6 pack
    cout<<"<<"<<npack<<" pack\n";
    vec.erase(vec.begin(), vec.end());
    vec.insert(vec.end(), strs[1].end()-strs[1].size()/2, strs[1].end());// end of 7th pack
    vec.insert(vec.end(), strs[2].begin(), strs[2].end());
    npack+=receiver.receive(vec.data(), vec.size());
    //<--8 pack
    cout<<"<<"<<npack<<" pack\n";

    //bin data
    cout<<"MIXED DATA"<<endl;
    char bin1[] = {'$', 5,0,0,0, (char)0xA1, (char)0xA2, (char)0xA3, (char)0xA4, (char)0xA5};
    vec.erase(vec.begin(), vec.end());
    vec.insert(vec.end(), bin1, bin1+sizeof(bin1));
    npack+=receiver.receive(vec.data(), vec.size());
    //<--9 pack
    cout<<"<<"<<npack<<" pack\n";
    char bin2[] = {'$', 10,0,0,0, (char)0xB1, (char)0xB2, (char)0xB3, (char)0xB4, (char)0xB5,
            (char)0xB6, (char)0xB7, (char)0xB8, (char)0xB9, (char)0xC0};
    //vec.erase(vec.begin(), vec.end());
    vec.insert(vec.end(), bin2, bin2+sizeof(bin2));
    npack+=receiver.receive(vec.data(), vec.size());
    //<--11 pack
    cout<<"<<"<<npack<<" pack\n";
    vec.erase(vec.begin(), vec.end());
    vec.insert(vec.end(), bin1, bin1+sizeof(bin1));
    vec.insert(vec.end(), strs[0].begin(), strs[0].end());
    vec.insert(vec.end(), bin2, bin2+sizeof(bin2)/2);
    npack+=receiver.receive(vec.data(), vec.size());
    //<--13 pack
    cout<<"<<"<<npack<<" pack\n";
    vec.erase(vec.begin(), vec.end());
    vec.insert(vec.end(), bin2+sizeof(bin2)/2, bin2+sizeof(bin2));//end of 14th pack
    vec.insert(vec.end(), strs[1].begin(), strs[1].end()-strs[1].size()/2);//start of 15th pack
    npack+=receiver.receive(vec.data(), vec.size());
    //<--14 pack
    cout<<"<<"<<npack<<" pack\n";
    vec.erase(vec.begin(), vec.end());
    vec.insert(vec.end(), strs[1].end()-strs[1].size()/2, strs[1].end());// end of 15th pack
    vec.insert(vec.end(), strs[2].begin(), strs[2].end());
    vec.insert(vec.end(), bin1, bin1+sizeof(bin1));
    npack+=receiver.receive(vec.data(), vec.size());
    //<--17 pack
    cout<<"<<"<<npack<<" pack\n";

    return 0;
}
