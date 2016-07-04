//
//  IDataHandler.h
//  pttask
//

#ifndef IDataHandler_h
#define IDataHandler_h

class IDataHandler {
public:
	virtual ~IDataHandler();
	virtual void handle_binary_data(const char* data, size_t size) = 0;
	virtual void handle_text_data(const char* data, size_t size) = 0;
};

#endif /* IDataHandler_h */
