#!/bin/bash
./refsw init $2
cd $2
git refsw share fetch $1
git checkout personal/ccbuild/$1
#git checkout shared/shared/$1
today=`date +%Y%m%d`
cd BSEAV/app/atlas/build
build_location=$(pwd)
echo $build_location
if [ "$#" -ge "2" ]
then
{
	destination=$2
	cd /project_it/cable_refsw/unified_releases
	if [ -d $destination ]
	then
	{
		echo "Directory already created"
		cd $destination
		destination=$(pwd)
		echo $destination
	}
	else
	{
		cd /project_it/cable_refsw/unified_releases/phase17.2_take2
		find . -type d | cpio -pdvm ../$destination
		cd ../$destination
		destination=$(pwd)
		echo $destination
	}
	fi
}
else
{
	cd /project_it/cable_refsw/unified_releases/phase17.2_take2
	find . -type d | cpio -pdvm ../build_script
	cd build_script
	destination=$(pwd)
}
fi

if [ "$#" -ge "3" ]
then
  reldate=$3
else
  reldate=$today
fi

cd $build_location
echo $build_location
#echo $reldate
#echo $destination
BRCM_PLAT_DOT_OVERRIDE=y

##########################################################################
#                       REFERENCE CABLE PLATFORMS                        #
##########################################################################
# 97425_3383ib_3383oob
#source /opt/brcm/scripts/plat  97425 b2 le VMS_SFF
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  CABLE_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_IB_SUPPORT=y NEXUS_MODE=proxy clean
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  CABLE_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_IB_SUPPORT=y NEXUS_MODE=proxy install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97425-linux-atlas.bin.tgz $destination/refsw-$reldate.97425b2_3383ib_3383oob-linux-atlas.bin.tgz
#make BDSP_3RDPARTY_ALGO_SUPPORT=n MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n CABLE_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_IB_SUPPORT=y NEXUS_MODE=proxy clean

# 97425_3128ib_3383oob
#source /opt/brcm/scripts/plat  97425 b2 le VMS_SFF
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  CABLE_SUPPORT=y  NEXUS_PLATFORM_7425_CABLE=y NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_MODE=proxy clean
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  CABLE_SUPPORT=y  NEXUS_PLATFORM_7425_CABLE=y NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y  NEXUS_MODE=proxy install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97425-linux-atlas.bin.tgz $destination/refsw-$reldate.97425b2_3128ib_3383oob-linux-atlas.bin.tgz
#make BDSP_3RDPARTY_ALGO_SUPPORT=n MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  CABLE_SUPPORT=y  NEXUS_PLATFORM_7425_CABLE=y NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_MODE=proxy clean

# 97425_3128ib_3128oob
#source /opt/brcm/scripts/plat  97425 b2 le VMS_SFF
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  BCM3128_OOB_CABLECARD_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y  NEXUS_MODE=proxy clean
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  BCM3128_OOB_CABLECARD_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y   NEXUS_MODE=proxy install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97425-linux-atlas.bin.tgz $destination/refsw-$reldate.97425b2_3128ib_3128oob-linux-atlas.bin.tgz
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  BCM3128_OOB_CABLECARD_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y  NEXUS_MODE=proxy clean

# 97425_3383_plus_3128ib_3383oob
source ../../../tools/build/plat  97425 b2 le VMS_SFF
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  CABLE_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_PLUS_BCM3128_IB_SUPPORT=y  NEXUS_MODE=proxy ESTB_SNMP_SUPPORT=n clean
make  -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external  MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  CABLE_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_PLUS_BCM3128_IB_SUPPORT=y  NEXUS_MODE=proxy ESTB_SNMP_SUPPORT=n install
#cp ../../../../obj.$NEXUS_PLATFORM.external/BSEAV/bin/refsw-$today.97425-linux-atlas.bin.tgz $destination/binaries/refsw-$reldate.97425b2_3383_plus_3128ib_3383oob-linux-atlas.bin.tgz
cp ../../../../obj.$NEXUS_PLATFORM.external/BSEAV/bin/refsw-$today.97425-$B_REFSW_ARCH-atlas.bin.tgz $destination/binaries/external/97425/refsw-$reldate.97425b2_3383b0_vms_sff-linux-atlas-cable.bin.tgz
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  CABLE_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_PLUS_BCM3128_IB_SUPPORT=y  NEXUS_MODE=proxy clean

