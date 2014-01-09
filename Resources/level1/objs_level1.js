sceneUnits = [
              //SceneryLayer
              {unit_type:"SceneryLayer",
			   attributes:{pos:[0.0,0.0],speed:[0.0,0.0],map1:"bg1.png",map2:"bg2.png"},
			  },
			  
              //GameLayer
			  {unit_type:"GameLayer",//scene unit type
			   gameObjs:[//objects of GameLayer
						 {obj_type:"Character",res:"Cowboy.ExportJson",armature:"Cowboy",pos:[100.0,200.0],scale:[-0.2,0.2],speed:[1.0,1.0],group:bdGlobals.OBJ_GROUP_BABE,state:bdGlobals.CHAR_STATE_IDLE},
						 {obj_type:"Character",res:"Cowboy.ExportJson",armature:"Cowboy",pos:[400.0,200.0],scale:[0.2,0.2],speed:[1.0,1.0],group:bdGlobals.OBJ_GROUP_ENEMY,state:bdGlobals.CHAR_STATE_WALKING},
						]
			  },
			  
            ];