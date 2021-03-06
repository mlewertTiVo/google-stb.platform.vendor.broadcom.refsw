/******************************************************************************
 *   (c)2011-2012 Broadcom Corporation
 *
 * This program is the proprietary software of Broadcom Corporation and/or its
 * licensors, and may only be used, duplicated, modified or distributed
 * pursuant to the terms and conditions of a separate, written license
 * agreement executed between you and Broadcom (an "Authorized License").
 * Except as set forth in an Authorized License, Broadcom grants no license
 * (express or implied), right to use, or waiver of any kind with respect to
 * the Software, and Broadcom expressly reserves all rights in and to the
 * Software and all intellectual property rights therein.  IF YOU HAVE NO
 * AUTHORIZED LICENSE, THEN YOU HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY,
 * AND SHOULD IMMEDIATELY NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE
 * SOFTWARE.  
 *
 * Except as expressly set forth in the Authorized License,
 *
 * 1.     This program, including its structure, sequence and organization,
 * constitutes the valuable trade secrets of Broadcom, and you shall use all
 * reasonable efforts to protect the confidentiality thereof, and to use this
 * information only in connection with your use of Broadcom integrated circuit
 * products.
 *
 * 2.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED
 * "AS IS" AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS
 * OR WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH
 * RESPECT TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL
 * IMPLIED WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR
 * A PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
 * ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME THE
 * ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
 *
 * 3.     TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR
 * ITS LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL,
 * INDIRECT, OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY
 * RELATING TO YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM
 * HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN
 * EXCESS OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1,
 * WHICHEVER IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY
 * FAILURE OF ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
 *****************************************************************************/

#ifndef __BCM_GUILLOCHE_H__
#define __BCM_GUILLOCHE_H__

#include "bsg_scene_node.h"
#include "bsg_material.h"
#include "bsg_camera.h"
#include "bsg_vector.h"
#include "bsg_animatable.h"

// Guilloche
//
// Creates a set of curves interpolated between a start and end curve
//
class Guilloche
{
public:
   enum
   {
      MAX_CONTROLS = 50 // Maximum total number of control points in each array
   };

   // Initialise the curves
   Guilloche(uint32_t numControl, const bsg::Vec2 *control1, const bsg::Vec2 *control2, uint32_t numSteps, uint32_t numLines);

   // Set the start and end colours for the curves
   void SetColorRamp(const bsg::Vec4 &c1Start, const bsg::Vec4 &c1Finish, const bsg::Vec4 &c2Start, const bsg::Vec4 &c2Finish);

   // Set the scale factors for the curve
   void SetScale(float horiz, float vertStart, float vertFinish);

   // Set the curve width (in 0 .. 1 screen space)
   void SetWidth(float width, bool useY = true);

   // Use this handle to include the Guilloche in your own graph
   const bsg::SceneNodeHandle   &GetRoot() const     { return m_root; }

   // Connect to these targets to control the form of the curve
   bsg::AnimTarget<float> &GetOffsetX()  { return m_material->GetUniform1f("u_offsetX");  }
   bsg::AnimTarget<float> &GetOffsetY()  { return m_material->GetUniform1f("u_offsetY");  }

   // Animation control for the width of the line
   bsg::AnimTarget<float> &GetWidth()    { return m_material->GetUniform1f("u_width");    }

   // Animation controls for the color ramps
   bsg::AnimTarget<bsg::Vec4>  &GetColor1Start()  { return m_material->GetUniform4f("u_color1Start");  }
   bsg::AnimTarget<bsg::Vec4>  &GetColor1Finish() { return m_material->GetUniform4f("u_color1Finish"); }
   bsg::AnimTarget<bsg::Vec4>  &GetColor2Start()  { return m_material->GetUniform4f("u_color2Start");  }
   bsg::AnimTarget<bsg::Vec4>  &GetColor2Finish() { return m_material->GetUniform4f("u_color2Finish"); }

   // Animation controls for the scale factors
   bsg::AnimTarget<float> &GetHorizScale()       { return m_material->GetUniform1f("u_horizScale");       }
   bsg::AnimTarget<float> &GetVertScaleStart()   { return m_material->GetUniform1f("u_vertScaleStart");   }
   bsg::AnimTarget<float> &GetVertScaleFinish()  { return m_material->GetUniform1f("u_vertScaleFinish");  }

private:
   bsg::SceneNodeHandle   m_root;
   bsg::MaterialHandle    m_material;
};

// GuillochePanel
//
// Render the provided Guilloche based on screen position "pos" and rotation "rot"
// pos is in the screen space (0,0) <= x,y <= (1,1) and refers to the point (0, 0) of the curve
// rot is the rotation (anticlockwise degrees) about pos
// 
class GuillochePanel
{
public:
   // We hold a reference to the guilloche, so it must persist.  We don't modify the guilloche, so it could be
   // used in other panels
   GuillochePanel(const Guilloche &guilloche, const bsg::Vec2 &pos, float rot);

   // Use this root to render the graph.
   const bsg::SceneNodeHandle &GetRoot() const { return m_root; }

private:
   const Guilloche      &m_guilloche;
   bsg::CameraHandle    m_camera;
   bsg::SceneNodeHandle m_root;
};

#endif // __BCM_GUILLOCHE_H__