# 97425_33843ib_33843oob
#source /opt/brcm/scripts/plat  97425 b2 le VMS_SFF
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  CABLE_SUPPORT=y NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_IB_SUPPORT=y  NEXUS_MODE=proxy clean
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  CABLE_SUPPORT=y NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_IB_SUPPORT=y  NEXUS_MODE=proxy install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97425-linux-atlas.bin.tgz $destination/refsw-$reldate.97425b2_33843ib_33843oob-linux-atlas.bin.tgz
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  CABLE_SUPPORT=y NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_IB_SUPPORT=y  NEXUS_MODE=proxy clean

# 97425_3128ib_33843oob
#source /opt/brcm/scripts/plat  97425 b2 le VMS_SFF
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  CABLE_SUPPORT=y NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_MODE=proxy clean
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  CABLE_SUPPORT=y NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_MODE=proxy install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97425-linux-atlas.bin.tgz $destination/refsw-$reldate.97425b2_3128ib_33843oob-linux-atlas.bin.tgz
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  CABLE_SUPPORT=y NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_MODE=proxy clean

# 97425_33843_plus_3128ib_33843oob
#source /opt/brcm/scripts/plat  97425 b2 le VMS_SFF
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  CABLE_SUPPORT=y NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_PLUS_BCM3128_IB_SUPPORT=y NEXUS_MODE=proxy clean
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  CABLE_SUPPORT=y NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_PLUS_BCM3128_IB_SUPPORT=y NEXUS_MODE=proxy install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97425-linux-atlas.bin.tgz $destination/refsw-$reldate.97425b2_33843_plus_3128ib_33843oob-linux-atlas.bin.tgz
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_POWER_MANAGEMENT=y PLAYBACK_IP_SUPPORT=y VIDEO_ENCODER_SUPPORT=y NETACCEL_SUPPORT=n LIVE_STREAMING_SUPPORT=y HLS_PROTOCOL_SUPPORT=n  CABLE_SUPPORT=y NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_PLUS_BCM3128_IB_SUPPORT=y NEXUS_MODE=proxy clean


# 97435_3383ib_3383oob
#source /opt/brcm/scripts/plat 97435 b0 le VMS_SFF
#make CABLE_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_IB_SUPPORT=y  NEXUS_MODE=proxy clean
#make CABLE_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_IB_SUPPORT=y  NEXUS_MODE=proxy install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97435-linux-atlas.bin.tgz $destination/refsw-$reldate.97435b0_3383ib_3383oob-linux-atlas.bin.tgz
#make CABLE_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_IB_SUPPORT=y  NEXUS_MODE=proxy clean

# 97435_3128ib_3383oob
#source /opt/brcm/scripts/plat 97435 b0 le VMS_SFF
#make CABLE_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_MODE=proxy clean
#make CABLE_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_MODE=proxy install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97435-linux-atlas.bin.tgz $destination/refsw-$reldate.97435b0_3128ib_3383oob-linux-atlas.bin.tgz
#make CABLE_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_MODE=proxy clean

# 97435_3128ib_3128oob
#source /opt/brcm/scripts/plat 97435 b0 le VMS_SFF
#make BCM3128_OOB_CABLECARD_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y  NEXUS_MODE=proxy clean
#make BCM3128_OOB_CABLECARD_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y  NEXUS_MODE=proxy install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97435-linux-atlas.bin.tgz $destination/refsw-$reldate.97435b0_3128ib_3128oob-linux-atlas.bin.tgz
#make BCM3128_OOB_CABLECARD_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y  NEXUS_MODE=proxy clean


# 97435_3383_plus_3128ib_3383oob
source ../../../tools/build/plat 97435 b0 le VMS_SFF
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.3383.external CABLE_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_PLUS_BCM3128_IB_SUPPORT=y MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_MODE=proxy ESTB_SNMP_SUPPORT=n clean
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.3383.external CABLE_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_PLUS_BCM3128_IB_SUPPORT=y MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_MODE=proxy ESTB_SNMP_SUPPORT=n install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97435-linux-atlas.bin.tgz $destination/refsw-$reldate.97435b0_3383_plus_3128ib_3383oob-linux-atlas.bin.tgz
cp ../../../../obj.$NEXUS_PLATFORM.3383.external/BSEAV/bin/refsw-$today.97435-$B_REFSW_ARCH-atlas.bin.tgz $destination/binaries/external/97435/refsw-$reldate.97435b0_3383b0_vms_sff-linux-atlas-cable.bin.tgz
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.3383.external CABLE_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_PLUS_BCM3128_IB_SUPPORT=y MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_MODE=proxy ESTB_SNMP_SUPPORT=n clean


