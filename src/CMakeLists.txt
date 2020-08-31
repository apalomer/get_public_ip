add_executable(get_public_ip
    getpublicip.cpp
    getpublicip.h
    main.cpp)
target_link_libraries(get_public_ip Qt5::Core Qt5::Widgets Qt5::Network)
