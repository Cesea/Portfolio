#include "stdafx.h"
#include "BmpFont.h"


BmpFont::BmpFont()
{
	charsCount = NULL;
	FontIMG = NULL;
}


BmpFont::~BmpFont()
{
}

void BmpFont::init(void)
{
	WordVector.clear();
}

void BmpFont::Load(const char* filePath)
{
	std::string st = filePath;
	std::string path;

	FILE* f = new FILE;
	f = fopen(filePath, "r");

	int NameNum = 0;

	NameNum = st.find_last_of('.');

	path = st.substr(0, NameNum + 1) + "png";

	char line[1024];
	char ch[128];

	video::TextureHandle FontHandle = VIDEO->CreateTexture(path, path);
	FontIMG = VIDEO->GetTexture(FontHandle);


	while (!feof(f))
	{

		fgets(line, 511, f);

		sscanf(line, "%s", ch);
		if (strcmp(ch, "chars") == 0)
		{
			sscanf(line, "chars count=%d", &charsCount);
		}
		else if (strcmp(ch, "char") == 0)
		{
			Word* NewWord = new Word;
			sscanf(line, "char id=%d x=%d y=%d width=%d height=%d xoffset=%d yoffset=%d xadvance=%d page=%d chnl=%d", &NewWord->id, &NewWord->x, &NewWord->y, &NewWord->width, &NewWord->height, &NewWord->Xoffset, &NewWord->Yoffset, &NewWord->Xadvance, &NewWord->page, &NewWord->chnl);

			WordVector.push_back(NewWord);
		}
	}

	fclose(f);
}

void BmpFont::RenderText(char* WORD, int X, int Y, float sizeX, float sizeY, D3DXCOLOR color)
{
	std::string RenderWord = WORD;

	int totalX = X;
	int totalY = Y;

	for (int i = 0; i < RenderWord.size(); ++i)
	{

		char ch = WORD[i];
		Word* wd = Search(ch);

		if (i == 0)
		{
			totalX = X;
		}
		RECT rt = { wd->x ,wd->y,wd->x + wd->width,wd->y + wd->height };
		LPRECT trp = &rt;

		
		SPRITEMANAGER->DrawTexture(FontIMG->_ptr, trp, totalX, Y - (wd->height * sizeY), sizeX, sizeY, 0, color, NULL);
		totalX += wd->width * sizeX;
	}

}

Word* BmpFont::Search(int id)
{
	for (int i = 0; i < charsCount; ++i)
	{
		if (id == WordVector[i]->id)
		{
			return WordVector[i];
		}
	}
}