# 97435_33843ib_33843oob
#source /opt/brcm/scripts/plat 97435 b0 le VMS_SFF
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n CABLE_SUPPORT=y NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_IB_SUPPORT=y  NEXUS_MODE=proxy clean
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n CABLE_SUPPORT=y NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_IB_SUPPORT=y  NEXUS_MODE=proxy install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97435-linux-atlas.bin.tgz $destination/refsw-$reldate.97435b0_33843ib_33843oob-linux-atlas.bin.tgz
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n CABLE_SUPPORT=y NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_IB_SUPPORT=y  NEXUS_MODE=proxy clean

# 97435_3128ib_33843oob
#source /opt/brcm/scripts/plat 97435 b0 le VMS_SFF
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n CABLE_SUPPORT=y NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_MODE=proxy clean
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n CABLE_SUPPORT=y NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_MODE=proxy install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97435-linux-atlas.bin.tgz $destination/refsw-$reldate.97435b0_3128ib_33843oob-linux-atlas.bin.tgz
#make MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n CABLE_SUPPORT=y NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_MODE=proxy clean

# 97435_33843_plus_3128ib_33843oob
source ../../../tools/build/plat 97435 b0 le VMS_SFF
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.33843.external MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n CABLE_SUPPORT=y NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_PLUS_BCM3128_IB_SUPPORT=y NEXUS_MODE=proxy ESTB_SNMP_SUPPORT=n clean
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.33843.external MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n CABLE_SUPPORT=y NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_PLUS_BCM3128_IB_SUPPORT=y NEXUS_MODE=proxy ESTB_SNMP_SUPPORT=n install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97435-linux-atlas.bin.tgz $destination/refsw-$reldate.97435b0_33843_plus_3128ib_33843oob-linux-atlas.bin.tgz
cp ../../../../obj.$NEXUS_PLATFORM.33843.external/BSEAV/bin/refsw-$today.97435-$B_REFSW_ARCH-atlas.bin.tgz $destination/binaries/external/97435/refsw-$reldate.97435b0_33843a0_vms_sff-linux-atlas-cable.bin.tgz
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.33843.external MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n CABLE_SUPPORT=y NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y NEXUS_PLATFORM_7425_CABLE=y  NEXUS_USE_7425_VMS_SFF=y NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y NEXUS_PLATFORM_DOCSIS_PLUS_BCM3128_IB_SUPPORT=y NEXUS_MODE=proxy clean


# 97429_3383ib_3383oob
source ../../../tools/build/plat  97429 b0 le
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external CABLE_SUPPORT=y NEXUS_PLATFORM_7429_CABLE=y BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_MODE=proxy VIDEO_ENCODER_SUPPORT=y BDSP_ENCODER_SUPPORT=y LIVE_STREAMING_SUPPORT=y NETACCEL_SUPPORT=n PLAYBACK_IP_SUPPORT=y AUTO_PSI_SUPPORT=y NEXUS_POWER_MANAGEMENT=y  clean
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external CABLE_SUPPORT=y NEXUS_PLATFORM_7429_CABLE=y BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_MODE=proxy VIDEO_ENCODER_SUPPORT=y BDSP_ENCODER_SUPPORT=y LIVE_STREAMING_SUPPORT=y NETACCEL_SUPPORT=n PLAYBACK_IP_SUPPORT=y AUTO_PSI_SUPPORT=y NEXUS_POWER_MANAGEMENT=y  install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97429-linux-atlas.bin.tgz $destination/refsw-$reldate.97429b0_3383ib_3383oob-linux-atlas.bin.tgz
cp ../../../../obj.$NEXUS_PLATFORM.external/BSEAV/bin/refsw-$today.97429-$B_REFSW_ARCH-atlas.bin.tgz $destination/binaries/external/97429/refsw-$reldate.97429b0_dvr-linux-atlas-cable.bin.tgz
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external CABLE_SUPPORT=y NEXUS_PLATFORM_7429_CABLE=y BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_MODE=proxy VIDEO_ENCODER_SUPPORT=y BDSP_ENCODER_SUPPORT=y LIVE_STREAMING_SUPPORT=y NETACCEL_SUPPORT=n PLAYBACK_IP_SUPPORT=y AUTO_PSI_SUPPORT=y NEXUS_POWER_MANAGEMENT=y  clean

