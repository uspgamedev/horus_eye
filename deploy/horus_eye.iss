﻿; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{14804803-0D1C-4CF8-933E-8202270C54AF}
AppName=Horus Eye
AppVersion=1.1.2
AppVerName=Horus Eye v1.1.2
AppPublisher=USPGameDev
AppPublisherURL=http://www.uspgamedev.org/
AppSupportURL=http://www.uspgamedev.org/
AppUpdatesURL=http://www.uspgamedev.org/
DefaultDirName={pf}\Horus Eye
DefaultGroupName=Horus Eye
AllowNoIcons=true
OutputBaseFilename=Horus_Eye-v1.1.2_Installer
Compression=lzma
SolidCompression=true
SetupIconFile=eye.ico
ArchitecturesInstallIn64BitMode=x64

[Languages]
Name: english; MessagesFile: compiler:Default.isl
Name: brazilianportuguese; MessagesFile: compiler:Languages\BrazilianPortuguese.isl

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: ..\bin\Win32-Release\Horus Eye.exe; DestDir: {app}; Check: not Is64BitInstallMode; Flags: ignoreversion
Source: ..\bin\x64-Release\Horus Eye.exe; DestDir: {app}; Check: Is64BitInstallMode; Flags: ignoreversion
Source: ..\lib\*.dll; DestDir: {app}; Check: not Is64BitInstallMode; Flags: ignoreversion
Source: ..\lib64\*.dll; DestDir: {app}; Check: Is64BitInstallMode; Flags: ignoreversion
Source: ..\data\*; DestDir: {app}\data; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ..\license\*; DestDir: {app}\license; Flags: ignoreversion recursesubdirs createallsubdirs
Source: dotNetFx40_Full_setup.exe; DestDir: {app}; 
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: {group}\Horus Eye; Filename: {app}\Horus Eye.exe; IconIndex: 0
Name: {group}\{cm:UninstallProgram,Horus Eye}; Filename: {uninstallexe}; IconIndex: 0
Name: {commondesktop}\Horus Eye; Filename: {app}\Horus Eye.exe; Tasks: desktopicon; IconIndex: 0
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\Horus Eye; Filename: {app}\Horus Eye.exe; Tasks: quicklaunchicon; IconIndex: 0

[Run]
Filename: {app}\dotNetFx40_Full_setup.exe; Parameters: "/passive /promptrestart"; WorkingDir: {app}\; StatusMsg: Installing CRT...
Filename: {app}\Horus Eye.exe; Description: {cm:LaunchProgram,Horus Eye}; Flags: nowait postinstall skipifsilent

