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
���ֱַ���
1.��Դ�ֱ��ʣ��ز��������ķֱ���
2.��Ʒֱ��ʣ����������ٱ�д����ʱ�����Ծ������Դ���豸��רע������
3.�豸�ֱ��ʣ������豸��ʵ�ʷֱ���
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
	//�ҵ�����
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	//ͨ�������ҵ���ʦ

	if (!glview) {   //���û�л���
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
		glview = GLViewImpl::createWithRect("FlyPlane", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
		glview = GLViewImpl::create("FlyPlane");
#endif
		director->setOpenGLView(glview);  //ȡ�û������Ұѻ���������
	}

	// turn on display FPS
	director->setDisplayStats(FALSE);   //���е�ʱ�� �Ƿ���ʾFPS ֡��

										// set FPS. the default value is 1.0/60 if you don't call this
	director->setAnimationInterval(1.0f / 60);

	FileUtils::getInstance()->addSearchPath("ui");
	FileUtils::getInstance()->addSearchPath("font");
	FileUtils::getInstance()->addSearchPath("sound");
	//���ļ�ϵͳ���˲���Ĭ�ϵ�resourcesĿ¼���� ��Ҫ�������uiĿ¼

	// Set the design resolution
	glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);
	/*
	���Э�飺
	EXACT_FIT��         �����߿�ȣ�������ȫ��
	NO_BORDER��         �����ڱ�  �Ŵ���ͼ ��������
	SHOW_ALL��          ��ʾȫ�� ���ܻ��кڱ�
	FIXED_HEIGHT��      ֻ�ܸ߶Ȳ��ܿ��
	FIXED_WIDTH��       ֻ�ܿ�� ���ܸ߶�
	UNKNOWN��           ����
	*/
	auto frameSize = glview->getFrameSize();

	//���ô��ڴ�С
	glview->setFrameSize(480, 640);

	// if the frame's height is larger than the height of medium size.
	/*
	���������ô���
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
	//��Ŀ����һ������������ɣ�һ������������һ��������� ��UI���
	auto scene = LoadingScene::createScene();

	// run  ���ݲ��ų���
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
