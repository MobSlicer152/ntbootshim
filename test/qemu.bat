@echo off

setlocal

set "fw=%PROGRAMFILES%\qemu\share\edk2-x86_64-code.fd"

qemu-system-x86_64 -m 1G -drive if=pflash,unit=0,format=raw,readonly=on,file="%fw%" -serial stdio -drive file=fat:rw:%~dp0out,format=raw -device qemu-xhci

