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

#include <config.h>
#include <stdlib.h>
#include <string.h>
#include <libexif/i18n.h>
#include "mnote-huawei-tag.h"

typedef struct _MnoteHuaweiTable		MnoteHuaweiTable;

struct _MnoteHuaweiTable {
	MnoteHuaweiTag tag;
	const char *name;
	const char *title;
	const char *description;
	MnoteHuaweiTagType TagType;
};

static const MnoteHuaweiTable huawei_table[] = {
	{MNOTE_HUAWEI_CAPTURE_MODE, "HwMnoteCaptureMode", N_("Capture Mode"), "CaptureMode"},
	{MNOTE_HUAWEI_BURST_NUMBER, "HwMnoteBurstNumber", N_("Burst Number"), "BurstNumber"},
	{MNOTE_HUAWEI_FRONT_CAMERA, "HwMnoteFrontCamera", N_("Front Camera"), "FrontCamera"},
	{MNOTE_HUAWEI_ROLL_ANGLE, "HwMnoteRollAngle", N_("Roll Angle"), "RollAngle"},
	{MNOTE_HUAWEI_PITCH_ANGLE, "HwMnotePitchAngle", N_("Pitch Angle"), "PitchAngle"},
	{MNOTE_HUAWEI_PHYSICAL_APERTURE, "HwMnotePhysicalAperture", N_("Physical Aperture"), "PhysicalAperture"},
    {MNOTE_HUAWEI_IS_XMAGE_SUPPORTED, "HwMnoteIsXmageSupported", N_("Is Xmage Supported"), "IsXmageSupported"},
    {MNOTE_HUAWEI_XMAGE_MODE, "HwMnoteXmageMode", N_("Xmage Mode"), "XmageMode"},
    {MNOTE_HUAWEI_XMAGE_LEFT, "HwMnoteXmageLeft", N_("Xmage Left"), "XmageLeft"},
    {MNOTE_HUAWEI_XMAGE_TOP, "HwMnoteXmageTop", N_("Xmage Top"), "XmageTop"},
    {MNOTE_HUAWEI_XMAGE_RIGHT, "HwMnoteXmageRight", N_("Xmage Right"), "XmageRight"},
    {MNOTE_HUAWEI_XMAGE_BOTTOM, "HwMnoteXmageBottom", N_("Xmage Bottom"), "XmageBottom"},
    {MNOTE_HUAWEI_CLOUD_ENHANCEMENT_MODE, "HwMnoteCloudEnhancementMode",
     N_("Cloud Enhancement Mode"), "CloudEnhancementMode"},
    {MNOTE_HUAWEI_FOCUS_MODE, "HwMnoteFocusMode", N_("Focus Mode"), "FocusMode"},
    {MNOTE_HUAWEI_WIND_SNAPSHOT_MODE, "HwMnoteWindSnapshotMode", N_("Wind Snapshot Mode"), "WindSnapshotMode"},
	{MNOTE_MOVING_PHOTO_VERSION, "MovingPhotoVersion", N_("Moving Photo Version"),
	"MovingPhotoVersion"},
	{MNOTE_MICRO_VIDEO_PRESENTATION_TIMESTAMP_US, "MicroVideoPresentationTimestampUS",
	N_("Video Presentation Timestamp US"), "VideoPresentationTimestampUS"},
    {MNOTE_MOVING_PHOTO_ID, "MovingPhotoId", N_("Moving Photo Id"), "MovingPhotoId"},
    {MNOTE_HUAWEI_AI_EDIT, "HwMnoteAiEdit", N_("AI Edit"), "AiEdit"},
	{MNOTE_HUAWEI_XTSTYLE_TEMPLATE_NAME, "HwMnoteXtStyleTemplateName",
	N_("XtStyle Template Name"), "XtStyleTemplateName"},
    {MNOTE_HUAWEI_XTSTYLE_CUSTOM_LIGHT_SHADOW, "HwMnoteXtStyleCustomLightAndShadow",
	N_("XtStyle Custom Light And Shadow"), "XtStyleCustomLightAndShadow"},
    {MNOTE_HUAWEI_XTSTYLE_CUSTOM_SATURATION, "HwMnoteXtStyleCustomSaturation",
	N_("XtStyle Custom Saturation"), "XtStyleCustomSaturation"},
    {MNOTE_HUAWEI_XTSTYLE_CUSTOM_HUE, "HwMnoteXtStyleCustomHue",
	N_("XtStyle Custom Hue"), "XtStyleCustomHue"},
    {MNOTE_HUAWEI_XTSTYLE_EXPOSUREPARAM_PARAM, "HwMnoteXtStyleExposureParam",
	N_("XtStyle Exposure Param"), "XtStyleExposureParam"},
	{MNOTE_HUAWEI_STARS_INFO, "HwMnoteStarsInfo",
	N_("Stars Info"), "StarsInfo"},
	
	{0, "HwUnknow", N_("Unknow Tag"), "UnknowTag"},
};

