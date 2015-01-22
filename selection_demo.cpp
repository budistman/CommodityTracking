#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <stdlib.h>
#include <CommodityTracking.h>

int main(int argc, char** argv) {
	VideoCapture stream(0);
	FrameHistory history(stream);

	int minimumArclength = 150, userSensitivity = 255, limbGracePeriod = 50;
	autoCalibrateSensitivity(&userSensitivity, stream, history, minimumArclength, 1, limbGracePeriod);

	Skeleton lastSkeleton;

	for(;;) {
		Skeleton skeleton = getSkeleton(stream, history, lastSkeleton, false, minimumArclength, userSensitivity, limbGracePeriod);
		lastSkeleton = skeleton;

		Mat output = Mat::zeros(512, 512, CV_8UC3);

		Point2d rightHand = skeleton.rightHand() * 512;
		Point2d leftHand = skeleton.leftHand() * 512;
		Point2d center = skeleton.center() * 512;

		circle(output, rightHand, 15, Scalar(0, 0, 255), 20);
		circle(output, leftHand, 15, Scalar(255, 0, 0), 20);
		circle(output, center, 15, Scalar(0, 255, 0), 20);
		line(output, center, rightHand, Scalar(255, 255, 255), 20);
		line(output, center, leftHand, Scalar(255, 255, 255), 20);

		imshow("Output", output);

		Mat vis = history.getLastFrame().clone();
		skeleton.visualize(vis);
		imshow("Frame (+skeleton)", vis);

		if(waitKey(1) == 27) {
			break;
		}

	}
}