cmake_minimum_required(VERSION 3.20)
project(backtest)

message(STATUS "start running cmake...")


include_directories("C:/Users/Nikita/Downloads/boost_1_79_0/boost_1_79_0")
include_directories("C:/Users/Nikita/Downloads/rapidjson/include")
include_directories("C:/Users/Nikita/Downloads/jsoncpp/include")

link_directories("C:/Users/Nikita/Downloads/boost_1_79_0/boost_1_79_0/stage/lib")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -cl -pthread -lboost_system")


add_executable(main
        main.cpp
        tests/simpleTests.hpp
        lib/candle.hpp
        lib/candles.cpp
        lib/candles.hpp
        lib/datetime.cpp
        lib/datetime.hpp
        backtest/backtest.hpp
        backtest/strategy.hpp
        lib/trade.hpp backtest/controller.hpp shared/config.hpp shared/config.cpp report/report.hpp)

target_link_libraries(main ${Boost_LIBRARIES})
