
####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

project(uit-datafw-someip)

set(CAPI_FIDLS
    ${CAPI_FIDL_DIR}/ControllerServer.fidl
    ${CAPI_FIDL_DIR}/ViewServer.fidl
    ${CAPI_FIDL_DIR}/DatafwTypes.fidl)

set(CAPI_LIB_NAME "uit_datafw")

include("CAPI-GenerateSomeIPBinding.cmake")
