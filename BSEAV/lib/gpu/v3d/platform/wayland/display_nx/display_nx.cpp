/******************************************************************************
 *  Copyright (C) 2017 Broadcom. The term "Broadcom" refers to Broadcom Limited and/or its subsidiaries.
 ******************************************************************************/
#include "display_nx.h"
#include "display_nx_priv.h"

#include <memory>
#include <cassert>
#include <list>
#include <atomic>
#include <string.h>

#include "EGL/egl.h"
#include "EGL/eglext_brcm.h"
#include "EGL/eglext_wayland.h"

#include "nxbitmap.h"
#include "nxwindowinfo.h"
#include "nxwindowstate.h"
#include "nxworker.h"
#include "../helpers/extent.h"
#include "wl_server.h"
#include "native_format.h"

#define MAX_SWAP_BUFFERS 3

enum
{
   NATIVE_WINDOW_INFO_MAGIC = 0xABBA601D,
   PIXMAP_INFO_MAGIC = 0x15EEB1A5
};

static BEGL_Error NxDefaultOrientation(void *context [[gnu::unused]])
{
   /* return BEGL_Success to remove any orientation flags and reset to bottom up rasterization */
   return BEGL_Fail;
}

static BEGL_BufferHandle NxBufferCreate(void *context,
      BEGL_PixmapInfoEXT const *bufferRequirements)
{
   auto display = static_cast<WLPL_NexusDisplay*>(context);
   auto heap = NXPL_MemHeap(display->memInterface);
   BEGL_BufferSettings settings;
   display->bufferGetRequirementsFunc(bufferRequirements, &settings);
   auto bitmap = new wlpl::NxBitmap(heap, &settings);
   return static_cast<BEGL_BufferHandle>(bitmap);
}

/* Destroy a buffer previously created with BufferCreate */
static BEGL_Error NxBufferDestroy(void *context [[gnu::unused]],
      BEGL_BufferDisplayState *bufferState)
{
   auto bitmap = static_cast<wlpl::NxBitmap *>(bufferState->buffer);
   delete bitmap;
   return BEGL_Success;
}

static BEGL_BufferHandle NxBufferDequeue(void *context, void *platformState,
      BEGL_BufferFormat format, int *fd)
{
   auto windowState = static_cast<wlpl::NxWindowState *>(platformState);

   auto nw = static_cast<wlpl::NxWindowInfo*>(windowState->GetWindowHandle());
   auto windowExtent = nw->GetExtent2D();

   helper::Extent2D bitmapExtent;

   auto bitmap = windowState->PopFreeQ();
   if (bitmap)
      bitmapExtent = bitmap->GetExtent2D();

   // resize or create
   if (windowExtent != bitmapExtent)
   {
      auto data = static_cast<WLPL_NexusDisplay *>(context);
      assert(data != NULL);

      BEGL_PixmapInfoEXT bufferRequirements =
      { };
      bufferRequirements.width = windowExtent.GetWidth();
      bufferRequirements.height = windowExtent.GetHeight();
      bufferRequirements.format = format;

      std::unique_ptr<wlpl::NxBitmap> tmp(
            static_cast<wlpl::NxBitmap*>(NxBufferCreate(context,
                  &bufferRequirements)));
      bitmap = std::move(tmp);
   }

   // not needed
   *fd = -1;

   // m_info contains other presentation info other than size, so update
   bitmap->UpdateWindowInfo(*nw);

   auto buffer = bitmap.release();
   return static_cast<BEGL_BufferHandle>(buffer);
}

