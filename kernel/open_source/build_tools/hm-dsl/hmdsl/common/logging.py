#!/usr/bin/env python3
# coding=utf-8
"""
Copyright (c) Huawei Technologies Co., Ltd. 2021-2021. All rights reserved.
Description: Logging for DSL
Author: Huawei OS Kernel Lab
Create: Thu 08 Apr 2021 05:00:48 PM CST
"""

from __future__ import absolute_import

from logging import Formatter, Logger, getLogger, setLoggerClass
from logging import StreamHandler
from logging import INFO
from typing import cast


__loggers = []


class DSLLogger(Logger):
    """
    DSL logger to log in certain format
    """
    def __init__(self, name: str):
        super(DSLLogger, self).__init__(name)

        str_hdr = StreamHandler()
        str_hdr.setFormatter(Formatter("[%(levelname)s - %(name)s] %(asctime)s %(message)s"))

        self.addHandler(str_hdr)


def get_logger(name: str, level=None) -> DSLLogger:
    """
    Get DSL logger which certain log level
    """
    log_level = INFO

    logger: DSLLogger = cast(DSLLogger, getLogger(name))
    logger.setLevel(log_level)
    __loggers.append(logger)

    if level:
        log_level = level
        set_log_level(log_level)

    return logger


def set_log_level(level):
    """
    To set the level of DSL logger
    """
    for logger in __loggers:
        logger.setLevel(level)


setLoggerClass(DSLLogger)
