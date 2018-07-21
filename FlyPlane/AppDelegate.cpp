#include "AppDelegate.h"
#include "GameScene.h"
#include "LoadingScene.h"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(480, 640);

/*
三种分辨率
1.资源分辨率：素材自身本来的分辨率
2.设计分辨率：便于我们再编写代码时，忽略具体的资源和设备而专注于其他
3.设备分辨率：具体设备的实际分辨率
*/

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
	AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::end();
#endif
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
	// set OpenGL context attributes: red,green,blue,alpha,depth,stencil
	GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8 };

	GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
	return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
	// initialize director
	//找到导演
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	//通过导演找到画师

	if (!glview) {   //如果没有画布
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("FlyPlane", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
		glview = GLViewImpl::create("FlyPlane");
#endif
		director->setOpenGLView(glview);  //取得画布并且把画布给导演
	}

	// turn on display FPS
	director->setDisplayStats(FALSE);   //运行的时候 是否显示FPS 帧率

										// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0f / 60);

	FileUtils::getInstance()->addSearchPath("ui");
	FileUtils::getInstance()->addSearchPath("font");
	FileUtils::getInstance()->addSearchPath("sound");
	//让文件系统除了查找默认的resources目录以外 还要找下面的ui目录

	// Set the design resolution
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
	/*
	解决协议：
	EXACT_FIT：         牺牲高宽比，拉伸至全屏
	NO_BORDER：         消除黑边  放大视图 牺牲内容
	SHOW_ALL：          显示全部 可能会有黑边
	FIXED_HEIGHT：      只管高度不管宽度
	FIXED_WIDTH：       只管宽度 不管高度
	UNKNOWN：           不管
	*/
	auto frameSize = glview->getFrameSize();

	//设置窗口大小
	glview->setFrameSize(480, 640);

	// if the frame's height is larger than the height of medium size.
	/*
	下面是无用代码
	if (frameSize.height > mediumResolutionSize.height)
	{
	director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
	}
	// if the frame's height is larger than the height of small size.
	else if (frameSize.height > smallResolutionSize.height)
	{
	director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
	}
	// if the frame's height is smaller than the height of medium size.
	else
	{
	director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
	}
	*/

	register_all_packages();

	// create a scene. it's an autorelease object
	//项目可以一到多个场景构成，一个场景可以有一到多个精灵 ，UI组成
	auto scene = LoadingScene::createScene();

	// run  导演播放场景
	director->runWithScene(scene);

	return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
	Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
	AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
	SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
	Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
	AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
	SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
	SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
