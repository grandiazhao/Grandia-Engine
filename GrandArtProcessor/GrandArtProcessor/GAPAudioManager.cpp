#include "GAPAudioManager.h"
#include "GAPMainManager.h"
#include <tchar.h>

GAPAudioManager::GAPAudioManager(GAPMainManager* mainMgr)
{
	g_bVorbisInit=false;
	g_hVorbisFileDLL=NULL;
	fn_ov_clear=NULL;
	fn_ov_comment=NULL;
	fn_ov_info=NULL;
	fn_ov_open_callbacks=NULL;
	fn_ov_pcm_total=NULL;
	fn_ov_read=NULL;
	g_mainMgr=mainMgr;

	ALFWInit();
	InitVorbisFile();
	if (!g_bVorbisInit)
	{
		ALFWShutdown();
	}
	if (!ALFWInitOpenAL(1))
	{
		ALFWShutdown();
	}
	sCallbacks.read_func=ov_read_func;
	sCallbacks.seek_func=ov_seek_func;
	sCallbacks.close_func=ov_close_func;
	sCallbacks.tell_func=ov_tell_func;
	uiSource=0;
	g_loopList=true;
	g_stopList=false;
	g_currentMusicNo=0;
	//playShortSound("stereo.wav",true);
	addDirectoryToCurrentPlayList(".\\music\\*");
	//playMusicList();
	//playOggMusic(g_pathList.at(0));
	//playOggMusic("d:\\stereo.ogg");
}

void GAPAudioManager::Swap(short &s1, short &s2)
{
	short sTemp = s1;
	s1 = s2;
	s2 = sTemp;
}

size_t ov_read_func(void *ptr, size_t size, size_t nmemb, void *datasource)
{
	return fread(ptr, size, nmemb, (FILE*)datasource);
}

int ov_seek_func(void *datasource, ogg_int64_t offset, int whence)
{
	return fseek((FILE*)datasource, (long)offset, whence);
}

int ov_close_func(void *datasource)
{
   return fclose((FILE*)datasource);
}

long ov_tell_func(void *datasource)
{
	return ftell((FILE*)datasource);
}

void GAPAudioManager::InitVorbisFile()
{	
	if (g_bVorbisInit)
		return;

	g_hVorbisFileDLL = LoadLibrary(_T("vorbisfile.dll"));
	int k=GetLastError();
	if (g_hVorbisFileDLL)
	{
		fn_ov_clear = (LPOVCLEAR)GetProcAddress(g_hVorbisFileDLL, "ov_clear");
		fn_ov_read = (LPOVREAD)GetProcAddress(g_hVorbisFileDLL, "ov_read");
		fn_ov_pcm_total = (LPOVPCMTOTAL)GetProcAddress(g_hVorbisFileDLL, "ov_pcm_total");
		fn_ov_info = (LPOVINFO)GetProcAddress(g_hVorbisFileDLL, "ov_info");
		fn_ov_comment = (LPOVCOMMENT)GetProcAddress(g_hVorbisFileDLL, "ov_comment");
		fn_ov_open_callbacks = (LPOVOPENCALLBACKS)GetProcAddress(g_hVorbisFileDLL, "ov_open_callbacks");

		if (fn_ov_clear && fn_ov_read && fn_ov_pcm_total && fn_ov_info &&
			fn_ov_comment && fn_ov_open_callbacks)
		{
			g_bVorbisInit = true;
		}
	}
	else
	{
		
	}
}

void GAPAudioManager::ShutdownVorbisFile()
{
	if (g_hVorbisFileDLL)
	{
		FreeLibrary(g_hVorbisFileDLL);
		g_hVorbisFileDLL = NULL;
	}
	g_bVorbisInit = false;
}

unsigned long GAPAudioManager::DecodeOggVorbis(OggVorbis_File *psOggVorbisFile, char *pDecodeBuffer, unsigned long ulBufferSize, unsigned long ulChannels)
{
	int current_section;
	long lDecodeSize;
	unsigned long ulSamples;
	short *pSamples;
	unsigned long ulBytesDone = 0;
	while (1)
	{
		lDecodeSize = fn_ov_read(psOggVorbisFile, pDecodeBuffer + ulBytesDone, ulBufferSize - ulBytesDone, 0, 2, 1, &current_section);
		if (lDecodeSize > 0)
		{
			ulBytesDone += lDecodeSize;

			if (ulBytesDone >= ulBufferSize)
				break;
		}
		else
		{
			break;
		}
	}
	if (ulChannels == 6)
	{		
		pSamples = (short*)pDecodeBuffer;
		for (ulSamples = 0; ulSamples < (ulBufferSize>>1); ulSamples+=6)
		{
			// WAVEFORMATEXTENSIBLE Order : FL, FR, FC, LFE, RL, RR
			// OggVorbis Order            : FL, FC, FR,  RL, RR, LFE
			Swap(pSamples[ulSamples+1], pSamples[ulSamples+2]);
			Swap(pSamples[ulSamples+3], pSamples[ulSamples+5]);
			Swap(pSamples[ulSamples+4], pSamples[ulSamples+5]);
		}
	}
	return ulBytesDone;
}

