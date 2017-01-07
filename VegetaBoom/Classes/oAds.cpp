#include "oAds.h"
/*
#include <stdio.h> 
#include <stdarg.h>

#include "firebase/admob.h"
#include "firebase/admob/types.h"
#include "firebase/app.h"
#include "firebase/future.h"

#include "firebase/admob/banner_view.h"

#ifdef _WIN32
#include <windows.h>
#endif  // _WIN32

void LogMessage(const char* format, ...)
{
    va_list list;
    va_start(list, format);
    vprintf(format, list);
    va_end(list);
    printf("\n");
    fflush(stdout);
}

bool ProcessEvents(int msec)
{
#ifdef _WIN32
    Sleep(msec);
#else
    usleep(msec * 1000);
#endif  // _WIN32
    return true;
}

WindowContext GetWindowContext()
{
    return nullptr;
}

oAds::oAds()
{

}

oAds::~oAds()
{

}

void oAds::Create()
{
#if defined(__ANDROID__)
    // Create the Firebase app.
    firebase::App* app =
        firebase::App::Create(firebase::AppOptions(),
                              your_jni_env,
                              your_android_activity);

    // Your Android AdMob app ID.
    const char* kAdMobAppID = "ca-app-pub-XXXXXXXXXXXXXXXX~NNNNNNNNNN";
#else
    // Create the Firebase app.
    firebase::App* app = firebase::App::Create(firebase::AppOptions());

    // Your iOS AdMob app ID.
    const char* kAdMobAppID = "ca-app-pub-XXXXXXXXXXXXXXXX~NNNNNNNNNN";
#endif  // __ANDROID__

    // Initialize the AdMob library with your AdMob app ID.
    firebase::admob::Initialize(*app, kAdMobAppID);
}
*/
