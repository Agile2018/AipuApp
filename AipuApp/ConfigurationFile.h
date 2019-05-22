#ifndef ConfigurationFile_h
#define ConfigurationFile_h

#include "Configuration.h"
#include "ErrorAipuLib.h"
#include "EnumError.h"
#include "LabelError.h"

class ConfigurationFile : public Configuration
{
public:
	ConfigurationFile();
	~ConfigurationFile();
	void ParseJSONToObject();
	void ParseMapToJSON();
	void SetNameFileConfigurationVideo(string name) {
		nameFileConfigurationVideo = name;
	}

	string GetNameFileConfigurationVideo() {
		return nameFileConfigurationVideo;
	}

	void SetNameFileConfigurationFaceModel(string name) {
		nameFileConfigurationFaceModel = name;
	}

	string GetNameFileConfigurationFaceModel() {
		return nameFileConfigurationFaceModel;
	}

	void SetNameDirectoryTempWork(string name) {
		nameDirectoryTempWork = name;
	}

	string GetNameDirectoryTempWork() {
		return nameDirectoryTempWork;
	}

	void SetNameDirectoryTraining(string name) {
		nameDirectoryTraining = name;
	}
	
	string GetNameDirectoryTraining() {
		return nameDirectoryTraining;
	}

	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();

private:
	ErrorAipuLib* error = new ErrorAipuLib();
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	void ObserverError();
	void SetValueJSONToConfiguration();
	string nameFileConfigurationVideo;
	string nameFileConfigurationFaceModel;
	string nameDirectoryTempWork;
	string nameDirectoryTraining;
	string nameFileConfiguration;
	string nameDirectoryConfiguration;
	const string PARAMS = "params";
	const string FILEVIDEO = "file_video";
	const string FILEFACE = "file_face";
	const string DIRECTORYWORK = "directory_work";
	const string DIRECTORYTRAIN = "directory_train";
	const string CONFIGURATION = "configuration";
	const string FILE_CONFIGURATION = "files_configuration";
};


#endif // !ConfigurationFile_h