static const MnoteHuaweiTable huawei_face_table[] = {
	{MNOTE_HUAWEI_FACE_INFO, "HwMnoteFacePointer", N_("Face Info"), "FaceInfo", MNOTE_HUAWEI_TAG_TYPE_IFD},
	{MNOTE_HUAWEI_FACE_VERSION, "HwMnoteFaceVersion", N_("Face Version"), "FaceVersion"},
	{MNOTE_HUAWEI_FACE_COUNT, "HwMnoteFaceCount", N_("Count"), "Count"},
	{MNOTE_HUAWEI_FACE_CONF, "HwMnoteFaceConf", N_("Conf"), "Conf"},
	{MNOTE_HUAWEI_FACE_SMILE_SCORE, "HwMnoteFaceSmileScore", N_("Smile Score"), "SmileScore"},
	{MNOTE_HUAWEI_FACE_RECT, "HwMnoteFaceRect", N_("Rect"), "Rect"},
	{MNOTE_HUAWEI_FACE_LEYE_CENTER, "HwMnoteFaceLeyeCenter", N_("LeyeCenter"), "LeyeCenter"},
	{MNOTE_HUAWEI_FACE_REYE_CENTER, "HwMnoteFaceReyeCenter", N_("Reye Center"), "ReyeCenter"},
	{MNOTE_HUAWEI_FACE_MOUTH_CENTER, "HwMnoteFaceMouthCenter", N_("Mouth Center"), "MouthCenter"},

	{0, "HwUnknow", N_("Unknow Tag"), "UnknowTag"},
};

static const MnoteHuaweiTable huawei_scene_table[] = {
	{MNOTE_HUAWEI_SCENE_INFO, "HwMnoteScenePointer", N_("Scene Info"), "SceneInfo", MNOTE_HUAWEI_TAG_TYPE_IFD},
	{MNOTE_HUAWEI_SCENE_VERSION, "HwMnoteSceneVersion", N_("Scene Version"), "SceneVersion"},
	{MNOTE_HUAWEI_SCENE_FOOD_CONF, "HwMnoteSceneFoodConf", N_("Food Conf"), "FoodConf"},
	{MNOTE_HUAWEI_SCENE_STAGE_CONF, "HwMnoteSceneStageConf", N_("Stage Conf"), "StageConf"},
	{MNOTE_HUAWEI_SCENE_BLUESKY_CONF, "HwMnoteSceneBlueSkyConf", N_("Blue Sky Conf"), "BlueSkyConf"},
	{MNOTE_HUAWEI_SCENE_GREENPLANT_CONF, "HwMnoteSceneGreenPlantConf", N_("GreenPlant Conf"), "GreenPlantConf"},
	{MNOTE_HUAWEI_SCENE_BEACH_CONF, "HwMnoteSceneBeachConf", N_("Beach Conf"), "BeachConf"},
	{MNOTE_HUAWEI_SCENE_SNOW_CONF, "HwMnoteSceneSnowConf", N_("Snow Conf"), "SnowConf"},
	{MNOTE_HUAWEI_SCENE_SUNSET_CONF, "HwMnoteSceneSunsetConf", N_("Sunset Conf"), "SunsetConf"},
	{MNOTE_HUAWEI_SCENE_FLOWERS_CONF, "HwMnoteSceneFlowersConf", N_("Flowers Conf"), "FlowersConf"},
	{MNOTE_HUAWEI_SCENE_NIGHT_CONF, "HwMnoteSceneNightConf", N_("Night Conf"), "NightConf"},
	{MNOTE_HUAWEI_SCENE_TEXT_CONF, "HwMnoteSceneTextConf", N_("Text Conf"), "TextConf"},

	{0, "HwUnknow", N_("Unknow Tag"), "UnknowTag"},
};

