# Requirements
- [luau-0.732](https://github.com/luau-lang/luau/archive/refs/tags/0.732.zip)

# Building Libraries
- Go to `Plugins/RenCore/RCoreLuau/Source/ThirdParty/Luau`.
- Run `mkdir cmake && cd cmake`
- Once the directory is created, run `cmake .. -DCMAKE_BUILD_TYPE=Release`
- This will generate a `.slnx` file if Visual Studio is installed.
- Open the solution and build `BUILD_ALL` with `Release` configuration.
- The libraries will be in `Release` folder.