#include "Video.h"

Video::Video()
{
}

Video::~Video()
{
}

void Video::CheckFramesBySecond(VideoCapture videoCapture) {
	if (configuration->GetFPS().empty()) {
		int fps = (int)(videoCapture.get(CAP_PROP_FPS));
		if (fps < 0) {
			videoCapture.set(CAP_PROP_FPS, FPS);	
			configuration->SetFPSIntToString(FPS);
		}
	}

}

void Video::RunVideo() {
	if (configuration->BuildPathVideo()) {
		VideoCapture capture;
		Mat frame;
		bool flagCapture = true;
		if (configuration->GetDeviceId() != NOT_DEVICE) {
			capture.open(configuration->GetDeviceId());
		}
		else {
			capture.open(configuration->GetURLVideo());
		}

		if (!capture.isOpened()) {
			error->CheckError(ERROR_FILE_NOT_FOUND, 
				error->medium, LABEL_ERROR_FILE_NOT_FOUND);			
			return;
		}
		CheckFramesBySecond(capture);
		int frameBySecond = configuration->GetFPSStringToInt();
		while (flagCapture) {
			if (!capture.read(frame)) {
				error->CheckError(ERROR_UNABLE_READ_NEXT, 
					error->medium, LABEL_ERROR_UNABLE_READ_NEXT);
				return;
			}
			else {
				frameout.on_next(frame);				
				if (waitKey(MILI_SECONDS / frameBySecond) >= 0) {
					flagCapture = false;
				}
			}
		}
		capture.release();
	}

}

void Video::WriteImageOnDisk(Mat frame) {
	string path = nameDirectory + "/" +
		configuration->GetImageTemp();
	imwrite(path, frame);

}
void Video::WriteBatchOfImagesOnDisk(Mat frame) {
	processedImages++;
	string fileTemp = nameDirectory + "/" +
		to_string(processedImages) + configuration->GetImageTemp();
	imwrite(fileTemp, frame);
	if (processedImages == batchTotalSize)
	{
		processedImages = 0;
	}

}

