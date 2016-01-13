file(GLOB DRIVER_LIB_SRC
    ${CMAKE_CURRENT_SOURCE_DIR}/driver/*.c
)

add_library(driver_lib STATIC
    ${DRIVER_LIB_SRC}
)

target_include_directories(driver_lib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
    ${CMAKE_SOURCE_DIR}
)

target_link_libraries(driver_lib PUBLIC
    ESP8266_SDK
    user_code
)
