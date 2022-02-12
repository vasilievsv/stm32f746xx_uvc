
;

cd C:\Program Files\USBPcap
USBPcapCMD.exe -d \\.\USBPcap3 -A  -o - | "C:\Program Files\Wireshark\Wireshark.exe" -k -i -