# 97429_33843ib_33843oob
#source /opt/brcm/scripts/plat  97429 b0 le
#make B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external CABLE_SUPPORT=y NEXUS_PLATFORM_7429_CABLE=y BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_MODE=proxy VIDEO_ENCODER_SUPPORT=y BDSP_ENCODER_SUPPORT=y LIVE_STREAMING_SUPPORT=y NETACCEL_SUPPORT=n PLAYBACK_IP_SUPPORT=y AUTO_PSI_SUPPORT=y NEXUS_POWER_MANAGEMENT=y  NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y clean
#make CABLE_SUPPORT=y NEXUS_PLATFORM_7429_CABLE=y BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_MODE=proxy VIDEO_ENCODER_SUPPORT=y BDSP_ENCODER_SUPPORT=y LIVE_STREAMING_SUPPORT=y NETACCEL_SUPPORT=n PLAYBACK_IP_SUPPORT=y AUTO_PSI_SUPPORT=y NEXUS_POWER_MANAGEMENT=y  NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97429-linux-atlas.bin.tgz $destination/refsw-$reldate.97429b0_33843ib_33843oob-linux-atlas.bin.tgz
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97429-linux-atlas.bin.tgz $destination/binaries/external/97429/refsw-$reldate.97429b0_dvr-linux-atlas-cable.bin.tgz
#make B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external CABLE_SUPPORT=y NEXUS_PLATFORM_7429_CABLE=y BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_MODE=proxy VIDEO_ENCODER_SUPPORT=y BDSP_ENCODER_SUPPORT=y LIVE_STREAMING_SUPPORT=y NETACCEL_SUPPORT=n PLAYBACK_IP_SUPPORT=y AUTO_PSI_SUPPORT=y NEXUS_POWER_MANAGEMENT=y  NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y clean

# 974295_3384ib_3384oob
source ../../../tools/build/plat  974295 A0 le
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external CABLE_SUPPORT=y NEXUS_PLATFORM_7429_CABLE=y BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_MODE=proxy VIDEO_ENCODER_SUPPORT=y BDSP_ENCODER_SUPPORT=y LIVE_STREAMING_SUPPORT=y NETACCEL_SUPPORT=n  AUTO_PSI_SUPPORT=y NEXUS_POWER_MANAGEMENT=y  NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y clean
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external CABLE_SUPPORT=y NEXUS_PLATFORM_7429_CABLE=y BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_MODE=proxy VIDEO_ENCODER_SUPPORT=y BDSP_ENCODER_SUPPORT=y LIVE_STREAMING_SUPPORT=y NETACCEL_SUPPORT=n  AUTO_PSI_SUPPORT=y NEXUS_POWER_MANAGEMENT=y  NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.$NEXUS_PLATFORM-linux-atlas.bin.tgz $destination/refsw-$reldate.974295a0_3384ib_3384oob-linux-atlas.bin.tgz
cp ../../../../obj.$NEXUS_PLATFORM.external/BSEAV/bin/refsw-$today.$NEXUS_PLATFORM-$B_REFSW_ARCH-atlas.bin.tgz $destination/binaries/external/$NEXUS_PLATFORM/refsw-$reldate.974295a0_dvr-linux-atlas-cable.bin.tgz
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external CABLE_SUPPORT=y NEXUS_PLATFORM_7429_CABLE=y BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_MODE=proxy VIDEO_ENCODER_SUPPORT=y BDSP_ENCODER_SUPPORT=y LIVE_STREAMING_SUPPORT=y NETACCEL_SUPPORT=n  AUTO_PSI_SUPPORT=y NEXUS_POWER_MANAGEMENT=y  NEXUS_PLATFORM_DOCSIS_BCM33843_SUPPORT=y clean

# 97439 A0 VMS_SFF
#source /opt/brcm/scripts/plat  97439 A0 le VMS_SFF
#make NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n BHDM_CEC_SUPPORT=n CDL_SUPPORT=n  DCC_SUPPORT=n ESTB_SNMP_SUPPORT=n  NEXUS_SYSTEM_CABLE=y CABLE_SUPPORT=y NEXUS_PLATFORM_7439_VMS=y clean
#make NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n BHDM_CEC_SUPPORT=n CDL_SUPPORT=n  DCC_SUPPORT=n ESTB_SNMP_SUPPORT=n  NEXUS_SYSTEM_CABLE=y CABLE_SUPPORT=y NEXUS_PLATFORM_7439_VMS=y install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97439-linux-atlas.bin.tgz $destination/binaries/external/97439/refsw-$reldate.97439a0_vms_sff-linux-atlas-cable.bin.tgz
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97439-linux-atlas.bin.tgz $destination/binaries/build_script/refsw-$reldate.97439a0_vms_sff-linux-atlas-cable.bin.tgz
#cp ../../../cable/bin/$NEXUS_PLATFORM/bcm*.bin $destination/binaries/docsis/$NEXUS_PLATFORM/
#cp ../../../cable/bin/$NEXUS_PLATFORM/boot*.bin $destination/binaries/docsis/$NEXUS_PLATFORM/
#make NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n BHDM_CEC_SUPPORT=n CDL_SUPPORT=n  DCC_SUPPORT=n ESTB_SNMP_SUPPORT=n  NEXUS_SYSTEM_CABLE=y CABLE_SUPPORT=y NEXUS_PLATFORM_7439_VMS=y clean

