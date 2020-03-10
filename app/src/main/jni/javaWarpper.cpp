#include <jni.h>
#include <string>
#include <iostream>
#include "include/Pipeline.h"
#include <time.h>
#include <android/log.h>

const char *JNI_TAG = "spd_car";
#define LOGI(fmt, args...) __android_log_print(ANDROID_LOG_INFO,  JNI_TAG, fmt, ##args)
//#define //LOGD(fmt, args...) __android_log_print(ANDROID_LOG_DEBUG, JNI_TAG, fmt, ##args)
#define LOGD(fmt, args...)
#define LOGE(fmt, args...) __android_log_print(ANDROID_LOG_ERROR, JNI_TAG, fmt, ##args)
#define LOGW(fmt, args...) __android_log_print(ANDROID_LOG_WARN,  JNI_TAG, fmt, ##args)
#define LOGF(fmt, args...) __android_log_print(ANDROID_LOG_FATAL, JNI_TAG, fmt, ##args)


std::string jstring2str(JNIEnv *env, jstring jstr) {
    char *rtn = NULL;
    jclass clsstring = env->FindClass("java/lang/String");
    jstring strencode = env->NewStringUTF("GB2312");
    jmethodID mid = env->GetMethodID(clsstring, "getBytes", "(Ljava/lang/String;)[B");
    jbyteArray barr = (jbyteArray) env->CallObjectMethod(jstr, mid, strencode);
    jsize alen = env->GetArrayLength(barr);
    jbyte *ba = env->GetByteArrayElements(barr, JNI_FALSE);
    if (alen > 0) {
        rtn = (char *) malloc(alen + 1);
        memcpy(rtn, ba, alen);
        rtn[alen] = 0;
    }
    env->ReleaseByteArrayElements(barr, ba, 0);
    std::string stemp(rtn);
    free(rtn);
    return stemp;
}


extern "C" {
JNIEXPORT jlong JNICALL
Java_com_pcl_lpr_utils_PlateRecognition_InitPlateRecognizer(
        JNIEnv *env, jobject obj,
        jstring detector_filename,
        jstring finemapping_prototxt, jstring finemapping_caffemodel,
        jstring segmentation_prototxt, jstring segmentation_caffemodel,
        jstring charRecognization_proto, jstring charRecognization_caffemodel,
        jstring segmentationfree_proto, jstring segmentationfree_caffemodel) {

    std::string detector_path = jstring2str(env, detector_filename);
    std::string finemapping_prototxt_path = jstring2str(env, finemapping_prototxt);
    std::string finemapping_caffemodel_path = jstring2str(env, finemapping_caffemodel);
    std::string segmentation_prototxt_path = jstring2str(env, segmentation_prototxt);
    std::string segmentation_caffemodel_path = jstring2str(env, segmentation_caffemodel);
    std::string charRecognization_proto_path = jstring2str(env, charRecognization_proto);
    std::string charRecognization_caffemodel_path = jstring2str(env, charRecognization_caffemodel);
    std::string segmentationfree_proto_path = jstring2str(env, segmentationfree_proto);
    std::string segmentationfree_caffemodel_path = jstring2str(env, segmentationfree_caffemodel);

    pr::PipelinePR *PR = new pr::PipelinePR(detector_path,
                                            finemapping_prototxt_path, finemapping_caffemodel_path,
                                            segmentation_prototxt_path,
                                            segmentation_caffemodel_path,
                                            charRecognization_proto_path,
                                            charRecognization_caffemodel_path,
                                            segmentationfree_proto_path,
                                            segmentationfree_caffemodel_path);
    return (jlong) PR;
}


JNIEXPORT jstring JNICALL
Java_com_pcl_lpr_utils_PlateRecognition_SimpleRecognization(
        JNIEnv *env, jobject obj,
        jlong matPtr, jlong object_pr) {
    clock_t start, finish;
    double totaltime;
    start = clock();

    pr::PipelinePR *PR = (pr::PipelinePR *) object_pr;
    cv::Mat &mRgb = *(cv::Mat *) matPtr;
    cv::Mat rgb;
//    cv::cvtColor(mRgb,rgb,cv::COLOR_RGBA2GRAY);
    cv::cvtColor(mRgb, rgb, cv::COLOR_RGBA2BGR);
    try {
        //1表示SEGMENTATION_BASED_METHOD在方法里有说明
        std::vector<pr::PlateInfo> list_res = PR->RunPiplineAsImage(rgb,
                                                                    pr::SEGMENTATION_FREE_METHOD);
        std::string concat_results;
        for (auto one:list_res) {
            if (one.confidence > 0.7)
                concat_results += one.getPlateName() + ",";
        }
        concat_results = concat_results.substr(0, concat_results.size() - 1);

        finish = clock();
        totaltime = (double) (finish - start) / CLOCKS_PER_SEC;

        jstring pJstring = env->NewStringUTF(concat_results.c_str());
        if (concat_results.size() > 0) {
            const char *str = env->GetStringUTFChars( pJstring, 0);
            LOGI("\nspend time：%f,car num:%s", totaltime, str);
        }
        return pJstring;
    } catch (std::exception e) {
        return env->NewStringUTF("");
    }
}
JNIEXPORT void JNICALL
Java_com_pcl_lpr_utils_PlateRecognition_ReleasePlateRecognizer(JNIEnv *env, jobject obj,
                                                               jlong object_re) {
    pr::PipelinePR *PR = (pr::PipelinePR *) object_re;
    delete PR;
}
}