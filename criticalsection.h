#ifndef CRITICAL_SECTION_H
#define CRITICAL_SECTION_H

#ifdef WIN32
#include <windows.h>

static CRITICAL_SECTION cs;

#define BEGIN_CRITICAL_SECTION EnterCriticalSection(&cs);
#define END_CRITICAL_SECTION LeaveCriticalSection(&cs);
#define INITIALIZE_CRITICAL_SECTION InitializeCriticalSection(&cs);

#else
#include <pthread.h>

static pthread_mutex_t cs = PTHREAD_MUTEX_INITIALIZER;

#define BEGIN_CRITICAL_SECTION pthread_mutex_lock(&cs);
#define END_CRITICAL_SECTION   pthread_mutex_unlock(&cs);
#define INITIALIZE_CRITICAL_SECTION
#endif

#endif