# 97439 B0 VMS_SFF (97252S VMS_SFF)
source ../../../tools/build/plat  97439 B0 le VMS_SFF
export CABLECARD_SUPPORT=y
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external BHDM_CEC_SUPPORT=n CDL_SUPPORT=n  NEXUS_FRONTEND_3128=y ESTB_SNMP_SUPPORT=n  NEXUS_SYSTEM_CABLE=y CABLE_SUPPORT=y NEXUS_FRONTEND_DOCSIS=y NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y clean
make  -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external BHDM_CEC_SUPPORT=n CDL_SUPPORT=n  NEXUS_FRONTEND_3128=y ESTB_SNMP_SUPPORT=n  NEXUS_SYSTEM_CABLE=y CABLE_SUPPORT=y NEXUS_FRONTEND_DOCSIS=y NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y install
cp ../../../../obj.$NEXUS_PLATFORM.external/BSEAV/bin/refsw-$today.97439-$B_REFSW_ARCH-atlas.bin.tgz $destination/binaries/external/97439/refsw-$reldate.97252S_vms_sff-linux-atlas-cable.bin.tgz
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external BHDM_CEC_SUPPORT=n CDL_SUPPORT=n  NEXUS_FRONTEND_3128=y ESTB_SNMP_SUPPORT=n  NEXUS_SYSTEM_CABLE=y CABLE_SUPPORT=y NEXUS_FRONTEND_DOCSIS=y NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y clean
unset CABLECARD_SUPPORT=y

# 93390 B0 VMS_SFF0 (97252S VMS_SFF)
source ../../../tools/build/plat  97439 B0 le VMS_SFF
unset NEXUS_USE_7252S_VMS_SFF
make -j BHDM_CEC_SUPPORT=n CDL_SUPPORT=n  ESTB_SNMP_SUPPORT=n  NEXUS_SYSTEM_CABLE=y NEXUS_FRONTEND_DOCSIS=y NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_USE_3390_VMS=y B_REFSW_OBJ_DIR=obj.93390.external clean
make -j BHDM_CEC_SUPPORT=n CDL_SUPPORT=n  ESTB_SNMP_SUPPORT=n  NEXUS_SYSTEM_CABLE=y NEXUS_FRONTEND_DOCSIS=y NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_USE_3390_VMS=y B_REFSW_OBJ_DIR=obj.93390.external install
cp ../../../../obj.93390.external/BSEAV/bin/refsw-$today.97439-$B_REFSW_ARCH-atlas.bin.tgz $destination/binaries/external/93390/refsw-$reldate.93390VMS52S-linux-atlas-cable.bin.tgz
make -j BHDM_CEC_SUPPORT=n CDL_SUPPORT=n  ESTB_SNMP_SUPPORT=n  NEXUS_SYSTEM_CABLE=y NEXUS_FRONTEND_DOCSIS=y NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_USE_3390_VMS=y B_REFSW_OBJ_DIR=obj.93390 clean

# 97268 A0 VMS
source ../../../tools/build/plat 97268 B0 32
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external NEXUS_SYSTEM_CABLE=y NEXUS_USE_7268_VMS=y  CABLE_SUPPORT=y CABLECARD_SUPPORT=n ESTB_CFG_SUPPORT=n NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n clean
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external NEXUS_SYSTEM_CABLE=y NEXUS_USE_7268_VMS=y  CABLE_SUPPORT=y CABLECARD_SUPPORT=n ESTB_CFG_SUPPORT=n NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n install
cp ../../../../obj.$NEXUS_PLATFORM.external/BSEAV/bin/refsw-$today.$NEXUS_PLATFORM-$B_REFSW_ARCH-atlas.bin.tgz $destination/binaries/external/$NEXUS_PLATFORM/refsw-$reldate.97268b0_vms-linux-atlas-cable.bin.tgz
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external NEXUS_SYSTEM_CABLE=y NEXUS_USE_7268_VMS=y  CABLE_SUPPORT=y CABLECARD_SUPPORT=n ESTB_CFG_SUPPORT=n NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n clean

