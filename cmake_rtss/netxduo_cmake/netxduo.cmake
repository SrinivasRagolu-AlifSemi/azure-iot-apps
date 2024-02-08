# Setting path for NETXDUO repo
set (NETXDUO_DIR        "${SRC_DIRECTORY}/netxduo")

# Setting paths and source files for NETX
set (NETXDUO_COMMON_DIR "${NETXDUO_DIR}/common")
file (GLOB NETXDUO_COMMON_SRC "${NETXDUO_COMMON_DIR}/src/*.c")
include_directories (${NETXDUO_COMMON_DIR}/inc)

set (NETXDUO_PORTS_DIR  "${NETXDUO_DIR}/ports/cortex_m55/ac6/inc")
include_directories (${NETXDUO_PORTS_DIR})
add_definitions (-DNXD_MQTT_CLOUD_ENABLE)
add_definitions (-DNX_SECURE_ENABLE)
add_definitions (-DNX_DNS_MAX_RETRIES=100)
add_definitions (-DNX_ENABLE_EXTENDED_NOTIFY_SUPPORT)
add_definitions (-DNX_DNS_CLIENT_USER_CREATE_PACKET_POOL)
add_definitions (-DNX_AZURE_DISABLE_IOT_SECURITY_MODULE)
add_definitions (-DNX_PACKET_ALIGNMENT=8)



# Setting path for Addon directory
set (NETXDUO_ADDONS_DIR "${NETXDUO_DIR}/addons")
#file (GLOB NETXDUO_ADDON_WEB    "${NETXDUO_ADDONS_DIR}/web/*.c")
file (GLOB NETXDUO_ADDON_DHCP   "${NETXDUO_ADDONS_DIR}/dhcp/nxd_dhcp_client.c")
file (GLOB NETXDUO_ADDON_AUTO_IP   "${NETXDUO_ADDONS_DIR}/auto_ip/nx_auto_ip.c")
file (GLOB NETXDUO_ADDON_DNS   "${NETXDUO_ADDONS_DIR}/dns/nxd_dns.c")
file (GLOB NETXDUO_ADDON_MQTT   "${NETXDUO_ADDONS_DIR}/mqtt/nxd_mqtt_client.c")
file (GLOB NETXDUO_ADDON_SNTP   "${NETXDUO_ADDONS_DIR}/sntp/nxd_sntp_client.c")
file (GLOB NETXDUO_ADDON_CLOUD   "${NETXDUO_ADDONS_DIR}/cloud/nx_cloud.c")
file (GLOB NETXDUO_ADDON_AZURE_IOT   "${NETXDUO_ADDONS_DIR}/azure_iot/*.c")
file (GLOB NETXDUO_ADDON_AZURE_IOT_CORE   "${NETXDUO_ADDONS_DIR}/azure_iot/azure-sdk-for-c/sdk/src/azure/core/*.c")
file (GLOB NETXDUO_ADDON_AZURE_IOT_IOT   "${NETXDUO_ADDONS_DIR}/azure_iot/azure-sdk-for-c/sdk/src/azure/iot/*.c")
file (GLOB NETXDUO_ADDON_AZURE_IOT_NO_HTTP   "${NETXDUO_ADDONS_DIR}/azure_iot/azure-sdk-for-c/sdk/src/azure/platform/az_nohttp.c")
file (GLOB NETXDUO_ADDON_AZURE_IOT_NO_PLATFORM   "${NETXDUO_ADDONS_DIR}/azure_iot/azure-sdk-for-c/sdk/src/azure/platform/az_noplatform.c")
file (GLOB NETXDUO_SECURE   "${NETXDUO_DIR}/nx_secure/src/*.c")
file (GLOB NETXDUO_CRYPTO  "${NETXDUO_DIR}/crypto_libraries/src/*.c")
include_directories (${NETXDUO_ADDONS_DIR}/web)
include_directories (${NETXDUO_ADDONS_DIR}/dhcp)
include_directories (${NETXDUO_ADDONS_DIR}/auto_ip)
include_directories (${NETXDUO_ADDONS_DIR}/dns)
include_directories (${NETXDUO_ADDONS_DIR}/mqtt)
include_directories (${NETXDUO_ADDONS_DIR}/sntp)
include_directories (${NETXDUO_ADDONS_DIR}/cloud)
include_directories (${NETXDUO_DIR}/nx_secure/ports)
include_directories (${NETXDUO_DIR}/nx_secure/inc)
include_directories (${NETXDUO_DIR}/crypto_libraries/inc)
include_directories (${NETXDUO_ADDONS_DIR}/azure_iot/azure-sdk-for-c/sdk/src/azure/core)
include_directories (${NETXDUO_ADDONS_DIR}/ports/cortex_m55/ac6/inc)
include_directories (${NETXDUO_ADDONS_DIR}/azure_iot)
include_directories (${NETXDUO_ADDONS_DIR}/azure_iot/azure-sdk-for-c/sdk/inc)
include_directories (${NETXDUO_ADDONS_DIR}/azure_iot/azure-sdk-for-c/sdk/inc/azure/iot)
include_directories (${NETXDUO_ADDONS_DIR}/azure_iot/azure-sdk-for-c/sdk/inc/azure/core)
file (GLOB NETXDUO_ADDON_FILES ${NETXDUO_ADDON_WEB} ${NETXDUO_ADDON_AUTO_IP} ${NETXDUO_ADDON_DHCP} ${NETXDUO_ADDON_DNS} ${NETXDUO_ADDON_MQTT} ${NETXDUO_ADDON_SNTP} ${NETXDUO_ADDON_CLOUD} ${NETXDUO_ADDON_AZURE_IOT_CORE} ${NETXDUO_ADDON_AZURE_IOT_IOT} ${NETXDUO_ADDON_AZURE_IOT_NO_HTTP} ${NETXDUO_ADDON_AZURE_IOT_NO_PLATFORM} ${NETXDUO_SECURE} ${NETXDUO_CRYPTO} ${NETXDUO_ADDON_AZURE_IOT})
# Collecting all netx related source files under one variable
file (GLOB NETXDUO_SRC ${NETXDUO_COMMON_SRC} ${NETXDUO_ADDON_FILES})

# Creating a Library file for NETXDUO_SRC
set (NETXDUO_LIB     "NETXDUO")
add_library (${NETXDUO_LIB} STATIC ${NETXDUO_SRC})
