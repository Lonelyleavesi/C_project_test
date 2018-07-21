#include"UFO.h"
#include"Constants.h"
bool Ufo::initWithSpriteFrameType(const UfoType& type){
	this->m_type = type;
	std::string frameName = "ufo1.png";
	switch (m_type)
	{
	case UfoType::FIREUP: frameName = StringUtils::format("ufo1.png");
		break;
	case UfoType::FIREMAX: frameName = StringUtils::format("ufo1.png");
		break;
	case UfoType::BOMB:	frameName = StringUtils::format("ufo2.png");
		break;
	default:
		break;
	}

	if (!Sprite::initWithSpriteFrameName(frameName))
	{
		return FALSE;
	}
	
	
	return TRUE;
}

Ufo* Ufo::create(const UfoType& type) {
	auto ufo = new Ufo();
	if (ufo && ufo->initWithSpriteFrameType(type))
	{
		ufo->autorelease(); //放入自动计数池  
		return ufo;
	}

	delete ufo;
	ufo = nullptr;
	return nullptr;
}

UfoType Ufo::getType() {
	return this->m_type;
}