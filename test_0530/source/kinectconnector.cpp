#include <iostream>
#include <stdexcept>
#include <pthread.h>

#include <XnCppWrapper.h> 
#include <XnVSessionManager.h>
#include <XnVPushDetector.h>
#include <XnVWaveDetector.h>

#include "kinectconnector.h"

// xml to initialize OpenNI
#define CONFIG_XML_PATH "../xml/Sample-Tracking.xml"



// callback for session start
void XN_CALLBACK_TYPE SessionStart(const XnPoint3D& ptFocusPoint, void* UserCxt)
{
	std::cout << "SessionStart" << std::endl;
}
// Callback for session end
void XN_CALLBACK_TYPE SessionEnd(void* UserCxt)
{
	std::cout << "SessionEnd" << std::endl;
}

// Callback for push detection
void XN_CALLBACK_TYPE Push(XnFloat fVelocity, XnFloat fAngle, void* UserCxt)
{
	//std::cout << "Push:" << fVelocity << "," << fAngle << std::endl;
}

// Callback for push detection stabilized
void XN_CALLBACK_TYPE Stabilized(XnFloat fVelocity, void* cxt)
{
	int *GesType = (int *)cxt;
	//std::cout << "Stabilized:" << fVelocity << std::endl;
	pthread_mutex_lock(&mutex);
	*GesType = 1;
	pthread_mutex_unlock(&mutex);
}

// Callback for wave detection
void XN_CALLBACK_TYPE Wave(void* cxt)
{
	int *GesType = (int *)cxt;
	//std::cout << "Wave" << std::endl;
	pthread_mutex_lock(&mutex);
	*GesType = 2;
	pthread_mutex_unlock(&mutex);
}

void *kinectconnector(void *p)
{

	kinectarg_t *kinearg = (kinectarg_t *)p;

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
		XnCallbackHandle pushCallback 			= pushDetector.RegisterPush(0, Push);
		XnCallbackHandle stabilizedCallback 	= pushDetector.RegisterStabilized((void *)&kinearg->GesType, Stabilized);
		XnCallbackHandle waveCallback 			= waveDetector.RegisterWave((void *)&kinearg->GesType, Wave);

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

	pthread_mutex_lock(&mutex);
	kinearg->sw = 0;
	pthread_mutex_unlock(&mutex);

}
