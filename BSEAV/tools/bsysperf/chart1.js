/******************************************************************************
* Broadcom Proprietary and Confidential. (c)2016 Broadcom. All rights reserved.
*
* This program is the proprietary software of Broadcom and/or its
* licensors, and may only be used, duplicated, modified or distributed pursuant
* to the terms and conditions of a separate, written license agreement executed
* between you and Broadcom (an "Authorized License").  Except as set forth in
* an Authorized License, Broadcom grants no license (express or implied), right
* to use, or waiver of any kind with respect to the Software, and Broadcom
* expressly reserves all rights in and to the Software and all intellectual
* property rights therein.  IF YOU HAVE NO AUTHORIZED LICENSE, THEN YOU
* HAVE NO RIGHT TO USE THIS SOFTWARE IN ANY WAY, AND SHOULD IMMEDIATELY
* NOTIFY BROADCOM AND DISCONTINUE ALL USE OF THE SOFTWARE.
*
* Except as expressly set forth in the Authorized License,
*
* 1. This program, including its structure, sequence and organization,
*    constitutes the valuable trade secrets of Broadcom, and you shall use all
*    reasonable efforts to protect the confidentiality thereof, and to use
*    this information only in connection with your use of Broadcom integrated
*    circuit products.
*
* 2. TO THE MAXIMUM EXTENT PERMITTED BY LAW, THE SOFTWARE IS PROVIDED "AS IS"
*    AND WITH ALL FAULTS AND BROADCOM MAKES NO PROMISES, REPRESENTATIONS OR
*    WARRANTIES, EITHER EXPRESS, IMPLIED, STATUTORY, OR OTHERWISE, WITH RESPECT
*    TO THE SOFTWARE.  BROADCOM SPECIFICALLY DISCLAIMS ANY AND ALL IMPLIED
*    WARRANTIES OF TITLE, MERCHANTABILITY, NONINFRINGEMENT, FITNESS FOR A
*    PARTICULAR PURPOSE, LACK OF VIRUSES, ACCURACY OR COMPLETENESS, QUIET
*    ENJOYMENT, QUIET POSSESSION OR CORRESPONDENCE TO DESCRIPTION. YOU ASSUME
*    THE ENTIRE RISK ARISING OUT OF USE OR PERFORMANCE OF THE SOFTWARE.
*
* 3. TO THE MAXIMUM EXTENT PERMITTED BY LAW, IN NO EVENT SHALL BROADCOM OR ITS
*    LICENSORS BE LIABLE FOR (i) CONSEQUENTIAL, INCIDENTAL, SPECIAL, INDIRECT,
*    OR EXEMPLARY DAMAGES WHATSOEVER ARISING OUT OF OR IN ANY WAY RELATING TO
*    YOUR USE OF OR INABILITY TO USE THE SOFTWARE EVEN IF BROADCOM HAS BEEN
*    ADVISED OF THE POSSIBILITY OF SUCH DAMAGES; OR (ii) ANY AMOUNT IN EXCESS
*    OF THE AMOUNT ACTUALLY PAID FOR THE SOFTWARE ITSELF OR U.S. $1, WHICHEVER
*    IS GREATER. THESE LIMITATIONS SHALL APPLY NOTWITHSTANDING ANY FAILURE OF
*    ESSENTIAL PURPOSE OF ANY LIMITED REMEDY.
******************************************************************************/
// SVG-VML-3D 1.3 by Lutz Tautenhahn 2002-2006
// The Author grants you a non-exclusive, royalty free, license to use,
// modify and redistribute this software.
// This software is provided "as is", without a warranty of any kind.

