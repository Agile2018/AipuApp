#include "ErrorAipuLib.h"

ErrorAipuLib::ErrorAipuLib()
{
}

ErrorAipuLib::~ErrorAipuLib()
{
}

void ErrorAipuLib::CheckError(int errorCode, ErrorWeight errorWeight,
	string message) {
	message += ", Code: " + to_string(errorCode);
	either->Clear();
	either->SetCode(errorWeight);
	either->SetLabel(message);
	shootError.on_next(either);

}