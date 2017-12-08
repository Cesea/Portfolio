#pragma once

#include <string>
#include <stdio.h>
#include <vector>

struct Word
{
	int id;
	int x;
	int y;
	int width;
	int height;
	int Xoffset;
	int Yoffset;
	int Xadvance;
	int page;
	int chnl;
};

class BmpFont
{
private:

	video::Texture* FontIMG;

	int charsCount;



	std::vector<Word*> WordVector;

public:

	void init(void);
	void Load(const char* filePath);
	void RenderText(char* WORD, int X, int Y, float sizeX, float sizeY, D3DXCOLOR color);

	Word* Search(int id);

	std::vector<Word*> GetWordVector(void) { return WordVector; }

	BmpFont();
	~BmpFont();
};

