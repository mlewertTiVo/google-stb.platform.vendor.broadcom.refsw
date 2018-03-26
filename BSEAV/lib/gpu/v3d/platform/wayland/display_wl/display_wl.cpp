/******************************************************************************
 *  Copyright (C) 2017 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 ******************************************************************************/
#include "display_wl.h"
#include "display_wl_priv.h"

#include <memory>
#include <cassert>
#include <list>
#include <atomic>
#include <string.h>

#include "EGL/egl.h"
#include "EGL/eglext_brcm.h"
#include "EGL/eglext_wayland.h"

#include "wlwindow.h"
#include "wlworker.h"
#include "wlbitmap.h"
#include "../helpers/extent.h"
#include "wl_client.h"
#include "wl_server.h"
#include "native_format.h"

#define MAX_SWAP_BUFFERS 3

static BEGL_Error WlDefaultOrientation(void *context [[gnu::unused]])
{
   /* return BEGL_Success to remove any orientation flags and reset to bottom up rasterization */
   return BEGL_Fail;
}

static BEGL_BufferHandle WlBufferDequeue(void *context [[gnu::unused]],
      void *platformState, BEGL_BufferFormat format, int *fd)
{
   auto display = static_cast<WLPL_WaylandDisplay *>(context);
   auto window = static_cast<wlpl::WlWindow *>(platformState);
   *fd = -1;
   auto bitmap = window->PopFreeQ(format, display->bufferGetRequirementsFunc);
   auto buffer = bitmap.release();
   return static_cast<BEGL_BufferHandle>(buffer);

}

static BEGL_Error WlBufferQueue(void *context [[gnu::unused]],
      void *platformState, BEGL_BufferHandle buffer, int swap_interval, int fd)
{
   if (buffer)
   {
      auto data = static_cast<WLPL_WaylandDisplay *>(context);
      auto window = static_cast<wlpl::WlWindow *>(platformState);
      std::unique_ptr<wlpl::WlBitmap> bitmap(
            static_cast<wlpl::WlBitmap*>(buffer));

      std::unique_ptr<helper::Semaphore> fence;
      if (fd != -1)
         fence = std::move(std::unique_ptr<helper::Semaphore>(static_cast<helper::Semaphore*>(data->hwInterface->FenceGet(data->hwInterface->context, fd))));
      else
         fence = std::move(std::unique_ptr<helper::Semaphore>(new helper::Semaphore(1)));

      std::unique_ptr<wlpl::DispItem<wlpl::WlBitmap>> dispItem(
            new wlpl::DispItem<wlpl::WlBitmap>(std::move(bitmap),
                  std::move(fence), swap_interval));
      window->PushDispQ(std::move(dispItem));
   }

   return BEGL_Success;
}

static BEGL_Error WlBufferCancel(void *context [[gnu::unused]],
      void *platformState, BEGL_BufferHandle buffer, int fd [[gnu::unused]])
{
   if (buffer)
   {
      //auto display = static_cast<WLPL_WaylandDisplay *>(context);
      auto window = static_cast<wlpl::WlWindow *>(platformState);
      std::unique_ptr<wlpl::WlBitmap> bitmap(
            static_cast<wlpl::WlBitmap*>(buffer));

      window->PushFreeQ(std::move(bitmap));
   }

   return BEGL_Success;
}

static void *WlWindowStateCreate(void *context, BEGL_WindowHandle window,
      bool secure)
{
   auto display = static_cast<WLPL_WaylandDisplay *>(context);
   struct wl_egl_window *wl_egl_window = (struct wl_egl_window *)window;

   unsigned buffers = MAX_SWAP_BUFFERS;
   auto val = getenv("V3D_DOUBLE_BUFFER");
   if (val && (val[0] == 't' || val[0] == 'T' || val[0] == '1'))
      buffers = 2;

   std::unique_ptr<wlpl::WlWindow> windowState(
         new wlpl::WlWindow(&display->client, wl_egl_window, buffers, secure));
   return static_cast<void*>(windowState.release());
}

static BEGL_Error WlWindowStateDestroy(void *context [[gnu::unused]],
      void *swapChainCtx)
{
   auto windowState = static_cast<wlpl::WlWindow *>(swapChainCtx);

   delete windowState;
   return BEGL_Success;
}

static BEGL_Error WlGetNativeBuffer(void *context [[gnu::unused]],
      uint32_t eglTarget, void *eglClientBuffer, void **buffer)
{
   switch (eglTarget)
   {
   case EGL_WAYLAND_BUFFER_WL:
   {
      struct wl_resource *resource =
            static_cast<struct wl_resource*>(eglClientBuffer);
      WlBufferMemory *memory = GetWlBufferMemory(resource);
      if (memory)
      {
         *buffer = memory;
         return BEGL_Success;
      }
      else
         return BEGL_Fail;
   }
   case BEGL_DEFAULT_BUFFER:
   {
      *buffer = eglClientBuffer;
      return BEGL_Success;
   }
   default:
      return BEGL_Fail;
   }
}

static BEGL_Error WlSurfaceGetInfo(void *context [[gnu::unused]],
      uint32_t target, void *buffer, BEGL_BufferSettings *settings)
{
   switch (target)
   {
   case EGL_WAYLAND_BUFFER_WL:
      *settings = static_cast<WlBufferMemory *>(buffer)->settings;
      return BEGL_Success;
   case BEGL_DEFAULT_BUFFER:
      *settings = static_cast<wlpl::WlBitmap*>(buffer)->GetWlBuffer().settings;
      return BEGL_Success;
   default:
      return BEGL_Fail;
   }
}

