#include "JonoField.h"

JonoField::JonoField(int mines)
{
	this->mines = mines;
}

void JonoField::Draw(Graphics& gfx) const
{
	gfx.DrawRect(0, 0, width * SpriteCodex::tileSize, height * SpriteCodex::tileSize, SpriteCodex::baseColor);

	for(Vei2 fieldPos = {0, 0}; fieldPos.y < height; fieldPos.y++)
	{
		for(fieldPos.x = 0; fieldPos.x < width; fieldPos.x++)
		{
			field[fieldPos.x + fieldPos.y * height].Draw(gfx);
		}
	}
}

JonoField::Tile::Tile()
{
	for(int i = 0; i < height; i++)
	{
		for(int j = 0; j < width; j++)
		{
			field[j + i * height].pos = Vei2(j, i);
		}
	}
}

void JonoField::Tile::Draw(Graphics& gfx) const
{
	switch(this->state)
	{
		case JonoField::Tile::State::Hidden:
			SpriteCodex::DrawTileButton(this->pos, gfx);
			break;
		case JonoField::Tile::State::Revealed:
			break;
		case JonoField::Tile::State::Flagged:
			break;
		default:
			break;
	}
}

bool JonoField::Tile::HasJono() const
{
	return false;
}
