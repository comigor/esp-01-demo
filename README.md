# ESP-01 demo

Theoretically, the [ESP-01 I've bought](https://www.aliexpress.com/item/4000140068083.html) should have been configured as [esp01_1m (Generic ESP8266 ESP-01 1M)](https://docs.platformio.org/en/latest/boards/espressif8266/esp01_1m.html) on PlatformIO. However, it works flawlessly (and out-of-the-box) when configured as [nodemcuv2 (ESP-12E)](https://docs.platformio.org/en/latest/boards/espressif8266/nodemcuv2.html).

I've kept both working configurations.

Oh – on Windows, install [CP210X](https://www.silabs.com/developers/usb-to-uart-bridge-vcp-drivers) and [CH340X](https://github.com/justcallmekoko/ESP32Marauder/blob/master/Drivers/CH34x_Install_Windows_v3_4.EXE) drivers.<sup>[1](https://askubuntu.com/questions/1403705/dev-ttyusb0-not-present-in-ubuntu-22-04)</sup>

And on Linux, [remove `brltty`](https://askubuntu.com/a/1417483) and/or [comment udev rules](https://askubuntu.com/a/1403754).<sup>[1](https://askubuntu.com/questions/1403705/dev-ttyusb0-not-present-in-ubuntu-22-04) [2](https://www.reddit.com/r/pop_os/comments/uheuov/fix_usb_to_ttl_disconnection_issue/)</sup>