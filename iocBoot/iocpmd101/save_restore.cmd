# Autosave/restore
save_restoreSet_Debug(0)
save_restoreSet_IncompleteSetsOk(1)
save_restoreSet_DatedBackupFiles(1)

system "install -m 0777 -d $(TOP)/as/req/"
system "install -m 0777 -d $(TOP)/as/save/"

set_savefile_path("${TOP}/as","/save")
set_requestfile_path("${TOP}/as","/req")

set_pass0_restoreFile("info_settings.sav")
set_pass0_restoreFile("info_positions.sav")

cd $(TOP)/as/req
makeAutosaveFiles()
cd ${TOP}/iocBoot/${IOC}/
