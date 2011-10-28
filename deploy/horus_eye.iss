; Script generated by the Inno Setup Script Wizard.
; SEE THE DOCUMENTATION FOR DETAILS ON CREATING INNO SETUP SCRIPT FILES!

[Setup]
; NOTE: The value of AppId uniquely identifies this application.
; Do not use the same AppId value in installers for other applications.
; (To generate a new GUID, click Tools | Generate GUID inside the IDE.)
AppId={{14804803-0D1C-4CF8-933E-8202270C54AF}
AppName=Horus Eye
AppVersion=1.1.0
;AppVerName=Horus Eye Release 1.1.0
AppPublisher=USPGameDev
AppPublisherURL=http://uspgamedev.org/
AppSupportURL=http://uspgamedev.org/
AppUpdatesURL=http://uspgamedev.org/
DefaultDirName={pf}\Horus Eye
DefaultGroupName=Horus Eye
AllowNoIcons=true
OutputBaseFilename=horus_eye
SetupIconFile=eye.ico
Compression=lzma
SolidCompression=true
AppVerName=1.1.0

[Languages]
Name: english; MessagesFile: compiler:Default.isl
Name: brazilianportuguese; MessagesFile: compiler:Languages\BrazilianPortuguese.isl

[Tasks]
Name: desktopicon; Description: {cm:CreateDesktopIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked
Name: quicklaunchicon; Description: {cm:CreateQuickLaunchIcon}; GroupDescription: {cm:AdditionalIcons}; Flags: unchecked; OnlyBelowVersion: 0,6.1

[Files]
Source: ..\Release\Horus Eye.exe; DestDir: {app}; Flags: ignoreversion
Source: ..\data\*; DestDir: {app}\data; Flags: ignoreversion recursesubdirs createallsubdirs
Source: ..\lib\*.dll; DestDir: {app}; Flags: ignoreversion
; NOTE: Don't use "Flags: ignoreversion" on any shared system files

[Icons]
Name: {group}\Horus Eye; Filename: {app}\Horus Eye.exe; IconIndex: 0
Name: {group}\{cm:UninstallProgram,Horus Eye}; Filename: {uninstallexe}
Name: {commondesktop}\Horus Eye; Filename: {app}\Horus Eye.exe; Tasks: desktopicon; IconIndex: 0
Name: {userappdata}\Microsoft\Internet Explorer\Quick Launch\Horus Eye; Filename: {app}\Horus Eye.exe; Tasks: quicklaunchicon; IconIndex: 0

[Run]
Filename: {app}\Horus Eye.exe; Description: {cm:LaunchProgram,Horus Eye}; Flags: nowait postinstall skipifsilent

