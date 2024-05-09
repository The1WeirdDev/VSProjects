#include <jni.h>
#include <iostream>
#include <string>

extern "C" JNIEXPORT jstring  JNICALL
Java_com_VoxelGame_1Android_1Client_VoxelGame_1Android_1Client_Method(JNIEnv * env, jobject obj) {
	std::string hello = "HELLO FROM C++ MCBRIDE";
	return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT class Java_com_VoxelGame_1Android_1Client_Hello {
public:
	JNICALL jint Hello();
};