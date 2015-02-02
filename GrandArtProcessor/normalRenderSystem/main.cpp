#include "GAFileIO.h"

void main()
{
	GAFileIO fio;
	fio.createNewFile("ogre.cfg");
	std::string str="";
	str="Render System=OpenGL Rendering Subsystem\n\n";
	str+="[OpenGL Rendering Subsystem]\n";
	str+="Colour Depth=32\n";
	str+="Display Frequency=N/A\n";
	str+="FSAA=0\n";
	str+="Full Screen=No\n";
	str+="RTT Preferred Mode=FBO\n";
	str+="VSync=No\n";
	str+="VSync Interval=1\n";
	str+="Video Mode=1024 x 768\n";
	str+="sRGB Gamma Conversion=No";
	fio.writeLine(str);
	str="\n[Direct3D9 Rendering Subsystem]\n";
    str+="Allow NVPerfHUD=No\n";
	str+="FSAA=0\n";
	str+="Floating-point mode=Fastest\n";
	str+="Full Screen=No\n";
	str+="Rendering Device=Monitor-1-Intel(R) HD Graphics Family\n";
	str+="Resource Creation Policy=Create on all devices\n";
	str+="VSync=No\n";
	str+="VSync Interval=1\n";
	str+="Video Mode=800 x 600 @ 32-bit colour\n";
	str+="sRGB Gamma Conversion=No";
	fio.writeLine(str);
	fio.endOutput();

	fio.createNewFile("plugins.cfg");
	str="PluginFolder=.\n\n";
	str+="Plugin=RenderSystem_Direct3D9\n";
	str+="Plugin=RenderSystem_GL\n";
	str+="Plugin=Plugin_ParticleFX\n";
	str+="Plugin=Plugin_BSPSceneManager\n";
	str+="Plugin=Plugin_CgProgramManager\n";
	str+="Plugin=Plugin_PCZSceneManager\n";
	str+="Plugin=Plugin_OctreeZone\n";
	str+="Plugin=Plugin_OctreeSceneManager\n";
	fio.writeLine(str);
	fio.endOutput();
}