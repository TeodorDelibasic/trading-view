#include "tradingView_Crawler.h"
#include "crawler.h"

JNIEXPORT jstring JNICALL Java_tradingView_Crawler_getInfo
(JNIEnv* env, jclass, jstring url) {

    const jclass stringClass = env->GetObjectClass(url);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray stringJbytes = (jbyteArray)env->CallObjectMethod(url, getBytes, env->NewStringUTF("UTF-8"));

    size_t length = (size_t)env->GetArrayLength(stringJbytes);
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, NULL);

    std::string ret = std::string((char*)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);

    Crawler cr;

    return (env)->NewStringUTF(cr(ret).c_str());
}