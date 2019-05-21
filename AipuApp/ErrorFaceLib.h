#ifndef ErrorFaceLib_h
#define ErrorFaceLib_h

#include "Error.h"
#include "iface.h"

class ErrorFaceLib: public Error
{
public:
	ErrorFaceLib();
	~ErrorFaceLib();
	void CheckError(int errorCode, ErrorWeight errorWeight = ErrorWeight::none, string message = "");

private:
	void BuildMessageErrorLicense(int errorCode, ErrorWeight errorWeight);
	void BuildMessageOtherError(int errorCode, ErrorWeight errorWeight);
	void BuildMessageOk();
};

#endif // !ErrorFaceLib_h

