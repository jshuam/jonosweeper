#include "JonoField.h"

JonoField::JonoField( int mines )
{
	std::random_device rd;
	std::mt19937 mt( rd() );
	std::uniform_int_distribution<int> xDist( 0, width - 1 );
	std::uniform_int_distribution<int> yDist( 0, height - 1 );

	for( int i = 0; i < mines; i++ )
	{
		Vei2 pos;
		do
		{
			pos = Vei2( xDist( mt ), yDist( mt ) );
		}
		while( GetTile(pos).HasJono() );

		GetTile(pos).SpawnJono();
	}
}

void JonoField::Draw( Graphics& gfx ) const
{
	gfx.DrawRect( 0, 0, width * SpriteCodex::tileSize, height * SpriteCodex::tileSize, SpriteCodex::baseColor );

	for( Vei2 fieldPos = { 0, 0 }; fieldPos.y < height; fieldPos.y++ )
	{
		for( fieldPos.x = 0; fieldPos.x < width; fieldPos.x++ )
		{
			GetTile(fieldPos).Draw( fieldPos * SpriteCodex::tileSize, gfx );
		}
	}
}

void JonoField::Tile::Draw( Vei2 pos, Graphics& gfx ) const
{
	switch( state )
	{
		case State::Hidden:
			SpriteCodex::DrawTileButton( pos, gfx );
			break;
		case State::Revealed:
			SpriteCodex::DrawTileJono( pos, gfx );
			break;
		case State::Flagged:
			SpriteCodex::DrawTileButton( pos, gfx );
			SpriteCodex::DrawTileFlag( pos, gfx );
			break;
		default:
			break;
	}
}

void JonoField::Tile::SpawnJono()
{
	hasJono = true;
	state = State::Revealed;
}

void JonoField::Tile::Reveal()
{
	state = State::Revealed;
}

bool JonoField::Tile::IsRevealed() 
{
	return state == State::Revealed;
}

bool JonoField::Tile::HasJono() const
{
	return hasJono;
}

JonoField::Tile& JonoField::GetTile( Vei2 pos )
{
	return field[pos.x + pos.y * width];
}

const JonoField::Tile& JonoField::GetTile( Vei2 pos ) const
{
	return field[pos.x + pos.y * width];
}
