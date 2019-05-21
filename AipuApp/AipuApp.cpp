// AipuApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//#include "pch.h"
#include <iostream>
#include "Management.h"

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
	management->SetNameDirectoryTraining("train1");
	/*cin.clear();
	cout << endl << "Press any key to continue...";
	cin.ignore();*/
}

int main()
{	
	//WriteConfiguration();
	//ReadConfiguration();
	//SetDirectoryWork();
	InitTrain();
	
    std::cout << "Verify Work!\n"; 
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
