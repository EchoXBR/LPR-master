//
// Created by 庾金科 on 22/10/2017.
//

#ifndef SWIFTPR_PIPLINE_H
#define SWIFTPR_PIPLINE_H

#include "PlateDetection.h"
#include "PlateSegmentation.h"
#include "CNNRecognizer.h"
#include "PlateInfo.h"
#include "FastDeskew.h"
#include "FineMapping.h"
#include "Recognizer.h"
#include "SegmentationFreeRecognizer.h"

namespace pr {
    //可能出现的字符集合;
    const std::vector<std::string> CH_PLATE_CODE{"京", "沪", "津", "渝", "冀", "晋", "蒙", "辽", "吉", "黑",
                                                 "苏", "浙", "皖", "闽", "赣", "鲁", "豫", "鄂", "湘", "粤",
                                                 "桂",
                                                 "琼", "川", "贵", "云", "藏", "陕", "甘", "青", "宁", "新",
                                                 "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",
                                                 "A",
                                                 "B", "C", "D", "E", "F", "G", "H", "J", "K", "L",
                                                 "M", "N", "P", "Q", "R", "S", "T", "U", "V", "W",
                                                 "X",
                                                 "Y", "Z", "港", "学", "使", "警", "澳", "挂", "军", "北",
                                                 "南", "广", "沈", "兰", "成", "济", "海", "民", "航", "空"};


    //定义不同的分割方法的编号;
    const int SEGMENTATION_FREE_METHOD = 0;
    const int SEGMENTATION_BASED_METHOD = 1;
//声明主类;
    class PipelinePR {
    public:
        //创建车牌位置检测模型实例指针;
        GeneralRecognizer *generalRecognizer;
        //创建抠出车牌图像模型实例指针;
        PlateDetection *plateDetection;
        //创建车牌分割模型实例指针;
        PlateSegmentation *plateSegmentation;
        //创建识别模型实例指针;
        FineMapping *fineMapping;
        //创建识别单个车牌图像的模型实例指针;
        SegmentationFreeRecognizer *segmentationFreeRecognizer;
        //构造函数;
        PipelinePR(std::string detector_filename,
                   std::string finemapping_prototxt, std::string finemapping_caffemodel,
                   std::string segmentation_prototxt, std::string segmentation_caffemodel,
                   std::string charRecognization_proto, std::string charRecognization_caffemodel,
                   std::string segmentationfree_proto, std::string segmentationfree_caffemodel);
        //析构函数;
        ~PipelinePR();

        //保存识别结果;
        std::vector<std::string> plateRes;
        //执行单个车牌的识别;
        std::vector<PlateInfo> RunPiplineAsImage(cv::Mat plateImage, int method);


    };


}
#endif //SWIFTPR_PIPLINE_H
