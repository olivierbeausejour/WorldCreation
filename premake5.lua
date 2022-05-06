workspace "WorldCreation"
	architecture "x86_64"
	startproject "WorldCreation"

	configurations
	{
		"Debug",
		"Release",
		"Dist"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "OpenFrameworks"
include "WorldCreation"
