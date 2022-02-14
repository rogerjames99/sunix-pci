I have just managed to get a Sunix pci-e parallel port card to provide the correct /proc/sys/dev file system structure to allow the board to be driven from the LinuCNC rtapi on modern Linux kernels. I have tested on both Linux v4 and v5.

This is how I went about it.

1. DO NOT use the sunix provided linux driver. If you have installed it purge it and its configuration files from your system.

2. Use lspci to find the domain id of your device. It is the bit at the start of each line that looks like this nn:nn.n e.g. 02:00.0.

3. Use lspci -vvns nn:nn.n  (Use your boards domain id) to find the vendor code and device codes. These are the two 4 digit hexadecimal codes separated by a colon at the end of the first line of output. Further down you will find the boards IRQ and io port address. Depending on  how many ports your particular board has there will a different number of IO port regions. In my 1S1P board the correct one was Region 1.

4. Add a file (I called mine sunix.conf) to /etc/modprobe containing the following lines.

   ```
   #LPC parport to PCI-E card
   alias parport_low_level parport_pc
   options parport_pc io=0xd050 irq=11,auto
   ```

5. Add a file to /etc/udev/rules.d (I called mine 99-sunix.rules) with following line. 

   ```
   ATTR{device}=="0x1999", ATTR{vendor}=="0x1fd4", RUN+="/sbin/modprobe parport_low_level"
   ```

That all you need to do. If you reboot you should a file structure something like under /proc/sys/dev/parport.

```
parport
├── default
│   ├── spintime
│   └── timeslice
└── parport0
    ├── autoprobe
    ├── autoprobe0
    ├── autoprobe1
    ├── autoprobe2
    ├── autoprobe3
    ├── base-addr
    ├── devices
    │   └── active
    ├── dma
    ├── irq
    ├── modes
    └── spintime

```

I have attached examples of the above two configuration files.
