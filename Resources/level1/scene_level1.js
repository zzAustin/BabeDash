sceneUnits = null;
particleDefs = null;

require("jsb.js");
require("level1/objs_level1.js");//fill in --sceneObjs--  --particleDefs--


function runThisScene()
{
  if(particleDefs != null)
  {
    log("particleDefs is not null, invoke constructParticleDefs");
	constructParticleDefs(particleDefs);
  }
  
  //create BDGameScene...
  var curScene = BD.BDGameScene.create();
 
  if(curScene)
  {
	log("---BabeDash--- GameScene created.");
  }
  else
  {
	log("---BabeDash--- GameScene creating failed.");
  }
  
   bdGlobals.curScene = curScene;

//-------------------------------construct the scene with scene units--------------------------------------------------------------  
  log("---constructing scene with scene units");
  for(var unitIdx in sceneUnits)
  {	
	var newUnit = null;
	var newObj = null;
	
	log("---adding a unit of scene");
	var newUnit = curScene.addSceneUnit(sceneUnits[unitIdx].unit_type);
	
	//SceneryLayer
	if(sceneUnits[unitIdx].unit_type == "SceneryLayer")
	{
		/*log("---the unit is a SceneryLayer");
		var attr = sceneUnits[unitIdx].attributes;
		newUnit.initWithAttributes(attr);
		bdGlobals.curScene.addChild(newUnit); */
	}
	
	//GameLayer
	if(sceneUnits[unitIdx].unit_type == "GameLayer")
	{
		log("---the unit is a GameLayer");
		bdGlobals.curGameLayer = newUnit;
		//----------------construct the game layer with objects--------------------------------
		for(var objIdx in sceneUnits[unitIdx].gameObjs)
		{
			log("---adding a game object to game layer...");
			newObj = newUnit.addGameObject(sceneUnits[unitIdx].gameObjs[objIdx]);
			
			if(sceneUnits[unitIdx].gameObjs[objIdx].skills != null)
			{
				newObj.skills = sceneUnits[unitIdx].gameObjs[objIdx].skills;
				newObj.playSkill = playSkill;
			}
			
			log("---about to add the GameLayer to scene...");
			//var sprt = cc.Sprite.create("res/HelloWorld.png");
			newUnit.addChild(newObj);
			log("---add to scene finished...");
		}
		//-------------------------------------------------------------------------
		bdGlobals.curScene.addChild(bdGlobals.curGameLayer);
	}
	
  }
//--------------------------------------------------------------------------------------------------------------- 

  var director = cc.Director.getInstance();
  
  if(director)
  {
	log("---BabeDash--- director retrieved.");
  }
  else
  {
	log("---BabeDash--- director retrieving failed.");
  }
  
  var runningScene = director.getRunningScene();
  
  var layer = cc.Layer.create();
  
  if(layer)
  {
	log("---BabeDash--- layer created.");
  }
  else
  {
	log("---BabeDash--- layer creating failed.");
  }
  
  var sprt = cc.Sprite.create("res/HelloWorld.png");
  
  if(sprt)
  {
	log("---BabeDash--- Sprite created.");
  }
  else
  {
	log("---BabeDash--- Sprite creating failed.");
  }
  
  //layer.addChild(sprt);
 // bdGlobals.curGameLayer.addChild(sprt);
  //curScene.addChild(layer);
  
  if (runningScene === null) director.runWithScene(bdGlobals.curScene);
  else director.replaceScene(bdGlobals.curScene);
}

runThisScene();


