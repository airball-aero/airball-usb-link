add_library(airball-usb-link
        Base16Encoding.cpp
        Telemetry.cpp)

add_executable(Base16EncodingTest
        Base16EncodingTest_main.cpp)
target_link_libraries(Base16EncodingTest
        airball-usb-link)

add_executable(AbstractSerialLinkTest
        AbstractSerialLinkTest_main.cpp)
target_link_libraries(AbstractSerialLinkTest
        airball-usb-link)
