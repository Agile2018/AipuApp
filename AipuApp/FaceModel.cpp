#include "FaceModel.h"

FaceModel::FaceModel()
{
	ObserverError();
}

FaceModel::~FaceModel()
{
	Terminate();
}

void FaceModel::ObserverError() {
	auto observerError = error->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionError = observerError.subscribe([this](Either* either) {
		//printf("%s \n", either->GetLabel());
		shootError.on_next(either);
	});

	auto observerErrorConfiguration = configuration->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionErrorConfiguration = observerErrorConfiguration.subscribe([this](Either* either) {
		//printf("%s \n", either->GetLabel());
		shootError.on_next(either);
	});

}

void FaceModel::Terminate() {
	int errorCode = IFACE_Terminate();
	error->CheckError(errorCode, error->less);

}

int FaceModel::DetectByBatch(void* facesDetected[BATCH_SIZE]) {
	int errorCode, countFacesDetected = 0;
	void* faceHandler;

	errorCode = IFACE_CreateFaceHandler(&faceHandler);
	error->CheckError(errorCode, error->medium);

	for (int i = 0; i < BATCH_SIZE; i++)
	{

		errorCode = IFACE_CreateFace(&(facesDetected[i]));
		error->CheckError(errorCode, error->medium);
	}

	for (int i = 0; i < BATCH_SIZE; i++)
	{
		int lenght, widthFace, heightFace;

		processedImages++;
		string fileImage = nameDirectory + "/" + to_string(processedImages) + nameFileImage;

		errorCode = IFACE_LoadImage(fileImage.c_str(), &widthFace, &heightFace, &lenght, NULL);
		if (errorCode == IFACE_OK) {
			unsigned char* rawImage = new unsigned char[lenght];
			errorCode = IFACE_LoadImage(fileImage.c_str(), &widthFace,
				&heightFace, &lenght, rawImage);
			error->CheckError(errorCode, error->medium);
			void* faceTemp;
			errorCode = IFACE_CreateFace(&(faceTemp));
			error->CheckError(errorCode, error->medium);
			int detectedFaces = configuration->GetMaxDetect();
			errorCode = IFACE_DetectFaces(rawImage, widthFace, heightFace,
				configuration->GetMinEyeDistance(), configuration->GetMaxEyeDistance(),
				faceHandler, &detectedFaces, &faceTemp);
			delete rawImage;
			error->CheckError(errorCode, error->medium);

			if (detectedFaces != EMPTY_FACE)
			{
				errorCode = IFACE_CloneEntity(faceTemp, facesDetected[countFacesDetected]);
				error->CheckError(errorCode, error->medium);
				countFacesDetected++;
			}
			errorCode = IFACE_ReleaseEntity(faceTemp);
			error->CheckError(errorCode, error->medium);

		}
		else {
			error->CheckError(errorCode, error->medium);
		}
		

	}
	errorCode = IFACE_ReleaseEntity(faceHandler);
	error->CheckError(errorCode, error->medium);

	return countFacesDetected;
}

int FaceModel::ModelByBatch() {
	int errorCode;
	void* facesDetected[BATCH_SIZE];
	int countFacesDetected = DetectByBatch(facesDetected);
	if (countFacesDetected != 0)
	{
		GetBatchModels(countFacesDetected, facesDetected);
	}	

	if (processedImages == batchTotalSize)
	{
		processedImages = 0;
	}

	for (int i = 0; i < BATCH_SIZE; i++)
	{
		errorCode = IFACE_ReleaseEntity(facesDetected[i]);
		error->CheckError(errorCode, error->medium);
	}
	return countFacesDetected;
}

