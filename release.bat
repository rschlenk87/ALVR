mkdir release-files
mkdir release-files\ALVR
mkdir release-files\ALVR\logs
xcopy /y ALVR\bin\Release\ALVR.exe release-files\ALVR
xcopy /y CrashReport\bin\Release\CrashReport.exe release-files\ALVR
xcopy /y ALVR\bin\Release\*.dll release-files\ALVR
xcopy /y/s/e/i driver release-files\ALVR\driver
xcopy /y libswresample\lib\*.dll release-files\ALVR\driver\bin\win64
xcopy /y add_firewall_rules.bat release-files\ALVR
xcopy /y remove_firewall_rules.bat release-files\ALVR

cd release-files
"C:\Program Files (x86)\7-Zip\7z.exe" a -tzip ALVR-portable-v.zip ALVR
cd ..

"C:\Program Files (x86)\Inno Setup 5\ISCC.exe" installer\installer.iss

pause
