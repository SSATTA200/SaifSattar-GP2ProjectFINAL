#include "AudioSound.h"


AudioSound::AudioSound()
{
	device = alcOpenDevice(NULL); //open sound card
	if (device == NULL)
	{
		std::cout << "cannot open sound card" << std::endl;
	}

	context = alcCreateContext(device, NULL);

	if (context == NULL)
	{
		std::cout << "cannot open context" << std::endl;
	}

	alcMakeContextCurrent(context);
}

AudioSound::~AudioSound()
{
	for (unsigned int i = 0; i < datas.size(); i++)
	{
		alDeleteSources(1, (unsigned int*)&datas[i].s_ID);
		if (datas[i].b_ID != -1)
		{
			alDeleteBuffers(1, (unsigned int*)&datas[i].b_ID);
			delete[] datas[i].buff;
		}
	}
	alcDestroyContext(context);
	alcCloseDevice(device);
}

bool AudioSound::isBigEnd()
{
	int a = 1;
	return !((char*)&a)[0];
}

int AudioSound::convertInt(char* buffer, int length)
{
	int a = 0;
	if (!isBigEnd())
		for (int i = 0; i < length; i++)
			((char*)&a)[i] = buffer[i];
	else
		for (int i = 0; i < length; i++)
			((char*)&a)[3 - i] = buffer[i];
	return a;
}

char* AudioSound::loadSoundWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size)
{
	char buffer[4];
	std::ifstream in(fn, std::ios::binary);
	in.read(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 4); //WAV
	in.read(buffer, 4);	//fmt
	in.read(buffer, 4); //16 bit
	in.read(buffer, 2); //1
	in.read(buffer, 2);
	chan = convertInt(buffer, 2);
	in.read(buffer, 4);
	samplerate = convertInt(buffer, 4);
	in.read(buffer, 4);
	in.read(buffer, 2);
	in.read(buffer, 2);
	bps = convertInt(buffer, 2);
	in.read(buffer, 4);
	in.read(buffer, 4);
	size = convertInt(buffer, 4);

	char* soundData = new char[size];
	in.read(soundData, size);
	return soundData;
}

unsigned int AudioSound::loadAudio(const char* filename)
{
	bool found = false;
	unsigned int sourceID, bufferID;
	char* soundData = NULL;
	for (unsigned int i = 0; i < datas.size(); i++)
	{
		if (datas[i].name == filename && datas[i].b_ID != -1)
		{
			bufferID = datas[i].b_ID;
			found = true;
			break;
		}
	}
	if(!found)
		{	
			int channel, sampleRate, bps, size;
			soundData = loadSoundWAV(filename, channel, sampleRate, bps, size);
			unsigned int format;
			alGenBuffers(1, &bufferID);

			if (channel == 1)
			{
				if (bps == 8)
				{
					format = AL_FORMAT_MONO8;
				}
				else
				{
					format = AL_FORMAT_MONO16;
				}
			}
			else
			{
				if (bps == 8)
				{
					format = AL_FORMAT_STEREO8;
				}
				else
				{
					format = AL_FORMAT_STEREO16;
				}

			}

			alBufferData(bufferID, format, soundData, size, sampleRate);
		}
	alGenSources(1, &sourceID);
	alSourcei(sourceID, AL_BUFFER, bufferID);
	alSourcef(sourceID, AL_REFERENCE_DISTANCE, 1.0f);
	datas.push_back(data(sourceID, (!found ? bufferID : -1), soundData, filename));
	return sourceID;
}

void AudioSound::deleteAudio(unsigned int id) 
{}

void AudioSound::playAudio(unsigned int id) 
{
	alSourcePlay(id);
}
void AudioSound::playAudio(unsigned int id, glm::vec3& pos) 
{
	alSource3f(id, AL_POSITION, pos.x, pos.y, pos.z);
	alSourcePlay(id);
}
void AudioSound::stopAudio(unsigned int id) 
{
	alSourceStop(id);
}
void AudioSound::setAudiolistener(glm::vec3& pos, glm::vec3& camLookAt) 
{
	alListener3f(AL_POSITION, pos.x, pos.y, -pos.z);
	float orinet[6] = { camLookAt.x,camLookAt.y,camLookAt.z,0,1,0 };
	alListenerfv(AL_ORIENTATION, orinet);
}