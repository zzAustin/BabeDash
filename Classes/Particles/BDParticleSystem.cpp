#include "Particles/BDParticleSystem.h"
#include "Particles/firePngData.h"
#include "BDBaseClasses/BDObject.h"
#include "Util/base64.h"
#include "support/zip_support/ZipUtils.h"
#include "GameManagers/BDResourceManager.h"
USING_NS_CC;

static CCTexture2D* getDefaultTexture()
{
	CCTexture2D* pTexture = NULL;
	CCImage* pImage = NULL;
	do 
	{
		bool bRet = false;
		const char* key = "__firePngData";
		pTexture = CCTextureCache::sharedTextureCache()->textureForKey(key);
		CC_BREAK_IF(pTexture != NULL);

		pImage = new CCImage();
		CC_BREAK_IF(NULL == pImage);
		bRet = pImage->initWithImageData((void*)__firePngData, sizeof(__firePngData), CCImage::kFmtPng);
		CC_BREAK_IF(!bRet);

		pTexture = CCTextureCache::sharedTextureCache()->addUIImage(pImage, key);
	} while (0);

	CC_SAFE_RELEASE(pImage);

	return pTexture;
}

BDParticleSystemDef::BDParticleSystemDef()
{
	pTextureData = NULL;
}

BDParticleSystemDef::~BDParticleSystemDef()
{
	CC_SAFE_DELETE(pTextureData);
}

bool BDParticleSystemDef::initWithFile(const char *plist)
{
	bool bRet = false;
	sPlistFile = CCFileUtils::sharedFileUtils()->fullPathForFilename(plist);
	CCDictionary *dict = CCDictionary::createWithContentsOfFileThreadSafe(sPlistFile.c_str());

	CCAssert( dict != NULL, "BDParticleSystemDef: file not found");

	// XXX compute path from a path, should define a function somewhere to do it
	std::string listFilePath = plist;
	if (listFilePath.find('/') != std::string::npos)
	{
		listFilePath = listFilePath.substr(0, listFilePath.rfind('/') + 1);
		bRet = this->initWithDictionary(dict, listFilePath.c_str());
	}
	else
	{
		bRet = this->initWithDictionary(dict, "");
	}

	dict->release();

	return bRet;
}

bool BDParticleSystemDef::initWithDictionary(CCDictionary *dictionary)
{
	return initWithDictionary(dictionary, "");
}