static BEGL_Error WlSurfaceChangeRefCount(void *context [[gnu::unused]],
      uint32_t target, void *buffer, BEGL_RefCountMode incOrDec)
{
   switch (target)
   {
   case EGL_WAYLAND_BUFFER_WL:
      if (incOrDec == BEGL_Increment)
         WlBufferMemoryRefInc(static_cast<WlBufferMemory *>(buffer));
      else
         WlBufferMemoryRefDec(static_cast<WlBufferMemory *>(buffer));
      return BEGL_Success;
   case BEGL_DEFAULT_BUFFER:
      return BEGL_Success;
   default:
      return BEGL_Fail;
   }
}

static bool WlBindWaylandDisplay(void *context,
      void *egl_display [[gnu::unused]], void *wl_display)
{
   WLPL_WaylandDisplay *display = static_cast<WLPL_WaylandDisplay *>(context);
   return BindWlDisplay(&display->binding,
         static_cast<struct wl_display*>(wl_display),
         NXPL_MemHeap(display->memInterface),
         NXPL_MemHeapSecure(display->memInterface));
}

static bool WlUnbindWaylandDisplay(void *context,
      void *egl_display [[gnu::unused]], void *wl_display)
{
   WLPL_WaylandDisplay *display = static_cast<WLPL_WaylandDisplay *>(context);
   return UnbindWlDisplay(&display->binding,
         static_cast<struct wl_display*>(wl_display));
}

static bool WlQueryBuffer(void *context [[gnu::unused]],
      void *display [[gnu::unused]], void* buffer, int32_t attribute,
      int32_t *value)
{
   struct wl_resource *resource = static_cast<struct wl_resource*>(buffer);
   WlBufferMemory *memory = GetWlBufferMemory(resource);
   if (!memory)
      return false;

   switch (attribute)
   {
   case EGL_WIDTH:
      *value = memory->settings.width;
      break;

   case EGL_HEIGHT:
      *value = memory->settings.height;
      break;

   case EGL_TEXTURE_FORMAT:
      *value = GetNativeFormat(memory->settings.format);
      break;

   case EGL_WAYLAND_Y_INVERTED_WL:
      *value = EGL_TRUE;
      break;

   default:
      return false;
   }
   return true;
}

struct WLPL_WaylandDisplayInterface: BEGL_DisplayInterface
{
   WLPL_WaylandDisplay display;
};

extern "C" BEGL_DisplayInterface *WLPL_CreateWaylandDisplayInterface(
      BEGL_MemoryInterface *memIface, BEGL_HWInterface *hwIface,
      BEGL_DisplayCallbacks *displayCallbacks, struct wl_display *display)
{
   assert(displayCallbacks->BufferGetRequirements);

   std::unique_ptr<WLPL_WaylandDisplayInterface> disp(
         new WLPL_WaylandDisplayInterface());
   if (!disp)
      return NULL;

   if (!CreateWlClient(&disp->display.client, display))
      return NULL;

   disp->context = &disp->display;
   disp->BufferDequeue = WlBufferDequeue;
   disp->BufferQueue = WlBufferQueue;
   disp->BufferCancel = WlBufferCancel;
   disp->DefaultOrientation = WlDefaultOrientation;
   disp->WindowPlatformStateCreate = WlWindowStateCreate;
   disp->WindowPlatformStateDestroy = WlWindowStateDestroy;
   disp->GetNativeBuffer = WlGetNativeBuffer;
   disp->SurfaceGetInfo = WlSurfaceGetInfo;
   disp->SurfaceChangeRefCount = WlSurfaceChangeRefCount;
   disp->BindWaylandDisplay = WlBindWaylandDisplay;
   disp->UnbindWaylandDisplay = WlUnbindWaylandDisplay;
   disp->QueryBuffer = WlQueryBuffer;

   disp->display.memInterface = memIface;
   disp->display.hwInterface = hwIface;
   disp->display.bufferGetRequirementsFunc =
         displayCallbacks->BufferGetRequirements;

   BEGL_HWInfo info;
   disp->display.hwInterface->GetInfo(disp->display.hwInterface->context,
         &info);
   if (info.techRev < 3)
   {
      NEXUS_MemoryStatus memStatus;
      NEXUS_Heap_GetStatus(NXPL_MemHeap(disp->display.memInterface),
            &memStatus);
      auto ttstart = memStatus.offset >> 30;
      auto ttend = (memStatus.offset + memStatus.size - 1) >> 30;
      if (ttstart != ttend)
      {
         printf(
               "\n\nWLPL : WLPL_CreateDisplayInterface() ERROR.\n"
               "The Heap you have selected in your platform layer straddles a 1GB boundary\n"
               "Start 0x%llX, Size %p\n", memStatus.offset,  memStatus.size);
         goto error;
      }
   }

   return disp.release();

error:
   DestroyWlClient(&disp->display.client);
   return nullptr;
}

extern "C" void WLPL_DestroyWaylandDisplayInterface(BEGL_DisplayInterface *p)
{
   auto disp = std::unique_ptr<WLPL_WaylandDisplayInterface>(
         static_cast<WLPL_WaylandDisplayInterface *>(p));
   if (disp)
      DestroyWlClient(&disp->display.client);
}
