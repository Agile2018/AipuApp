#include <iostream>
#include "Management.h"
#include "Innovatrics.h"

void WriteConfiguration() {
	Management* management = new Management();
	auto observerError = management->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionError = observerError.subscribe([](Either* either) {
		cout << either->GetLabel() << endl;		

	});
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
	management->SetNameDirectoryWorkTemporal("camera1");
	management->SetNameDirectoryTraining("train1");
	management->SaveConfigurationFile("directory1.txt");
}

void ReadConfiguration() {
	Management* management = new Management();
	auto observerError = management->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionError = observerError.subscribe([](Either* either) {
		string s = either->GetLabel();
		const char *cstr = s.c_str();
		printf("%s \n", cstr);

	});
	management->LoadConfiguration("directory1.txt");
	cout << management->GetStringJSONVideo() << endl;
	cout << management->GetStringJSONFaceModel() << endl;
	cout << management->GetStringJSONFiles() << endl;
}

void RunVideo() {
	Management* management = new Management();
	auto observerError = management->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionError = observerError.subscribe([](Either* either) {
		string s = either->GetLabel();
		const char *cstr = s.c_str();
		printf("%s \n", cstr);

	});
	auto frame_input
		= management->observableFrame.map([](Mat img) {
		return img;
	});

	auto subscriptionFrame = frame_input.subscribe([](Mat img) {
		cv::imshow("video", img);
	});

	management->LoadConfiguration("directory1.txt");
	management->RunVideo();

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
	//InitTrain();
	InitInnovatricsLibrary();
	RunVideo();
	
 //   std::cout << "Verify Work!\n"; 
	//cin.clear();
	//cout << endl << "Press any key to continue...";
	//cin.ignore();

}
