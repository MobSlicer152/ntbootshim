@echo off

setlocal

set "id={49ddbfb8-5333-11f1-83a5-8c882b6543b4}"
set "store=%~dp0out\EFI\Microsoft\Boot\BCD"

del "%store%"
bcdedit /createstore "%store%"

bcdedit /store "%store%" /create {bootmgr}
bcdedit /store "%store%" /set {bootmgr} path \EFI\Microsoft\Boot\bootmgfw.efi

bcdedit /store "%store%" /create "%id%" /d "NT Boot Shim" /application osloader
bcdedit /store "%store%" /set "%id%" systemroot \Shim
bcdedit /store "%store%" /set "%id%" kernel ntbootshim.exe
bcdedit /store "%store%" /set "%id%" hal dummyhal.dll
bcdedit /store "%store%" /set "%id%" path \Shim\winload.efi
bcdedit /store "%store%" /set "%id%" device boot
bcdedit /store "%store%" /set "%id%" osdevice boot
bcdedit /store "%store%" /set "%id%" testsigning on

bcdedit /store "%store%" /displayorder "%id%"
bcdedit /store "%store%" /default "%id%"

bcdedit /store "%store%" /enum

del /a:h "%store%.LOG?"