static BEGL_Error NxBufferQueue(void *context, void *platformState,
      BEGL_BufferHandle buffer, int swap_interval, int fd)
{
   if (buffer)
   {
      auto data = static_cast<WLPL_NexusDisplay *>(context);
      auto windowState = static_cast<wlpl::NxWindowState *>(platformState);
      std::unique_ptr<wlpl::NxBitmap> bitmap(
            static_cast<wlpl::NxBitmap*>(buffer));

      std::unique_ptr<helper::Semaphore> fence;
      if (fd != -1)
         fence = std::move(std::unique_ptr<helper::Semaphore>(static_cast<helper::Semaphore*>(data->hwInterface->FenceGet(data->hwInterface->context, fd))));
      else
         fence = std::move(std::unique_ptr<helper::Semaphore>(new helper::Semaphore(1)));

      std::unique_ptr<wlpl::DispItem<wlpl::NxBitmap>> dispItem(
            new wlpl::DispItem<wlpl::NxBitmap>(std::move(bitmap),
                  std::move(fence), swap_interval));
      windowState->PushDispQ(std::move(dispItem));
   }

   return BEGL_Success;
}

static BEGL_Error NxBufferCancel(void *context, void *platformState,
      BEGL_BufferHandle buffer, int fd [[gnu::unused]])
{
   if (buffer)
   {
      auto data = static_cast<WLPL_NexusDisplay *>(context);
      auto windowState = static_cast<wlpl::NxWindowState *>(platformState);
      std::unique_ptr<wlpl::NxBitmap> bitmap(
            static_cast<wlpl::NxBitmap*>(buffer));

      windowState->PushFreeQ(std::move(bitmap));
   }

   return BEGL_Success;
}

static void *NxWindowStateCreate(void *context, BEGL_WindowHandle window,
      bool secure)
{
   unsigned buffers = MAX_SWAP_BUFFERS;
   auto val = getenv("V3D_DOUBLE_BUFFER");
   if (val && (val[0] == 't' || val[0] == 'T' || val[0] == '1'))
      buffers = 2;

   std::unique_ptr<wlpl::NxWindowState> windowState(
         new wlpl::NxWindowState(context, window, secure));

   if (windowState)
   {
      if (!windowState->Init(context, buffers))
      {
         printf("failed to initialize window surface\n");
         exit(0);
      }
   }

   return static_cast<void*>(windowState.release());
}

static BEGL_Error NxWindowStateDestroy(void *context [[gnu::unused]],
      void *swapChainCtx)
{
   auto windowState = static_cast<wlpl::NxWindowState *>(swapChainCtx);

   delete windowState;
   return BEGL_Success;
}

