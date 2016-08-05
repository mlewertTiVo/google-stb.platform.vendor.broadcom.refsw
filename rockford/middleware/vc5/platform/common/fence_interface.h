/*=============================================================================
Broadcom Proprietary and Confidential. (c)2016 Broadcom.
All rights reserved.
=============================================================================*/

#ifndef __FENCE_INTERFACE_H__
#define __FENCE_INTERFACE_H__

#include <stdint.h>
#include <stdbool.h>
#include "interface.h"

#define FENCE_WAIT_ININITE 0xffffffff

struct FenceInterface
{
   struct Interface base;

   int invalid_fence;
   void (*create)(void *context, int *fence); /* may be NULL */
   void (*destroy)(void *context, int fence); /* may be NULL if create is null*/
   bool (*keep)(void *context, int fence);    /* may be NULL if create is null*/
   bool (*wait)(void *context, int fence, uint32_t timeoutms); /* may be NULL if create is null*/
   void (*signal)(void *context, int fence);  /* may be NULL if create is null*/
};

void FenceInterface_Create(const struct FenceInterface *fi, int *fence);

void FenceInterface_Destroy(const struct FenceInterface *fi,
      int *fence);

bool FenceInterface_Keep(const struct FenceInterface *fi, int fence);

bool FenceInterface_Wait(const struct FenceInterface *fi, int fence,
      uint32_t timeoutms);

void FenceInterface_Signal(const struct FenceInterface *fi, int fence);

void FenceInterface_WaitAndDestroy(const struct FenceInterface *fi,
      int *fence);

#endif /* __FENCE_INTERFACE_H__ */
