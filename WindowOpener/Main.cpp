#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#define GLM_ENABLE_EXPERIMENTAL

#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>
#include <vector>
#include <cstdint>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>




#include"shaderClass.h"
#include"bmp_loader.h"
#include"VAO.h"
#include"VBO.h"
#include"EBO.h"
#include"Camera.h"
#include "GUI.h"
#include"Meshes.h"
#include "Cube.h"


const unsigned int width = 1920;
const unsigned int height = 1080;

// Vertices coordinates

std::vector<Vertex> vertices = {
	{ {-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f} },
	{ {-0.5f,  0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f} },
	{ { 0.5f,  0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },
	{ { 0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f} }
};




//AUDIO
//GLOBAL
std::vector<int16_t> audio_data;   // filled by read_wav_file
size_t playback_position = 0;      // sample amount.
bool finished_playing = false;     //set once whole file is played.
float volume = 1.0f;  // 1.0 = full volume.

PWAVEHDR pWaveHdr1, pWaveHdr2; //pwave is a pointer to the normal wavehdr

#pragma pack(push, 1) //accounts for padding.
struct Twavheader //tvaw reads the 1st 44 bytes. important stuff!
{
	char chunk_ID[4];
	uint32_t chunk_size;
	char format[4];

	char sub_chunk1_ID[4];
	uint32_t sub_chunk1_size; //uint, stores 16 or 32 bits. u for unsigned.
	uint16_t audio_format;
	uint16_t num_channels;
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint16_t block_align;
	uint16_t bits_per_sample;

	char sub_chunk2_ID[4];
	uint32_t sub_chunk2_size;
};
#pragma pack(pop) //account for padding

Twavheader wav; // global so main() can read its fields after parsing

glm::vec3 sourcePos = glm::vec3(0.0f, 0.0f, 0.0f);  //THIS IS THE SOUND ORIGIN, SOOOO IMPORTANT

//SHAPE GENERATOR
std::vector<Mesh> cubes;
//SHAPE GENERATOR


//GLOBAL
//AUDIO

//AUDIO
#define OUT_BUFFER_SIZE (570 * 4) // samples per buffer

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
bool read_wav_file(std::string fname)
{
	std::ifstream wavfile(fname, std::ios::binary);

	if (!wavfile.is_open())
	{
		std::cerr << "CANT OPEN THE FILE: " << fname << std::endl;
		return false;
	}

	wavfile.read(reinterpret_cast<char*>(&wav), sizeof(Twavheader));

	if (std::string(wav.format, 4) != "WAVE" || std::string(wav.chunk_ID, 4) != "RIFF")
	{
		wavfile.close();
		std::cerr << "THIS IS NOT A WAVE NOR RIFF FILE" << std::endl;
		return false;
	}

	std::cout << "FileName:" << fname << std::endl;
	std::cout << "Channels: " << wav.num_channels << std::endl;
	std::cout << "Sample Rate: " << wav.sample_rate << " Hz" << std::endl;
	std::cout << "Bits Per Sample: " << wav.bits_per_sample << " bits" << std::endl;

	if (wav.bits_per_sample != 16)
	{
		std::cerr << "I CAN ONLY SUPPORT 16 BIT PCM WAV RIGHT NOW" << std::endl;
		return false;
	}

	audio_data.resize(wav.sub_chunk2_size / sizeof(int16_t));
	wavfile.read(reinterpret_cast<char*>(audio_data.data()), wav.sub_chunk2_size);
	wavfile.close();

	return true;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1
int FillBuffer(short* pBuffer)
{
	size_t remaining = audio_data.size() - playback_position;
	size_t toCopy = (remaining < OUT_BUFFER_SIZE) ? remaining : OUT_BUFFER_SIZE;

	for (size_t i = 0; i < toCopy; i++)
	{
		int16_t sample = audio_data[playback_position + i];
		pBuffer[i] = (int16_t)(sample * volume);
	}
	playback_position += toCopy;

	if (toCopy < OUT_BUFFER_SIZE)
		memset(pBuffer + toCopy, 0, (OUT_BUFFER_SIZE - toCopy) * sizeof(int16_t));

	if (playback_position >= audio_data.size())
		finished_playing = true;

	return (int)toCopy;
}

//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
void CALLBACK waveOutProc(HWAVEOUT hwo, UINT uMsg, DWORD_PTR dwInstance, DWORD_PTR dwParam1, DWORD_PTR dwParam2)
{
	if (uMsg == WOM_DONE)
	{
		if (finished_playing)
			return;

		WAVEHDR* pHdr = (WAVEHDR*)dwParam1;
		FillBuffer((short*)pHdr->lpData);
		waveOutWrite(hwo, pHdr, sizeof(WAVEHDR));
	}
}
//AUDIO



int main()

{
	glfwInit(); //initializing the thing

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //a kind of package thingie, the core profile is modern.	

	GLFWwindow* window = glfwCreateWindow(width, height, "Novicii", NULL, NULL); //creates the acursed object. first "NULL" is for fullscreen.

	if (window == NULL)
	{
		std::cout << "FUCK MY LIFE IT DIDNT WORK" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window); //without this, the window is useless.

	gladLoadGL();

	glViewport(0, 0, width, height); //0.0 is the bottom left. 

	//TEST
	std::vector<GLuint> indices = { 0, 2, 1, 0, 3, 2 };

	Texture tex;
	tex.id = loadTextureFromBMP("C:\\Users\\tallm\\source\\repos\\bmp reader\\bmp reader\\stratosphereBMP.bmp");
	tex.type = "diffuse";
	tex.path = "C:\\Users\\tallm\\source\\repos\\bmp reader\\bmp reader\\stratosphereBMP.bmp";

	//TEST





	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");


	//STRATOSPSERE

	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(reinterpret_cast<GLfloat*>(vertices.data()), vertices.size() * sizeof(Vertex));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices.data(), indices.size() * sizeof(GLuint));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 8 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	VAO1.LinkAttrib(VBO1, 2, 2, GL_FLOAT, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	//STRATOSPSHERE 

	//stratospshere.

	int imgwidth, imgheight;
	std::vector<uint8_t> pixels;

	loadBMP("C:\\Users\\tallm\\source\\repos\\bmp reader\\bmp reader\\stratosphereBMP.bmp", imgwidth, imgheight, pixels);

	// Variables that help the rotation of the stratospshere
	//float rotation = 0.0f;
	//double prevTime = glfwGetTime();

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 2.0f));

	//AUDIO
	if (!read_wav_file("C:\\Users\\tallm\\Downloads\\emptyroom.wav"))
		return 1;

	HWAVEOUT hWaveOut;
	WAVEFORMATEX waveformat = {};

	pWaveHdr1 = (PWAVEHDR)malloc(sizeof(WAVEHDR));
	pWaveHdr2 = (PWAVEHDR)malloc(sizeof(WAVEHDR));
	short* pBuffer1 = (short*)malloc(OUT_BUFFER_SIZE * sizeof(short));
	short* pBuffer2 = (short*)malloc(OUT_BUFFER_SIZE * sizeof(short));

	waveformat.wFormatTag = WAVE_FORMAT_PCM;
	waveformat.nChannels = wav.num_channels;
	waveformat.nSamplesPerSec = wav.sample_rate;
	waveformat.wBitsPerSample = wav.bits_per_sample;
	waveformat.nBlockAlign = (waveformat.wBitsPerSample / 8) * waveformat.nChannels;
	waveformat.nAvgBytesPerSec = waveformat.nBlockAlign * waveformat.nSamplesPerSec;

	waveOutOpen(&hWaveOut, WAVE_MAPPER, &waveformat, (DWORD_PTR)waveOutProc, 0, CALLBACK_FUNCTION);

	pWaveHdr1->lpData = (LPSTR)pBuffer1;
	pWaveHdr1->dwBufferLength = OUT_BUFFER_SIZE * sizeof(short);
	pWaveHdr1->dwFlags = 0;
	waveOutPrepareHeader(hWaveOut, pWaveHdr1, sizeof(WAVEHDR));

	pWaveHdr2->lpData = (LPSTR)pBuffer2;
	pWaveHdr2->dwBufferLength = OUT_BUFFER_SIZE * sizeof(short);
	pWaveHdr2->dwFlags = 0;
	waveOutPrepareHeader(hWaveOut, pWaveHdr2, sizeof(WAVEHDR));

	FillBuffer(pBuffer1);
	waveOutWrite(hWaveOut, pWaveHdr1, sizeof(WAVEHDR));
	FillBuffer(pBuffer2);
	waveOutWrite(hWaveOut, pWaveHdr2, sizeof(WAVEHDR));
	//AUDIO


	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexImage2D(
		GL_TEXTURE_2D,
		0,
		GL_RGB,
		imgwidth,
		imgheight,
		0,			//legacy, unimportant.
		GL_RGB,
		GL_UNSIGNED_BYTE,
		pixels.data()
);
	

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		

	//stratospshere.
	

//GUI
	GUI gui;
	gui.Init();
	int playButtonID = gui.AddButton(0.8f, 0.8f, 0.2f, "C:\\Users\\tallm\\Downloads\\castlewalls.bmp");
	int quitButtonID = gui.AddButton(-0.9f, 0.9f, 0.1f, "C:\\Users\\tallm\\Downloads\\castlewalls.bmp");

	//DROPDOWN TRIGGERS AND MENU ITEMS!!!
	int menuButtonID = gui.AddButton(0.0f, 0.9f, 0.1f, "C:\\Users\\tallm\\Downloads\\cubeBMP.bmp");
	int menuSquareID = gui.AddButton(0.0f, 0.7f, 0.08f, "C:\\Users\\tallm\\Downloads\\cubeBMP.bmp");
	int menuSlabID = gui.AddButton(0.0f, 0.5f, 0.08f, "C:\\Users\\tallm\\Downloads\\cubeBMP.bmp");
	
	int myDropwdownID = gui.AddDropdown(menuButtonID, { menuSquareID, menuSlabID });

	//Now all i have to do is add a new line here for a new button!! innovative!
	// (xaxisf, yaxisf, scalef)
//GUI

	// Main while loop
		while (!glfwWindowShouldClose(window))
		{
			// Specify the color of the background
			glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
			// Clean the back buffer and assign the new color to it
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Tell OpenGL which Shader Program we want to use
			shaderProgram.Activate();
			// Bind the VAO so OpenGL knows to use it 

			

			camera.Inputs(window);

			//AUDIO
			float dx = camera.Position.x - sourcePos.x;
			float dy = camera.Position.y - sourcePos.y;
			float dz = camera.Position.z - sourcePos.z;
			float distance = sqrtf(dx * dx + dy * dy + dz * dz);

			float referenceDistance = 1.0f;
			float newVolume = referenceDistance / (referenceDistance + distance);
			if (newVolume > 1.0f) newVolume = 1.0f;
			volume = newVolume;
			//AUDIO

			camera.updateMatrix(45.0f, 0.1f, 100.0f);

			camera.Matrix(shaderProgram, "camMatrix");

			VAO1.Bind();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture_id);
			GLint texUniform = glGetUniformLocation(shaderProgram.ID, "u_Texture");
			glUniform1i(texUniform, 0);
		


			// Draw primitives, number of indices, datatype of indices, index of indices
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


			//GUI			for (auto& cube : cubes)
			for (auto& cube : cubes)
				cube.Draw(shaderProgram);



			gui.Update(window, width, height); //HOVER
			
			if (gui.IsButtonClicked(playButtonID))
			{
				std::cout << "IM SO COOL\n";
			}

			if (gui.IsButtonClicked(quitButtonID))
			{
				glfwSetWindowShouldClose(window, true);
			}
			
			//IF YOU WANT YOUR NEW BUTTON TO DO SOMETHING
			//INSERT THE ACT HERE.
			//ButtonInit TO ADD A NEW COLOR, ButtonRender TO ENABLE IT


			gui.Render();
			if (gui.IsButtonJustClicked(menuSquareID))
			{
				glm::vec3 spawnPos = camera.Position + camera.Orientation * 3.0f; // 3 units in front
				auto [cubeVerts, cubeIndices] = MakeCube(spawnPos, 0.25f);
				cubes.emplace_back(cubeVerts, cubeIndices, std::vector<Texture>{ tex });
			}

			if (gui.IsButtonClicked(menuSlabID))
			{
				std::cout << "SLAB COMING SOON!\n";
			}
			//GUI

			// Swap the back buffer with the front buffer
			glfwSwapBuffers(window);
			// Take care of all GLFW events
			glfwPollEvents();



		}



	// Delete all the objects we've created
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	//GUI
	gui.Cleanup();
	// GUI
	//AUDIO
	waveOutReset(hWaveOut);
	waveOutUnprepareHeader(hWaveOut, pWaveHdr1, sizeof(WAVEHDR));
	waveOutUnprepareHeader(hWaveOut, pWaveHdr2, sizeof(WAVEHDR));
	waveOutClose(hWaveOut);
	free(pWaveHdr1);
	free(pWaveHdr2);
	free(pBuffer1);
	free(pBuffer2);
	//AUDIO
	shaderProgram.Delete();
	glDeleteTextures(1, &texture_id);
	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Terminate GLFW before ending the program
	glfwTerminate();
	return 0;
}