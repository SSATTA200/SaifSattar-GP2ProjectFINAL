#pragma once

#include <glm\glm.hpp>
#include <AL\al.h>
#include <AL\alc.h>
#include <vector>
#include <cstring>
#include <string>
#include <iostream>
#include <fstream>
#include <SDL\SDL.h>
#include <glm/gtc/type_ptr.hpp>

class AudioSound
{
	struct data {
		int s_ID, b_ID;
		char* buff;
		std::string name;
		data(unsigned int sI, unsigned int bI, char* b, const char* n)
		{
			this->s_ID = sI;
			this->b_ID = bI;
			this->buff = b;
			this->name = n;
		}

	};

	struct Vector3
	{
		float x, y, z;
	};
	std::vector<data> datas;
	ALCcontext* context;
	ALCdevice *device;
	bool isBigEnd();
	int convertInt(char* buffer, int length);
	char* loadSoundWAV(const char* fn, int& chan, int& samplerate, int& bps, int& size);

public:
	AudioSound();
	~AudioSound();
	unsigned int loadAudio(const char* filename);
	void deleteAudio(unsigned int id);
	void playAudio(unsigned int id);
	void playAudio(unsigned int id, glm::vec3& pos);
	void stopAudio(unsigned int id);
	void setAudiolistener(glm::vec3& pos, glm::vec3& camLookAt);

private:
	
};

