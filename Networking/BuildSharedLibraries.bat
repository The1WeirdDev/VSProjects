cd ../
g++ -D NETWORKING_STATIC_BUILD -D ANDROID_BUILD -D ASIO_STANDALONE -D BOOST_ALL_NO_LIB -D DBOOST_ALL_NO_LIB -std=gnu++11 -c -INetworking/include/ Networking/include/Networking/Packet.cpp Networking/include/Networking/pch.cpp Networking/include/Networking/dllmain.cpp
g++ -shared -o Libraries/Networking/Shared/Networking.so dllmain.o pch.o Packet.o
ar rvs Libraries/Networking/Shared/Networking.a dllmain.o pch.o Packet.o
rm -f *.o
@PAUSE