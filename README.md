# Azure IoT Applications
This repo is created to build Azure IoT applications using the CMake build system. This repo contains the setup file [`setup_user_env.sh`](./setup_user_env.sh/),   
to set environment variables from the terminal. [`run.sh`](./run.sh/) script is used to build the sample application, which we can verify the following features.

* Telemetry
* Cloud-to-Device message
* Direct method
* Device twin

It uses the Ensemble Ethernet driver along with [netxduo](https://github.com/eclipse-threadx/netxduo) middleware for connectivity and [azure-iot-middleware](https://github.com/Azure/azure-sdk-for-c)
to connect azure iothub using SAS security tokens

# Setup
Ensemble devkit B1 has been used to run the sample applications. Built image deployed on M55/HE core

# Host requirements to build

* Ubuntu 20.04 LTS or Ubuntu MATE 20.04 LTS
* CMake version 3.23.2 or higher
* Toolchain
  * Option-1: Arm clang compiler toolchain (ARMDS 2022.0)
  * Option-2: Arm GNU GCC compiler toolchain (10.3-2021.10)

# Azure IoT cloud setup
## Create an IoT Hub

You should have valid Azure subscription to use the Azure IoT services.
You can use Azure CLI to create an IoT hub that handles events and messaging for your device.

## To create an IoT hub:
### method 1

Azure IoT Hub is a managed service hosted in the cloud that acts as a central message hub for communication between an IoT application and its attached devices. You can refer to Azure documentation to create IoTHub using [Create IoTHub](https://learn.microsoft.com/en-us/azure/iot-hub/iot-hub-create-through-portal)
### method 2
1. In your CLI console, run the az extension add command to add the Microsoft Azure IoT Extension for Azure CLI to your CLI shell. The IOT Extension adds IoT Hub, IoT Edge, and IoT Device Provisioning Service (DPS) specific commands to Azure CLI.
   
  ```bash
  $ az extension add --name azure-iot
  ```

2. Run the az group create command to create a resource group. The following command creates a resource group named MyResourceGroup in the eastus region.
Note: Optionally, to set an alternate location, run az account list-locations to see available locations. Then specify the alternate location in the following command in place of eastus.

  ```bash
  $ az group create --name MyResourceGroup --location eastus
  ```

3. Run the az iot hub create command to create an IoT hub. It might take a few minutes to create an IoT hub.
YourIotHubName. Replace this placeholder below with the name you chose for your IoT hub. An IoT hub name must be globally unique in Azure. This placeholder is used in the rest of this tutorial to represent your unique IoT hub name.

  ```bash
  $ az iot hub create --resource-group MyResourceGroup --name {YourIoTHubName}
  ```

4. After the IoT hub is created, view the JSON output in the console, and copy the hostName value to a safe place. You use this value in a later step. The hostname value looks like the following example:
  ##### {Your IoT hub name}.azure-devices.net

## Register a new device in the IoT hub

### method 1:
In this section, you create a new device instance and register it with the IoTHub you created using the documentation [Registration of a new device](https://learn.microsoft.com/en-us/azure/iot-hub/iot-hub-create-through-portal) 

You will use the connection information for the newly registered device to securely connect your physical device in a later section.
To register a device:

### method 2:

1. In your console, run the az iot hub device-identity create command. This creates the simulated device identity.
YourIotHubName: Replace this placeholder below with the name you chose for your IoT hub.
myiotdevice: You can use this name directly for the device in CLI commands in this tutorial. Optionally, use a different name.

```bash
$az iot hub device-identity create --device-id myiotdevice --hub-name {YourIoTHubName}
```

2. After the device is created, view the JSON output in the console, and copy the deviceId and primaryKey values to use in a later step.
Confirm that you have copied the following values from the JSON output to use in the next section:

a. hostName
b. deviceId
c. primaryKey

|   String               |  Value                            | 
|------------------------|-----------------------------------|
|   HOST_NAME            |  {Your IoT hub hostName value}    |
|   DEVICE_ID            |  {Your deviceID value}            |
|   DEVICE_SYMMETRIC_KEY |  {Your primaryKey value}          |

These values need to be updated in [`sample_config.h`](./source/netxduo/addons/azure_iot/samples/sample_config.h) to connect the IoTHub in this project

# Build Instructions

1. Clone the git repo
   
  ```bash
  git clone https://github.com/SrinivasRagolu-AlifSemi/azure-iot-apps.git
  ```

2. Install the toolchain(``ARMCLAG/GCC``) and apply the license
   
3. Update the variables ``COMPILER``, ``ARM_PRODUCT_DEF``, ``COMPILER_BIN_PATH`` in setup_user_env.sh according to your host setup

4. Update the ``SAMPLE_IPV4_ADDRESS`` in [``main.c``](.//source/netxduo/addons/azure_iot/samples/main.c) according to your network setup
   
5. Enable the features in the application by commenting ``DISABLE_TELEMETRY_SAMPLE``, ``DISABLE_C2D_SAMPLE``, ``DISABLE_DIRECT_METHOD_SAMPLE``, ``DISABLE_DEVICE_TWIN_SAMPLE``
   
6. Update the ``HOST_NAME``, ``DEVICE_ID`` and ``DEVICE_SYMMETRIC_KEY`` values derived in the above section.
   #### Register a new device in the IoT hub
   
7. Run the CMake command to build the image depending on the configuration required

   ```bash
   $./run.sh DEVICE=RTSS_HE/RTSS_HP PACK=ENSEMBLE OS=THREADX BOOT=MRAM/TCM TEST_APP=main CLEAN=YES/NO DEVELOPER=YES
   ```
8. Image binaries should be in the directory ``${PWD}/build_rtss/ARMCLANG/exec/ensemble/threadx/HE/MRAM``` to validate.




