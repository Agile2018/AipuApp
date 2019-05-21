#ifndef Error_h
#define Error_h

#include "Codes.h"
#include "rx.hpp"
#include "Either.h"
namespace Rx {
	using namespace rxcpp;
	using namespace rxcpp::subjects;
	using namespace rxcpp::operators;
	using namespace rxcpp::util;

}

class Error
{
public:
	enum ErrorWeight
	{
		gross = GROSS_ERROR,
		medium = MEDIUM_ERROR,
		less = LESS_ERROR,
		none = PROCESS_OK
	};
	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();
	virtual void CheckError(int errorCode, ErrorWeight srrorFace = ErrorWeight::none, string message = "") {};

protected:
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	Either* either = new Either();
};


#endif // !Error_h

