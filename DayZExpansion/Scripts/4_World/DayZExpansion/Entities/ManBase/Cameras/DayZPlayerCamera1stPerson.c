/**
 * DayZPlayerCamera1stPerson.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class DayZPlayerCamera1stPerson
{
	override void OnUpdate( float pDt, out DayZPlayerCameraResult pOutResult )
	{
		super.OnUpdate( pDt, pOutResult );
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCamera1stPerson::OnUpdate Start");
		#endif

		pOutResult.m_fInsideCamera = 1.0;
		pOutResult.m_fPositionModelSpace = 0.0;
		pOutResult.m_bUpdateWhenBlendOut = false;
		pOutResult.m_fIgnoreParentRoll = 0;
		pOutResult.m_iDirectBone = -1;
		pOutResult.m_iDirectBoneMode = 3;
		pOutResult.m_fPositionModelSpace = 1.0;
		pOutResult.m_fUseHeading = 0.0;

		ExpansionVehicleScript vehicle;
		if ( Class.CastTo( vehicle, m_pPlayer.GetParent() ) )
		{
			PlayerBase.Cast( m_pPlayer ).SetHeadInvisible( true );
		} else
		{
			PlayerBase.Cast( m_pPlayer ).SetHeadInvisible( false );
		}

		HumanInputController input = m_pPlayer.GetInputController();
		float heading = -input.GetHeadingAngle() * Math.RAD2DEG;
		float yaw = m_pPlayer.GetOrientation()[0];
		float parentYaw = 0;

		Object parent;
		if ( Class.CastTo( parent, m_pPlayer.GetParent() ) )
		{
			parentYaw = parent.GetOrientation()[0];
		}

		vector rot;

		if ( m_pPlayer.GetCommand_Vehicle() || PlayerBase.Cast( m_pPlayer ).GetCommand_ExpansionVehicle() )
		{
			rot[0] = m_fLeftRightAngle;
		} else
		{
			rot[0] = m_fLeftRightAngle - yaw + heading + parentYaw;
		}

		rot[1] = m_CurrentCameraPitch;
		rot[2] = 0;

		Math3D.YawPitchRollMatrix( rot, pOutResult.m_CameraTM );
		pOutResult.m_CameraTM[3] = m_pPlayer.GetBonePositionMS( m_iBoneIndex ) + m_OffsetLS;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("DayZPlayerCamera1stPerson::OnUpdate End");
		#endif
	}
}