bool BDParticleSystemDef::initWithDictionary(CCDictionary *dictionary, const char *dirname)
{
	bool bRet = false;
	unsigned char *buffer = NULL;
	unsigned char *deflated = NULL;
	CCImage *image = NULL;
	
	fMaxParticles = dictionary->valueForKey("maxParticles")->intValue();

	// angle
	fAngle = dictionary->valueForKey("angle")->floatValue();
	fAngle = dictionary->valueForKey("angleVariance")->floatValue();

	// duration
	fDuration = dictionary->valueForKey("duration")->floatValue();

	// blend function 
	iBlendFuncSource = dictionary->valueForKey("blendFuncSource")->intValue();
	iBlendFuncDestination = dictionary->valueForKey("blendFuncDestination")->intValue();

	// color
	fStartColorRed = dictionary->valueForKey("startColorRed")->floatValue();
	fStartColorGreen = dictionary->valueForKey("startColorGreen")->floatValue();
	fStartColorBlue = dictionary->valueForKey("startColorBlue")->floatValue();
	fStartColorAlpha = dictionary->valueForKey("startColorAlpha")->floatValue();

	fStartColorVarianceRed = dictionary->valueForKey("startColorVarianceRed")->floatValue();
	fStartColorVarianceGreen = dictionary->valueForKey("startColorVarianceGreen")->floatValue();
	fStartColorVarianceBlue = dictionary->valueForKey("startColorVarianceBlue")->floatValue();
	fStartColorVarianceAlpha = dictionary->valueForKey("startColorVarianceAlpha")->floatValue();

	fFinishColorRed = dictionary->valueForKey("finishColorRed")->floatValue();
	fFinishColorGreen = dictionary->valueForKey("finishColorGreen")->floatValue();
	fFinishColorBlue = dictionary->valueForKey("finishColorBlue")->floatValue();
	fFinishColorAlpha = dictionary->valueForKey("finishColorAlpha")->floatValue();

	fFinishColorVarianceRed = dictionary->valueForKey("finishColorVarianceRed")->floatValue();
	fFinishColorVarianceGreen = dictionary->valueForKey("finishColorVarianceGreen")->floatValue();
	fFinishColorVarianceBlue = dictionary->valueForKey("finishColorVarianceBlue")->floatValue();
	fFinishColorVarianceAlpha = dictionary->valueForKey("finishColorVarianceAlpha")->floatValue();

	// particle size
	fStartParticleSize = dictionary->valueForKey("startParticleSize")->floatValue();
	fStartParticleSizeVariance = dictionary->valueForKey("startParticleSizeVariance")->floatValue();
	fFinishParticleSize = dictionary->valueForKey("finishParticleSize")->floatValue();
	fFinishParticleSizeVariance = dictionary->valueForKey("finishParticleSizeVariance")->floatValue();

	// position
	fSourcePositionx = dictionary->valueForKey("sourcePositionx")->floatValue();
	fSourcePositiony = dictionary->valueForKey("sourcePositiony")->floatValue(); 
	fSourcePositionVariancex = dictionary->valueForKey("sourcePositionVariancex")->floatValue();
	fSourcePositionVariancey = dictionary->valueForKey("sourcePositionVariancey")->floatValue();

	// Spinning
	fRotationStart = dictionary->valueForKey("rotationStart")->floatValue();
	fRotationStartVariance = dictionary->valueForKey("rotationStartVariance")->floatValue();
	fRotationEnd = dictionary->valueForKey("rotationEnd")->floatValue();
	fRotationEndVariance = dictionary->valueForKey("rotationEndVariance")->floatValue();

	iEmitterType = dictionary->valueForKey("emitterType")->intValue();

	// Mode A: Gravity + tangential accel + radial accel
	if( iEmitterType == kCCParticleModeGravity ) 
	{
		// gravity
		fGravityx = dictionary->valueForKey("gravityx")->floatValue();
		fGravityy = dictionary->valueForKey("gravityy")->floatValue();

		// speed
		fSpeed = dictionary->valueForKey("speed")->floatValue();
		fSpeedVariance = dictionary->valueForKey("speedVariance")->floatValue();

		// radial acceleration
		fRadialAcceleration = dictionary->valueForKey("radialAcceleration")->floatValue();
		fRadialAccelVariance = dictionary->valueForKey("radialAccelVariance")->floatValue();

		// tangential acceleration
		fTangentialAcceleration = dictionary->valueForKey("tangentialAcceleration")->floatValue();
		fTangentialAccelVariance = dictionary->valueForKey("tangentialAccelVariance")->floatValue();

		// rotation is dir
		bRotationIsDir = dictionary->valueForKey("rotationIsDir")->boolValue();
	}
	// or Mode B: radius movement
	else if( iEmitterType == kCCParticleModeRadius ) 
	{
		fMaxRadius = dictionary->valueForKey("maxRadius")->floatValue();
		fMaxRadiusVariance = dictionary->valueForKey("maxRadiusVariance")->floatValue();
		fMinRadius = dictionary->valueForKey("minRadius")->floatValue();
		fRotatePerSecond = dictionary->valueForKey("rotatePerSecond")->floatValue();
		fRotatePerSecondVariance = dictionary->valueForKey("rotatePerSecondVariance")->floatValue();

	} else {
		CCAssert( false, "Invalid emitterType in config file");
	}

	// life span
	fParticlelifespan = dictionary->valueForKey("particleLifespan")->floatValue();
	fParticleLifespanVariance = dictionary->valueForKey("particleLifespanVariance")->floatValue();


	// texture        
	// Try to get the texture from the cache
	sTextureName = dictionary->valueForKey("textureFileName")->getCString();

	size_t rPos = sTextureName.rfind('/');

	if (rPos != std::string::npos)
	{
		std::string textureDir = sTextureName.substr(0, rPos + 1);

		if (dirname != NULL && textureDir != dirname)
		{
			sTextureName = sTextureName.substr(rPos+1);
			sTextureName = std::string(dirname) + sTextureName;
		}
	}
	else
	{
		if (dirname != NULL)
		{
			sTextureName = std::string(dirname) + sTextureName;
		}
	}

	CCTexture2D *tex = NULL;

	if (sTextureName.length() > 0)
	{
		// set not pop-up message box when load image failed
		bool bNotify = CCFileUtils::sharedFileUtils()->isPopupNotify();
		CCFileUtils::sharedFileUtils()->setPopupNotify(false);
		tex = CCTextureCache::sharedTextureCache()->addImage(sTextureName.c_str());
		// reset the value of UIImage notify
		CCFileUtils::sharedFileUtils()->setPopupNotify(bNotify);
	}

	if(tex == NULL)
	{                        
		const char *textureData = dictionary->valueForKey("textureImageData")->getCString();
		CCAssert(textureData, "");

		int dataLen = strlen(textureData);
		if(dataLen != 0)
		{
			// if it fails, try to get it from the base64-gzipped data    
			int decodeLen = base64Decode((unsigned char*)textureData, (unsigned int)dataLen, &buffer);
			CCAssert( buffer != NULL, "CCParticleSystem: error decoding textureImageData");

			int deflatedLen = ZipUtils::ccInflateMemory(buffer, decodeLen, &deflated);
			CCAssert( deflated != NULL, "CCParticleSystem: error ungzipping textureImageData");

			// For android, we should retain it in VolatileTexture::addCCImage which invoked in CCTextureCache::sharedTextureCache()->addUIImage()
			image = new CCImage();
			bool isOK = image->initWithImageData(deflated, deflatedLen);
			CCAssert(isOK, "CCParticleSystem: error init image with Data");

			CCTextureCache::sharedTextureCache()->addUIImage(image, sTextureName.c_str());

			image->release();
		}
	}

	bRet = true;

	CC_SAFE_DELETE_ARRAY(buffer);
	CC_SAFE_DELETE_ARRAY(deflated);
	return bRet;
}


