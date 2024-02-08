This directory contains all the cmake files which includes the header directories and source files required to compile and generate binaries for Cortex-M55 core.

1. device_cmake     --> This contains cmake file which has the details related to Devices extsys0 and extsys1 (RTSS_HP/RTSS_HE, M55_HP/M55_HE).

2. drivers_cmake    --> This contains cmake file which has the details of source files and header files of all the supporting drivers for cortex_m55.

3. ensemble_cmake   --> This contains cmake file which has the details of source files and header files with respect to Ensemble package.

4. modem_ss_cmake   --> This contains cmake file which has the details of Middleware source files and header files with respect to modem core and GNSS core.

5. netxduo_cmake    --> This contains cmake file which has the details of source files and header files with respect to netowrk module.

6. os_cmake         --> This contains cmake files which has the details of OS related source files and header files to include with respect to different Operating Systems. Also it has details of Test applications w.r.t each OS.

7. toolchains       --> This contains cmake file which has the compiler toolchain related details.

8. usbx_cmake       --> This contains cmake file which has the details of source files and header files of driver USBX.

9. CMakeLists.txt   --> This is the main cmake file, which links all the above cmake files based on the configuration and requirement. This also has Test application source files related inclusions for the build.

10. utilities_func.cmake --> This cmake file has helper functions and macro definitions required for the build.

11. rtss_compiler_config.cmake --> This script can be used to configure default compiler arguments.