#define GET_TAG_INFO(tb, tb_size, tag, idx, ret) do {	\
									for (int i = 0; i<tb_size; i++) { \
										if (tb[i].tag == tag) {ret = tb[i].idx; return ret;}\
									} \
									ret = tb[tb_size-1].idx;\
								} while (0)

const int HUAWEI_TABLE = 9;
const int HUAWEI_FACE_TABLE = 8;

const MnoteHuaweiTable* get_tag_table(MnoteHuaweiTag tag, int* size)
{
	if ((tag >> HUAWEI_TABLE) & 1) {
		*size = sizeof (huawei_table) / sizeof (huawei_table[0]);
		return huawei_table;
	}

	if ((tag >> HUAWEI_FACE_TABLE) & 1) {
		*size = sizeof (huawei_face_table) / sizeof (huawei_face_table[0]);
		return huawei_face_table;
	}

	*size = sizeof (huawei_scene_table) / sizeof (huawei_scene_table[0]);
	return huawei_scene_table;
}

MnoteHuaweiTag get_tag_owner_tag(MnoteHuaweiTag tag)
{
	MnoteHuaweiTag owner_tag = MNOTE_HUAWEI_INFO;
	do {
		if (((tag >> HUAWEI_TABLE) & 1) ||
			(tag == MNOTE_HUAWEI_FACE_INFO) ||
			(tag == MNOTE_HUAWEI_SCENE_INFO))
			break;

		if ((tag >> HUAWEI_FACE_TABLE) & 1) {
			owner_tag = MNOTE_HUAWEI_FACE_INFO;
			break;
		}

		owner_tag = MNOTE_HUAWEI_SCENE_INFO;
	} while(0);
	return owner_tag;
}

int is_ifd_tag(MnoteHuaweiTag tag)
{
	int b = 0;
	if ((tag == MNOTE_HUAWEI_INFO) ||
		(tag == MNOTE_HUAWEI_FACE_INFO) ||
		(tag == MNOTE_HUAWEI_SCENE_INFO))
		b = 1;
	return b;
}

MnoteHuaweiTag mnote_huawei_tag_from_name(const char *name)
{
	int i;
	MnoteHuaweiTag hw_tag = MNOTE_HUAWEI_INFO;
	int size = sizeof (huawei_table) / sizeof (huawei_table[0]) - 1;
	for (i = 0; i < size; i++) {
		if (!strcmp(huawei_table[i].name, name)) {
			hw_tag = huawei_table[i].tag;
			goto End;
		}
	}

	size = sizeof (huawei_face_table) / sizeof (huawei_face_table[0]) - 1;
	for (i = 0; i < size; i++) {
		if (!strcmp(huawei_face_table[i].name, name)) {
			hw_tag = huawei_face_table[i].tag;
			goto End;
		}
	}

	size = sizeof (huawei_scene_table) / sizeof (huawei_scene_table[0]) - 1;
	for (i = 0; i < size; i++) {
		if (!strcmp(huawei_scene_table[i].name, name)) {
			hw_tag = huawei_scene_table[i].tag;
			goto End;
		}
	}

End:
	return hw_tag;
}

const char *
mnote_huawei_tag_get_name (MnoteHuaweiTag tag)
{
	const char* p = NULL;
	int size = 0;
	const MnoteHuaweiTable* tb = get_tag_table(tag, &size);
	GET_TAG_INFO(tb, size, tag, name, p);
	return p;
}

const char *
mnote_huawei_tag_get_title (MnoteHuaweiTag tag)
{
	const char* p = NULL;
	int size = 0;
	const MnoteHuaweiTable* tb = get_tag_table(tag, &size);
	GET_TAG_INFO(tb, size, tag, title, p);
	return p;
}

const char *
mnote_huawei_tag_get_description (MnoteHuaweiTag tag)
{
	const char* p = NULL;
	int size = 0;
	const MnoteHuaweiTable* tb = get_tag_table(tag, &size);
	GET_TAG_INFO(tb, size, tag, description, p);
	return p;
}

MnoteHuaweiTagType
mnote_huawei_tag_type (MnoteHuaweiTag tag)
{
	MnoteHuaweiTagType p = 0;
	int size = 0;
	const MnoteHuaweiTable* tb = get_tag_table(tag, &size);
	GET_TAG_INFO(tb, size, tag, TagType, p);
	return p;
}
