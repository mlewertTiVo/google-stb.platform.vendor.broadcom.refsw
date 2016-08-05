/*=============================================================================
Broadcom Proprietary and Confidential. (c)2012 Broadcom.
All rights reserved.
=============================================================================*/

#include "../glxx/gl_public_api.h"
#include "../common/khrn_int_common.h"
#include "../glxx/glxx_server.h"
#include "libs/util/dglenum/dglenum.h"

GL_APICALL void GL_APIENTRY glPolygonModeBRCM(GLenum mode)
{
   GLXX_SERVER_STATE_T *state = glxx_lock_server_state(OPENGL_ES_ANY);
   if (!state)
      return;

   GLenum error = GL_NO_ERROR;
   if(!(mode == GL_FILL_BRCM || mode == GL_LINE_BRCM || mode == GL_POINT_BRCM))
      error = GL_INVALID_VALUE;
   else
   {
      state->fill_mode = mode;
      state->dirty.cfg = KHRN_RENDER_STATE_SET_ALL;
      state->dirty.stuff = KHRN_RENDER_STATE_SET_ALL;
   }

   if (error != GL_NO_ERROR)
      glxx_server_state_set_error(state, error);

   glxx_unlock_server_state();
}
