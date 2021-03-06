#ifndef FaceModel_h
#define FaceModel_h

#include "iface.h"
#include "ConfigurationIFace.h"
#include "ErrorFaceLib.h"
#include "Molded.h"

class FaceModel
{
public:
	FaceModel();
	~FaceModel();
	void SetNameFileImage(string name) {
		nameFileImage = name;
	}

	void SetNameDirectory(string name) {
		nameDirectory = name;
	}

	void ResetCountProcessedImages() {
		processedImages = 0;
	}

	void Terminate();
	int ModelByBatch();
	int ModelOneToOne();
	
	ConfigurationIFace* configuration = new ConfigurationIFace();
	Rx::subject<Molded*> templateImage;
	Rx::observable<Molded*> observableTemplate = templateImage.get_observable();
	Rx::subject<Either*> errorSubject;
	Rx::observable<Either*> observableError = errorSubject.get_observable();
	/*Rx::subject<vector<Molded*>> modelList;
	Rx::observable<vector<Molded*>> observableModelList = modelList.get_observable();*/
private:
	const string nameFileCropImage = "imgCrop.png";
	ErrorFaceLib* error = new ErrorFaceLib();
	Rx::subscriber<Either*> shootError = errorSubject.get_subscriber();
	int batchTotalSize = BATCH_TOTAL_SIZE;
	int processedImages = 0;
	string nameFileImage;
	string nameDirectory;
	Rx::subscriber<Molded*> templateOut = templateImage.get_subscriber();
	void FaceCropImage(void* face, string pathImage);
	//Rx::subscriber<vector<Molded*>> modelListOut = modelList.get_subscriber();
	int DetectByBatch(void* facesDetected[BATCH_SIZE]);
	void GetBatchModels(int countFacesDetected, void* facesDetected[BATCH_SIZE]);
	void CreateTemplate(void* face);
	int GetOneModel(unsigned char* rawImage, int width, int height);
	void ObserverError();
};


#endif // !FaceModel_h

