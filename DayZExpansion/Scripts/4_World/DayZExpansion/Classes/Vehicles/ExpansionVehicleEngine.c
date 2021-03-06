/*
 * ExpansionVehicleEngine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum ExpansionVehicleEngineType
{
	NONE,
	RWD,
	FWD,
	AWD
};

class ExpansionVehicleEngine
{
	private ExpansionVehicleScript m_Vehicle;
	
	private float m_Inertia;

	private float m_TorqueMax;
	private float m_TorqueRPM;
	
	private float m_PowerMax;
	private float m_PowerRPM;
	
	private float m_RPMIdle;
	private float m_RPMMin;
	private float m_RPMClutch;
	private float m_RPMRedline;
	private float m_RPMMax;
	
	private float m_RPM;
	private float m_RPMPrevious;
	private float m_Torque;
	private float m_Power;
	
	void ExpansionVehicleEngine( ExpansionVehicleScript vehicle )
	{
		m_Vehicle = vehicle;

		string path;
		
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Engine inertia";
		m_Inertia = GetGame().ConfigGetFloat( path );
		
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Engine torqueMax";
		m_TorqueMax = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Engine torqueRpm";
		m_TorqueRPM = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Engine powerMax";
		m_PowerMax = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Engine powerRpm";
		m_PowerRPM = GetGame().ConfigGetFloat( path );

		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Engine rpmIdle";
		m_RPMIdle = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Engine rpmMin";
		m_RPMMin = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Engine rpmClutch";
		m_RPMClutch = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Engine rpmRedline";
		m_RPMRedline = GetGame().ConfigGetFloat( path );
		path = "CfgVehicles " + m_Vehicle.GetType() + " VehicleSimulation Engine rpmMax";
		m_RPMMax = GetGame().ConfigGetFloat( path );
	}
	
	void ~ExpansionVehicleEngine()
	{
	}

	void ExpansionDebugUI( string message = "" )
	{
		ExpansionDebugger.Display( EXPANSION_DEBUG_VEHICLE_ENGINE, message );
	}

	void OnUpdate( float pDt, float pThrottle, float pGR )
	{
		float axleDiff = GetDifferential();
		
		m_RPM = Math.AbsFloat( GetWheelAngularVelocity() * pGR ) * axleDiff * 60.0 / ( Math.PI2 );
		m_RPM = Math.Clamp( m_RPM, m_RPMMin, m_RPMMax );
		
		m_Torque = LoopupTorque( m_RPM ) * pThrottle;
		
		if ( m_Torque < 0.0 )
			m_Torque = 0.0;

		ExpansionDebugUI( "Axle Differential: " + axleDiff );
		ExpansionDebugUI( "Gear Ratio: " + pGR );
		ExpansionDebugUI( "Torque: " + m_Torque );
		ExpansionDebugUI( "RPM: " + m_RPM );

		ApplyAxleTorque( m_Torque * pGR * m_Inertia );
	}

	protected void ApplyAxleTorque( float torque )
	{
		
	}

	float GetRPM()
	{
		return m_RPM;
	}

	float GetRPMRedline()
	{
		return m_RPMRedline;
	}

	float GetRPMMax()
	{
		return m_RPMMax;
	}
	
	private float LoopupTorque( float rpm )
	{
		if ( rpm > m_TorqueRPM )
			return m_TorqueMax * ( m_RPMMax - rpm ) / ( m_RPMMax - m_TorqueRPM );
		
		return ( m_TorqueMax * rpm ) / ( m_TorqueRPM );
	}

	float GetTorque()
	{
		return m_Torque;
	}

	float GetPower()
	{
		return m_Power;
	}

	protected float GetDifferential()
	{
		return 0;
	}

	protected float GetWheelAngularVelocity()
	{
		return 0;
	}
};

class ExpansionVehicleEngineRWD : ExpansionVehicleEngine
{
	private ExpansionVehicleAxle m_Axle;

	void ExpansionVehicleEngineRWD( ExpansionVehicleScript vehicle )
	{
		m_Axle = vehicle.GetAxle( vehicle.GetAxleCount() - 1 );
	}

	protected override void ApplyAxleTorque( float torque )
	{
		m_Axle.ApplyTorque( torque );
	}

	protected override float GetDifferential()
	{
		return m_Axle.GetFinalRatio();
	}

	protected override float GetWheelAngularVelocity()
	{
		return m_Axle.GetAngularVelocity();
	}
};

class ExpansionVehicleEngineFWD : ExpansionVehicleEngine
{
	private ExpansionVehicleAxle m_Axle;

	void ExpansionVehicleEngineFWD( ExpansionVehicleScript vehicle )
	{
		m_Axle = vehicle.GetAxle( 0 );
	}

	protected override void ApplyAxleTorque( float torque )
	{
		m_Axle.ApplyTorque( torque );
	}

	protected override float GetDifferential()
	{
		return m_Axle.GetFinalRatio();
	}

	protected override float GetWheelAngularVelocity()
	{
		return m_Axle.GetAngularVelocity();
	}
};

class ExpansionVehicleEngineAWD : ExpansionVehicleEngine
{
	private ExpansionVehicleAxle m_AxleF;
	private ExpansionVehicleAxle m_AxleB;

	void ExpansionVehicleEngineAWD( ExpansionVehicleScript vehicle )
	{
		m_AxleF = vehicle.GetAxle( 0 );
		m_AxleB = vehicle.GetAxle( vehicle.GetAxleCount() - 1 );
	}

	protected override void ApplyAxleTorque( float torque )
	{
		m_AxleF.ApplyTorque( torque );
		m_AxleB.ApplyTorque( torque );
	}

	protected override float GetDifferential()
	{
		return ( m_AxleF.GetFinalRatio() + m_AxleB.GetFinalRatio() ) * 0.5;
	}

	protected override float GetWheelAngularVelocity()
	{
		return ( m_AxleF.GetAngularVelocity() + m_AxleB.GetAngularVelocity() ) * 0.5;
	}
};