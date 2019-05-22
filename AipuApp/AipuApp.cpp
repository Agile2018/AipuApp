#include <iostream>
#include "Management.h"
#include "Innovatrics.h"

void WriteConfiguration() {
	Management* management = new Management();
	management->SetPathVideo("rtsp://192.168.0.8:554");
	management->SetFPSVideo("30");
	management->SetImageTempVideo("imgtemp1.png");
	management->SetPasswordVideo("admin");
	management->SetUserVideo("admin");
	management->SaveConfigurationVideo("video1.txt");
	management->SetMaxDetect(1);
	management->SetMaxEyeDistance(200);
	management->SetMinEyeDistance(25);
	management->SetPrecision(600);
	management->SaveConfigurationFaceModel("detect1.txt");
}

void ReadConfiguration() {
	Management* management = new Management();
	management->SetNameFileConfigurationFace("detect1.txt");
	management->SetNameFileConfigurationVideo("video1.txt");
	cout << management->GetStringJSONVideo() << endl;
	cout << management->GetStringJSONFaceModel() << endl;
}

void SetDirectoryWork() {
	Management* management = new Management();
	management->SetNameDirectoryWorkTemporal("camera1");
}

void InitTrain() {
	Management* management = new Management();
	auto observerError = management->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionError = observerError.subscribe([](Either* either) {
		cout << either->GetLabel() << endl;
		string s = either->GetLabel();
		const char *cstr = s.c_str();
		printf("%s \n", cstr);

	});
	management->SetNameDirectoryTraining("train1");
}

void InitInnovatricsLibrary() {
	Innovatrics* innovatrics = new Innovatrics();
	
	auto observerError = innovatrics->observableError.map([](Either* either){
		return either;
	});

	auto subscriptionError = observerError.subscribe([](Either* either) {
		cout << either->GetLabel() << endl;
	});
	innovatrics->SetParamsLibrary();
	innovatrics->InitLibrary();
}

int main()
{	
	//WriteConfiguration();
	//ReadConfiguration();
	//SetDirectoryWork();
	InitTrain();
	//InitInnovatricsLibrary();
	
	
    std::cout << "Verify Work!\n"; 
	cin.clear();
	cout << endl << "Press any key to continue...";
	cin.ignore();

}