# 97445 E0 VMS_SFF
source ../../../tools/build/plat  97445 E0 le VMS_SFF
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n BHDM_CEC_SUPPORT=n CDL_SUPPORT=n  DCC_SUPPORT=n ESTB_SNMP_SUPPORT=n  NEXUS_SYSTEM_CABLE=y CABLE_SUPPORT=y clean
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n BHDM_CEC_SUPPORT=n CDL_SUPPORT=n  DCC_SUPPORT=n ESTB_SNMP_SUPPORT=n  NEXUS_SYSTEM_CABLE=y CABLE_SUPPORT=y install
cp ../../../../obj.$NEXUS_PLATFORM.external/BSEAV/bin/refsw-$today.97445-$B_REFSW_ARCH-atlas.bin.tgz $destination/binaries/external/97445/refsw-$reldate.97445e0_vms_sff-linux-atlas-cable.bin.tgz
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n BHDM_CEC_SUPPORT=n CDL_SUPPORT=n  DCC_SUPPORT=n ESTB_SNMP_SUPPORT=n  NEXUS_SYSTEM_CABLE=y CABLE_SUPPORT=y ESTB_SNMP_SUPPORT=n clean


# 97584_3349
source ../../../tools/build/plat 97584 a0 le
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external NEXUS_MODE=proxy MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n VIDEO_ENCODER_SUPPORT=y BDSP_ENCODER_SUPPORT=y NEXUS_REVERSE_RMAGNUM_SUPPORT=y clean
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external NEXUS_MODE=proxy MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n VIDEO_ENCODER_SUPPORT=y BDSP_ENCODER_SUPPORT=y NEXUS_REVERSE_RMAGNUM_SUPPORT=y  install
#cp ../../../../obj.$NEXUS_PLATFORM.external/BSEAV/bin/refsw-$today.97584-linux-atlas.bin.tgz $destination/refsw-$reldate.97584a0_3349-linux-atlas.bin.tgz
cp ../../../../obj.$NEXUS_PLATFORM.external/BSEAV/bin/refsw-$today.97584-$B_REFSW_ARCH-atlas.bin.tgz $destination/binaries/external/97584/refsw-$reldate.97584a0_3349a0-linux-atlas-cable.bin.tgz
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external NEXUS_MODE=proxy MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n VIDEO_ENCODER_SUPPORT=y BDSP_ENCODER_SUPPORT=y NEXUS_REVERSE_RMAGNUM_SUPPORT=y clean


# 97241_3349

source ../../../tools/build/plat  97241 b0 le
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external NEXUS_PLATFORM_7241_DCSFBTSFF=y  NEXUS_REVERSE_RMAGNUM_SUPPORT=y NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n VIDEO_ENCODER_SUPPORT=y BDSP_ENCODER_SUPPORT=y LIVE_STREAMING_SUPPORT=y NETACCEL_SUPPORT=n PLAYBACK_IP_SUPPORT=y AUTO_PSI_SUPPORT=y NEXUS_POWER_MANAGEMENT=y clean
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external NEXUS_PLATFORM_7241_DCSFBTSFF=y  NEXUS_REVERSE_RMAGNUM_SUPPORT=y NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n VIDEO_ENCODER_SUPPORT=y BDSP_ENCODER_SUPPORT=y LIVE_STREAMING_SUPPORT=y NETACCEL_SUPPORT=n PLAYBACK_IP_SUPPORT=y AUTO_PSI_SUPPORT=y NEXUS_POWER_MANAGEMENT=y  install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97241-linux-atlas.bin.tgz $destination/refsw-$reldate.97241b0_3349-linux-atlas.bin.tgz
cp ../../../../obj.$NEXUS_PLATFORM.external/BSEAV/bin/refsw-$today.97241-$B_REFSW_ARCH-atlas.bin.tgz $destination/binaries/external/97241/refsw-$reldate.97241b0_3349a0-linux-atlas-cable.bin.tgz
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external NEXUS_PLATFORM_7241_DCSFBTSFF=y  NEXUS_REVERSE_RMAGNUM_SUPPORT=y NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n VIDEO_ENCODER_SUPPORT=y BDSP_ENCODER_SUPPORT=y LIVE_STREAMING_SUPPORT=y NETACCEL_SUPPORT=n PLAYBACK_IP_SUPPORT=y AUTO_PSI_SUPPORT=y NEXUS_POWER_MANAGEMENT=y clean

# 97250_cd2

