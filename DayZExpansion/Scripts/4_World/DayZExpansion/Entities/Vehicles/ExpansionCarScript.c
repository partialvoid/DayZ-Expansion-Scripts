/*
 * ExpansionCarScript.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * Copyright (c) 2005 Erwin Coumans http://continuousphysics.com/Bullet/
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionCarScript
 * @brief		This class handle bicycle simulation
 **/
class ExpansionCarScript extends ExpansionVehicleScript
{
	private ExpansionCarController m_CarController;

	private ref ExpansionVehicleGearbox m_Gearbox;
	private ref ExpansionVehicleEngine m_Engine;
	
	private float m_Turn;
	private float m_TargetTurn;

	private float m_Brake;
	
	private float m_Throttle;

	void ExpansionCarScript()
	{
		Class.CastTo( m_CarController, m_Controller );
		
		m_Gearbox = new ExpansionVehicleGearbox( this );
		m_Engine = new ExpansionVehicleEngineRWD( this );
	}

	override ExpansionController GetControllerInstance()
	{
		return new ExpansionCarController( this );
	}

	ExpansionCarController GetController()
	{
		return m_CarController;
	}

	override int GetAnimInstance()
	{
		return ExpansionVehicleAnimInstances.EXPANSION_MH6;
	}

	override int GetSeatAnimationType( int posIdx )
	{
		switch( posIdx )
		{
		case 0:
			return DayZPlayerConstants.VEHICLESEAT_DRIVER;
		}

		return 0;
	}

	override bool CrewCanGetThrough( int posIdx )
	{
		return true;
	}

	override bool CanReachDoorsFromSeat( string pDoorsSelection, int pCurrentSeat )
	{
		return true;		
	}
	
	override bool IsVitalCarBattery()
	{
		return false;
	}

	override bool IsVitalSparkPlug()
	{
		return false;
	}
	
	override bool IsVitalRadiator()
	{
		return false;
	}
	
	override bool IsVitalGlowPlug()
	{
		return false;
	}

	override bool IsVitalEngineBelt()
	{
		return false;
	}

	override bool IsVitalTruckBattery()
	{
		return false;
	}

	override float GetCameraDistance()
	{
		return 4;
	}
	
	override float EngineGetRPMMax()
	{
		return m_Engine.GetRPMMax();
	}

	override float EngineGetRPMRedline()
	{
		return m_Engine.GetRPMRedline();
	}

	override float EngineGetRPM()
	{
		return m_Engine.GetRPM();
	}

	override int GetGearsCount()
	{
		return m_Gearbox.Count();
	}

	override float GetClutch()
	{
		return m_Gearbox.GetClutch();
	}

	override float GetSteering()
	{
		return m_Turn;
	}

	override float GetThrottle()
	{
		return m_Throttle;
	}

	override float GetBraking()
	{
		return m_Brake;
	}

	float Sign( float number )
	{
		if ( number < 0 )
			return -1;
		return 1;
	}

	protected override void OnHumanPilot( PlayerBase driver, float pDt )
	{
		m_TargetTurn = ( m_CarController.GetTurnRight() - m_CarController.GetTurnLeft() );
		
		m_Throttle = Math.Clamp( m_Throttle + Math.Clamp( m_CarController.GetForward() - m_Throttle, -1.0 * pDt, 1.0 * pDt ), 0.0, 1.0 );
		
		m_Brake = m_CarController.GetBackward();

		float absForwardSpeed = Math.AbsFloat( m_LinearVelocityMS[2] ) + 1.0;

		if ( Math.AbsFloat( m_TargetTurn ) > Math.AbsFloat( m_Turn ) )
		{
			m_Turn += ( m_TargetTurn - m_Turn ) * pDt * 30.0 / absForwardSpeed;
		} else if ( Math.AbsFloat( m_TargetTurn ) < Math.AbsFloat( m_Turn ) )
		{
			m_Turn += ( m_TargetTurn - m_Turn ) * pDt * 100.0 / absForwardSpeed;
		}
		
		ExpansionDebugUI( "Turn: " + m_Turn );
		ExpansionDebugUI( "Target Turn: " + m_TargetTurn );
	}

	protected override void OnAIPilot( ExpansionAIBase driver, float pDt )
	{
	}

	protected override void OnPreSimulation( float pDt )
	{
		super.OnPreSimulation( pDt );

		if ( !m_IsPhysicsHost )
			return;
		
		int gear = m_Gearbox.GetCurrentGear();
		if ( m_Throttle == 0.0 && gear != CarGear.NEUTRAL && !m_HasDriver )
		{
			m_Brake = 1.0;
		}

		float wheelBase = Math.AbsFloat( m_Wheels[2].GetInitialWheelPosition()[2] - m_Wheels[0].GetInitialWheelPosition()[2] );
		float turnRadius = ( Math.DEG2RAD * m_Axles[0].GetMaxSteeringAngle() ) / ( Math.PI2 );

		float leftBackWheel = Math.AbsFloat( m_Wheels[2].GetInitialWheelPosition()[0] ) * -Sign( m_Turn );
		float rightBackWheel = Math.AbsFloat( m_Wheels[3].GetInitialWheelPosition()[0] ) * Sign( m_Turn );

		float leftFrontWheelSteer = Math.Atan2( wheelBase / ( turnRadius + leftBackWheel ), 1 );
		float rightFrontWheelSteer = Math.Atan2( wheelBase / ( turnRadius + rightBackWheel ), 1 );
		leftFrontWheelSteer = Math.AbsFloat( Math.RAD2DEG * leftFrontWheelSteer * 2.0 / Math.PI ) * m_Turn;
		rightFrontWheelSteer = Math.AbsFloat( Math.RAD2DEG * rightFrontWheelSteer * 2.0 / Math.PI ) * m_Turn;
		
		ExpansionDebugUI( "LF Steer: " + leftFrontWheelSteer );
		ExpansionDebugUI( "RF Steer: " + rightFrontWheelSteer );

		m_Wheels[0].SetSteering( leftFrontWheelSteer );
		m_Wheels[1].SetSteering( rightFrontWheelSteer );

		if ( EngineIsOn() )
		{
			m_Engine.OnUpdate( pDt, m_Throttle, m_Gearbox.OnUpdate( m_ClutchState, m_CarController.GetGear(), pDt ) );
			
			ApplyAxleBrake( 0, m_Brake );
			ApplyAxleBrake( 1, m_Brake );
		} else
		{
			ApplyAxleBrake( 0, 1.0 );
			ApplyAxleBrake( 1, 1.0 );
		}
	}
}