### Demo运行说明
原地址 https://github.com/AleynP/LPR.git
打开项目肯定会报编译错误，要做以下修改：
1. 用AS打开项目
2. 设置项目NDK为 NDK-r14b
3. 先修改 CMakeLists.txt 文件， 把第19行修改成你本地的 OpenCV SDK 的对应路径。OpenCV我用的是4.1.2
4. 跟据自己的开发平台，设置app下的build.gradle  第 23 行代码 是否要注释。我的是mac ，用这个参数arguments "-DANDROID_STL=c++_shared"
完成以上步骤后再运行项目，就没有问题了。
