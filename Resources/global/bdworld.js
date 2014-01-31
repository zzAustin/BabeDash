var bdGlobals = {};

//character state
bdGlobals.CHAR_STATE_IDLE = 1;
bdGlobals.CHAR_STATE_WALKING = 2;

bdGlobals.OBJECT_BASEOBJ = -1;
bdGlobals.OBJECT_CHARACTER = 0;
bdGlobals.OBJECT_ITEM = 1;
	
//object group
bdGlobals.OBJ_GROUP_BABE = 1;
bdGlobals.OBJ_GROUP_ENEMY = 2;
bdGlobals.OBJ_GROUP_ITEM = 3;

//action type
bdGlobals.BDACTION_BASE = 0;
bdGlobals.BDACTION_STAY_IDLE = 1;
bdGlobals.BDACTION_WALK = 2;
bdGlobals.BDACTION_JUMP = 3;

//skill type
bdGlobals.BDSKILL_BASE = 0;
bdGlobals.BDSKILL_JUMP = 1;

//particle position type
bdGlobals.particleFree = 0;
bdGlobals.particleRelative = 1;
bdGlobals.particleGrouped = 2;



function handleCollision(objA,objB)
{
	log("---handleCollision---");
	objB.playSkill("Jump");
}

function playSkill(skillName)
{
    log("--playSkill--");
	if(this.skills[skillName].particle != null && this.skills[skillName].particle != undefined)
	{
	    log("this skill has particle...");
		if(this.playParticle(this.skills[skillName].particle.name,this.skills[skillName].particle.positionType))
		{
			log("particle played...");
		}
		else {
			log("particle not played...");
			return false;
		}
	}
	else
	{
		log("this skill has no particle");
	}
	
	if(this.skills[skillName].action != null && this.skills[skillName].action != undefined)
	{
	    log("this skill has action...");
		
		if(this.skills[skillName].action == bdGlobals.BDACTION_JUMP)
		{
			log("the action is jump, set it...");
			if(this.setJumpAction(-0.1))
			{
				log("set action successfully...");
			}
			else
			{
				log("set action failed...")
			}
		}
	}
	else
	{
		log("this skill has no action");
	}
	
	return true;
}