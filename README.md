# ArtAuction
Art Auction, inspirated by "Modern Art", is a game that trade modern paintings and make your gallery the best. 

## Development Environment
- cocos2d-x v4
- Visual Studio 2022
- C++20
- Sqlite3

## How to Build the Project
1. Clone this repository
```powershell
git clone https://github.com/droplet92/ArtAuction
```
2. Move to the repository
```powershell
cd ArtAuction
```
3. Run 'setup.bat'
```powershell
./setup.bat
```
It checks out the cocos2d-x 4.0 and GoogleTest submodules, downloads the engine's third-party libraries, and generates the Visual Studio solution. It needs Git, CMake, and Windows 10 or later for `curl` and `tar`.
Clone without `--recurse-submodules`: cocos2d-x's own nested submodules use the retired `git://` protocol and are not needed.
4. Move to win32-build directory and open ArtAuction.sln
5. Done!

## How to Run the Tests
The game rules live in the `ArtAuctionCore` library (`Core/`), which builds without cocos2d-x and is covered by GoogleTest unit tests in `tests/`.
```powershell
cmake --build win32-build --config Debug --target ArtAuctionCoreTests
ctest --test-dir win32-build -C Debug
```
Configure with `-DARTAUCTION_BUILD_TESTS=OFF` to skip them.
