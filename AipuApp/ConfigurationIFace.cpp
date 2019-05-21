#include "ConfigurationIFace.h"

ConfigurationIFace::ConfigurationIFace()
{
}

ConfigurationIFace::~ConfigurationIFace()
{
}

void ConfigurationIFace::SetValueJSONToConfiguration() {
	const Json::Value params = jsonBody[PARAMS];
	for (Json::Value::const_iterator it = params.begin();
		it != params.end(); ++it) {

		if (it.key().asString() == MAXFACES) {
			maxDetect = it->asInt();
		}
		else if (it.key().asString() == MINEYE) {
			minEyeDistance = it->asInt();
		}
		else if (it.key().asString() == MAXEYE) {
			maxEyeDistance = it->asInt();
		}
		else if (it.key().asString() == PRECISION) {
			accuracy = it->asInt();
		}

	}

}

void ConfigurationIFace::ParseJSONToObject() {
	jsonBody.clear();
	stringJSON = manageFile->ReadFileText();
	bool parsingSuccessful = reader.parse(stringJSON, jsonBody);
	if (parsingSuccessful)
	{
		SetValueJSONToConfiguration();
	}
	else {
		error->CheckError(ERROR_PARSING, error->medium, LABEL_ERROR_PARSING);
	}

}

void ConfigurationIFace::ParseMapToJSON() {
	jsonBody.clear();
	jsonParams.clear();
	std::map<std::string, std::int16_t> params;
	params.insert(std::pair<std::string, std::int16_t>(MAXFACES, maxDetect));
	params.insert(std::pair<std::string, std::int16_t>(MINEYE, minEyeDistance));
	params.insert(std::pair<std::string, std::int16_t>(MAXEYE, maxEyeDistance));
	params.insert(std::pair<std::string, std::int16_t>(PRECISION, accuracy));
	std::map<std::string, std::int16_t>::const_iterator it = params.begin(),
		end = params.end();
	for (; it != end; ++it) {
		jsonParams[it->first] = it->second;

	}

	jsonBody[CONFIGURATION] = DETECT_CONFIGURATION;
	jsonBody[PARAMS] = jsonParams;

	SaveConfiguration();

}