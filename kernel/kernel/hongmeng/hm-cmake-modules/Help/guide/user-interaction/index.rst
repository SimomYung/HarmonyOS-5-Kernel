User Interaction Guide
**********************

.. only:: html

   .. contents::

Introduction
============
Cmakeng provides PreHmbuild mechanism,approve user inject themselves
config before Hmbuild

.. code-block:: console

include(HongmengCMakeNG/Kconfig)
include(HongmengCMakeNG/PreHmbuild)
# There user call PreHmbuild function
hmbuild_config_function()
include(HongmengCMakeNG/HMbuild)

Extend Target Property
=======================
Set target property to manage hmbuild behavior.

.. code-block:: cmake

  hmbuild_config_target(<target>
                        [PROPERTY <name> [value1 ...]])

Config a target called ``<target>``.  The target should be defined in your hmbuild
by lib- or exec- command The options are:

``PROPERTY``
  Specify the property which you want to use

========================================== =============================================================
 Property                                   Meaning
========================================== =============================================================
HMBUILD_LINK_WHOLE_BUILTIN                 IF set True,hmbuild will link builtin.a with --whole-archieve
                                           Dynamic lib always link with --whole-archieve
HMBUILD_NO_DEFAULT_LINK                    IF set True,executable not link default libs
HMBUILD_CMAKETARGET_RAW                    Get cmake raw target through this variable, and then can
                                           append source to which target
HMBUILD_CMAKETARGET_FINAL                  Get the finally target that generates binary, and then can
                                           execute post build depneds which target
========================================== =============================================================
