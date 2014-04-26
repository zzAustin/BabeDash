#ifndef _BD_PARTICLE_
#define _BD_PARTICLE_

#include "cocos2d.h"

class BDParticleSystemDef:public cocos2d::CCObject
{
public:
	BDParticleSystemDef();
	~BDParticleSystemDef();

	bool initWithFile(const char *plist);
	bool initWithDictionary(cocos2d::CCDictionary *dictionary);
	bool initWithDictionary(cocos2d::CCDictionary *dictionary, const char *dirname);

	int fMaxParticles;
	float fAngle;
	float fAngleVar;
	float fDuration;
	int iBlendFuncSource;
	int iBlendFuncDestination;
	float fStartColorRed;
	float fStartColorGreen;
	float fStartColorBlue;
	float fStartColorAlpha;
	float fStartColorVarianceRed;
	float fStartColorVarianceGreen;
	float fStartColorVarianceBlue;
	float fStartColorVarianceAlpha;
	float fFinishColorRed;
	float fFinishColorGreen;
	float fFinishColorBlue;
	float fFinishColorAlpha;
	float fFinishColorVarianceRed;
	float fFinishColorVarianceGreen;
	float fFinishColorVarianceBlue;
	float fFinishColorVarianceAlpha;
	float fStartParticleSize;
	float fStartParticleSizeVariance;
	float fFinishParticleSize;
	float fFinishParticleSizeVariance;
	float fSourcePositionx;
	float fSourcePositiony;
	float fSourcePositionVariancex;
	float fSourcePositionVariancey;
	float fRotationStart;
	float fRotationStartVariance;
    float fRotationEnd;
	float fRotationEndVariance;
	int   iEmitterType;//kCCParticleModeGravity,kCCParticleModeRadius
	float fParticlelifespan;
	float fParticleLifespanVariance;

	//kCCParticleModeGravity
	float fGravityx;
	float fGravityy;
    float fSpeed;
	float fSpeedVariance;
	float fRadialAcceleration;
	float fRadialAccelVariance;
    float fTangentialAcceleration;
	float fTangentialAccelVariance;
	bool  bRotationIsDir; 

	//kCCParticleModeRadius
	float fMaxRadius;
	float fMaxRadiusVariance;
	float fMinRadius;
    float fRotatePerSecond;
	float fRotatePerSecondVariance;

	std::string sTextureName;
	char *pTextureData;	

	std::string sPlistFile;
};

	
class BDParticleSystem: public cocos2d::CCParticleSystemQuad
{
public:
    BDParticleSystem();
	BDParticleSystem(bool ext);
    virtual ~BDParticleSystem();
    bool init(unsigned int numberOfParticles);
	bool init();
    virtual bool initWithTotalParticles(unsigned int numberOfParticles);
	virtual bool InitWithDef(BDParticleSystemDef* def);
    void update(float dt);
    static BDParticleSystem* create();
	BDParticleSystem* create(bool ext);
	static BDParticleSystem* create(const char *plistFile,bool ext);
	static BDParticleSystem* createWithTotalParticles(unsigned int numberOfParticles,bool ext);
    static BDParticleSystem* createWithTotalParticles(unsigned int numberOfParticles);
	virtual cocos2d::CCPoint convertToWorldSpaceExt(const cocos2d::CCPoint& nodePoint);
	void initParticleExt(cocos2d::tCCParticle* particle);
	bool addParticleExt();
	bool IsFinished();
	void SetIsFinished(bool bFinished);
	

protected:
	bool m_bExt;
	bool m_bFinished;
};
#endif