void FaceModel::GetBatchModels(int countFacesDetected, void* facesDetected[BATCH_SIZE]) {
	int errorCode;
	int templateBatchDataSize;

	char** batchTemplates = new char*[BATCH_SIZE];
	void* faceHandler;

	errorCode = IFACE_CreateFaceHandler(&faceHandler);
	error->CheckError(errorCode, error->medium);

	errorCode = IFACE_CreateTemplateBatch(faceHandler, BATCH_SIZE, facesDetected,
		&templateBatchDataSize, NULL);
	error->CheckError(errorCode, error->medium);

	for (int i = 0; i < BATCH_SIZE; ++i)
	{

		batchTemplates[i] = new char[templateBatchDataSize];
	}

	errorCode = IFACE_CreateTemplateBatch(faceHandler, BATCH_SIZE, facesDetected,
		&templateBatchDataSize, batchTemplates);

	error->CheckError(errorCode, error->medium);

	for (int i = 0; i < countFacesDetected; i++)
	{
		Molded* model = new Molded();
		model->SetMoldImage(batchTemplates[i]);
		model->SetMoldSize(templateBatchDataSize);
		templateOut.on_next(model);
	}

	for (int i = 0; i < BATCH_SIZE; ++i)
	{
		delete batchTemplates[i];
	}
	errorCode = IFACE_ReleaseEntity(faceHandler);
	error->CheckError(errorCode, error->medium);
}

int FaceModel::ModelOneToOne() {
	int lenght, width, height, errorCode, templates;
	string fileImage = nameDirectory + "/" + nameFileImage;
	errorCode = IFACE_LoadImage(fileImage.c_str(), &width, &height, &lenght, NULL);
	if (errorCode == IFACE_OK)
	{
		unsigned char* rawImage = new unsigned char[lenght];
		errorCode = IFACE_LoadImage(fileImage.c_str(), &width,
			&height, &lenght, rawImage);
		if (errorCode != IFACE_OK)
		{
			error->CheckError(errorCode, error->medium);
		}
		else
		{
			templates = GetOneModel(rawImage, width, height);
		}
		delete rawImage;
	}
	else {
		error->CheckError(errorCode, error->medium);
	}
	return templates;
}

void FaceModel::CreateTemplate(void* face) {
	int errorCode;
	int templateSize;
	void* faceHandler;	

	errorCode = IFACE_CreateFaceHandler(&faceHandler);
	error->CheckError(errorCode, error->medium);
	errorCode = IFACE_CreateTemplate(face, faceHandler, 0, &templateSize, NULL);
	if (errorCode == IFACE_OK) {
		char* templateData = new char[templateSize];
		errorCode = IFACE_CreateTemplate(face, faceHandler, 0,
			&templateSize, templateData);
		if (errorCode != IFACE_OK) {
			error->CheckError(errorCode, error->medium);
		}
		else
		{
			Molded* model = new Molded();
			model->SetMoldImage(templateData);
			model->SetMoldSize(templateSize);
			templateOut.on_next(model);
		}
		delete[] templateData;
	}
	else {
		error->CheckError(errorCode, error->medium);
	}
	errorCode = IFACE_ReleaseEntity(faceHandler);
	error->CheckError(errorCode, error->medium);

}

int FaceModel::GetOneModel(unsigned char* rawImage, int width, int height) {
	int detectedFaces = configuration->GetMaxDetect();
	int errorCode;
	void* faceTemp;
	void* faceHandler;

	errorCode = IFACE_CreateFaceHandler(&faceHandler);
	error->CheckError(errorCode, error->medium);
	errorCode = IFACE_CreateFace(&(faceTemp));
	error->CheckError(errorCode, error->medium);
	errorCode = IFACE_DetectFaces(rawImage, width, height,
		configuration->GetMinEyeDistance(), configuration->GetMaxEyeDistance(), 
		faceHandler, &detectedFaces, &faceTemp);
	
	if (errorCode == IFACE_OK) {
		if (detectedFaces != EMPTY_FACE)
		{
			CreateTemplate(faceTemp);
		}
	}
	else {
		error->CheckError(errorCode, error->medium);
	}

	errorCode = IFACE_ReleaseEntity(faceTemp);
	error->CheckError(errorCode, error->medium);
	errorCode = IFACE_ReleaseEntity(faceHandler);
	error->CheckError(errorCode, error->medium);

	return detectedFaces;
}