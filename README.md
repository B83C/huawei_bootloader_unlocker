# Huawei Bootloader Unlocker
This tool lets you unlock the bootloader of your Huawei / Honor Device for free even if the code can't be found.

### How To use it
1. Clone the repository and compile files:
    1. `git clone https://github.com/B83C/huawei_bootloader_unlocker && cd huawei_bootloader_unlocker && make`
2. Install Android Developer Tools
3. Enable USB Debugging on your device
4. Enable OEM Unlock
3. Connect your device to your computer
4. Use ```adb reboot bootloader``` to reboot your device in bootloader mode
5. Run the program
    1. `sudo ./unlock`
6. Wait for it

## FAQ & Troubleshooting
**The app doesn't detect my device. What could be the issue?**
The issue is most likely caused by the lack of permission from udev.
*Fix*
Run the compiled piece of code with root access. For example, ```sudo ./unlock```
If the problem still persists, kindly look into this : [Click here](https://stackoverflow.com/questions/53887322/adb-devices-no-permissions-user-in-plugdev-group-are-your-udev-rules-wrong)

**

# License
This is a modified/improved version of [this original project](https://github.com/Martazza/Huawei-Bootloader-Unlocker)
```
MIT License

Copyright (c) 2021 B83C

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```