source ../../../tools/build/plat 97250 B0 cd2 le
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external NEXUS_MODE=proxy MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n VIDEO_ENCODER_SUPPORT=y BDSP_ENCODER_SUPPORT=y NEXUS_REVERSE_RMAGNUM_SUPPORT=y clean
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external NEXUS_MODE=proxy MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n VIDEO_ENCODER_SUPPORT=y BDSP_ENCODER_SUPPORT=y NEXUS_REVERSE_RMAGNUM_SUPPORT=y  install
cp ../../../../obj.$NEXUS_PLATFORM.external/BSEAV/bin/refsw-$today.97250-$B_REFSW_ARCH-atlas.bin.tgz $destination/binaries/external/97250/refsw-$reldate.97250cd2_vms_sff-linux-atlas-cable.bin.tgz
make -j B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external NEXUS_MODE=proxy MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n VIDEO_ENCODER_SUPPORT=y BDSP_ENCODER_SUPPORT=y NEXUS_REVERSE_RMAGNUM_SUPPORT=y clean

# 97278 A0 VMS_SFF

source ../../../tools/build/plat 97278 A0 VMS_SFF

make -j BHDM_CEC_SUPPORT=n CDL_SUPPORT=n  ESTB_SNMP_SUPPORT=n  NEXUS_SYSTEM_CABLE=y NEXUS_FRONTEND_DOCSIS=y NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_USE_3390_VMS=y B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external clean
make -j BHDM_CEC_SUPPORT=n CDL_SUPPORT=n  ESTB_SNMP_SUPPORT=n  NEXUS_SYSTEM_CABLE=y NEXUS_FRONTEND_DOCSIS=y NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_USE_3390_VMS=y B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external install
cp ../../../../obj.$NEXUS_PLATFORM.external/BSEAV/bin/refsw-$today.97278-$B_REFSW_ARCH-linux-atlas.bin.tgz $destination/binaries/external/97278/refsw-$reldate.97278a0_vms_sff-linux-atlas.bin.tgz
make -j BHDM_CEC_SUPPORT=n CDL_SUPPORT=n  ESTB_SNMP_SUPPORT=n  NEXUS_SYSTEM_CABLE=y NEXUS_FRONTEND_DOCSIS=y NEXUS_MODE=proxy BDSP_3RDPARTY_ALGO_SUPPORT=n NEXUS_USE_3390_VMS=y B_REFSW_OBJ_DIR=obj.$NEXUS_PLATFORM.external clean

#97420_3255

#source ../../tools/build/plat 97420 c1 le
#make PLAYBACK_IP_SUPPORT=y NEXUS_MODE=proxy SMP=y NEXUS_POWER_MANAGEMENT=y BCEC_SUPPORT=y NEXUS_PLATFORM_7420_CABLE=y NEXUS_POWER_STANDBY=n NEXUS_PLATFORM_7410_UMA_32b=n LIVE_MEDIA_SUPPORT=y NEXUS_ZORDER_PIP_SWAP=y BPWR_TESTPORT=y clean
#make PLAYBACK_IP_SUPPORT=y NEXUS_MODE=proxy SMP=y NEXUS_POWER_MANAGEMENT=y BCEC_SUPPORT=y NEXUS_PLATFORM_7420_CABLE=y NEXUS_POWER_STANDBY=n NEXUS_PLATFORM_7410_UMA_32b=n LIVE_MEDIA_SUPPORT=y NEXUS_ZORDER_PIP_SWAP=y BPWR_TESTPORT=y install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97420-linux-atlas.bin.tgz $destination/binaries/internal/97420/refsw-$reldate.97420c1_3255-linux-atlas.bin.tgz
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97420-linux-atlas.bin.tgz $destination/binaries/build_script/refsw-$reldate.97420c1_3255-linux-atlas.bin.tgz
#make PLAYBACK_IP_SUPPORT=y NEXUS_MODE=proxy SMP=y NEXUS_POWER_MANAGEMENT=y BCEC_SUPPORT=y NEXUS_PLATFORM_7420_CABLE=y NEXUS_POWER_STANDBY=n NEXUS_PLATFORM_7410_UMA_32b=n LIVE_MEDIA_SUPPORT=y NEXUS_ZORDER_PIP_SWAP=y BPWR_TESTPORT=y clean


#97125

#source /opt/brcm/scripts/plat 97125 c0 le
#make CABLE_SUPPORT=y NEXUS_MODE=proxy MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n clean
#make CABLE_SUPPORT=y NEXUS_MODE=proxy MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97125-linux-atlas.bin.tgz $destination/refsw-$reldate.97125c0-cable-linux-atlas.bin.tgz
#make CABLE_SUPPORT=y NEXUS_MODE=proxy MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n clean


