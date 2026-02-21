#!/usr/bin/env python
# Copyright (c) 2022 Huawei Device Co., Ltd.
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os
import shutil
oriPath = os.path.dirname(os.path.abspath(__file__))
out_path = os.path.dirname(os.path.dirname(os.path.dirname(oriPath)))
newPath = oriPath + "/out/gen"
isExists=os.path.exists(newPath)
# 判断结果
if not isExists:
    print (newPath)
else:
    # 如果目录存在则清除后重建
    shutil.rmtree(newPath,True)
    os.makedirs(newPath) 
    print (newPath)