BDParticleSystem::BDParticleSystem()
{
	m_bExt = false;
	m_bFinished = false;
}

BDParticleSystem::BDParticleSystem(bool ext)
{
	m_bExt = ext;
	m_bFinished = false;
}


BDParticleSystem::~BDParticleSystem()
{
	
}

bool BDParticleSystem::init(unsigned int numberOfParticles)
{
	return initWithTotalParticles(numberOfParticles);
}

bool BDParticleSystem::init()
{
	return initWithTotalParticles(600);
}


BDParticleSystem* BDParticleSystem::create(bool ext)
{
	BDParticleSystem* pRet = new BDParticleSystem(ext);
	if (pRet && pRet->init())
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

BDParticleSystem* BDParticleSystem::createWithTotalParticles(unsigned int numberOfParticles,bool ext)
{
	BDParticleSystem* pRet = new BDParticleSystem(ext);
	if (pRet && pRet->initWithTotalParticles(numberOfParticles))
	{
		pRet->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(pRet);
	}
	return pRet;
}

BDParticleSystem* BDParticleSystem::create(const char *plistFile,bool ext)
{
	BDParticleSystem *pRet = new BDParticleSystem(ext);
	BDParticleSystemDef* def = NULL;
	CCDictionary* dict = BDResourceManager::GetInstance()->GetParticleDefs();
	def = (BDParticleSystemDef*)dict->objectForKey(plistFile);

	if (pRet && pRet->InitWithDef(def))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return pRet;
}

bool BDParticleSystem::InitWithDef(BDParticleSystemDef* def)
{
	bool bRet = false;

	// self, not super
	if(this->initWithTotalParticles(def->fMaxParticles))
	{
		// angle
		m_fAngle = def->fAngle;
		m_fAngleVar = def->fAngleVar;

		// duration
		m_fDuration = def->fDuration;

		// blend function 
		m_tBlendFunc.src = def->iBlendFuncSource;
		m_tBlendFunc.dst = def->iBlendFuncDestination;

		// color
		m_tStartColor.r = def->fStartColorRed;
		m_tStartColor.g = def->fStartColorGreen;
		m_tStartColor.b = def->fStartColorBlue;
		m_tStartColor.a = def->fStartColorAlpha;

		m_tStartColorVar.r = def->fStartColorVarianceRed;
		m_tStartColorVar.g = def->fStartColorVarianceGreen;
		m_tStartColorVar.b = def->fStartColorVarianceBlue;
		m_tStartColorVar.a = def->fStartColorVarianceAlpha;

		m_tEndColor.r = def->fFinishColorRed;
		m_tEndColor.g = def->fFinishColorGreen;
		m_tEndColor.b = def->fFinishColorBlue;
		m_tEndColor.a = def->fFinishColorAlpha;

		m_tEndColorVar.r = def->fFinishColorVarianceRed;
		m_tEndColorVar.g = def->fFinishColorVarianceGreen;
		m_tEndColorVar.b = def->fFinishColorVarianceBlue;
		m_tEndColorVar.a = def->fFinishColorVarianceAlpha;

		// particle size
		m_fStartSize = def->fStartParticleSize;
		m_fStartSizeVar = def->fStartParticleSizeVariance;
		m_fEndSize = def->fFinishParticleSize;
		m_fEndSizeVar = def->fFinishParticleSizeVariance;

		// position
		float x = def->fSourcePositionx;
		float y = def->fSourcePositiony;
		this->setPosition( ccp(x,y) );            
		m_tPosVar.x = def->fSourcePositionVariancex;
		m_tPosVar.y = def->fSourcePositionVariancey;

		// Spinning
		m_fStartSpin = def->fRotationStart;
		m_fStartSpinVar = def->fRotationStartVariance;
		m_fEndSpin= def->fRotationEnd;
		m_fEndSpinVar= def->fRotationEndVariance;

		m_nEmitterMode = def->iEmitterType;

		// Mode A: Gravity + tangential accel + radial accel
		if( m_nEmitterMode == kCCParticleModeGravity ) 
		{
			// gravity
			modeA.gravity.x = def->fGravityx;
			modeA.gravity.y = def->fGravityy;

			// speed
			modeA.speed = def->fSpeed;
			modeA.speedVar = def->fSpeedVariance;

			// radial acceleration
			modeA.radialAccel = def->fRadialAcceleration;
			modeA.radialAccelVar = def->fRadialAccelVariance;

			// tangential acceleration
			modeA.tangentialAccel = def->fTangentialAcceleration;
			modeA.tangentialAccelVar = def->fTangentialAccelVariance;

			// rotation is dir
			modeA.rotationIsDir = def->bRotationIsDir;
		}

		// or Mode B: radius movement
		else if( m_nEmitterMode == kCCParticleModeRadius ) 
		{
			modeB.startRadius = def->fMaxRadius;
			modeB.startRadiusVar = def->fMaxRadiusVariance;
			modeB.endRadius = def->fMinRadius;
			modeB.endRadiusVar = 0.0f;
			modeB.rotatePerSecond = def->fRotatePerSecond;
			modeB.rotatePerSecondVar = def->fRotatePerSecondVariance;

		} else {
			CCAssert( false, "Invalid emitterType in config file");
		}

		// life span
		m_fLife = def->fParticlelifespan;
		m_fLifeVar = def->fParticleLifespanVariance;

		// emission Rate
		m_fEmissionRate = m_uTotalParticles / m_fLife;

		//don't get the internal texture if a batchNode is used
		if (!m_pBatchNode)
		{
			// Set a compatible default for the alpha transfer
			m_bOpacityModifyRGB = false;

			CCTexture2D *tex = NULL;

			tex = CCTextureCache::sharedTextureCache()->textureForKey(def->sTextureName.c_str());

			if (tex)
			{
				setTexture(tex);
			}
			CCAssert( this->m_pTexture != NULL, "BDParticleSystem: error loading the texture");
		}
		bRet = true;
	}

	return bRet;
}

bool BDParticleSystem::initWithTotalParticles(unsigned int numberOfParticles)
{
	if( CCParticleSystemQuad::initWithTotalParticles(numberOfParticles) )
	{
		// additive
		this->setBlendAdditive(true);

		// duration
		m_fDuration = kCCParticleDurationInfinity;

		// Gravity Mode
		setEmitterMode(kCCParticleModeGravity);

		// Gravity Mode: gravity
		setGravity(ccp(0.0,0.0));

		// Gravity mode: radial acceleration
		setRadialAccel(860.0);
		setRadialAccelVar(100.0);

		setTangentialAccel(0.0);
		setTangentialAccel(0.0);

		// Gravity mode: speed of particles
		setSpeed(20);
		setSpeedVar(0);


		// angle
		m_fAngle = 90;
		m_fAngleVar = 180;

		// emitter position
		CCSize winSize = CCDirector::sharedDirector()->getWinSize();
		this->setPosition(ccp(winSize.width/2, winSize.height/2));
		setPosVar(CCPointZero);

		// life of particles
		m_fLife = 0.4f;
		m_fLifeVar = 0.2f;

		// size, in pixels
		m_fStartSize = 60.0f;
		m_fStartSizeVar = 10.0f;
		m_fEndSize = kCCParticleStartSizeEqualToEndSize;

		// emits per seconds
		m_fEmissionRate = m_uTotalParticles/m_fLife;

		// color of particles
		m_tStartColor.r = 0.76f;
		m_tStartColor.g = 0.25f;
		m_tStartColor.b = 0.12f;
		m_tStartColor.a = 1.0f;
		m_tStartColorVar.r = 0.0f;
		m_tStartColorVar.g = 0.0f;
		m_tStartColorVar.b = 0.0f;
		m_tStartColorVar.a = 0.0f;
		m_tEndColor.r = 0.0f;
		m_tEndColor.g = 0.0f;
		m_tEndColor.b = 0.0f;
		m_tEndColor.a = 1.0f;
		m_tEndColorVar.r = 0.0f;
		m_tEndColorVar.g = 0.0f;
		m_tEndColorVar.b = 0.0f;
		m_tEndColorVar.a = 0.0f;

		CCTexture2D* pTexture = getDefaultTexture();
		if (pTexture != NULL)
		{
			setTexture(pTexture);
		}

		return true;
	}
	return false;
}

void BDParticleSystem::initParticleExt(tCCParticle* particle)
{
	// timeToLive
	// no negative life. prevent division by 0
	particle->timeToLive = m_fLife + m_fLifeVar * CCRANDOM_MINUS1_1();
	particle->timeToLive = MAX(0, particle->timeToLive);

	// position
	particle->pos.x = m_tSourcePosition.x + m_tPosVar.x * CCRANDOM_MINUS1_1();

	particle->pos.y = m_tSourcePosition.y + m_tPosVar.y * CCRANDOM_MINUS1_1();


	// Color
	ccColor4F start;
	start.r = clampf(m_tStartColor.r + m_tStartColorVar.r * CCRANDOM_MINUS1_1(), 0, 1);
	start.g = clampf(m_tStartColor.g + m_tStartColorVar.g * CCRANDOM_MINUS1_1(), 0, 1);
	start.b = clampf(m_tStartColor.b + m_tStartColorVar.b * CCRANDOM_MINUS1_1(), 0, 1);
	start.a = clampf(m_tStartColor.a + m_tStartColorVar.a * CCRANDOM_MINUS1_1(), 0, 1);

	ccColor4F end;
	end.r = clampf(m_tEndColor.r + m_tEndColorVar.r * CCRANDOM_MINUS1_1(), 0, 1);
	end.g = clampf(m_tEndColor.g + m_tEndColorVar.g * CCRANDOM_MINUS1_1(), 0, 1);
	end.b = clampf(m_tEndColor.b + m_tEndColorVar.b * CCRANDOM_MINUS1_1(), 0, 1);
	end.a = clampf(m_tEndColor.a + m_tEndColorVar.a * CCRANDOM_MINUS1_1(), 0, 1);

	particle->color = start;
	particle->deltaColor.r = (end.r - start.r) / particle->timeToLive;
	particle->deltaColor.g = (end.g - start.g) / particle->timeToLive;
	particle->deltaColor.b = (end.b - start.b) / particle->timeToLive;
	particle->deltaColor.a = (end.a - start.a) / particle->timeToLive;

	// size
	float startS = m_fStartSize + m_fStartSizeVar * CCRANDOM_MINUS1_1();
	startS = MAX(0, startS); // No negative value

	particle->size = startS;

	if( m_fEndSize == kCCParticleStartSizeEqualToEndSize )
	{
		particle->deltaSize = 0;
	}
	else
	{
		float endS = m_fEndSize + m_fEndSizeVar * CCRANDOM_MINUS1_1();
		endS = MAX(0, endS); // No negative values
		particle->deltaSize = (endS - startS) / particle->timeToLive;
	}

	// rotation
	float startA = m_fStartSpin + m_fStartSpinVar * CCRANDOM_MINUS1_1();
	float endA = m_fEndSpin + m_fEndSpinVar * CCRANDOM_MINUS1_1();
	particle->rotation = startA;
	particle->deltaRotation = (endA - startA) / particle->timeToLive;

	// position
	if( m_ePositionType == kCCPositionTypeFree ) 
	{
		particle->startPos = this->convertToWorldSpaceExt(CCPointZero);
	}
	else if ( m_ePositionType == kCCPositionTypeRelative ) 
	{
		particle->startPos = m_obPosition;
	}

	// direction
	float a = CC_DEGREES_TO_RADIANS( m_fAngle + m_fAngleVar * CCRANDOM_MINUS1_1() );    

	// Mode Gravity: A
	if (m_nEmitterMode == kCCParticleModeGravity) 
	{
		CCPoint v(cosf( a ), sinf( a ));
		float s = modeA.speed + modeA.speedVar * CCRANDOM_MINUS1_1();

		// direction
		particle->modeA.dir = ccpMult( v, s );

		// radial accel
		particle->modeA.radialAccel = modeA.radialAccel + modeA.radialAccelVar * CCRANDOM_MINUS1_1();


		// tangential accel
		particle->modeA.tangentialAccel = modeA.tangentialAccel + modeA.tangentialAccelVar * CCRANDOM_MINUS1_1();

		// rotation is dir
		if(modeA.rotationIsDir)
			particle->rotation = -CC_RADIANS_TO_DEGREES(ccpToAngle(particle->modeA.dir));
	}

	// Mode Radius: B
	else 
	{
		// Set the default diameter of the particle from the source position
		float startRadius = modeB.startRadius + modeB.startRadiusVar * CCRANDOM_MINUS1_1();
		float endRadius = modeB.endRadius + modeB.endRadiusVar * CCRANDOM_MINUS1_1();

		particle->modeB.radius = startRadius;

		if(modeB.endRadius == kCCParticleStartRadiusEqualToEndRadius)
		{
			particle->modeB.deltaRadius = 0;
		}
		else
		{
			particle->modeB.deltaRadius = (endRadius - startRadius) / particle->timeToLive;
		}

		particle->modeB.angle = a;
		particle->modeB.degreesPerSecond = CC_DEGREES_TO_RADIANS(modeB.rotatePerSecond + modeB.rotatePerSecondVar * CCRANDOM_MINUS1_1());
	}    
}

bool BDParticleSystem::addParticleExt()
{
	if (this->isFull())
	{
		return false;
	}

	tCCParticle * particle = &m_pParticles[ m_uParticleCount ];
	this->initParticleExt(particle);
	++m_uParticleCount;

	return true;
}

bool BDParticleSystem::IsFinished()
{
	return m_bFinished;
}

void BDParticleSystem::SetIsFinished(bool bFinished)
{
	m_bFinished = bFinished;
}


// ParticleSystem - MainLoop
void BDParticleSystem::update(float dt)
{
	CC_PROFILER_START_CATEGORY(kCCProfilerCategoryParticles , "CCParticleSystem - update");

	if (m_bIsActive && m_fEmissionRate)
	{
		float rate = 1.0f / m_fEmissionRate;
		//issue #1201, prevent bursts of particles, due to too high emitCounter
		if (m_uParticleCount < m_uTotalParticles)
		{
			m_fEmitCounter += dt;
		}

		while (m_uParticleCount < m_uTotalParticles && m_fEmitCounter > rate) 
		{
			this->addParticleExt();
			m_fEmitCounter -= rate;
		}

		m_fElapsed += dt;
		if (m_fDuration != -1 && m_fDuration < m_fElapsed)
		{
			this->stopSystem();
		}
	}

	m_uParticleIdx = 0;

	CCPoint currentPosition = CCPointZero;
	if (m_ePositionType == kCCPositionTypeFree)
	{
		currentPosition = this->convertToWorldSpaceExt(CCPointZero); 
	}
	else if (m_ePositionType == kCCPositionTypeRelative)
	{
		currentPosition = m_obPosition;
	}

	if (m_bVisible)
	{
		while (m_uParticleIdx < m_uParticleCount)
		{
			tCCParticle *p = &m_pParticles[m_uParticleIdx];

			// life
			p->timeToLive -= dt;

			if (p->timeToLive > 0) 
			{
				// Mode A: gravity, direction, tangential accel & radial accel
				if (m_nEmitterMode == kCCParticleModeGravity) 
				{
					CCPoint tmp, radial, tangential;

					radial = CCPointZero;
					// radial acceleration
					if (p->pos.x || p->pos.y)
					{
						radial = ccpNormalize(p->pos);
					}
					tangential = radial;
					radial = ccpMult(radial, p->modeA.radialAccel);

					// tangential acceleration
					float newy = tangential.x;
					tangential.x = -tangential.y;
					tangential.y = newy;
					tangential = ccpMult(tangential, p->modeA.tangentialAccel);

					// (gravity + radial + tangential) * dt
					tmp = ccpAdd( ccpAdd( radial, tangential), modeA.gravity);
					tmp = ccpMult( tmp, dt);
					p->modeA.dir = ccpAdd( p->modeA.dir, tmp);
					tmp = ccpMult(p->modeA.dir, dt);
					p->pos = ccpAdd( p->pos, tmp );
				}

				// Mode B: radius movement
				else 
				{                
					// Update the angle and radius of the particle.
					p->modeB.angle += p->modeB.degreesPerSecond * dt;
					p->modeB.radius += p->modeB.deltaRadius * dt;

					p->pos.x = - cosf(p->modeB.angle) * p->modeB.radius;
					p->pos.y = - sinf(p->modeB.angle) * p->modeB.radius;
				}

				// color
				p->color.r += (p->deltaColor.r * dt);
				p->color.g += (p->deltaColor.g * dt);
				p->color.b += (p->deltaColor.b * dt);
				p->color.a += (p->deltaColor.a * dt);

				// size
				p->size += (p->deltaSize * dt);
				p->size = MAX( 0, p->size );

				// angle
				p->rotation += (p->deltaRotation * dt);

				//
				// update values in quad
				//

				CCPoint    newPos;

				if (m_ePositionType == kCCPositionTypeFree || m_ePositionType == kCCPositionTypeRelative) 
				{
					CCPoint diff = ccpSub( currentPosition, p->startPos );
					newPos = ccpSub(p->pos, diff);
				} 
				else
				{
					newPos = p->pos;
				}

				// translate newPos to correct position, since matrix transform isn't performed in batchnode
				// don't update the particle with the new position information, it will interfere with the radius and tangential calculations
				if (m_pBatchNode)
				{
					newPos.x+=m_obPosition.x;
					newPos.y+=m_obPosition.y;
				}

				updateQuadWithParticle(p, newPos);
				//updateParticleImp(self, updateParticleSel, p, newPos);

				// update particle counter
				++m_uParticleIdx;
			} 
			else 
			{
				// life < 0
				int currentIndex = p->atlasIndex;
				if( m_uParticleIdx != m_uParticleCount-1 )
				{
					m_pParticles[m_uParticleIdx] = m_pParticles[m_uParticleCount-1];
				}
				if (m_pBatchNode)
				{
					//disable the switched particle
					m_pBatchNode->disableParticle(m_uAtlasIndex+currentIndex);

					//switch indexes
					m_pParticles[m_uParticleCount-1].atlasIndex = currentIndex;
				}


				--m_uParticleCount;

				if( m_uParticleCount == 0 && m_bIsAutoRemoveOnFinish )
				{
					this->unscheduleUpdate();
					m_pParent->removeChild(this, true);
					return;
				}

				//mark the BDParticleSystem finished
				if( m_uParticleCount == 0)
				{
					SetIsFinished(true);
				}
			}
		} //while
		m_bTransformSystemDirty = false;
	}
	if (! m_pBatchNode)
	{
		postStep();
	}

	CC_PROFILER_STOP_CATEGORY(kCCProfilerCategoryParticles , "CCParticleSystem - update");
}

CCPoint BDParticleSystem::convertToWorldSpaceExt(const CCPoint& nodePoint)
{
	CCNode* pParent = getParent();
	BDObject* pObj = NULL;
	CCPoint ret;
	pObj = dynamic_cast<BDObject*>(pParent);
	if(pObj != NULL && m_bExt)
		ret = pObj->GetLogicPosition();
	else
		ret = CCPointApplyAffineTransform(nodePoint, nodeToWorldTransform());

	return ret;
}
