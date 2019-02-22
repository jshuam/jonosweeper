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
			field[fieldPos.x + fieldPos.y * height].Draw(Vei2(fieldPos.x * SpriteCodex::tileSize, fieldPos.y * SpriteCodex::tileSize), gfx);
		}
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
