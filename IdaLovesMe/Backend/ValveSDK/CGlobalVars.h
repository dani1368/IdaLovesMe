#pragma once

class CGlobalVars
{
public:
	float	realtime; // 0x0000
	int		framecount; // 0x0004
	float	absoluteframetime; // 0x0008
	float	absoluteframestarttimestddev; // 0x000C
	float	curtime; // 0x0010
	float	frametime; // 0x0014
	int		maxclients; // 0x0018
	int		tickcount; // 0x001C
	float	interval_per_tick; // 0x0020
	float	interpolation_amount; // 0x0024
	int		simTicksThisFrame; // 0x0028
	int		network_protocol; // 0x002C
	char	pad00[14];
};