#ifndef Management_h
#define Management_h

#include "FaceModel.h"
#include "FlowTrend.h"
#include "Train.h"
#include "Movement.h"
#include "Video.h"
#include "ConfigurationFile.h"
#include <ctime>

const string DIRECTORY_CONFIGURATION = "configuration";

using namespace std;

class Management
{
public:
	Management();
	~Management();

	void LoadConfiguration(string nameFile) {
		configurationFile->SetNameFileConfiguration(nameFile);
		configurationFile->ParseJSONToObject();
		SetNameFileConfigurationVideo(configurationFile->GetNameFileConfigurationVideo());
		SetNameFileConfigurationFace(configurationFile->GetNameFileConfigurationFaceModel());
		video->SetNameDirectory(configurationFile->GetNameDirectoryTempWork());
		faceModel->SetNameDirectory(configurationFile->GetNameDirectoryTempWork());
		train->CreateTrainingDirectory(configurationFile->GetNameDirectoryTraining());
		flowTrend->SetDirectory(configurationFile->GetNameDirectoryTraining());

	}

	void SetNameDirectoryWorkTemporal(string name) {
		configurationFile->SetNameDirectoryTempWork(name);

	}

	void SetNameDirectoryTraining(string name) {		
		configurationFile->SetNameDirectoryTraining(name);
		//flowTrend->Init();
	}

	void SetPathVideo(string valuePath) {
		video->configuration->SetPath(valuePath);
	}
	void SetUserVideo(string valueUser) {
		video->configuration->SetUser(valueUser);
	}
	void SetPasswordVideo(string valuePassword) {
		video->configuration->SetPassword(valuePassword);
	}
	void SetResolutionVideo(string valueResolution) {
		video->configuration->SetResolution(valueResolution);
	}
	void SetFPSVideo(string valueFps) {
		video->configuration->SetFPS(valueFps);
	}
	void SetImageTempVideo(string name) {
		video->configuration->SetImageTemp(name);
	}

	void SaveConfigurationVideo(string nameFile) {
		video->configuration->SetNameFileConfiguration(nameFile);
		video->configuration->ParseMapToJSON();		
		configurationFile->SetNameFileConfigurationVideo(nameFile);
	}

	void SetMinEyeDistance(int minDistance) {
		faceModel->configuration->SetMinEyeDistance(minDistance);
	}
	void SetMaxEyeDistance(int maxDistance) {
		faceModel->configuration->SetMaxEyeDistance(maxDistance);
	}

	void SetMaxDetect(int maxFacesDetect) {
		faceModel->configuration->SetMaxDetect(maxFacesDetect);
	}

	void SetPrecision(int precision) {
		faceModel->configuration->SetPrecision(precision);
	}

	void SaveConfigurationFaceModel(string nameFile) {
		faceModel->configuration->SetNameFileConfiguration(nameFile);
		faceModel->configuration->ParseMapToJSON();
		configurationFile->SetNameFileConfigurationFaceModel(nameFile);
	}

	void SaveConfigurationFile(string nameFile) {
		configurationFile->SetNameFileConfiguration(nameFile);
		configurationFile->ParseMapToJSON();
	}

	string GetStringJSONVideo() {
		return video->configuration->GetStringJSON();
	}

	string GetStringJSONFaceModel() {
		return faceModel->configuration->GetStringJSON();
	}
	string GetStringJSONFiles() {
		return configurationFile->GetStringJSON();
	}

	void RunVideo();

	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();

	Rx::subject<Mat> frame;
	Rx::observable<Mat> observableFrame = frame.get_observable();

private:
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	Rx::subscriber<Mat> frameOut = frame.get_subscriber();
	ConfigurationFile* configurationFile = new ConfigurationFile();
	FaceModel* faceModel = new FaceModel();
	Video* video = new Video();
	FlowTrend* flowTrend = new FlowTrend();
	Train* train = new Train();
	Movement* movement = new Movement();
	std::chrono::steady_clock::time_point startCountTime = std::chrono::steady_clock::now();
	int countImagesDetected = 0;
	int workMode = SINGLE;
	bool flagNextFrame = false;
	double timeDurationSingleDetection = 0;
	void SetDirectoryConfiguration();
	void ObserverError();
	void SaveDataTraining(int quantityDetected, int day, int hour, int minute);
	void VerifyTrainingLapse();
	int SetStateFlow(int minute);
	void GetModelOneToOne(Mat image);
	void GetModelsByBatch(Mat image);
	void ProcessImage(Mat image);
	void ObserverVideo();
	void ObserverTemplateImage();
	void SetNameFileConfigurationVideo(string name) {
		video->configuration->SetNameFileConfiguration(name);
		video->configuration->ParseJSONToObject();
		faceModel->SetNameFileImage(video->configuration->GetImageTemp());

	}

	void SetNameFileConfigurationFace(string name) {
		faceModel->configuration->SetNameFileConfiguration(name);
		faceModel->configuration->ParseJSONToObject();

	}


};

#endif // !Management_h