#97459

#source /opt/brcm/scripts/plat 97420 c0 le
#export PLATFORM=97459
#export NEXUS_PLATFORM=97459
#export LINUX=/opt/brcm/linux-2.6.37-2.5/7405b0
#make  CABLE_SUPPORT=y NEXUS_MODE=proxy MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n clean
#make  CABLE_SUPPORT=y NEXUS_MODE=proxy MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n install
#cp ../../../../obj.$NEXUS_PLATFORM/BSEAV/bin/refsw-$today.97459-linux-atlas.bin.tgz $destination/refsw-$reldate.97459-linux-atlas.bin.tgz
#make CABLE_SUPPORT=y  NEXUS_MODE=proxy MEDIA_RMFF_SUPPORT=n MEDIA_ASF_SUPPORT=n BDSP_3RDPARTY_ALGO_SUPPORT=n clean
#unset PLATFORM
#unset NEXUS_PLATFORM

#3325

#source /opt/brcm/scripts/plat  97425 b2 le VMS_SFF
#export PLATFORM=93383sms
#export NEXUS_PLATFORM=93383sms
#export POWERSTANDBY_SUPPORT=y
#export NEXUS_POWER_MANAGEMENT=y
#export NEXUS_POWER_STANDBY=n
#export PLAYBACK_IP_SUPPORT=y
#export NETACCEL_SUPPORT=n
#export LIVEMEDIA_SUPPORT=n
#export MEDIA_AVI_SUPPORT=y
#export MEDIA_ASF_SUPPORT=n
#export BHDM_CEC_SUPPORT=n
#export LIVE_STREAMING_SUPPORT=y
#export HLS_PROTOCOL_SUPPORT=n
#export NEXUS_PLATFORM_7425_CABLE=y
#export TUNER_ENABLE_7425_CABLE=y
#export NEXUS_USE_7425_VMS_SFF=y
#export NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT=y
#export NEXUS_PLATFORM_DOCSIS_IB_SUPPORT=y
#export DVREXTLIB_DRM_SUPPORT=y
#export DVREXTLIB_TRANSCODE=y
#export DVREXTLIB_MEDIANODE_ONDEMAND_CACHING=y


#build_loc=$(pwd)
#cd ../../../../rockford/unittests/cable/dvr/
#make clean
#make
#make install
#cd $build_loc
#cd ../../../../nexus/examples/dvr
#make record_allpass_from_qam
#cd ../encoder
#make transcode_qam_to_ts
#cd ../frontend
#make tune_qam

#cd $build_loc
#cd ../../../../nexus/lib/playback_ip/apps
#make ip_streamer

#cd $build_loc
#cd ../../../../obj.$NEXUS_PLATFORM/nexus/bin

#tar czvf refsw-$reldate.93383sms_tests.bin.tgz *
#cp  refsw-$reldate.93383sms_tests.bin.tgz $destination/binaries/build_script/refsw-$reldate.93383sms_tests.bin.tgz
#cp  refsw-$reldate.93383sms_tests.bin.tgz $destination/binaries/internal/93383sms/refsw-$reldate.93383b0_sms_tests.bin.tgz
#cd $build_loc
#make clean

#unset NEXUS_MODE
#unset PLATFORM
#unset NEXUS_PLATFORM
#unset POWERSTANDBY_SUPPORT
#unset NEXUS_POWER_MANAGEMENT
#unset NEXUS_POWER_STANDBY
#unset PLAYBACK_IP_SUPPORT
#unset NETACCEL_SUPPORT
#unset LIVEMEDIA_SUPPORT
#unset MEDIA_AVI_SUPPORT
#unset MEDIA_ASF_SUPPORT
#unset BHDM_CEC_SUPPORT
#unset LIVE_STREAMING_SUPPORT
#unset HLS_PROTOCOL_SUPPORT
#unset NEXUS_PLATFORM_7425_CABLE
#unset TUNER_ENABLE_7425_CABLE
#unset NEXUS_USE_7425_VMS_SFF
#unset NEXUS_PLATFORM_DOCSIS_OOB_SUPPORT
#unset NEXUS_PLATFORM_DOCSIS_IB_SUPPORT
#unset DVREXTLIB_DRM_SUPPORT
#unset DVREXTLIB_TRANSCODE
#unset DVREXTLIB_MEDIANODE_ONDEMAND_CACHING


