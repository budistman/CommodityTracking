#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <CommodityTracking.h>

int main(int argc, char** argv) {
	VideoCapture stream(0);
	FrameHistory history(stream);

	int minimumArclength = 150;
	int userSensitivity = 255;
	int limbGracePeriod = 50;

	autoCalibrateSensitivity(&userSensitivity, stream, history, minimumArclength, 1, limbGracePeriod);

	int showOriginal = 0, showSkeleton = 0, _flip = 0;

	namedWindow("Settings", 1);
	createTrackbar("Minimum Arc Length", "Settings", &minimumArclength, 500);
	createTrackbar("Sensitivity", "Settings", &userSensitivity, 1000);

	createTrackbar("Show original?", "Settings", &showOriginal, 1);
	createTrackbar("Show skeleton?", "Settings", &showSkeleton, 1);
	createTrackbar("Flip?", "Settings", &_flip, 1);

	Skeleton lastSkeleton;

	for(;;) {
		Mat visualization;

		if(showOriginal) {
			visualization = history.getLastFrame().clone();
		} else {
			visualization = Mat::zeros(history.getLastFrame().size(), CV_8UC3);
		}
		

		lastSkeleton.visualize(visualization);
		imshow("Visualization", visualization);

		if(waitKey(1) == 27) {
			break;
		}

		Skeleton skeleton = getSkeleton(stream, history, lastSkeleton, _flip, minimumArclength, userSensitivity, limbGracePeriod);
		lastSkeleton = skeleton;
	}

	return 0;
}