/**
 * ExpansionWorldMappingModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionWorldMappingModuleRPC
{
	INVALID = 20040,
	TurnOn,
	TurnOff,
	Load,
	COUNT
}

class ExpansionWorldMappingModule: JMModuleBase
{
	static ref ScriptInvoker SI_LampEnable = new ScriptInvoker();
	static ref ScriptInvoker SI_LampDisable = new ScriptInvoker();

	private autoptr array< vector > m_LightGenerators;

	private autoptr map< string, ref array< Object > > m_Objects;
	private string m_WorldName;
	
	protected ExpansionInteriorBuildingModule m_InteriorModule;
 	
	// ------------------------------------------------------------
	// ExpansionWorldMappingModule Constructor
	// ------------------------------------------------------------
	void ExpansionWorldMappingModule()
	{
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::ExpansionWorldMappingModule - Start");
		#endif

		m_LightGenerators = new array< vector >;

		m_Objects = new map< string, ref array< Object > >;
		
		ExpansionSettings.SI_General.Insert( OnSettingsUpdated );
		
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::ExpansionWorldMappingModule - End");
		#endif
	}
 	
	// ------------------------------------------------------------
	// ExpansionWorldMappingModule Destructor
	// ------------------------------------------------------------
	void ~ExpansionWorldMappingModule()
	{
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::~ExpansionWorldMappingModule - Start");
		#endif
		
		ExpansionSettings.SI_General.Remove( OnSettingsUpdated );
		
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::~ExpansionWorldMappingModule - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion OnInit
	// ------------------------------------------------------------
	override void OnInit()
	{
		super.OnInit();
	}
 	
	// ------------------------------------------------------------
	// Expansion OnMissionLoaded
	// ------------------------------------------------------------
	override void OnMissionLoaded()
	{
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::OnMissionLoaded - Start");
		#endif
		
		m_WorldName = AdjustWorldName( g_Game.GetWorldName() );

		if ( !IsMissionOffline() && IsMissionClient() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Send( NULL, ExpansionWorldMappingModuleRPC.Load, true );
		}
		
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::OnMissionLoaded - End");
		#endif
	}
 	
	// ------------------------------------------------------------
	// Expansion OnMissionFinish
	// ------------------------------------------------------------   
	override void OnMissionFinish()
	{
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::OnMissionFinish - Start");
		#endif
		
		UnloadMapping( m_Objects.GetKeyArray() );
		
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::OnMissionFinish - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion OnSettingsUpdated
	// ------------------------------------------------------------
	override void OnSettingsUpdated()
	{
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::OnSettingsUpdated - Start");
		#endif
		
		if ( !GetExpansionSettings().GetGeneral() )
			return;

		if ( g_Game.IsLoading() )
			return;

		if ( GetExpansionSettings().GetGeneral().Mapping && GetExpansionSettings().GetGeneral().Mapping.UseCustomMappingModule )
		{
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLaterByName( this, "LoadMappings", 500 );
		}
		else
		{
			UnloadMapping( m_Objects.GetKeyArray() );
		}

		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::OnSettingsUpdated - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion LoadMappings
	// ------------------------------------------------------------
	private void LoadMappings()
	{
		TStringArray load = new TStringArray;
		TStringArray unload = new TStringArray;
		
		FindDifference( GetExpansionSettings().GetGeneral().Mapping.Mapping, load, unload );

		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::LoadMappings - load.Count() : " + load.Count() + " unload.Count() : " + unload.Count());
		#endif

		UnloadMapping( unload );
		LoadMapping( load );
	}
	
	// ------------------------------------------------------------
	// Expansion FindDifference
	// ------------------------------------------------------------	
	private void FindDifference( TStringArray newFiles, out TStringArray load, out TStringArray unload )
	{
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::FindDifference - Start");
		#endif
		
		for ( int i = 0; i < newFiles.Count(); ++i )
		{
			#ifdef EXPANSION_MAPPING_MODULE_DEBUG
			EXLogPrint("ExpansionWorldMappingModule::FindDifference newFiles[" + i +  "] : " + newFiles[i]);
			#endif

			if ( !m_Objects.Contains( newFiles[i] ) )
			{
				load.Insert( newFiles[i] );
			}
		}
		
		for ( int j = 0; j < m_Objects.Count(); ++j )
		{
			string objName = m_Objects.GetKey( j );
			#ifdef EXPANSION_MAPPING_MODULE_DEBUG
			EXLogPrint("ExpansionWorldMappingModule::FindDifference m_Objects[" + j +  "] : " + objName );
			#endif

			if ( newFiles.Find( objName ) == -1 )
			{
				unload.Insert( objName );
			}
		}
		
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::FindDifference - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion LoadMapping
	// ------------------------------------------------------------	
	private void LoadMapping( TStringArray files )
	{
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::LoadMapping - Start files.Count() : " + files.Count());
		#endif
		
		m_InteriorModule = ExpansionInteriorBuildingModule.Cast( GetModuleManager().GetModule( ExpansionInteriorBuildingModule ) );
		
		if ( m_InteriorModule )
		{
			m_InteriorModule.LoadCachedCollisions();
			
			for ( int i = 0; i < files.Count(); ++i )
			{
				LoadFile( files[i] );
			}
			
			m_InteriorModule.SaveCachedCollisions();
		}
		
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::LoadMapping - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion UnloadMapping
	// ------------------------------------------------------------	
	private void UnloadMapping( TStringArray files )
	{
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::UnloadMapping - Start");
		#endif
		
		for ( int i = 0; i < files.Count(); i++ )
		{
			array< Object > objects = m_Objects.Get( files[i] );
			
			if ( objects )
			{
				#ifdef EXPANSION_MAPPING_MODULE_DEBUG
				EXLogPrint("ExpansionWorldMappingModule::UnloadMapping start file : " + files[i]);
				#endif

				for ( int j = 0; j < objects.Count(); j++ )
				{
					if ( objects[j] && !objects[j].ToDelete() )
						GetGame().ObjectDelete(objects[j]);
				}
				
				objects.Clear();
				m_Objects.Remove( files[i] );

				#ifdef EXPANSION_MAPPING_MODULE_DEBUG
				EXLogPrint("ExpansionWorldMappingModule::UnloadMapping end file : " + files[i]);
				#endif
			} 
		}
		
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::UnloadMapping - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion DeleteMapObjects
	// ------------------------------------------------------------		
	private void DeleteMapObjects( TStringArray files )
	{
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::DeleteMapObjects - Start");
		#endif
		
		for ( int i = 0; i < files.Count(); i++ )
		{
			LoadToDeleteFile( files[i] );
		}
		
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::DeleteMapObjects - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion AdjustWorldName
	// ------------------------------------------------------------	
	private string AdjustWorldName( string name )
	{
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::AdjustWorldName - Start");
		#endif
		
		string nName = name;
		nName.ToLower();
		if ( nName.Contains( "chernarusplus" ) )
		{
			return "chernarusplus";
		}

		return nName;
		
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::AdjustWorldName - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion GetObjectFromFile
	// ------------------------------------------------------------
	private bool GetObjectFromFile( FileHandle file, out string name, out vector position, out vector rotation, out string special = "false" )
	{
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::GetObjectFromFile - Start");
		#endif
		
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		//EXLogPrint("ExpansionWorldMappingModule::GetObjectFromFile - Handle file: " + file.ToString());
		#endif
		
		string line;
		int lineSize = FGets( file, line );

		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::GetObjectFromFile - Line: " + line);
		#endif
		
		if ( lineSize < 1 )
			return false;
		
		ref TStringArray tokens = new TStringArray;
		line.Split( "|", tokens );

		name = tokens.Get( 0 );		
		position = tokens.Get( 1 ).ToVector();
		rotation = tokens.Get( 2 ).ToVector();	
		special = tokens.Get( 3 );
		
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::GetObjectFromFile - name: " + name);
		EXLogPrint("ExpansionWorldMappingModule::GetObjectFromFile - position: " + position.ToString());
		EXLogPrint("ExpansionWorldMappingModule::GetObjectFromFile - rotation: " + position.ToString());
		EXLogPrint("ExpansionWorldMappingModule::GetObjectFromFile - special: " + special);
		#endif
		
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::GetObjectFromFile - End");
		#endif

		return true;
	}
	
	// ------------------------------------------------------------
	// Expansion FixObjectCollision
	// ------------------------------------------------------------
	private void FixObjectCollision( Object obj )
	{
		vector roll = obj.GetOrientation();
		roll[2] = roll[2] - 1;
		obj.SetOrientation( roll );
		roll[2] = roll[2] + 1;
		obj.SetOrientation( roll );
	}
	
	// ------------------------------------------------------------
	// Expansion LoadFile
	// ------------------------------------------------------------
	private void LoadFile( string name )
	{
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::LoadFile - Start name : " + name);
		#endif

		m_InteriorModule = ExpansionInteriorBuildingModule.Cast( GetModuleManager().GetModule( ExpansionInteriorBuildingModule ) );
		
		if (!m_InteriorModule)
			return;
		
		string className;
		vector position;
		vector rotation;
		string special;

		string filePath = EXPANSION_MAPPING_FOLDER + m_WorldName + "/" + name + EXPANSION_MAPPING_EXT;
		FileHandle file = OpenFile( filePath, FileMode.READ );
	
		if ( !file )
			return;

		array< Object > objects = new array< Object >;
		
		while ( GetObjectFromFile( file, className, position, rotation, special ) )
		{
			bool collisionBox = false;
			
			Object obj;
			if (!m_InteriorModule.m_CachedCollision.Find(className, collisionBox))
			{
				#ifdef EXPANSION_MAPPING_MODULE_DEBUG
				EXLogPrint("Spawning object with chached collition: " + className + " on pos: " + position.ToString());
				#endif
				obj = GetGame().CreateObjectEx( className, position, ECE_CREATEPHYSICS|ECE_UPDATEPATHGRAPH|ECE_LOCAL );
				
				obj.SetFlags(EntityFlags.STATIC, false);
				
				if( position )
					obj.SetPosition( position );
				
				if( rotation )
					obj.SetOrientation( rotation );

				//FixObjectCollision( obj );

				/*if ( obj.CanAffectPathgraph() )
				{
					obj.SetAffectPathgraph( true, false );
					GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 100, false, obj );
				}*/
				
				#ifdef EXPANSION_MAPPING_MODULE_DEBUG
				EXLogPrint("Succesfully spawned object with chached collition: " + className + " on pos: " + position.ToString());
				#endif
				if ( !obj )
				{
					m_InteriorModule.m_CachedCollision.Insert( className, false );
					continue;
				}
				/*
				BuildingBase buildingBase = BuildingBase.Cast(obj);
				if (buildingBase)
					buildingBase.RemoveFromInteriorsBuildings();*/
				
				vector minMax[2];
				collisionBox = obj.GetCollisionBox(minMax);
				
				m_InteriorModule.m_CachedCollision.Insert(className, collisionBox);
				
				GetGame().ObjectDelete(obj);
			}
			
			#ifdef EXPANSION_MAPPING_MODULE_DEBUG
			EXLogPrint( "ExpansionWorldMappingModule::LoadFile Attempt to create object " + className + " collision : " + collisionBox + " at " + position + " from file:" + filePath + ".");
			#endif

			//! Only spawn object with collision at server side, and object without collision at client side 
			if (collisionBox && IsMissionClient() && GetGame().IsMultiplayer()) continue;
			if (!collisionBox && IsMissionHost() && GetGame().IsMultiplayer()) continue;
			
			int flags = ECE_CREATEPHYSICS|ECE_UPDATEPATHGRAPH;
			if ( IsMissionClient() )
				flags = ECE_CREATEPHYSICS|ECE_UPDATEPATHGRAPH|ECE_LOCAL;
			
			obj = GetGame().CreateObjectEx( className, position, flags );
			if ( !obj )
				continue;
			
			#ifdef EXPANSION_MAPPING_MODULE_DEBUG
			EXLogPrint( "ExpansionWorldMappingModule::LoadFile Created object " + className + " collision : " + collisionBox + " at " + position + " from file:" + filePath + ".");
			#endif
			
			obj.SetFlags(EntityFlags.STATIC, false);
						
			obj.SetPosition( position );
			obj.SetOrientation( rotation );
			
			FixObjectCollision( obj );

			if ( obj.CanAffectPathgraph() )
			{
				obj.SetAffectPathgraph( true, false );
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( GetGame().UpdatePathgraphRegionByObject, 100, false, obj );
			}
			
			ItemBase item = ItemBase.Cast( obj );
			if ( item )
			{
				item.SetTakeable( false );
				
				if ( IsMissionHost() ) 
					item.SetLifetimeMax(1.0);
			}
			
			Building building = Building.Cast( obj );
			if ( building )
			{
				GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).CallLater( this.CheckDoors, 100, false, building );
			}
			
			if ( special == "true")
				ProcessObject( obj );
			
			objects.Insert( obj );
		}

		m_Objects.Insert( name, objects );

		CloseFile( file );

		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::LoadFile - End name : " + name);
		#endif
	}
		
	// ------------------------------------------------------------
	// Expansion CheckDoors
	// ------------------------------------------------------------
	private void CheckDoors( Building building )
	{
		string cfg_doors;
		int door_sources_count;
		int i_selection;
		int door_index;
		
		if ( building )
		{
			cfg_doors = "cfgVehicles " + building.GetType() + " " + "Doors ";
			door_sources_count = GetGame().ConfigGetChildrenCount( cfg_doors );
			
			if ( door_sources_count > 0 )
			{
				//! Make sure to check for closed doors first and open them
				for ( i_selection = 0; i_selection < door_sources_count; i_selection++ )
				{
					door_index = building.GetDoorIndex( i_selection );

					//! Check if door index is valid
					if ( door_index != -1 )
					{
						if ( !building.IsDoorOpen( door_index ) )
						{
							building.OpenDoor( door_index );
						}
					}
				}
				//! After all doors are opened close them again
				for ( i_selection = 0; i_selection < door_sources_count; i_selection++ )
				{
					door_index = building.GetDoorIndex( i_selection );

					//! Check if door index is valid
					if ( door_index != -1 )
					{
						if ( building.IsDoorOpen( door_index ) )
						{
							building.CloseDoor( door_index );
						}
					}
				}
			}
		}
	}
	
	// ------------------------------------------------------------
	// Expansion ProcessObject
	// ------------------------------------------------------------	
	private void ProcessObject( Object obj )
	{
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint( "Try to process mapping object: " + obj.ClassName() );
		#endif

		if ( obj.IsInherited( ExpansionPointLight ) )
		{
			ExpansionPointLight light = ExpansionPointLight.Cast( obj );
			if ( light )
				light.SetDiffuseColor(1,0,0);
			
			#ifdef EXPANSION_MAPPING_MODULE_DEBUG
			EXLogPrint( "Processed mapping object: " + obj.ClassName() + " as ExpansionPointLight!" );
			#endif

		} else if ( obj.IsKindOf( "Fireplace" ) )
		{
			Fireplace fireplace = Fireplace.Cast( obj );
			if ( fireplace )
			{
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(fireplace.GetInventory().CreateAttachment, 60 * 1000, true, "Bark_Oak");
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(fireplace.GetInventory().CreateAttachment, 60 * 1000, true, "Firewood");
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(fireplace.GetInventory().CreateAttachment, 60 * 1000, true, "WoodenStick");
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(fireplace.StartFire, 60 * 1000, true);
			}

			#ifdef EXPANSION_MAPPING_MODULE_DEBUG
			EXLogPrint( "Processed mapping object: " + obj.ClassName() + " as Fireplace!" );
			#endif

		} else if ( obj.IsInherited( BarrelHoles_ColorBase ) )
		{
			BarrelHoles_Red barrel = BarrelHoles_Red.Cast( obj );
			if ( barrel ) 
			{
				barrel.Open();

				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(barrel.GetInventory().CreateAttachment, 60 * 1000, true, "Bark_Oak");
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(barrel.GetInventory().CreateAttachment, 60 * 1000, true, "Firewood");
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(barrel.GetInventory().CreateAttachment, 60 * 1000, true, "WoodenStick");
				GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(barrel.StartFire, 60 * 1000, true);
			}

			#ifdef EXPANSION_MAPPING_MODULE_DEBUG
			EXLogPrint( "Processed mapping object: " + obj.ClassName() + " as BarrelHoles_ColorBase!" );
			#endif

		} else if ( obj.IsKindOf( "Roadflare" ) )
		{
			Roadflare flare = Roadflare.Cast( obj );
			if ( flare ) 
			{
				flare.GetCompEM().SetEnergy(999999);
				flare.GetCompEM().SwitchOn();
				flare.SwitchLight(false); //! Flickering
			}

			#ifdef EXPANSION_MAPPING_MODULE_DEBUG
			EXLogPrint( "Processed mapping object: " + obj.ClassName() + " as Roadflare!" );
			#endif

		} else
		{
			#ifdef EXPANSION_MAPPING_MODULE_DEBUG
			EXLogPrint( "Processed mapping object: " + obj.ClassName() + " as Default!" );
			#endif
		}
	}
	
	// ------------------------------------------------------------
	// Expansion LoadFile
	// ------------------------------------------------------------
	private void LoadToDeleteFile( string name )
	{
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::LoadFile - Start");
		#endif

		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint( "Attempting to load mapping file: " + name );
		#endif

		Object obj;
		string className;
		vector position;
		vector rotation;

		string filePath = EXPANSION_MAPPING_FOLDER + m_WorldName + "/" + name + EXPANSION_MAPPING_EXT;
		FileHandle file = OpenFile( filePath, FileMode.READ );
	
		if ( !file )
			return;
		
		while ( GetObjectFromFile( file, className, position, rotation ) )
		{
			array<Object> objects = new array<Object>;
			array<CargoBase> proxies = new array<CargoBase>;
			
			GetGame().GetObjectsAtPosition3D( position, 5, objects, proxies );
			
			for ( int j = 0; j < objects.Count(); j++ )
			{
				obj = objects.Get(j);
				if ( obj.GetType() == className )
				{
					GetGame().GetCallQueue( CALL_CATEGORY_SYSTEM ).Call( GetGame().ObjectDelete, obj ); 
					
					if ( obj )
					{
						obj.SetPosition( "0 0 0" );
					}
				}
			}
			
			objects.Clear();
			proxies.Clear();
		}

		CloseFile( file );

		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint( "Created all objects from mapping file: " + filePath );
		#endif

		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("ExpansionWorldMappingModule::LoadFile - End");
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion GetRPCMin
	// ------------------------------------------------------------	
	override int GetRPCMin()
	{
		return ExpansionWorldMappingModuleRPC.INVALID;
	}
	
	// ------------------------------------------------------------
	// Expansion GetRPCMax
	// ------------------------------------------------------------	
	override int GetRPCMax()
	{
		return ExpansionWorldMappingModuleRPC.COUNT;
	}
	
	// ------------------------------------------------------------
	// Expansion OnRPC
	// ------------------------------------------------------------	
	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
		switch ( rpc_type )
		{
		case ExpansionWorldMappingModuleRPC.TurnOn:
			RPC_TurnOn( ctx, sender, target );
			break;
		case ExpansionWorldMappingModuleRPC.TurnOff:
			RPC_TurnOff( ctx, sender, target );
			break;
		case ExpansionWorldMappingModuleRPC.Load:
			RPC_Load( ctx, sender, target );
			break;
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_TurnOn
	// ------------------------------------------------------------	
	private void RPC_TurnOn( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( IsMissionHost() )
			return;

		vector position;
		if ( ctx.Read( position ) )
		{
			m_LightGenerators.Insert( position );
			SI_LampEnable.Invoke( position );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_TurnOff
	// ------------------------------------------------------------	
	private void RPC_TurnOff( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( IsMissionHost() )
			return;

		vector position;
		if ( ctx.Read( position ) )
		{
			m_LightGenerators.RemoveItem( position );
			SI_LampDisable.Invoke( position );
		}
	}
	
	// ------------------------------------------------------------
	// Expansion RPC_Load
	// ------------------------------------------------------------	
	private void RPC_Load( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		if ( IsMissionHost() )
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( m_LightGenerators );
			rpc.Send( NULL, ExpansionWorldMappingModuleRPC.Load, true, senderRPC );
		} else
		{
			ctx.Read( m_LightGenerators );

			for ( int i = 0; i < m_LightGenerators.Count(); i++ )
			{
				SI_LampEnable.Invoke( m_LightGenerators[i] );
			}

			ExpansionSettings.SI_General.Invoke();
		}
	}
	
	// ------------------------------------------------------------
	// Expansion TurnOnGenerator
	// ------------------------------------------------------------	
	void TurnOnGenerator( vector position )
	{
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("TurnOnGenerator - Start - " + position );
		#endif

		m_LightGenerators.Insert( position );

		if ( IsMissionOffline() )
		{
			SI_LampEnable.Invoke( position );
		} else
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( position );
			rpc.Send( NULL, ExpansionWorldMappingModuleRPC.TurnOn, true, NULL );
		}

		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("TurnOnGenerator - End - " + position );
		#endif
	}
	
	// ------------------------------------------------------------
	// Expansion TurnOffGenerator
	// ------------------------------------------------------------	
	void TurnOffGenerator( vector position )
	{
		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("TurnOffGenerator - Start - " + position );
		#endif

		m_LightGenerators.RemoveItem( position );

		if ( IsMissionOffline() )
		{
			SI_LampDisable.Invoke( position );
		} 
		else
		{
			ScriptRPC rpc = new ScriptRPC();
			rpc.Write( position );
			rpc.Send( NULL, ExpansionWorldMappingModuleRPC.TurnOff, true, NULL );
		}

		#ifdef EXPANSION_MAPPING_MODULE_DEBUG
		EXLogPrint("TurnOffGenerator - End - " + position );
		#endif
	}
}