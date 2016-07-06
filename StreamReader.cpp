//
//  StreamReader.cpp
//  pttask
//
#include <iostream>
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include "StreamReader.h"

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
