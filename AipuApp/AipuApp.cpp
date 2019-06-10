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
	management->SetNameDatabase("dbass");
	management->SetConnectStringDatabase("mongodb://localhost:27017");
	management->SaveConfigurationDatabase("database1.txt");
	management->SetNameDirectoryWorkTemporal("camera1");
	management->SetNameDirectoryTraining("train1");
	management->SaveConfigurationFile("directory1.txt");
}

void WriteConfiguration2() {
	Management* management = new Management();
	auto observerError = management->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionError = observerError.subscribe([](Either* either) {
		cout << either->GetLabel() << endl;

	});
	management->SetPathVideo("rtsp://192.168.0.12:554/axis-media/media.amp?videocodec=h264&resolution=640x480");
	management->SetFPSVideo("40");
	management->SetImageTempVideo("imgtemp2.png");
	management->SetPasswordVideo("admin");
	management->SetUserVideo("root");
	management->SetResolutionVideo(""); //1280x960
	management->SaveConfigurationVideo("video2.txt");
	management->SetMaxDetect(1);
	management->SetMaxEyeDistance(200);
	management->SetMinEyeDistance(25);
	management->SetPrecision(600);
	management->SaveConfigurationFaceModel("detect2.txt");
	management->SetNameDatabase("dbass");
	management->SetConnectStringDatabase("mongodb://localhost:27017");
	management->SaveConfigurationDatabase("database2.txt");
	management->SetNameDirectoryWorkTemporal("camera2");
	management->SetNameDirectoryTraining("train2");
	management->SaveConfigurationFile("directory2.txt");
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

void RunVideo1() {
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
		cv::imshow("video1", img);
	});

	auto observerDatabase = management->observableUserJSON.map([](string jsonUser) {
		return jsonUser;
	});

	auto subscriptionDatabase = observerDatabase.subscribe([](string jsonUser) {
		const char *cstr = jsonUser.c_str();
		printf("%s \n", cstr);

	});

	management->LoadConfiguration("directory1.txt");
	management->RunVideo();

}

void RunVideo2() {
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
		cv::imshow("video2", img);
	});

	management->LoadConfiguration("directory2.txt");
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
	//WriteConfiguration2();
	//ReadConfiguration();
	//SetDirectoryWork();
	//InitTrain();
	InitInnovatricsLibrary();
	RunVideo1();
	/*std::thread sv1(RunVideo1);
	std::thread sv2(RunVideo2);
	sv1.join();
	sv2.join();*/
	
  /*  std::cout << "Verify Work!\n"; 
	cin.clear();
	cout << endl << "Press any key to continue...";
	cin.ignore();*/

}
