//
//  StreamReader.cpp
//  pttask
//
#include <iostream>
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include "StreamReader.h"
int StreamReader::receive_via_string(const char *stream, size_t bytes_read){
    size_t npos=0;
    uint32_t packlen;
    std::string s(stream, bytes_read);
    std::string::iterator it=str.begin();
    while(it<=str.end()){
        if(*it==StartSymbol){
            packlen = reinterpret_cast<uint32_t*>(&stream[i+1]);
            if(i+packlen<=bytes_read){
                handler->handle_binary_data(&s.c_str()[npos+1+sizeof(uint32_t)], packlen);
                npos+=packlen;
            }
        }
    }
}
int StreamReader::receive(const char *stream, size_t bytes_read){
    std::string s()
    size_t strstart;
    uint32_t packlen;
    buffer.reserve(buffer.size()+bytes_read);

    s()

    for(size_t i=0; i<bytes_read;){
        if(stream[i]==StartSymbol){
            packlen = reinterpret_cast<uint32_t*>(&stream[i+1]);
            if(i+packlen<=bytes_read){

            }
        }else{
            strstart = 0;

        }
    }
}
