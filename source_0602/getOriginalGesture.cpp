#include <iostream>
#include <stdexcept>
#include <pthread.h>

#include "getOriginalGesture.h"


void *getOrogonalGesture(void *p)
{


	kinectarg_t *arg = (kinectarg_t *)p;
	gesturearg_t gesturearg = {arg->GesType, arg->mutex};
	

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
		XnCallbackHandle pushCallback 		= pushDetector.RegisterPush((void *)&gesturearg, Pushed);
		XnCallbackHandle stabilizedCallback 	= pushDetector.RegisterStabilized((void *)&gesturearg, Stabilized);
		XnCallbackHandle waveCallback 		= waveDetector.RegisterWave((void *)&gesturearg, Waved);

		/*Register Detector to sessionManager*/
		sessionManager.AddListener(&pushDetector);
		sessionManager.AddListener(&waveDetector);

		/*Start to detect gestures*/
		context.StartGeneratingAll();
		std::cout << "Start to detect push or wave or RaiseHand" << std::endl;
/*
		pthread_t thread;
		pthread_mutex_t mutex;
		pthread_mutex_init(&mutex, NULL);
		pthread_create(&thread, NULL, (void* (*)(void*))getOriginalGesture, (void *)(&gesgetterarg));

*/
	
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

	setsw(arg->sw, 0, arg->mutex);
//	pthread_join(thread, NULL);

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
//
// Callback for push detection
void XN_CALLBACK_TYPE Pushed(XnFloat fVelocity, XnFloat fAngle, void* cxt)
{
	gesturearg_t *arg = (gesturearg_t *)cxt;
	setGesType(arg->GesType , 3, arg->mutex);
}

// Callback for push detection stabilized
void XN_CALLBACK_TYPE Stabilized(XnFloat fVelocity, void* cxt)
{
	gesturearg_t *arg = (gesturearg_t *)cxt;
	setGesType(arg->GesType , 1, arg->mutex);
}

// Callback for wave detection
void XN_CALLBACK_TYPE Waved(void* cxt)
{
	gesturearg_t *arg = (gesturearg_t *)cxt;
	setGesType(arg->GesType , 2, arg->mutex);
}
