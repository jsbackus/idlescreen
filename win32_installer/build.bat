@echo off
#makensis /Ocompiler.log /DQTDIR=%QTDIR% installer.nsi
makensis /DQTDIR=%QTDIR% installer.nsi
