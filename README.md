#QtAndroidPlayer
1；需要安装windows平台下的 Android qt版本，并下载Google SDK 和NDK(android-ndk-r10d),
	还有 apache-ant(我用的1.8.4版本) ,具体环境部署可参照 http://blog.csdn.net/foruok/article/details/17305697等
2：用QtCreator 打开MediaPlayer.pro

3；工程类型选择 选择" Android for armeabi-v7a..." 然后构建 工程 编译结束后目录下 会有MediaPlayer.apk 安装即可运行

4：工程的ffmepg库集成编译进了lirtmp 使用的是armv7a指令集编译的， 如果使用比armv7a老的指令集处理器会报错 如:armv5te 。

5：具体代码实现 是按照音频进行同步播放的 ， 就是把视频同步到音频  如果视频播放过慢 那么直接丢弃视频帧，如果视频播放过快就稍微暂停小会。

6：具体如何同步的 可参照具体代码实现。

7：视频绘制用了性能不太好的 在QWidget上用QPainter绘制，可提取代码 绘制用opengl 绘制。 音频用的是QAudioOutput ，也可用其他高性能的播放库。

659998323@qq.com
