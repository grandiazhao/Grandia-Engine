#include "GrandArtProcessor.h"
#include <locale.h>
#include <tchar.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include "windows.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMain( HINSTANCE hInst, HINSTANCE, LPSTR strCmdLine, INT)
#else
int main(int argc, char **argv)
#endif
{
    // Create application object
	setlocale(LC_ALL,"Chinese-simplified");
    GrandArtProcessor gap;
    try {
		gap.go();
    } catch( Exception& e ) {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		GAPMisc misc;
		MessageBox( NULL, misc.Ansi2WChar(e.getFullDescription().c_str()), _T("An exception has occured!"), MB_OK | MB_ICONERROR | MB_TASKMODAL);
#else
        std::cerr << "An exception has occured: " << e.getFullDescription();
#endif
    }


    return 0;
}

#ifdef __cplusplus
}
#endif
