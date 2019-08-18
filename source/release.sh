if [ -d build ] 
then
    rm -rf build
fi

mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Release && 
    cmake --build . && mv -t .. compile_commands.json && cd ..

