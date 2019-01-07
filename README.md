# RT-Thread #
RT-Thread for LS


### Download and compile the toolchain
https://github.com/riscv/riscv-tools


### Download the Scons
#### Ubuntu

    sudo apt-get install scons


#### Other system
Follow the tutorial form the link: https://www.scons.org/

### Config the toolchain
1. Open the file```rtconfig.py```
2. Find the```EXEC_PATH```below the```toolchains path```
3. Add your toolchain path after the```EXEC_PATH```

### Compile the RT-Thread

    cd RT-Thread && scons
    
When it completed,the console will print the informations like this：

    riscv64-unknown-elf-size rtthread.elf
      text	   data	    bss	    dec	    hex	filename
     44725	   1365	   6007	  52097	   cb81	rtthread.elf
    scons: done building targets.

Then you can load the```rtthread.elf```into your board
