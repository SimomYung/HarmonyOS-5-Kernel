/*
 * Copyright (c) 2024 Huawei Device Co., Ltd.
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
#ifndef foodownmixhfoo
#define foodownmixhfoo
#include <pulse/channelmap.h>
#include <config.h>
#include <pulsecore/bitset.h>

/* formats that can be downmixed with channelMixMatrix */
typedef enum pa_channel_layout_index {
    PA_CHANNEL_LAYOUT_OTHER = -1,
    PA_CHANNEL_LAYOUT_STEREO = 0,
    PA_CHANNEL_LAYOUT_5POINT1,
    PA_CHANNEL_LAYOUT_5POINT1POINT2,
    PA_CHANNEL_LAYOUT_5POINT1POINT4,
    PA_CHANNEL_LAYOUT_7POINT1,
    PA_CHANNEL_LAYOUT_7POINT1POINT2,
    PA_CHANNEL_LAYOUT_7POINT1POINT4,
    PA_CHANNEL_LAYOUT_COUNT
} pa_channel_layout_index_t;

#define RESCALE_COEF 10000
#define SKIPPED_CHANNEL_COUNT 32
enum channel_position_downmix {
    PA_DOWNMIX_POSITION_TOP_CENTER = PA_CHANNEL_POSITION_TOP_CENTER - SKIPPED_CHANNEL_COUNT,   /* AUX0 - AUX31 unused */
    PA_DOWNMIX_POSITION_TOP_FRONT_LEFT,           /**< Apple calls this 'Vertical Height Left' */
    PA_DOWNMIX_POSITION_TOP_FRONT_RIGHT,          /**< Apple calls this 'Vertical Height Right' */
    PA_DOWNMIX_POSITION_TOP_FRONT_CENTER,         /**< Apple calls this 'Vertical Height Center' */
    PA_DOWNMIX_POSITION_TOP_REAR_LEFT,            /**< Microsoft and Apple call this 'Top Back Left' */
    PA_DOWNMIX_POSITION_TOP_REAR_RIGHT,           /**< Microsoft and Apple call this 'Top Back Right' */
    PA_DOWNMIX_POSITION_TOP_REAR_CENTER,          /**< Microsoft and Apple call this 'Top Back Center' */
    PA_DOWNMIX_CHANNEL_MAX
};

extern const uint16_t channelDownmixMatrix[PA_CHANNEL_LAYOUT_COUNT][PA_DOWNMIX_CHANNEL_MAX][PA_DOWNMIX_CHANNEL_MAX];

/* check if input channel map is supported by downmix matrix, otherwise return PA_FORMAT_OTHER*/
pa_channel_layout_index_t pa_channel_map_to_index(const pa_channel_map *map) PA_GCC_PURE;
int pa_to_downmix_position(const pa_channel_position_t channel_position);

#endif