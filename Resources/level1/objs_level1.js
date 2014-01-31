sceneUnits = [
			  //Scenery Far layer
              {unit_type:"SceneryLayer",
			   attributes:{pos:[0.0,0.0],speed:[2.0,0.0],map1:"scene_far1.png",map2:"scene_far2.png"},
			  },
			  
              //Scenery Near layer
              {unit_type:"SceneryLayer",
			   attributes:{pos:[0.0,0.0],speed:[0.0,0.0],map1:"scene_near1.png",map2:"scene_near2.png"},
			  },
			  
              //GameLayer
			  {unit_type:"GameLayer",//scene unit type
			   gameObjs:[//objects of GameLayer
						 {obj_type:"Character",
						  res:"Cowboy.ExportJson",armature:"Cowboy",pos:[400.0,230.0],scale:[-0.2,0.2],speed:[1.0,3.0],
						  group:bdGlobals.OBJ_GROUP_BABE,
						  //state:bdGlobals.CHAR_STATE_IDLE,
						  state:bdGlobals.CHAR_STATE_WALKING,
						  movement_component:{speed:[4.0,6.0]},
						  skills:{
								Jump:{action:bdGlobals.BDACTION_JUMP,particle:{name:"galaxy.plist",positionType:bdGlobals.particleFree}},
						     }
						  },
						  
						  {obj_type:"Character",
						   res:"Cowboy.ExportJson",armature:"Cowboy",pos:[830.0,100.0],scale:[-0.2,0.2],speed:[1.0,1.0],
						   group:bdGlobals.OBJ_GROUP_ENEMY,state:bdGlobals.CHAR_STATE_WALKING,
						   movement_component:{speed:[4.0,0.0]},
						  },
						   {obj_type:"Character",
						   res:"Cowboy.ExportJson",armature:"Cowboy",pos:[830.0,230.0],scale:[0.2,0.2],speed:[1.0,1.0],
						   group:bdGlobals.OBJ_GROUP_ENEMY,state:bdGlobals.CHAR_STATE_WALKING,
						   movement_component:{speed:[-0.5,5.0]},
						   skills:{
								Jump:{action:bdGlobals.BDACTION_JUMP,particle:{name:"boilingfoam.plist",positionType:bdGlobals.particleFree}},
						    }
						  },
						  {obj_type:"Character",
						   res:"Cowboy.ExportJson",armature:"Cowboy",pos:[1830.0,230.0],scale:[0.2,0.2],speed:[1.0,1.0],
						   group:bdGlobals.OBJ_GROUP_ENEMY,state:bdGlobals.CHAR_STATE_WALKING,
						   movement_component:{speed:[-0.5,5.0]},
						   skills:{
								Jump:{action:bdGlobals.BDACTION_JUMP,particle:{name:"galaxy.plist",positionType:bdGlobals.particleFree}},
						    }
						  },
						]
			  },
			  
			    //SceneryLayer
              {unit_type:"SceneryLayer",
			   attributes:{pos:[0.0,0.0],speed:[0.0,0.0],map1:"scene_front1.png",map2:"scene_front2.png"},
			  },
			  
            ];
			
particleDefs = ["lavaflow.plist","galaxy.plist","boilingfoam.plist"];