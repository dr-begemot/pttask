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
    StreamReader(std::shared_ptr<IDataHandler> &ph): handler(ph){
        preBmBc();
    }
    ~StreamReader(){}
    
    virtual int receive(const char* stream, size_t bytes_read);
    int receive_via_string(const char *stream, size_t bytes_read);
protected:
    void preBmBc();
    std::vector<char>::const_iterator find_horspool(const std::vector<char> &vec, std::vector<char>::iterator &it);
    std::vector<char>::iterator incit(std::vector<char> &vec, std::vector<char>::iterator &it);

    int receive_fast(const char *stream, size_t bytes_read);
    int receive_memeco(const char *stream, size_t bytes_read);

    static const char StartSymbol = 0x24;
    static const size_t Offset = 1+sizeof(uint32_t);
    const char StrEnd[4]={'\r','\n','\r','\n'};
    char bmBc[0xFF];

    std::vector<char> buffer;
    std::shared_ptr<IDataHandler> handler;
};

#endif /* StreamReader_h */
