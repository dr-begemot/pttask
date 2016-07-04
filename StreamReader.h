//
//  StreamReader.h
//  pttask
//

#ifndef StreamReader_h
#define StreamReader_h

#include <vector>
#include "IStreamReader.h"

#define USUAL_SIZE 4096

class StreamReader: public IStreamReader {
public:
    virtual int receive(const char* stream, size_t bytes_read);
protected:
    std::vector<char> v;
};

#endif /* StreamReader_h */