function random(vv)
{ return(Math.floor(Math.random()*vv));
}
var sidx=0, value='v', color='c', scale='s'; // these values will be lower case for TX and upper case for RX
var start_idx=0;
var S=new Array(2), B=new Array(2), Xmax=16, Ymax=12; // CAD was Xmax=5, Ymax=9
//useSVG=true;//you can also change this by hand
function Init()
{ MyLoad();
  for(sidx=0; sidx<SVGObjects.length; sidx++) {
      if (useSVG)
      { if (! SVGObjects[sidx])
        { setTimeout("Init()",100);
          return;
        }
        //alert("one new Scene3D(500,500)");
        S[sidx]=new Scene3D(SVGObjects[sidx],0,500,500);
      }
      else {
          S[sidx]=new Scene3D(document.getElementById("Scene1"),1);
      }
      S[sidx].Zoom.y=8/(Date.UTC(2002,8,15,0,0,0)-Date.UTC(2002,0,15,0,0,0));
      S[sidx].Zoom.z=0.1;
      B[sidx]=new BoundingBox(S[sidx], "#80ff80", "#0000ff");
      Color=new Array("#FF0000","#FF8000","#FFFF00");
      Price=new Array(3);
      var i, j;
      for (i=0; i<3; i++) Price[i]=new Array(8);
      for (i=0; i<3; i++)
      { Price[i][0]=60-20*i+random(5);
        for (j=1; j<=7; j++)
        { Price[i][j]=Price[i][j-1]+random(10)-4+2*i-random(2*i+1);
          b=new Box3D(S[sidx], i+0.7, Date.UTC(2002,j-1,23,0,0,0), 0, i+1.3, Date.UTC(2002,j,9,0,0,0), Price[i][j], Color[i], Color[i], "#000000", 1);
          b.SetId(Price[i][j]);
          b.SetEventAction("click",parent.ClickMsg);
        }
      }
      S[sidx].AutoCenter();
      S[sidx].Dist*=1.8;
      S[sidx].YM+=50;
      S[sidx].ZoomAll*=0.9;
      B[sidx].SetBorder(0,Date.UTC(2002,0,2,0,0,0),0, 4,Date.UTC(2002,7,30,0,0,0),70);
      B[sidx].GridDelta.x=1;
      B[sidx].Scale.x="function ApplesOrangesBananas";
      B[sidx].Label.x="";
      B[sidx].Scale.y=4;
      B[sidx].Label.y="";
      B[sidx].Scale.z="$";
      B[sidx].Label.z="Price";
      S[sidx].ChangeViewer(-40,60);
      S[sidx].ChangeLight(-30,30);
      S[sidx].OrderWeight.z=0.05;
      S[sidx].Sort();
      S[sidx].Draw();
  }
  return true;
}
function ClickMsg(evt)
{ if (evt) alert(evt.target.id+" $"); //SVG
  else alert(this.id+" $"); //VML
}
function ApplesOrangesBananas(xx)
{ if (xx==1) return("Test1");//Apples
  if (xx==2) return("Test2");//Oranges
  if (xx==3) return("Test3");//Bananas
  return("");
}
function ChangeViewer(ttheta, ffi)
{
  if (sidx == 1) SwapTxRx(0);
  S[sidx].ChangeViewer(ttheta, ffi);
  S[sidx].Sort();
  //alert("ChangeViewer: sidx=" + sidx + ";   RadioRxTx: (" + GetRxTxRadioButtonSelected() + ")" );
  S[sidx].Draw();
  if (sidx == 1) SwapTxRx(1);
  return true;
}
function ChangeLight(ttheta, ffi)
{
  if (sidx == 1) SwapTxRx(0);
  S[sidx].ChangeLight(ttheta, ffi);
  S[sidx].Draw();
  if (sidx == 1) SwapTxRx(1);
  return true;
}
var viewerzoomed=0;
function ZoomViewer(vv)
{ if ((viewerzoomed+vv>5)||(viewerzoomed+vv<-5)) return;
  if (sidx == 1) SwapTxRx(0);
  viewerzoomed+=vv;
  if (vv>0) S[sidx].Dist*=0.8;
  else S[sidx].Dist/=0.8;
  S[sidx].Sort();
  S[sidx].Draw();
  if (sidx == 1) SwapTxRx(1);
  return true;
}
function Shift(hh, vv)
{
  if (sidx == 1) SwapTxRx(0);
  S[sidx].XM+=hh;
  S[sidx].YM+=vv;
  //alert("XM:" + S[sidx].XM + "; YM:" + S[sidx].YM );
  S[sidx].Draw();
  if (sidx == 1) SwapTxRx(1);
  return true;
}
var picturezoomed=0;
function ZoomPicture(vv)
{ if ((picturezoomed+vv>5)||(picturezoomed+vv<-5)) return;
  if (sidx == 1) SwapTxRx(0);
  picturezoomed+=vv;
  if (vv>0) S[sidx].ZoomAll*=1.1;
  else S[sidx].ZoomAll/=1.1;
  //alert("ZoomPicture:" + picturezoomed + "; S[sidx].ZoomAll:" + S[sidx].ZoomAll );
  S[sidx].Draw();
  if (sidx == 1) SwapTxRx(1);
  return true;
}
onload=Init;
