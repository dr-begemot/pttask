//
//  main.cpp
//  pttask
//

#include <iostream>

#include "IDataHandler.h"
#include "StreamReader.h"

using namespace std;

class MyDataHandler: public IDataHandler {
public:
    void handle_binary_data(const char* data, size_t size){
        cout<<">>bin data ("<<size<<" bytes)"<<endl;
    }
    void handle_text_data(const char* data, size_t size){
        cout<<">>str data ("<<size<<" bytes): "<<endl;
        for(size_t i=0; i<size; i++){
            if (data[i] == '\r')
                cout<<"\r";
            else if (data[i] == '\n')
                cout<<"\n";
            else
                cout<<data[i];
        }
        cout<<endl;

    }
};

//void push_str_back

int main(int argc, const char * argv[]) {
    // insert code here...
    cout << "Hello, World!\n";
    shared_ptr<IDataHandler> handler(new MyDataHandler());
    shared_ptr<StreamReader> receiver(new StreamReader(handler));

    /*vector<char> vec;
    const char t1[] = "test1\r\n\r\n"{5,0,0,0, 't','2','A','B','C'};
    vec.insert((buffer.end(), stream, &stream[bytes_read-1]);
    const char t2[] = {'$',5,0,0,0, 't','2','A','B','C'};//*/

    vector<vector<char>> vec;
    //vec.push_back(string("test1\r\n\r\n").)
    return 0;
}
