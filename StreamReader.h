//
//  StreamReader.h
//  pttask
//

#ifndef StreamReader_h
#define StreamReader_h

#include <vector>

#include "IStreamReader.h"
#include "IDataHandler.h"

#define USUAL_SIZE 4096

class StreamReader: public IStreamReader {
public:
    StreamReader(std::shared_ptr<IDataHandler> h): handler(h){};
    
    virtual int receive(const char* stream, size_t bytes_read);
protected:
    std::vector<char> buffer;
    std::shared_ptr<IDataHandler> handler;
};

#endif /* StreamReader_h */
