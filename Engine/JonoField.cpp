#include "JonoField.h"

JonoField::JonoField(int mines)
{
	this->mines = mines;
	SpawnJonos();
}

void JonoField::Draw(Graphics& gfx) const
{
	gfx.DrawRect(0, 0, width * SpriteCodex::tileSize, height * SpriteCodex::tileSize, SpriteCodex::baseColor);

	for(Vei2 fieldPos = {0, 0}; fieldPos.y < height; fieldPos.y++)
	{
		for(fieldPos.x = 0; fieldPos.x < width; fieldPos.x++)
		{
			field[fieldPos.x + fieldPos.y * height].Draw(Vei2(fieldPos.x * SpriteCodex::tileSize, fieldPos.y * SpriteCodex::tileSize), gfx);
		}
	}
}

void JonoField::SpawnJonos()
{
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_int_distribution<int> xDist(0, width);
	std::uniform_int_distribution<int> yDist(0, height);

	for(int i = 0; i < mines; i++)
	{
		Vei2 pos;
		do
		{
			pos = Vei2(xDist(mt), yDist(mt));
		}
		while(field[pos.x * width + pos.y * height].HasJono());

		field[pos.x * width + pos.y * height].SpawnJono();
	}
}

void JonoField::Tile::Draw(Vei2 pos, Graphics& gfx) const
{
	switch(this->state)
	{
		case JonoField::Tile::State::Hidden:
			SpriteCodex::DrawTileButton(pos, gfx);
			break;
		case JonoField::Tile::State::Revealed:
			SpriteCodex::DrawTileJono(pos, gfx);
			break;
		case JonoField::Tile::State::Flagged:
			SpriteCodex::DrawTileButton(pos, gfx);
			SpriteCodex::DrawTileFlag(pos, gfx);
			break;
		default:
			break;
	}
}

void JonoField::Tile::SpawnJono()
{
	state = JonoField::Tile::State::Revealed;
	hasJono = true;
}

bool JonoField::Tile::HasJono() const
{
	return hasJono;
}
