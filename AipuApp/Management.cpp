#include "Management.h"

Management::Management()
{
	SetDirectoryConfiguration();
	ObserverError();
	ObserverVideo();	
}

Management::~Management()
{
}

void Management::SetDirectoryConfiguration() {
	faceModel->configuration->SetNameDirectory(DIRECTORY_CONFIGURATION);
	video->configuration->SetNameDirectory(DIRECTORY_CONFIGURATION);
}

void Management::ObserverError() {

	auto faceModelError = faceModel->error->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionFaceModelError = faceModelError.subscribe([this](Either* either) {		
		cout << either->GetLabel() << endl;
		shootError.on_next(either);
	});

	auto configurationFaceModelError = faceModel->configuration->error->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionConfigurationFaceModelError = configurationFaceModelError.subscribe([this](Either* either) {
		cout << either->GetLabel() << endl;
		shootError.on_next(either);

	});

	auto videoError = video->error->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionVideoError = videoError.subscribe([this](Either* either) {
		cout << either->GetLabel() << endl;
		shootError.on_next(either);
	});

	auto configurationVideoError = video->configuration->error->observableError.map([](Either* either) {
		return either;
	});

	auto subscriptionConfigurationVideoError = configurationVideoError.subscribe([this](Either* either) {
		cout << either->GetLabel() << endl;
		shootError.on_next(either);

	});

	auto flowTrendError = flowTrend->error->observableError.map([](Either* either) {		
		return either;
	});

	auto subscriptionFlowTrendError = flowTrendError.subscribe([this](Either* either) {
		cout << either->GetLabel() << endl;
		//shootError.on_next(either);
	});


}

void Management::SaveDataTraining(int quantityDetected) {
	std::time_t rawtime = std::time(nullptr);
	struct tm *timeinfo;
	localtime_s(timeinfo, &rawtime);
	int hour = timeinfo->tm_hour;
	int day = timeinfo->tm_wday;
	int min = (hour * SIXTY_MINUTES) + timeinfo->tm_min;
	string fileName = to_string(day) + "-" + to_string(hour) + ".txt";
	train->SetMinute(min);
	train->SetQuantityOfImages(quantityDetected);
	train->WriteDataTraining(fileName);

}

void Management::VerifyTrainingLapse() {
	std::chrono::steady_clock::time_point endCountTime = std::chrono::steady_clock::now();
	int diff = std::chrono::duration_cast<std::chrono::minutes>(endCountTime - startCountTime).count();
	if (diff >= ONE_MINUTE)
	{
		std::thread(&Management::SaveDataTraining, this,
			countImagesDetected).detach();
		startCountTime = std::chrono::steady_clock::now();
		countImagesDetected = 0;
		std::time_t rawtime = std::time(nullptr);
		struct tm *timeinfo;
		localtime_s(timeinfo, &rawtime);
		int hour = timeinfo->tm_hour;
		int min = (hour * SIXTY_MINUTES) + timeinfo->tm_min;

		workMode = SetStateFlow(min + ONE_MINUTE);

	}

}

int Management::SetStateFlow(int minute) {
	int valueTrend = flowTrend->GetFlowTrendForMinute(minute);
	video->ResetCountProcessedImages();
	faceModel->ResetCountProcessedImages();
	
	if (timeDurationSingleDetection != 0.0)
	{
		int imagesWait = static_cast<int>((SIXTY_SECONDS / timeDurationSingleDetection) / 2);

		if (valueTrend > imagesWait)
		{

			return SINGLE;
		}
		if (valueTrend <= imagesWait && valueTrend != 0)
		{
			return OPTION_BATCH;
		}
		else
		{
			return MOVEMENT;
		}

	}
	return SINGLE;

}

void Management::GetModelOneToOne(Mat image) {
	video->WriteImageOnDisk(image);

	chrono::steady_clock sc;
	auto start = sc.now();
	int count = faceModel->ModelOneToOne();
	if (count != 0)
	{
		auto end = sc.now();
		auto time_span = static_cast<chrono::duration<double>>(end - start);
		timeDurationSingleDetection = time_span.count();

	}
	countImagesDetected += count;

}

void Management::GetModelsByBatch(Mat image) {
	video->WriteBatchOfImagesOnDisk(image);
	if (video->GetProcessedImages() % MULTIPLE_OF_TEN == 0) {
		countImagesDetected += faceModel->ModelByBatch();
	}

}

void Management::ProcessImage(Mat image) {
	VerifyTrainingLapse();

	switch (workMode)
	{
	case SINGLE:
		GetModelOneToOne(image);
		cout << "SINGLE DETECTION" << endl;
		break;
	case OPTION_BATCH:
		GetModelsByBatch(image);
		cout << "BATCH DETECTION" << endl;
		break;
	case MOVEMENT:
		if (movement->SetImage(image))
		{
			if (movement->IsThereMovement()) {
				workMode = SINGLE;
			}
			cout << "MOVEMENT DETECTION" << endl;
		}
		break;
	default:
		break;
	}

	flagNextFrame = false;

}

void Management::ObserverVideo() {
	auto frame
		= video->observableImage.map([](Mat image) {
		return image;
	});

	auto subscription = frame.subscribe([this](Mat image) {
		cv::resize(image, image, cv::Size(), 0.5, 0.5, INTER_LINEAR_EXACT);

		if (!flagNextFrame) {
			flagNextFrame = true;

			std::thread squ(&Management::ProcessImage, this, image);
			squ.detach();

		}		
		//frameOut.on_next(p);
	});
}

void Management::RunVideo() {
	video->RunVideo();
}