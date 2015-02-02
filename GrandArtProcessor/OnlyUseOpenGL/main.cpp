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
	fio.endOutput();

	fio.createNewFile("plugins.cfg");
	str="PluginFolder=.\n\n";
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