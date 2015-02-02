#ifndef GAPAUDIOMANAGER_H
#define GAPAUDIOMANAGER_H

#include "Common.h"

#include "Framework.h"
#include "vorbisfile.h"

class GAPMainManager;

class GAExport GAPAudioManager
{
#define ALBUFFERS 4
private:
	struct MusicParam
	{
		std::string musicPath;
		bool isLoop;
	};
	typedef std::vector<std::string> MusicPathList;
	int g_currentMusicNo;
	bool g_loopList;
	bool g_stopList;
	MusicPathList g_pathList;
	GAPMainManager* g_mainMgr;
public:
	GAPAudioManager(GAPMainManager *mainMgr);

	bool addMusicToCurrentPlayList(std::string musicPath);
	bool addDirectoryToCurrentPlayList(std::string directory);

	void playMusic(std::string musicPath);
	void playMusicList();
	void nextMusic();
	void previousMusic();
	void pauseMusic();
	void stopMusic();

	void stopShortSound();
	void pauseShortSound();
	bool playShortSound(std::string path,bool loop=false);
	bool playOggMusic(std::string path);
	bool playWavMusic(std::string path);

	bool update();
private:
	bool clearBuffer();
	bool updateBuffer();
	bool updateMusicList();
	bool updateShortSound();
private:
	ALuint staticSource;
	ALuint staticBuffer;
	ALint staticLoop;
	ALint staticState;

private:
	ov_callbacks	sCallbacks;
	OggVorbis_File	sOggVorbisFile;
	vorbis_info		*psVorbisInfo;
	ALuint		    uiBuffers[ALBUFFERS];
	ALuint		    uiSource;
	ALuint			uiBuffer;
	ALint			iState;
	ALint			iLoop;
	ALint			iBuffersProcessed, iQueuedBuffers;
	unsigned long	ulFrequency;
	unsigned long	ulFormat;
	unsigned long	ulChannels;
	unsigned long	ulBufferSize;
	unsigned long	ulBytesWritten;
	char			*pDecodeBuffer;
	~GAPAudioManager()
	{
		ShutdownVorbisFile();
		ALFWShutdownOpenAL();
		ALFWShutdown();
	}
private:
	HINSTANCE g_hVorbisFileDLL;
	bool g_bVorbisInit;
	void InitVorbisFile();
	void ShutdownVorbisFile();
	unsigned long DecodeOggVorbis(OggVorbis_File *psOggVorbisFile, char *pDecodeBuffer, unsigned long ulBufferSize, unsigned long ulChannels);
	void Swap(short &s1, short &s2);

	typedef int (*LPOVCLEAR)(OggVorbis_File *vf);
	typedef long (*LPOVREAD)(OggVorbis_File *vf,char *buffer,int length,int bigendianp,int word,int sgned,int *bitstream);
	typedef ogg_int64_t (*LPOVPCMTOTAL)(OggVorbis_File *vf,int i);
	typedef vorbis_info * (*LPOVINFO)(OggVorbis_File *vf,int link);
	typedef vorbis_comment * (*LPOVCOMMENT)(OggVorbis_File *vf,int link);
	typedef int (*LPOVOPENCALLBACKS)(void *datasource, OggVorbis_File *vf,char *initial, long ibytes, ov_callbacks callbacks);
	LPOVCLEAR			fn_ov_clear;
	LPOVREAD			fn_ov_read;
	LPOVPCMTOTAL		fn_ov_pcm_total;
	LPOVINFO			fn_ov_info;
	LPOVCOMMENT			fn_ov_comment;
	LPOVOPENCALLBACKS	fn_ov_open_callbacks;
};

size_t ov_read_func(void *ptr, size_t size, size_t nmemb, void *datasource);
int ov_seek_func(void *datasource, ogg_int64_t offset, int whence);
int ov_close_func(void *datasource);
long ov_tell_func(void *datasource);

#endif