static BEGL_Error NxGetNativeBuffer(void *context [[gnu::unused]],
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

static BEGL_Error NxSurfaceGetInfo(void *context [[gnu::unused]],
      uint32_t target, void *buffer, BEGL_BufferSettings *settings)
{
   switch (target)
   {
   case EGL_WAYLAND_BUFFER_WL:
      *settings = static_cast<WlBufferMemory *>(buffer)->settings;
      return BEGL_Success;

   case BEGL_DEFAULT_BUFFER:
      static_cast<wlpl::NxBitmap*>(buffer)->GetCreateSettings(settings);
      return BEGL_Success;

   default:
      return BEGL_Fail;
   }
}

static BEGL_Error NxSurfaceChangeRefCount(void *context [[gnu::unused]],
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

static bool NxBindWaylandDisplay(void *context,
      void *egl_display [[gnu::unused]], void *wl_display)
{
   WLPL_NexusDisplay *display = static_cast<WLPL_NexusDisplay *>(context);
   return BindWlDisplay(&display->binding,
         static_cast<struct wl_display*>(wl_display),
         NXPL_MemHeap(display->memInterface),
         NXPL_MemHeapSecure(display->memInterface));
}

static bool NxUnbindWaylandDisplay(void *context,
      void *egl_display [[gnu::unused]], void *wl_display)
{
   WLPL_NexusDisplay *display = static_cast<WLPL_NexusDisplay *>(context);
   return UnbindWlDisplay(&display->binding,
         static_cast<struct wl_display*>(wl_display));
}

static bool NxQueryBuffer(void *context [[gnu::unused]],
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

struct WLPL_NexusDisplayInterface: BEGL_DisplayInterface
{
   WLPL_NexusDisplay display;
};

extern "C" BEGL_DisplayInterface *WLPL_CreateNexusDisplayInterface(
      BEGL_MemoryInterface *memIface, BEGL_HWInterface *hwIface,
      BEGL_DisplayCallbacks *displayCallbacks)
{
   assert(displayCallbacks->BufferGetRequirements);

   std::unique_ptr<WLPL_NexusDisplayInterface> disp(
         new WLPL_NexusDisplayInterface());
   if (!disp)
      return NULL;

   disp->context = &disp->display;
   disp->BufferCreate = NxBufferCreate;
   disp->BufferDestroy = NxBufferDestroy;
   disp->BufferDequeue = NxBufferDequeue;
   disp->BufferQueue = NxBufferQueue;
   disp->BufferCancel = NxBufferCancel;
   disp->DefaultOrientation = NxDefaultOrientation;
   disp->WindowPlatformStateCreate = NxWindowStateCreate;
   disp->WindowPlatformStateDestroy = NxWindowStateDestroy;
   disp->GetNativeBuffer = NxGetNativeBuffer;
   disp->SurfaceGetInfo = NxSurfaceGetInfo;
   disp->SurfaceChangeRefCount = NxSurfaceChangeRefCount;
   disp->BindWaylandDisplay = NxBindWaylandDisplay;
   disp->UnbindWaylandDisplay = NxUnbindWaylandDisplay;
   disp->QueryBuffer = NxQueryBuffer;

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
               "Start 0x%llX, Size %p\n", memStatus.offset, memStatus.size);
         goto error;
      }
   }

   return disp.release();

error:
   return NULL;
}

extern "C" void WLPL_DestroyNexusDisplayInterface(BEGL_DisplayInterface *p)
{
   auto disp = std::unique_ptr<WLPL_NexusDisplayInterface>(
         static_cast<WLPL_NexusDisplayInterface *>(p));
}

extern "C" void WLPL_GetDefaultNexusWindowInfoEXT(WLPL_NexusWindowInfoEXT *info)
{
   if (info)
   {
      memset(info, 0, sizeof(WLPL_NexusWindowInfoEXT));
      static NEXUS_BlendEquation colorBlend =
      { NEXUS_BlendFactor_eSourceColor, NEXUS_BlendFactor_eSourceAlpha, false,
            NEXUS_BlendFactor_eDestinationColor,
            NEXUS_BlendFactor_eInverseSourceAlpha, false,
            NEXUS_BlendFactor_eZero };
      static NEXUS_BlendEquation alphaBlend =
      { NEXUS_BlendFactor_eSourceAlpha, NEXUS_BlendFactor_eOne, false,
            NEXUS_BlendFactor_eDestinationAlpha, NEXUS_BlendFactor_eOne, false,
            NEXUS_BlendFactor_eZero };
      info->colorBlend = colorBlend;
      info->alphaBlend = alphaBlend;
      info->type = WLPL_2D;
      info->magic = NATIVE_WINDOW_INFO_MAGIC;
   }
}

extern "C" void *WLPL_CreateNexusWindowEXT(const WLPL_NexusWindowInfoEXT *info)
{
   printf("%s: BEGIN\n", __FUNCTION__);
   if (!info)
      return NULL;

   assert(info->magic == NATIVE_WINDOW_INFO_MAGIC);

   std::unique_ptr<wlpl::NxWindowInfo> nw(new wlpl::NxWindowInfo(info));
   if (!nw)
      return NULL;

   if (!nw->Init())
      return NULL;

   return static_cast<void*>(nw.release());
}

extern "C" void WLPL_DestroyNexusWindow(void *native)
{
   auto nw = static_cast<wlpl::NxWindowInfo*>(native);

   if (nw)
      nw->Term();
   delete nw;
}