bool GAPAudioManager::addDirectoryToCurrentPlayList(std::string dirpath)
{
	HANDLE file;
	WIN32_FIND_DATA fileData;
	g_pathList.clear();
	std::string line;
	std::string param;
	GAPMisc misc;
	file = FindFirstFile(misc.Ansi2WChar(dirpath.c_str()), &fileData);
	FindNextFile(file, &fileData);
	while(FindNextFile(file, &fileData))
	{
		line=misc.WChar2Ansi(fileData.cFileName);
		std::string stline=dirpath.substr(0,dirpath.length()-1)+line;
		int pos=stline.find_last_of('.');
		if(pos>0)
		{
			param=stline.substr(pos);
			if(param==".ogg")
			{
				g_pathList.push_back(stline);
			}
		}
	}
	return true;
}

void GAPAudioManager::playMusicList()
{
	if(g_pathList.size()>0)
	{
		playOggMusic(g_pathList.at(0));
		g_stopList=false;
		g_currentMusicNo=0;
	}
	else
	{
		g_stopList=true;
	}
}
void GAPAudioManager::playMusic(std::string musicPath)
{
}

bool GAPAudioManager::playOggMusic(std::string musicPath)
{
	ulFrequency=ulFormat=ulChannels=ulBufferSize=ulBytesWritten=0;
	// Open the OggVorbis file
	FILE *pOggVorbisFile = fopen(musicPath.c_str(), "rb");
	if (!pOggVorbisFile)
	{
		ShutdownVorbisFile();
		ALFWShutdownOpenAL();
		ALFWShutdown();
		return false;
	}
	// Create an OggVorbis file stream
	if (fn_ov_open_callbacks(pOggVorbisFile, &sOggVorbisFile, NULL, 0, sCallbacks) == 0)
	{
		// Get some information about the file (Channels, Format, and Frequency)
		psVorbisInfo = fn_ov_info(&sOggVorbisFile, -1);
		if (psVorbisInfo)
		{
			ulFrequency = psVorbisInfo->rate;
			ulChannels = psVorbisInfo->channels;
			if (psVorbisInfo->channels == 1)
			{
				ulFormat = AL_FORMAT_MONO16;
				// Set BufferSize to 250ms (Frequency * 2 (16bit) divided by 4 (quarter of a second))
				ulBufferSize = ulFrequency >> 1;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				ulBufferSize -= (ulBufferSize % 2);
			}
			else if (psVorbisInfo->channels == 2)
			{
				ulFormat = AL_FORMAT_STEREO16;
				// Set BufferSize to 250ms (Frequency * 4 (16bit stereo) divided by 4 (quarter of a second))
				ulBufferSize = ulFrequency;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				ulBufferSize -= (ulBufferSize % 4);
			}
			else if (psVorbisInfo->channels == 4)
			{
				ulFormat = alGetEnumValue("AL_FORMAT_QUAD16");
				// Set BufferSize to 250ms (Frequency * 8 (16bit 4-channel) divided by 4 (quarter of a second))
				ulBufferSize = ulFrequency * 2;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				ulBufferSize -= (ulBufferSize % 8);
			}
			else if (psVorbisInfo->channels == 6)
			{
				ulFormat = alGetEnumValue("AL_FORMAT_51CHN16");
				// Set BufferSize to 250ms (Frequency * 12 (16bit 6-channel) divided by 4 (quarter of a second))
				ulBufferSize = ulFrequency * 3;
				// IMPORTANT : The Buffer Size must be an exact multiple of the BlockAlignment ...
				ulBufferSize -= (ulBufferSize % 12);
			}
		}

		if (ulFormat != 0)
		{
			pDecodeBuffer = (char*)malloc(ulBufferSize);
			if (!pDecodeBuffer)
			{
				fn_ov_clear(&sOggVorbisFile);
				ShutdownVorbisFile();
				ALFWShutdownOpenAL();
				ALFWShutdown();
				return false;
			}
			alGenBuffers( ALBUFFERS, uiBuffers );
			alGenSources( 1, &uiSource );
			for (iLoop = 0; iLoop < ALBUFFERS; iLoop++)
			{
				ulBytesWritten = DecodeOggVorbis(&sOggVorbisFile, pDecodeBuffer, ulBufferSize, ulChannels);
				if (ulBytesWritten)
				{
					alBufferData(uiBuffers[iLoop], ulFormat, pDecodeBuffer, ulBytesWritten, ulFrequency);
					alSourceQueueBuffers(uiSource, 1, &uiBuffers[iLoop]);
				}
			}
			alSourcePlay(uiSource);
		}
	}
	return true;
}

