#ifndef ErrorAipuLib_h
#define ErrorAipuLib_h

#include "Error.h"
#include <iostream>  
#include <string>

using namespace std;

class ErrorAipuLib: public Error
{
public:
	ErrorAipuLib();
	~ErrorAipuLib();
	void CheckError(int errorCode, ErrorWeight errorWeight = ErrorWeight::none,
		string message = "");
private:

};


#endif // !ErrorAipuLib_h

