/*
 * Copyright (C) 2024 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef LIBEXIF_MNOTE_HUAWEI_TAG_H
#define LIBEXIF_MNOTE_HUAWEI_TAG_H

#include <libexif/exif-data.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

enum _MnoteHuaweiTag {
	MNOTE_HUAWEI_INFO				= 0xFFFF,
	MNOTE_HUAWEI_CAPTURE_MODE		= 0x0200,
	MNOTE_HUAWEI_BURST_NUMBER		= 0x0201,
	MNOTE_HUAWEI_FRONT_CAMERA		= 0x0202,
	MNOTE_HUAWEI_ROLL_ANGLE			= 0x0203,
	MNOTE_HUAWEI_PITCH_ANGLE		= 0x0204,
	MNOTE_HUAWEI_PHYSICAL_APERTURE	= 0x0205,
    MNOTE_HUAWEI_IS_XMAGE_SUPPORTED   = 0x0206,
    MNOTE_HUAWEI_XMAGE_MODE         = 0x0207,
    MNOTE_HUAWEI_XMAGE_LEFT         = 0x0208,
    MNOTE_HUAWEI_XMAGE_TOP          = 0x0209,
    MNOTE_HUAWEI_XMAGE_RIGHT        = 0x020a,
    MNOTE_HUAWEI_XMAGE_BOTTOM       = 0x020b,
    MNOTE_HUAWEI_CLOUD_ENHANCEMENT_MODE = 0x020c,
	MNOTE_HUAWEI_FOCUS_MODE			= 0x020d,
    MNOTE_HUAWEI_WIND_SNAPSHOT_MODE = 0x020e,
	MNOTE_MOVING_PHOTO_VERSION		= 0x020f,
	MNOTE_MICRO_VIDEO_PRESENTATION_TIMESTAMP_US	= 0x0210,
    MNOTE_MOVING_PHOTO_ID			= 0x0211,
    MNOTE_HUAWEI_AI_EDIT			= 0x0212,
	MNOTE_HUAWEI_XTSTYLE_TEMPLATE_NAME = 0x0300,
    MNOTE_HUAWEI_XTSTYLE_CUSTOM_LIGHT_SHADOW = 0x0301,
    MNOTE_HUAWEI_XTSTYLE_CUSTOM_SATURATION = 0x0302,
    MNOTE_HUAWEI_XTSTYLE_CUSTOM_HUE = 0x0303,
    MNOTE_HUAWEI_XTSTYLE_EXPOSUREPARAM_PARAM = 0x0304,
	MNOTE_HUAWEI_STARS_INFO = 0x0305,

	MNOTE_HUAWEI_SCENE_INFO			= 0x0000, /* subtree */
	MNOTE_HUAWEI_SCENE_VERSION		= 0x0001, 
	MNOTE_HUAWEI_SCENE_FOOD_CONF	= 0x0002, 
	MNOTE_HUAWEI_SCENE_STAGE_CONF	= 0x0003, 
	MNOTE_HUAWEI_SCENE_BLUESKY_CONF	= 0x0004, 
	MNOTE_HUAWEI_SCENE_GREENPLANT_CONF	= 0x0005, 
	MNOTE_HUAWEI_SCENE_BEACH_CONF	= 0x0006, 
	MNOTE_HUAWEI_SCENE_SNOW_CONF	= 0x0007, 
	MNOTE_HUAWEI_SCENE_SUNSET_CONF	= 0x0008, 
	MNOTE_HUAWEI_SCENE_FLOWERS_CONF	= 0x0009, 
	MNOTE_HUAWEI_SCENE_NIGHT_CONF	= 0x000a, 
	MNOTE_HUAWEI_SCENE_TEXT_CONF	= 0x000b, 

	MNOTE_HUAWEI_FACE_INFO			= 0x0100, /* subtree */
	MNOTE_HUAWEI_FACE_VERSION 		= 0x0101,
	MNOTE_HUAWEI_FACE_COUNT 		= 0x0102,
	MNOTE_HUAWEI_FACE_CONF 			= 0x0103,
	MNOTE_HUAWEI_FACE_SMILE_SCORE 	= 0x0104,
	MNOTE_HUAWEI_FACE_RECT		 	= 0x0105,
	MNOTE_HUAWEI_FACE_LEYE_CENTER 	= 0x0106,
	MNOTE_HUAWEI_FACE_REYE_CENTER 	= 0x0107,
	MNOTE_HUAWEI_FACE_MOUTH_CENTER 	= 0x0108,
};
typedef enum _MnoteHuaweiTag MnoteHuaweiTag;

enum _MnoteHuaweiTagType {
	MNOTE_HUAWEI_TAG_TYPE_UNKONW,
	MNOTE_HUAWEI_TAG_TYPE_TAG,
	MNOTE_HUAWEI_TAG_TYPE_IFD,
	MNOTE_HUAWEI_TAG_TYPE_STRUCT,
};
typedef enum _MnoteHuaweiTagType MnoteHuaweiTagType;

MnoteHuaweiTag mnote_huawei_tag_from_name(const char *name);
const char *mnote_huawei_tag_get_name(MnoteHuaweiTag);
const char *mnote_huawei_tag_get_title(MnoteHuaweiTag);
const char *mnote_huawei_tag_get_description(MnoteHuaweiTag);
MnoteHuaweiTagType mnote_huawei_tag_type(MnoteHuaweiTag);
MnoteHuaweiTag get_tag_owner_tag(MnoteHuaweiTag tag);
int is_ifd_tag(MnoteHuaweiTag tag);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* !defined(LIBEXIF_MNOTE_HUAWEI_TAG_H) */