bool GAPAudioManager::clearBuffer()
{
	if(uiSource==0) return false;
	alSourceStop(uiSource);
	alSourcei(uiSource, AL_BUFFER, 0);
	if (pDecodeBuffer)
	{
		free(pDecodeBuffer);
		pDecodeBuffer = NULL;
	}
	alDeleteSources( 1, &uiSource );
	alDeleteBuffers( 4, uiBuffers );
	fn_ov_clear(&sOggVorbisFile);
	uiSource=0;
	return true;
}

bool GAPAudioManager::updateBuffer()
{
	if(uiSource==0) return false;

	iBuffersProcessed = 0;
	alGetSourcei(uiSource, AL_BUFFERS_PROCESSED, &iBuffersProcessed);
	while (iBuffersProcessed)
	{
		uiBuffer = 0;
		alSourceUnqueueBuffers(uiSource, 1, &uiBuffer);
		ulBytesWritten = DecodeOggVorbis(&sOggVorbisFile, pDecodeBuffer, ulBufferSize, ulChannels);
		if (ulBytesWritten)
		{
			alBufferData(uiBuffer, ulFormat, pDecodeBuffer, ulBytesWritten, ulFrequency);
			alSourceQueueBuffers(uiSource, 1, &uiBuffer);
		}

		iBuffersProcessed--;
	}
	alGetSourcei(uiSource, AL_SOURCE_STATE, &iState);
	if (iState != AL_PLAYING)
	{
		alGetSourcei(uiSource, AL_BUFFERS_QUEUED, &iQueuedBuffers);
		if (iQueuedBuffers)
		{
			alSourcePlay(uiSource);
		}
	}
	return true;
}

bool GAPAudioManager::updateMusicList()
{
	if(uiSource==0) return false;
	if(g_stopList) return false;
	alGetSourcei(uiSource,AL_SOURCE_STATE,&iState);
	std::string nextMusic="";
	if(iState!=AL_PLAYING)
	{
		g_currentMusicNo++;
		if(g_currentMusicNo>=g_pathList.size())
		{
			if(g_loopList)
			{
				g_currentMusicNo-=g_pathList.size();
			}
			else
			{
				g_stopList=true;
				return true;
			}
		}
		nextMusic=g_pathList.at(g_currentMusicNo);
		//清除当前音乐占用的内存
		clearBuffer();
		playOggMusic(nextMusic);
	}
	return true;
}

bool GAPAudioManager::update()
{
	updateBuffer();
	updateShortSound();
	bool ret=updateMusicList();
	return ret;
}

bool GAPAudioManager::updateShortSound()
{
	if(staticSource==0) return false;
	alGetSourcei(staticSource,AL_SOURCE_STATE,&staticState);
	if(staticState!=AL_PLAYING)
	{
		stopShortSound();
	}
	return true;
}
bool GAPAudioManager::playShortSound(std::string path,bool loop)
{
	alGenBuffers(1, &staticBuffer );
	if (!ALFWLoadWaveToBuffer(path.c_str(), staticBuffer))
	{
		return false;
	}
    alGenSources( 1, &staticSource );
	alSourcei( staticSource, AL_BUFFER, staticBuffer );
	if(loop) alSourcei(staticSource,AL_LOOPING,AL_TRUE);
    alSourcePlay( staticSource );
	return true;
}

void GAPAudioManager::pauseShortSound()
{
	if(staticSource==0) return;
	alSourcePause(staticSource);
}

void GAPAudioManager::stopShortSound()
{
	alSourceStop(staticSource);
	alDeleteSources(1, &staticSource);
	alDeleteBuffers(1, &staticBuffer);
	staticSource=0;
}