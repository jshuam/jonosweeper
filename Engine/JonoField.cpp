#include "JonoField.h"

void JonoField::Draw(Graphics& gfx) const
{
	gfx.DrawRect(0, 0, Graphics::ScreenWidth, Graphics::ScreenHeight, SpriteCodex::baseColor);

	for(Vei2 fieldPos = {0, 0}; fieldPos.y < height; fieldPos.y++)
	{
		for(fieldPos.x = 0; fieldPos.x < width; fieldPos.x++)
		{
			field[fieldPos.x * width + fieldPos.y * height].Draw(gfx);
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
