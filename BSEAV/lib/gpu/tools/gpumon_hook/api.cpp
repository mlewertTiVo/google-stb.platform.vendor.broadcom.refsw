/******************************************************************************
 *  Copyright (C) 2016 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 ******************************************************************************/
#include "api.h"
#include "debuglog.h"
#include <stdlib.h>
#include <stdio.h>

#ifdef ANDROID
#include <log/log.h>
#include <cutils/properties.h>
#endif

#ifdef WIN32
#include <windows.h>

typedef HMODULE LibHandle;
typedef FARPROC FuncPtr;

static LibHandle OpenDll(const char *libName)
{
   return LoadLibraryA(libName);
}

static FuncPtr GetFunc(LibHandle h, const char *symbol)
{
   if (h != NULL)
      return GetProcAddress(h, symbol);

   return NULL;
}

static void CloseDll(LibHandle h)
{
   if (h != NULL)
      FreeLibrary(h);
}

#else

#include <dlfcn.h>

typedef void * LibHandle;
typedef void (*FuncPtr)(void);

static LibHandle OpenDll(const char *libName)
{
   return dlopen(libName, RTLD_LAZY);
}

static FuncPtr GetFunc(LibHandle h, const char *symbol)
{
   if (h != NULL)
      return (FuncPtr)dlsym(h, symbol);

   return NULL;
}

/*
static void CloseDll(LibHandle h)
{
   if (h != NULL)
      dlclose(h);
}
*/
#endif

static LibHandle logLibHandle = NULL;

#define HOOK(f) \
   { \
   FuncPtr tmp = GetFunc(logLibHandle, #f); \
   FuncPtr *t = (FuncPtr *)(&(table->real_##f)); \
   *t = tmp; \
   if (table->real_##f == NULL) \
      printf("%s = %p\n", #f, table->real_##f); \
   }

static int hook_is_enabled( void )
{
   return true;
}

extern bool fill_real_func_table(REAL_GL_API_TABLE *table)
{
   /* Attach to the real GL dll */
   if (hook_is_enabled())
   {
#ifndef ANDROID
      char *realDLL = getenv("GPUMonitorDriverDLL");
      if (realDLL == NULL)
      {
         debug_log(DEBUG_ERROR, "*** GPUMonitorDriverDLL environment variable must point to the real VC5 driver library\n");
         return false;
      }
#else
      char val[PROPERTY_VALUE_MAX];
      char *realDLL = &val[0];
      property_get("debug.egl.hw.gpumon.driver", val, "/vendor/lib/egl/libGLES_nexus.so");
#endif
      logLibHandle = OpenDll(realDLL);
      if (logLibHandle == NULL)
      {
         debug_log(DEBUG_ERROR, "*** %s does not appear to be a valid dll\n", realDLL);
         return false;
      }
   }
   else
      return false;

   // This table content is auto-generated by running ./gen_hook_tables.py in v3dv3/tools/v3d/hook_codegen
   #include "apic.inc"

#ifdef BCG_ABSTRACT_PLATFORM
   /* These entry points are only available with BCG's runtime loadable platforms */
   HOOK(BEGL_RegisterSchedInterface);
   HOOK(BEGL_RegisterDisplayInterface);
   HOOK(BEGL_RegisterMemoryInterface);
   HOOK(BEGL_PlatformAboutToShutdown);
#endif
   return true;
}
