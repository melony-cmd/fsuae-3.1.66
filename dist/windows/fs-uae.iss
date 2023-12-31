[Setup]
AppName=FS-UAE
AppVersion=3.1.66
AppVerName=FS-UAE
DefaultDirName={localappdata}\fs-uae
DefaultGroupName=FS-UAE
UninstallDisplayIcon={app}\Emulator\fs-uae.exe
OutputBaseFilename=fs-uae_3.1.66_windows_
OutputDir=../..
PrivilegesRequired=lowest
ShowLanguageDialog=no
DisableDirPage=yes
; DisableWelcomePage=yes
DisableReadyPage=yes
DisableStartupPrompt=yes
DisableProgramGroupPage=yes

[Files]
Source: "fs-uae_3.1.66_windows_\*"; DestDir: "{app}\"; Flags: recursesubdirs ignoreversion;

[installDelete]
Type: filesandordirs; Name: "{app}\Emulator"
Type: filesandordirs; Name: "{app}\Launcher"
Type: filesandordirs; Name: "{app}\Game Center"
Type: filesandordirs; Name: "{app}\share"
Type: filesandordirs; Name: "{app}\licenses"

Type: files; Name: "{userprograms}\FS-UAE\FS-UAE Emulator.lnk"
Type: files; Name: "{app}\CAPSImg.dll"
Type: files; Name: "{app}\COPYING.txt"
Type: files; Name: "{app}\example.conf"
Type: files; Name: "{app}\freetype6.dll"
Type: files; Name: "{app}\fs-uae.exe"
Type: files; Name: "{app}\fs-uae.log"
Type: files; Name: "{app}\libfreetype-6.dll"
Type: files; Name: "{app}\libgcc_s_dw2-1.dll"
Type: files; Name: "{app}\intl.dll"
Type: files; Name: "{app}\libglib-2.0-0.dll"
Type: files; Name: "{app}\libgthread-2.0-0.dll"
Type: files; Name: "{app}\libiconv-2.dll"
Type: files; Name: "{app}\libintl-8.dll"
Type: files; Name: "{app}\libpng14-14.dll"
Type: files; Name: "{app}\libpng15-15.dll"
Type: files; Name: "{app}\libstdc++-6.dll"
Type: files; Name: "{app}\OpenAL32.dll"
Type: files; Name: "{app}\README.txt"
Type: files; Name: "{app}\SDL.dll"
Type: files; Name: "{app}\stdout.txt"
Type: files; Name: "{app}\zlib1.dll"

[Icons]
;Name: "{group}\FS-UAE Emulator"; Filename: "{app}\fs-uae.exe"

[Registry]
Root: HKCU; Subkey: "SOFTWARE\Classes\Applications\fs-uae.exe\shell\open"; ValueType: string; ValueName: "FriendlyAppName"; ValueData: "FS-UAE Emulator"; Flags: uninsdeletekey

Root: HKCU; Subkey: "SOFTWARE\Classes\Fengestad.FS-UAE.Config.1"; ValueType: string; ValueName: ""; ValueData: "FS-UAE Config File"; Flags: uninsdeletekey
Root: HKCU; Subkey: "SOFTWARE\Classes\Fengestad.FS-UAE.Config.1"; ValueType: string; ValueName: "FriendlyTypeName"; ValueData: "FS-UAE Config File"; Flags: uninsdeletekey
Root: HKCU; Subkey: "SOFTWARE\Classes\Fengestad.FS-UAE.Config.1\shell\open"; ValueType: string; ValueName: "FriendlyAppName"; ValueData: "FS-UAE Emulator"; Flags: uninsdeletekey
Root: HKCU; Subkey: "SOFTWARE\Classes\Fengestad.FS-UAE.Config.1\shell\open\command"; ValueType: string; ValueName: ""; ValueData: """{app}\Emulator\fs-uae.exe"" ""%1"""; Flags: uninsdeletekey
; Root: HKCU; Subkey: "SOFTWARE\Classes\Fengestad.FS-UAE.Config.1\DefaultIcon"; ValueType: string; ValueName: ""; ValueData: "{app}\fs-uae.exe,2"; Flags: uninsdeletekey

Root: HKCU; Subkey: "SOFTWARE\Classes\.fs-uae"; ValueType: string; ValueName: ""; ValueData: "Fengestad.FS-UAE.Config.1"; Flags: uninsdeletekey
Root: HKCU; Subkey: "SOFTWARE\Classes\.fs-uae\OpenWithProgids"; ValueType: binary; ValueName: "Fengestad.FS-UAE.Config.1"; ValueData: ""; Flags: uninsdeletekey
