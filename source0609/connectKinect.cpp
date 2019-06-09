#include <iostream>
#include <stdexcept>
#include <pthread.h>

#include <XnCppWrapper.h> 
#include <XnVSessionManager.h>
#include <XnVPushDetector.h>
#include <XnVWaveDetector.h>

#include "connectKinect.h"


// xml to initialize OpenNI
#define CONFIG_XML_PATH "../xml/Sample-Tracking.xml"
void XN_CALLBACK_TYPE SessionStart(const XnPoint3D& ptFocusPoint, void* cxt);
void XN_CALLBACK_TYPE SessionEnd(void* cxt);
void XN_CALLBACK_TYPE Pushed(XnFloat fVelocity, XnFloat fAngle, void* cxt);
void XN_CALLBACK_TYPE Stabilized(XnFloat fVelocity, void* cxt);
void XN_CALLBACK_TYPE Waved(void* cxt);

#define Stay	0
#define RecON	1
#define RecOFF	2
#define ZoomIn	3
#define ZoomOut	4

KinectConnector::KinectConnector()
{
	sw = 1;
	GesType = 0;
	pthread_mutex_init(&mutex, NULL);
}

int KinectConnector::getGesType()
{
	pthread_mutex_lock(&mutex);
	int ret = GesType;
	GesType = 0;
	pthread_mutex_unlock(&mutex);
	return ret; 
}
int KinectConnector::getsw()
{
	return sw;
}

void KinectConnector::setGesType(int type)
{
	pthread_mutex_lock(&mutex);
	GesType = type;
	pthread_mutex_unlock(&mutex);
}


void *KinectConnector::getGesture()
{
    try {

		/*Initialize context of OpenNI with xml)*/
		xn::Context context;
		XnStatus rc = context.InitFromXmlFile(CONFIG_XML_PATH);
		if(rc != XN_STATUS_OK) {
			throw std :: runtime_error(::xnGetStatusString(rc));
		}

		/*Initialize SessionManager of NITE*/
		XnVSessionManager sessionManager;
		rc = sessionManager.Initialize(&context, "Wave,Click,RaiseHand", "RaiseHand");
		if(rc != XN_STATUS_OK) {
			throw std :: runtime_error(::xnGetStatusString(rc));
		}

		/*Register callbacks*/
		XnVHandle sessionCallnack = sessionManager.RegisterSession(0, &SessionStart, &SessionEnd);

		/*Create Detector*/
		XnVPushDetector pushDetector;
		XnVWaveDetector waveDetector;
		XnCallbackHandle pushCallback 		= pushDetector.RegisterPush((void *)this, Pushed);
		XnCallbackHandle stabilizedCallback 	= pushDetector.RegisterStabilized((void *)this, Stabilized);
		XnCallbackHandle waveCallback 		= waveDetector.RegisterWave((void *)this, Waved);

		/*Register Detector to sessionManager*/
		sessionManager.AddListener(&pushDetector);
		sessionManager.AddListener(&waveDetector);

		/*Start to detect gestures*/
		context.StartGeneratingAll();
		std::cout << "Start to detect push or wave or RaiseHand" << std::endl;
		// Main loop
		while (!xnOSWasKeyboardHit()) {	
			/*update data*/
			context.WaitAndUpdateAll();
			sessionManager.Update(&context);
		}


		/*Delete callbacks*/
		pushDetector.UnregisterPush(pushCallback);
		pushDetector.UnregisterStabilized(stabilizedCallback);
		sessionManager.UnregisterSession(sessionCallnack);	

	}
	catch (std::exception& ex) {
		std::cout << ex.what() << std::endl;	

	}
        sw = 0;
}

// callback for session start
void XN_CALLBACK_TYPE SessionStart(const XnPoint3D& ptFocusPoint, void* cxt)
{
	std::cout << "SessionStart" << std::endl;
}
// Callback for session end
void XN_CALLBACK_TYPE SessionEnd(void* cxt)
{
	std::cout << "SessionEnd" << std::endl;
}


// Callback for push detection stabilized
void XN_CALLBACK_TYPE Stabilized(XnFloat fVelocity, void* cxt)
{
	KinectConnector* kinect  = (KinectConnector *)cxt;
	kinect->setGesType(RecON);
}

// Callback for wave detection
void XN_CALLBACK_TYPE Waved(void* cxt)
{
	KinectConnector* kinect  = (KinectConnector *)cxt;
	kinect->setGesType(RecOFF);
}

//
// Callback for push detection
void XN_CALLBACK_TYPE Pushed(XnFloat fVelocity, XnFloat fAngle, void* cxt)
{
	KinectConnector* kinect  = (KinectConnector *)cxt;
	kinect->setGesType(ZoomIn);
}

