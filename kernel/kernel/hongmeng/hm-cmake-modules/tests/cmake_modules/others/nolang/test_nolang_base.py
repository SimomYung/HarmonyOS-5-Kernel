#!/usr/bin/env python
# -*- coding: UTF-8 -*

# Copyright (C) Huawei Technologies Co., Ltd. 2021. All rights reserved.
# Author: Huawei OS Kernel Lab
# Create: Sun Sep 26 20:30:53 2021

import unittest
from tools.projectmodule.nolangproject import NoLangProject
from tools.buildmodule.buildproject import HmBuildProject
from tools.hmtest import HmBuildTestCase


class TestNoLang(HmBuildTestCase):
    def setUp(self):
        super().setUp()
        self.nolangProject = NoLangProject()
        self.nolangProject.add_project_header("header.h", ["test"])
        self.nolangProject.add_installinc(filename="header.h")
        self.buildProject = HmBuildProject(
            self.nolangProject, self.workPath, self.sysrootPath,
            self.insatllCmakeModlulePath)

    def tearDown(self):
        super().tearDown()
        self.buildProject.clean_all()

    def test_base(self):
        self.buildProject.build()
        self.assertMaKeSuccess(self.buildProject)


if __name__ == '__main__':
    unittest.main()
