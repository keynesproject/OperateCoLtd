#ifndef __O_ADS_H__
#define __O_ADS_H__
/*
#if defined(__ANDROID__)
#include <android/native_activity.h>
#include <jni.h>
#elif defined(__APPLE__)
extern "C" {
#include <objc/objc.h>
}  // extern "C"
#endif  // __ANDROID__

// Defined using -DANDROID_MAIN_APP_NAME=some_app_name when compiling this file.
#ifndef FIREBASE_TESTAPP_NAME
#define FIREBASE_TESTAPP_NAME "android_main"
#endif  // FIREBASE_TESTAPP_NAME

// Cross platform logging method.
// Implemented by android/android_main.cc or ios/ios_main.mm.
extern "C" void LogMessage(const char* format, ...);

// Platform-independent method to flush pending events for the main thread.
// Returns true when an event requesting program-exit is received.
bool ProcessEvents(int msec);

// WindowContext represents the handle to the parent window.  It's type
// (and usage) vary based on the OS.
#if defined(__ANDROID__)
typedef jobject WindowContext; // A jobject to the Java Activity.
#elif defined(__APPLE__)
typedef id WindowContext;      // A pointer to an iOS UIView.
#else
typedef void* WindowContext;   // A void* for any other environments.
#endif

#if defined(__ANDROID__)                               
JNIEnv* GetJniEnv();           // Get the JNI environment.
jobject GetActivity();         // Get the activity.
#endif  // defined(__ANDROID__)

// Returns a variable that describes the window context for the app. On Android
// this will be a jobject pointing to the Activity. On iOS, it's an id pointing
// to the root view of the view controller.
WindowContext GetWindowContext();


class oAds
{
public:
    oAds();
    ~oAds();

    void Create();

};

*/
#endif
