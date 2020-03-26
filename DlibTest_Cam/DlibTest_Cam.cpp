#include <iostream>
#include "../Dlib_Cam/Camera.h"

using namespace Dlib::Cam;



int main () {

	Identifier cameraIdentifier ("SpinnakerCam", 0);
	Camera* p_cameraInstance;
	try {
		p_cameraInstance = Camera::GetInstance (cameraIdentifier.DEVICE_TYPE,
			cameraIdentifier.ID);
	} catch (std::exception ex) {
		std::cout << "Failed to initialize camera "
			<< (std::string)cameraIdentifier << ": "
			<< ex.what () << std::endl;
		return -1;
	}
	std::cout << "Instance info: " << p_cameraInstance->DEVICE_INFO ()
		<< std::endl;

	p_cameraInstance->setFrameSize ({ 800, 600 });
	p_cameraInstance->start ();
	while (true) {
		auto isNewFrame = p_cameraInstance->updateFrame ();
		if (isNewFrame) {
			cv::imshow ("Frame Image", p_cameraInstance->FRAME ().IMAGE ());
			if (cv::waitKey (10) == 27)
				break;
		}
	}
	Camera::ReleaseInstance (p_cameraInstance);
	return 0;

}