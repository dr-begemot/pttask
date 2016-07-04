//
//  IStreamReader.h
//  pttask
//

#ifndef IStreamReader_h
#define IStreamReader_h

class IStreamReader {
public:
    virtual int receive(const char* stream, size_t bytes_read) = 0;
};

#endif /* IStreamReader_h */
