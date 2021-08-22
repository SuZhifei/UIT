
####################################################################################################

# Company       : Huizhou Desay SV Automotive Co., Ltd.

# Division      : Automotive Electronics, Desay Corporation

# Business Unit : Central Technology

# Department    : Advanced Development (Huizhou)

####################################################################################################

project(uit-screenfw-someip)

set(CAPI_FIDLS
	${CAPI_FIDL_DIR}/ScreenfwTypes.fidl
    ${CAPI_FIDL_DIR}/Controller.fidl
	${CAPI_FIDL_DIR}/Listener.fidl
	)

set(CAPI_LIB_NAME "uit_screenfw")

include("CAPI-GenerateSomeIPBinding.cmake")
