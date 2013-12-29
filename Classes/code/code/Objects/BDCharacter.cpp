#include "Objects/BDCharacter.h"
USING_NS_CC;

extension::CCArmature* BDCharacter::CreateArmature(char* srcFile,char* armatureName)
{
	extension::CCArmature* pArmature = NULL;
	extension::CCArmatureDataManager::sharedArmatureDataManager()->addArmatureFileInfo(srcFile);//add the armatrues in the data to cache
	pArmature = extension::CCArmature::create(armatureName);//create armatrue from cache

	return pArmature;
}

void BDCharacter::PlayAnimation(char* anim)
{
	extension::CCArmature* pArmature = NULL;
	pArmature->getAnimation()->play("Walk");
}
