//
//  StreamReader.h
//  pttask
//

#ifndef StreamReader_h
#define StreamReader_h

#include <vector>

#include "IStreamReader.h"
#include "IDataHandler.h"

class StreamReader: public IStreamReader {
public:
    StreamReader(std::shared_ptr<IDataHandler> &ph): handler(ph){
        preBmBc();
    }
    ~StreamReader(){}
    
    virtual int receive(const char* stream, size_t bytes_read);
protected:
    void preBmBc();
    std::vector<char>::const_iterator find_horspool(const std::vector<char> &vec, std::vector<char>::iterator &it);

    static const char StartSymbol = 0x24;// '$'
    static const size_t Offset = 1+sizeof(uint32_t);
    const char StrEnd[4]={'\r','\n','\r','\n'};
    char bmBc[0xFF];

    // указатель на обработчик пакетов
    std::shared_ptr<IDataHandler> handler;
    // буфер для обработки и храниения "промежуточных" пакетов
    std::vector<char> buffer;
};

#endif /* StreamReader_h */
