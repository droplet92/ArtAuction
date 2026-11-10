# Unit tests for ArtAuctionCore. Run them with `ctest -C Debug` from the build directory.
add_executable(ArtAuctionCoreTests
    AiPlayerTest.cpp
    AuctionTest.cpp
    GameTest.cpp
    LocalSessionTest.cpp
    PlayerTest.cpp
    ReputationTest.cpp
    TestGames.h
    WholeGameTest.cpp
    )
target_link_libraries(ArtAuctionCoreTests PRIVATE ArtAuctionCore GTest::gtest_main)

if(MSVC)
    target_compile_options(ArtAuctionCoreTests PRIVATE /utf-8)
endif()

include(GoogleTest)
gtest_discover_tests(ArtAuctionCoreTests)
