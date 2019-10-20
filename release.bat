mkdir release-files
mkdir release-files\AirQuest
mkdir release-files\AirQuest\logs
xcopy /y ALVR\bin\Release\AirQuest.exe release-files\AirQuest
xcopy /y CrashReport\bin\Release\CrashReport.exe release-files\AirQuest
xcopy /y ALVR\bin\Release\*.dll release-files\AirQuest
xcopy /y/s/e/i driver release-files\AirQuest\driver
xcopy /y libswresample\lib\*.dll release-files\AirQuest\driver\bin\win64
xcopy /y add_firewall_rules.bat release-files\AirQuest
xcopy /y remove_firewall_rules.bat release-files\AirQuest

cd release-files
"C:\Program Files (x86)\7-Zip\7z.exe" a -tzip AirQuest-portable-v.zip AirQuest
cd ..

"C:\Program Files (x86)\Inno Setup 5\ISCC.exe" installer\installer.iss

pause
